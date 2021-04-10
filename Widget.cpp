#include "Widget.h"
#include "ui_Widget.h"
#include "QtAxExcelEngine.h"
#include "Config.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <thread>
#include <QCoreApplication>
#include <future>
#include <QFuture>
#include "XmlModel.h"
#include <QDesktopServices>
#include <QHeaderView>
#include <QMutex>
#include <QMutexLocker>

static const QString excelFitter = "execel (*.xl* )";

static QStringList targetFullNameColumns;
enum Marks{
    StartNo = 1,
    EndNo = 10,
};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget),m_promise(nullptr),m_promiseXmlConfig(nullptr)
{
    ui->setupUi(this);
    m_readEngine = new QtAxExcelEngine();
    if(Config::getInstance().getPath(Config::PathType::OutPutPath).isEmpty()){
        Config::getInstance().setPath(Config::PathType::OutPutPath , QCoreApplication::applicationDirPath());
    }
    ui->editWrite->setText(Config::getInstance().getPath(Config::PathType::OutPutPath));

    //Init xml config
    setMinimumWidth(900);

    m_xmlModel = new XmlModel(ui->treeView);
    ui->treeView->setModel(m_xmlModel);
    ui->spinRowReadTemp->setValue(Config::getInstance().getRowNoReadTemplateStart());
    connect(ui->spinRowReadTemp, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int i){
        Config::getInstance().setRowNoReadTemplateStart(i);
        if(!ui->editTempletePath->text().isEmpty())
            resolveTemplate(ui->editTempletePath->text());
    });

}

Widget::~Widget()
{
    safeDelate(m_readEngine);
    safeDelate(m_promise);
    safeDelate(m_promiseXmlConfig);
    delete ui;
}

void Widget::on_btnStart_clicked()
{
    if(!m_promise)
        return;
    auto finishedResolve = m_promise->get_future().get();
    safeDelate(m_promise);
    if(!finishedResolve)
        return;

    QMap<QString,QVariantList> mapData;
    int bret;
    bret = m_readEngine-> initialize(false);
    if (!bret)
    {
        fprintf(stderr, "initialize excel fail.\n");
    }
    
    //测试相对路径打开，
    if(Config::getInstance().getFileName(Config::FileNames::ExcelRead).isEmpty()){
        QMessageBox::warning(this , "错误" ,"待读取的源Excel为空","确定");
        return;
    }
    bret = m_readEngine-> open(Config::getInstance().getFileName(Config::FileNames::ExcelRead),false);
    if (!bret)
    {
        QMessageBox::warning(this , "错误" ,"读取Excel失败","确定");
        return;
    }
    
    bret =  m_readEngine->loadSheet(1, false);
    if (!bret)
    {
        QMessageBox::warning(this , "错误" ,"加载Excel失败","确定");
        return;
    }
    for(int  i = 1 ; i <= m_readEngine->columnCount(); ++i ){
        if(!Config::getInstance().getFullNameColumns().contains(m_readEngine->getCell(1 ,i) .toString()))
            continue;
        else{
            auto key = m_readEngine->getCell(1 ,i) .toString();
            QVariantList data_list;
            for(int j = ui->spinRowStartRead->value() ; j <= ui->spinRowEndRead->value() ; j++)
                data_list<<m_readEngine->getCell(j,i).toString();
            mapData.insert(key ,data_list);
        }
    }

    if(!mapData.size()){
        m_readEngine->finalize();
        return;
    }
    
    //Add new data sheet
    bret = m_readEngine->newOne();
    if (!bret)
    {
        fprintf(stderr, "Open excel fail.\n");
    }
    m_readEngine->insertSheet(Config::getInstance().getFileName(Config::FileNames::ExcelRead));

    //Insert the data
    int columnWrited = 0;
    for(auto i : Config::getInstance().getFullNameColumns()){
        if(!Config::getInstance().code(i).isEmpty()){
            ++columnWrited;
            QVariantList values;
            values << Config::getInstance().code(i);    //Column head
            values << mapData.value(i);
            for(int i = 1 ; i <= values.size() ; i ++)
                m_readEngine->setCell(i ,columnWrited , values.at(i-1));
        }
    }

    //Format the output filename
    QString outputExcelName = Config::getInstance().getPath(Config::PathType::OutPutPath);
    outputExcelName +=QString("/" + QString("output_")+  Config::getInstance().getFileName(Config::FileNames::ExcelRead).split('/').last());
    m_readEngine->saveAs(outputExcelName);
    
    m_readEngine->finalize();
    QMessageBox::information(this, "提示" ,"导出完成");
    
}

