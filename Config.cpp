#include "Config.h"
#include <QObject>
#include <QFile>
#include <QSettings>
#include <QMap>
#include <QDebug>
#include <QUuid>
#include <QCoreApplication>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "xlsxdocument.h"
#include "ModelColumnAliasConfirm.h"

static const QString GroupPathSetting = "PathSetting";
static QStringList DefaultInvalidHeaders = QStringList{"序号", "时间","PixelDistance","测试结果",
        "任务号" ,"工件号","NO" ,"Time","测试结果"};
static const QMap<Config::FileNames ,QString> MapFileNames = {
    std::pair<Config::FileNames ,QString>{Config::FileNames::ExcelRead,"ExcelRead"},
    std::pair<Config::FileNames ,QString>{Config::FileNames::ExcelWrite,"ExcelWrite"},
    std::pair<Config::FileNames ,QString>{Config::FileNames::XmlWrite,"XmlWrite"},
    std::pair<Config::FileNames ,QString>{Config::FileNames::TemplateExcel,"TemplateExcel"},
    std::pair<Config::FileNames ,QString>{Config::FileNames::XmlRead,"XmlRead"},
};
static const QMap<Config::PathType ,QString> MapPathNames = {
    std::pair<Config::PathType ,QString>{Config::PathType::OutPutPath,"OutPutPath"},
};


char* Config::QStringToChar(QString s)
{
    char* ptr;
    QByteArray ba;
    ba=s.toLatin1();
    ptr=ba.data();
    return ptr;
}

const QString Config::code(const QString &fullColumnName)
{
    return m_codeColumnName.value(fullColumnName,"");
}

const QString Config::deCode(const QString &shortCOlumnName)
{
    return m_codeColumnName.key(shortCOlumnName,"");
}

const QString Config::appPath()
{
    return QCoreApplication::applicationDirPath();
}


void Config::setConfig(QString qstrnodename,QString qstrkeyname,QVariant qvarvalue)
{
    m_setting.setValue(QString("%1/%2").arg(qstrnodename).arg(qstrkeyname), qvarvalue);
    m_setting.sync();
}

QVariant Config::getConfig(const QString nodeName, const QString keyName, const QString defaultValue)
{
    if(!m_setting.contains(QString("%1/%2").arg(nodeName).arg(keyName)))
        if(!defaultValue.isEmpty()){
            setConfig(nodeName ,keyName ,defaultValue);
            return defaultValue;
        }
    return m_setting.value(QString("%1/%2").arg(nodeName).arg(keyName));

}

const QString Config::getLastDirOpen(Config::FileNames type)
{
    return m_setting.value(QString("%1/%2").arg(GroupPathSetting).arg(MapFileNames.value(type))).toString();
}

void Config::setLastFileOpen(Config::FileNames type, const QString &filePath)
{
    auto tempCopy = filePath;
    auto dirPath = tempCopy.replace(filePath.split('/').last() ,"");
    setConfig(GroupPathSetting ,MapFileNames.value(type) ,dirPath);
    setFileName(type ,filePath);
}


const QString Config::getFileName(Config::FileNames name)
{
    return m_mapFileNames.value(name,"");
}

void Config::setFileName(Config::FileNames name, const QString &fileName)
{
    m_mapFileNames.insert(name ,fileName);
}

void Config::setPath(Config::PathType type, const QString &path)
{
    setConfig(GroupPathSetting , MapPathNames.value(type) ,path);
}

const QString Config::getPath(Config::PathType type)
{
    return getConfig(GroupPathSetting , MapPathNames.value(type)).toString();
}

int Config::getRowNoReadTemplateStart()
{
    return getConfig("Number","RowForSyncTemplate","0").toInt();
}

void Config::setRowNoReadTemplateStart(int no)
{
    setConfig("Number","RowForSyncTemplate",no);
}


Config &Config::getInstance()
{
    static Config instance;
    return instance;
}

Config::~Config()
{
    m_db.close();

}

void Config::registerAliasModel(ModelColumnAliasConfirm *model)
{
    m_modelAlias = model;
}

const QString Config::findFullName(const QString alias)
{
    if(m_resultColumns.contains(alias))
        return alias;
    auto findFullNameInDb = [&](const QString aliasName){
        QString sql = QString("SELECT `%1` FROM `%2` WHERE `%3`=`%4`").arg(DbColumnSetting.at(FULLNAME))
                .arg(ColumnMapTableName).arg(DbColumnSetting.at(ALIAS)).arg(aliasName);
        QSqlQuery query(m_db);
        if(query.exec(sql))
            if(query.next())
                return query.value(0).toString();
        return QString("");
    };
    auto fullNameInDb = findFullNameInDb(alias);
    return m_resultColumns.contains(fullNameInDb) ? fullNameInDb : "";

}

void Config::onTemplatePathChanged(const QString &templatePath)
{
    QXlsx::Document xlsReader(templatePath);
    m_templateColumns.clear();
    for(int i = 1 ; i < xlsReader.dimension().columnCount() +1 ; i ++){
        auto keyName = xlsReader.read(ExcelKeyNo ,i).toString();
        if(keyName.isEmpty())
            continue;
        if(!m_invalidHeaders.contains(keyName))
            m_templateColumns<<keyName;
    }
}

void Config::onMachineResultChanged(const QString &reusltPaht)
{
    QXlsx::Document xlsReader(reusltPaht);
    m_resultColumns.clear();
    for(int i = 1 ; i < xlsReader.dimension().columnCount() +1 ; i ++){
        auto keyName = xlsReader.read(ExcelKeyNo ,i).toString();
        if(keyName.isEmpty())
            continue;
        if(!m_invalidHeaders.contains(keyName))
            m_resultColumns<<keyName;
    }

}

bool Config::initColumnMapping()
{
    if(!(m_templateColumns.size() && m_resultColumns.size()))
        return false;
    if(!m_modelAlias)
        return false;
    m_modelAlias->init(m_templateColumns);
    return true;
}

const QStringList Config::getTemplateColumnSetting()
{
    return m_templateColumns;
}

const QStringList Config::getPreparedResultColumnSetting()
{
    return  m_modelAlias ? m_modelAlias->getPreparedResultColumnNames() : QStringList();
}

const QStringList &Config::getOrignalResultCoumnSetting()
{
    return m_resultColumns;
}

Config::Config() :m_setting("config.ini",QSettings::IniFormat),
    m_modelAlias(nullptr)
{
    m_dbPath = appPath() + "/" +SqlLiteFileName;
    m_db = QSqlDatabase::addDatabase("QSQLITE",QUuid::createUuid().toString());
    m_db.setDatabaseName(m_dbPath);
    if(!initDb()){
        QMessageBox::warning(nullptr ,"错误" ,QString("初始化数据库失败%1").arg(m_db.lastError().text()),"确定");
        exit(-1);
    }
    m_invalidHeaders = DefaultInvalidHeaders;
}
bool Config::initDb()
{
    auto formatSqlite = [&](){
        if(!m_db.open())
            return false;
        QSqlQuery query(m_db);
        QString sql =QString("\
        CREATE TABLE %1 (\
            `%2`	TEXT NOT NULL UNIQUE,\
            `%3`	TEXT NOT NULL,\
            PRIMARY KEY(`Alias`)\
        );").arg(ColumnMapTableName).arg(DbColumnSetting.at(ALIAS).arg(DbColumnSetting.at(FULLNAME)));
        if(!query.exec(sql))
            return false;
        return true;
    };
    QFile file(m_dbPath);
    if(!file.exists())
        return  formatSqlite();
    return m_db.open();
}
