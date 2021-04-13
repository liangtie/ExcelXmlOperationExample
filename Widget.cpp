#include "Widget.h"
#include "ui_Widget.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <thread>
#include <QCoreApplication>
#include <future>
#include <QFuture>
#include <QHeaderView>
#include <QMutex>
#include <QMutexLocker>
#include <QFileSelector>
#include <QDir>
#include <QProcess>
#include <QDesktopServices>
#include <QHeaderView>

#include "xlsxdocument.h"
#include "ModelColumnAliasConfirm.h"
#include "XmlModel.h"
#include "Config.h"
#include "DelegateCombox.h"


static const QString excelFitter = "execel (*.xl* )";

static QStringList targetFullNameColumns;
/**
 * @brief The TemplateMarks enum marks in the template excel
 */
enum TemplateMarks{
    StartNo = 1,
    VerticalHeaderColumn = 1,
    ReservedRowCount = 20,
};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    if(Config::getInstance().getPath(Config::PathType::OutPutPath).isEmpty()){
        Config::getInstance().setPath(Config::PathType::OutPutPath , QCoreApplication::applicationDirPath());
    }
    QDir checkDir(Config::getInstance().getPath(Config::PathType::OutPutPath));
    ui->editWrite->setText(checkDir.exists()?
                               Config::getInstance().getPath(Config::PathType::OutPutPath) :
                               Config::getInstance().appPath());
    if(!checkDir.exists())
        Config::getInstance().setPath(Config::PathType::OutPutPath ,Config::getInstance().appPath());

    //Init xml config
    setMinimumWidth(900);

    m_modelXmlConfirm = new XmlModel(ui->treeView);
    ui->treeView->setModel(m_modelXmlConfirm);
    ui->spinRowReadTemp->setValue(Config::getInstance().getRowNoReadTemplateStart());
    //Init columns confirm model
    m_modelAliasConfirm = new ModelColumnAliasConfirm(ui->tableView);
    ui->tableView->setModel(m_modelAliasConfirm);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    auto delegate = new DelegateCombox(ui->tableView);
    ui->tableView->setItemDelegateForColumn(ModelColumnAliasConfirm::ColumnFullName ,delegate);
    ui->tableView->setEditTriggers(QAbstractItemView::CurrentChanged|
                                   QAbstractItemView::DoubleClicked|
                                   QAbstractItemView::SelectedClicked);
    Config::getInstance().registerAliasModel(m_modelAliasConfirm);
    ui->lableInfor->setTextFormat(Qt::RichText);
    connect(ui->spinRowReadTemp, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int i){
        Config::getInstance().setRowNoReadTemplateStart(i);
    });
    connect(m_modelAliasConfirm ,&ModelColumnAliasConfirm::updateStatus ,[&](const QString &str){
        ui->lableInfor->setText(str);
    });
    connect(ui->cmbSymbol ,  QOverload<int>::of(&QComboBox::currentIndexChanged) ,[&](int idx){
        Config::getInstance().enableXmlWriteNegative( 0 == idx);
    });
    connect(ui->spinRate ,QOverload<int>::of(&QSpinBox::valueChanged) ,[&](int times){
        Config::getInstance().setXmlWriteTimes(times);
    });

    //Init Status
    m_modelAliasConfirm->isColumnMappingReady();
    ui->cmbSymbol->setCurrentIndex(1);
    ui->spinRate->setValue(1);
    showMaximized();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btnStart_clicked()
{
    if(!m_modelAliasConfirm->isColumnMappingReady()){
        return;
    }

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
            QVariantList data_list;
            for(int j = ui->spinRowStartRead->value() ; j <= ui->spinRowEndRead->value() ; j++)
                data_list<<xlsReader.read(j,i).toString();
            mapData.insert(key ,data_list);
        }

    }
    if(!mapData.size())
        return;
    QXlsx::Document xlsWriter;
    //Insert the data
    int columnWrited = 0;
    for(auto i : Config::getInstance().getTemplateColumnSetting()){
        if(!Config::getInstance().aliasToFullName(i).isEmpty()){
            ++columnWrited;
            QVariantList values;
            values << i;    //Column head
            values << mapData.value(Config::getInstance().aliasToFullName(i));
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
        ui->textEdit->append("机台结果路径:" + fileToRead);
        Config::getInstance().setLastFileOpen(Config::FileNames::ExcelRead, fileToRead);
        Config::getInstance().onMachineResultChanged(fileToRead);
    }
    auto resolveExcelInfor = [&]( const QString filePath){
        QXlsx::Document xlsReader(filePath);
        int foundFirst = 0;
        if("序号" == xlsReader.read(1 ,1).toString()){
            for(int i = 1 ; i < xlsReader.dimension().rowCount() +ReservedRowCount ; i ++){
                qDebug() <<  xlsReader.read( i ,VerticalHeaderColumn).toString();
                if( StartNo == xlsReader.read( i ,VerticalHeaderColumn).toInt()){
                    ui->spinRowStartRead->setValue(i);
                    foundFirst = i;
                }
                if(foundFirst){
                    qDebug() <<"Trying to fetch  row "<<i;
                    if( xlsReader.read(i ,VerticalHeaderColumn).isNull() || (!xlsReader.read(i,VerticalHeaderColumn).isValid())){
                        ui->spinRowEndRead->setValue(i -1);
                        qDebug() <<"Setting the end row to "<<i-1;
                        break;
                    }
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
        ui->textEdit->append("Xml配置路径:" + xmlToRead);
        Config::getInstance().setLastFileOpen(Config::FileNames::XmlRead, xmlToRead);
        m_modelXmlConfirm->loadXml(xmlToRead);
    }

}
void Widget::on_btnXmlStart_clicked()
{
    if(!m_modelAliasConfirm->isColumnMappingReady()){
        return;
    }

    resolveAjustedValueInTemplate(Config::getInstance().getFileName(Config::FileNames::TemplateExcel));

    if(!m_valueForCalc.size()){
        QMessageBox::warning(nullptr,"错误" ,"请先设置有效的模板路径或模板读取行数");
        return;
    }
    m_modelXmlConfirm->syncCaculateResult(m_valueForCalc);
    auto outputXmlPath = Config::getInstance().getPath(Config::PathType::OutPutPath)
            +"/" + Config::getInstance().getFileName(Config::FileNames::XmlRead).split('/').last();
    if( m_modelXmlConfirm->saveXmlAs(outputXmlPath)){
        auto workerShowXml = [&](const QString strLogPath){
            QProcess showXml;
            showXml.setProgram("notepad");
            QStringList argument;
            argument << strLogPath;
            showXml.setArguments(argument);
            showXml.start();
            showXml.waitForStarted();
            showXml.waitForFinished();

        };
        std::thread worker(workerShowXml , outputXmlPath);
        worker.detach();

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
    Config::getInstance().onTemplatePathChanged(TempleteToRead);
    ui->textEdit->append("模板路径:" + TempleteToRead);
}

void Widget::on_btnConfirm_clicked()
{
    if(!Config::getInstance().initColumnMapping()){
        QMessageBox::warning(this ,"错误","请先配置模板及结果路径");
        return;
    }

}

void Widget::resolveAjustedValueInTemplate(const QString filePath)
{
    auto resolveExcutor = [&](const QString filePath){
        const int KeyRowNum = 1;
        int ValueRowNum = ui->spinRowReadTemp->value();
        m_valueForCalc.clear();
        QXlsx::Document xlsReader(filePath);
        for(int i = 1 ; i < xlsReader.dimension().columnCount() +1 ; i ++){
            auto keyName = xlsReader.read(KeyRowNum ,i).toString();
            auto columnNameInResult = Config::getInstance().aliasToFullName(keyName);
            auto targetCell = xlsReader.cellAt(ValueRowNum ,i);
            if(targetCell)
                if(targetCell->value().isValid()){
                    qDebug() << targetCell->value();
                    auto value = targetCell->value().toDouble();
                    m_valueForCalc<<QPair<QString,double>{columnNameInResult , value };
                }
        }

    };
    resolveExcutor(filePath);

}