void Widget::on_btnRead_clicked()
{

    auto fileToRead = QFileDialog::getOpenFileName(nullptr,"选择要读取的Excel", Config::getInstance().getLastDirOpen(Config::FileNames::ExcelRead) ,excelFitter);
    if(!fileToRead.isEmpty()){
        ui->editRead->setText(fileToRead);
        Config::getInstance().setLastFileOpen(Config::FileNames::ExcelRead, fileToRead);
    }

    auto resolveExcelInfor = [&]( const QString filePath){
        m_readEngine-> initialize(false);
        if(m_readEngine->open(filePath,false)){
            if(m_readEngine->loadSheet(1 ,false)){
                int foundFirst = 0;
                if("序号" == m_readEngine->getCell( 1 ,1).toString()){
                    for(int i = 1 ; i < m_readEngine->rowCount() +1 ; i ++){
                        qDebug() <<  m_readEngine->getCell( i ,1).toString();
                        if( StartNo == m_readEngine->getCell( i ,1).toInt()){
                            ui->spinRowStartRead->setValue(i);
                            foundFirst = i;
                        }
                        if(EndNo ==  m_readEngine->getCell( i ,1).toInt())
                            ui->spinRowEndRead->setValue(i);
                    }
                }

            }
            else{
                qDebug() << "Load excel failed";
                if(m_promise)
                    m_promise->set_value(false);

            }
        }
        else{
            qDebug() << "Open excel failed";
            if(m_promise)
                m_promise->set_value(false);
        }
        m_readEngine->finalize();
        if(m_promise)
            m_promise->set_value_at_thread_exit(true);
    };

    if(!fileToRead.contains(".xl"))
        return;
    if(!m_promise)
        m_promise = new std::promise<bool> ;
    std::thread resolveWorker(resolveExcelInfor,fileToRead);
    resolveWorker.detach();

    
}

void Widget::on_btnWrite_clicked()
{
    auto fileToWrite = QFileDialog::getExistingDirectory(nullptr,"选择输出文件路径",Config::getInstance().getPath(Config::PathType::OutPutPath));
    if(!fileToWrite.isEmpty()){
        ui->editWrite->setText(fileToWrite);
        Config::getInstance().setPath(Config::PathType::OutPutPath, fileToWrite);
    }
}

void Widget::on_btnXmlPath_clicked()
{
    auto xmlToRead = QFileDialog::getOpenFileName(nullptr,"选择Xml文件路径",Config::getInstance().getLastDirOpen(Config::FileNames::XmlRead) ,("xml(*.xml)"));
    if(!xmlToRead.isEmpty()){
        ui->editXmlPath->setText(xmlToRead);
        Config::getInstance().setLastFileOpen(Config::FileNames::XmlRead, xmlToRead);
        m_xmlModel->loadXml(xmlToRead);
    }

}
void Widget::on_btnXmlStart_clicked()
{
#if 0
    if(!m_promiseXmlConfig)
        return;
    if(!m_promiseXmlConfig->get_future().get()){
        QMessageBox::warning(nullptr,"错误","解析模板错误", "确定");
        safeDelate(m_promiseXmlConfig);
        return;
    }
    safeDelate(m_promiseXmlConfig);
#endif
    if(!m_valueForCalc.size()){
        QMessageBox::warning(nullptr,"错误" ,"请先设置有效的模板路径或模板读取行数");
        return;
    }
    m_xmlModel->syncCaculateResult(m_valueForCalc);
    auto outputXmlPath = Config::getInstance().getPath(Config::PathType::OutPutPath)
            +"/" + Config::getInstance().getFileName(Config::FileNames::XmlRead).split('/').last();
    if( m_xmlModel->saveXmlAs(outputXmlPath))
        QDesktopServices::openUrl(outputXmlPath);
    else
        QMessageBox::warning(nullptr,"错误" ,"导出失败","确定");
}

void Widget::on_btnTempletePath_clicked()
{
    auto TempleteToRead = QFileDialog::getOpenFileName(nullptr,"选择模板Excel文件路径",Config::getInstance().getLastDirOpen(Config::FileNames::TemplateExcel) ,excelFitter);
    if(TempleteToRead.isEmpty())
        return;

    ui->editTempletePath->setText(TempleteToRead);
    Config::getInstance().setLastFileOpen(Config::FileNames::TemplateExcel, TempleteToRead);
    resolveTemplate(TempleteToRead);


}

void Widget::resolveTemplate(const QString filePath)
{
    safeDelate(m_promiseXmlConfig);
//    m_promiseXmlConfig = new std::promise<bool>;
    auto resolveExcutor = [&](const QString filePath){
        const int KeyRowNum = 1;
        int ValueRowNum = ui->spinRowReadTemp->value();
        m_valueForCalc.clear();

        m_readEngine-> initialize(false);
        if(m_readEngine->open(filePath,false)){
            if(m_readEngine->loadSheet(1 ,false)){
                for(int i = 1 ; i < m_readEngine->columnCount() +1 ; i ++){
                    auto keyName = m_readEngine->getCell(KeyRowNum ,i).toString();
                    if(Config::getInstance().getShortNameColumns().contains(keyName)){
                        auto fullKeyName = Config::getInstance().deCode(keyName);
                        auto value = m_readEngine->getCell(ValueRowNum , i).toDouble();
                        m_valueForCalc<<QPair<QString,double>{fullKeyName , value };
                    }
                }
                if(m_promiseXmlConfig)
                    m_promiseXmlConfig->set_value_at_thread_exit(true);

            }
            else{
                qDebug() << "Load excel failed";
                if(m_promiseXmlConfig)
                    m_promiseXmlConfig->set_value_at_thread_exit(false);

            }
        }
        else{
            qDebug() << "Open excel failed";
            if(m_promiseXmlConfig)
                m_promiseXmlConfig->set_value_at_thread_exit(false);
        }
        m_readEngine->finalize();

    };
    resolveExcutor(filePath);
    QMessageBox::information(nullptr,"提示","解析Excel结束");
//    std::thread resolveWorker(resolveExcutor,filePath);
//    resolveWorker.detach();

}

