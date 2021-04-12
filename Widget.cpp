#include "Widget.h"
#include "ui_Widget.h"
#include "Config.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <thread>
#include <QCoreApplication>
#include <future>
#include <QFuture>
#include "XmlModel.h"
#include <QHeaderView>
#include <QMutex>
#include <QMutexLocker>
#include <QFileSelector>
#include <QProcess>
#include <QDesktopServices>
#include "xlsxdocument.h"

static const QString excelFitter = "execel (*.xl* )";

static QStringList targetFullNameColumns;
enum Marks{
    StartNo = 1,
    EndNo = 10,
};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
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
    delete ui;
}

void Widget::on_btnStart_clicked()
{

    //Start obtain target infor
    QMap<QString,QVariantList> mapData;
    {
        auto xlsName = Config::getInstance().getFileName(Config::FileNames::ExcelRead);
        QFile fileCheck (xlsName);
        if(!fileCheck.exists()){
            QMessageBox::warning(this , "错误" ,"不存在待读取的Excel","确定");
            return;
        }
        QXlsx::Document xlsReader(xlsName,this);
        if (!xlsReader.selectSheet(xlsReader.sheetNames().first()))
        {
            QMessageBox::warning(this , "错误" ,"加载Excel失败","确定");
            return;
        }
        for(int  i = 1 ; i <= xlsReader.dimension().lastColumn() ; ++i ){
            auto key = xlsReader.read(1 ,i).toString();
            if(!Config::getInstance().getFullNameColumns().contains(key))
                continue;
            else{
                QVariantList data_list;
                for(int j = ui->spinRowStartRead->value() ; j <= ui->spinRowEndRead->value() ; j++)
                    data_list<<xlsReader.read(j,i).toString();
                mapData.insert(key ,data_list);
            }
        }


    }
    if(!mapData.size())
        return;
    QXlsx::Document xlsWriter;
    //Insert the data
    int columnWrited = 0;
    for(auto i : Config::getInstance().getFullNameColumns()){
        if(!Config::getInstance().code(i).isEmpty()){
            ++columnWrited;
            QVariantList values;
            values << Config::getInstance().code(i);    //Column head
            values << mapData.value(i);
            for(int i = 1 ; i <= values.size() ; i ++)
                xlsWriter.write( i, columnWrited ,values.at(i-1));
        }
    }

    //Format the output filename
    QString outputExcelName = Config::getInstance().getPath(Config::PathType::OutPutPath);
    outputExcelName +=QString("/" + QString("output_")+  Config::getInstance().getFileName(Config::FileNames::ExcelRead).split('/').last());
    xlsWriter.saveAs(outputExcelName);
    QDesktopServices::openUrl(outputExcelName);
    
}

void Widget::on_btnRead_clicked()
{

    auto fileToRead = QFileDialog::getOpenFileName(nullptr,"选择要读取的Excel", Config::getInstance().getLastDirOpen(Config::FileNames::ExcelRead) ,excelFitter);
    if(!fileToRead.isEmpty()){
        ui->editRead->setText(fileToRead);
        Config::getInstance().setLastFileOpen(Config::FileNames::ExcelRead, fileToRead);
    }

    auto resolveExcelInfor = [&]( const QString filePath){
        QXlsx::Document xlsReader(filePath);
        int foundFirst = 0;
        if("序号" == xlsReader.read(1 ,1).toString()){
            for(int i = 1 ; i < xlsReader.dimension().rowCount() +1 ; i ++){
                qDebug() <<  xlsReader.read( i ,1).toString();
                if( StartNo == xlsReader.read( i ,1).toInt()){
                    ui->spinRowStartRead->setValue(i);
                    foundFirst = i;
                }
                if(EndNo ==  xlsReader.read( i ,1).toInt()){
                    ui->spinRowEndRead->setValue(i);
                    break;
                }
            }
        }

    };

    if(!fileToRead.contains(".xl"))
        return;
    resolveExcelInfor(fileToRead);

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
    if(!m_valueForCalc.size()){
        QMessageBox::warning(nullptr,"错误" ,"请先设置有效的模板路径或模板读取行数");
        return;
    }
    m_xmlModel->syncCaculateResult(m_valueForCalc);
    auto outputXmlPath = Config::getInstance().getPath(Config::PathType::OutPutPath)
            +"/" + Config::getInstance().getFileName(Config::FileNames::XmlRead).split('/').last();
    if( m_xmlModel->saveXmlAs(outputXmlPath)){
        auto workerShowXml = [](const QString){

        };
        QString strLogPath = outputXmlPath;
        QProcess showXml(this);
        showXml.setProgram("notepad");
        QStringList argument;
        argument << strLogPath;
        showXml.setArguments(argument);
        showXml.start();
        showXml.waitForStarted();
        showXml.waitForFinished();

    }
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
    //    m_promiseXmlConfig = new std::promise<bool>;
    auto resolveExcutor = [&](const QString filePath){
        const int KeyRowNum = 1;
        int ValueRowNum = ui->spinRowReadTemp->value();
        m_valueForCalc.clear();
        QXlsx::Document xlsReader(filePath);
                for(int i = 1 ; i < xlsReader.dimension().rowCount() +1 ; i ++){
                    auto keyName = xlsReader.read(KeyRowNum ,i).toString();
                    if(Config::getInstance().getShortNameColumns().contains(keyName)){
                        auto fullKeyName = Config::getInstance().deCode(keyName);
                        auto value = xlsReader.read(ValueRowNum , i).toDouble();
                        m_valueForCalc<<QPair<QString,double>{fullKeyName , value };
                    }
                }

    };
    resolveExcutor(filePath);
    QMessageBox::information(nullptr,"提示","解析Excel结束");

}

