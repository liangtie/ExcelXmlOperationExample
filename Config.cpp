#include "Config.h"
#include <QObject>
#include <QFile>
#include <QSettings>
#include <QMap>
#include <QDebug>

//Config node
static const QString g_nodePath = "PathSetting";
static const QString g_nodeColumn = "ColumnSetting";

//Config key
static const QString g_keyFullColumn = "SortedFullColumns";
static const QString g_keyAbbrColumn = "SortedAbbrColumns";

static const QStringList invalidHeaders = QStringList{"序号", "时间","PixelDistance","测试结果",
        "任务号" ,"工件号"};

//Deafaults
//简称是全称的子集，简称和全称从第一个元素开始一一对应，默认带有简称的列才是有效列
#if 0   //
static QString DefaultFullColumns = "FAI3-SE3-Y-distance	FAI5-SE3-Y-distance	FAI7-SE2-X-distance	FAI8-SE2-X-distance	FAI9-SE2-X-distance	FAI11-SS1-X-distance	FAI14-SE1-Y-distance	FAI20-RC-X1-distance	FAI20-RC-X2-distance	FAI20-RC-X3-distance	FAI20-RC-X4-distance	FAI22-SS1-position	FAI22-SS1-X-distance	FAI22-SS1-Y-distance	FAI22-SS2-position	FAI22-SS2-X-distance	FAI22-SS2-Y-distance	FAI22-SS3-position	FAI22-SS3-X-distance	FAI22-SS3-Y-distance	FAI22-SS4-position	FAI22-SS4-X-distance	FAI22-SS4-Y-distance	FAI22-SS5-position	FAI22-SS5-X-distance	FAI22-SS5-Y-distance	FAI22-SS6-position	FAI22-SS6-X-distance	FAI22-SS6-Y-distance	FAI36-SE7-Y1-distance	FAI36-SE7-Y2-distance	FAI36-SE7-Y3-distance	FAI37-SE7-Y1-distance	FAI37-SE7-Y2-distance	FAI37-SE7-Y3-distance	FAI37-SE7-Y4-distance	FAI38-SE7-Y1-distance	FAI38-SE7-Y2-distance	FAI38-SE7-Y3-distance	FAI38-SE7-Y4-distance	FAI44-SE1-Y1-distance	FAI44-SE1-Y2-distance	FAI47-SE2-X-distance	FAI48-SE2-X-distance	FAI49-SE5-Y-distance	FAI66-SE3-Y1-distance	FAI66-SE3-Y2-distance	FAI68-RC-Y1-distance	FAI68-RC-Y2-distance	FAI68-RC-Y3-distance	FAI68-RC-Y4-distance	FAI69-RC-X-position	FAI70-RC-Y-position	FAI72-SE4-X-distance	FAI73-D-diam	FAI74-D-position	FAI74-D-X-distance	FAI74-D-Y-distance	FAI75-L-diam	FAI76-L-position	FAI76-L-X-distance	FAI76-L-Y-distance	FAI69-RC-X-distance	FAI70-RC-Y-distance	FAI91-SS1-diam	FAI91-SS2-diam	FAI91-SS3-diam	FAI91-SS4-diam	FAI91-SS5-diam	FAI91-SS6-diam";
static QString DefaultAbbrColumns = "FAI3	FAI5	FAI7	FAI8	FAI9	FAI11	FAI14	FAI20.1	FAI20.2	FAI20.3	FAI20.4	FAI22.1	FAI22.1X	FAI22.1Y	FAI22.2	FAI22.2X	FAI22.2Y	FAI22.3	FAI22.3X	FAI22.3Y	FAI22.4	FAI22.4X	FAI22.4Y	FAI22.5	FAI22.5X	FAI22.5Y	FAI22.6	FAI22.6X	FAI22.6Y	FAI36.1	FAI36.2	FAI36.3	FAI37.1	FAI37.2	FAI37.3	FAI37.4	FAI38.1	FAI38.2	FAI38.3	FAI38.4	FAI44.1	FAI44.2	FAI47	FAI48	FAI49	FAI66.1	FAI66.2	FAI68.1	FAI68.2	FAI68.3	FAI68.4	FAI69	FAI70	FAI72	FAI73	FAI74	FAI74X	FAI74Y	FAI75	FAI76	FAI76X	FAI76Y";
#endif

//575
#if 0
static QString DefaultFullColumns = "FAI2-SE7-X-distance	FAI3-SE6-Y-distance	FAI4-SE3-SE6-Y-distance	FAI5-SE5-Y-distance	FAI6-SE1-SE9-Y-distance	FAI7-SE4-X-distance	FAI76-SS5-X-distance	FAI81-SS7-Y-distance	FAI82-SS2-Y-distance	FAI83-SS6-Y-distance	FAI84-SS8-Y-distance	FAI85-SS4-Y-distance	FAI88-SS2-X-distance	FAI89-SS6-X-distance	FAI90-SS4-X-distance	FAI91-SS8-X-distance	FAI92-SS3-X-distance	FAI209-SS7-X-distance	FAI210-SS5-Y-distance	FAI211-SS3-Y-distance	FAI27-S1-X-distance	FAI29-S1-Y-distance	FAI30-D-diam	FAI34-L-diam	FAI35-L-position	FAI35-L-X-distance	FAI35-L-Y-distance	FAI38-R-diam	FAI39-R-position	FAI39-R-X-distance	FAI39-R-Y-distance	FAI44-S1-X-parallelism	FAI45-S1-Y-parallelism	FAI46-D-position	FAI46-D-X-distance	FAI46-D-Y-distance	FAI47-D-roundness	FAI48-L-roundness	FAI49-R-roundness	FAI101-S1-Y1-distance	FAI101-S1-Y2-distance	FAI171-SS1-X-distance	FAI172-SS1-Y-distance	FAI183-LD-Y-distance	FAI185-LD-Y-distance	FAI186-LD-X-distance	FAI187-LD-X-distance	FAI188-LD-X-distance	FAI189-LD-X-distance	FAI198-S1-Y-distance	FAI227-S1-X-distance	FAI200-S2-Y-distance	FAI201-S2-X-distance	S2-diam	S1-diam	S8-diam	S3-diam	S7-diam	S6-diam	S4-diam	S5-diam	FAI232-SE1-SE2-Y-distance	Line-B-parallelism	Line-C-parallelism	Line-D-parallelism	Line-E-parallelism	Line-F-parallelism	Line-G-parallelism";
static QString DefaultAbbrColumns = "FAI2-SE7-X-distance	FAI3-SE6-Y-distance	FAI4-SE3-SE6-Y-distance	FAI5-SE5-Y-distance	FAI6-SE1-SE9-Y-distance	FAI7-SE4-X-distance	FAI76-SS5-X-distance	FAI81-SS7-Y-distance	FAI82-SS2-Y-distance	FAI83-SS6-Y-distance	FAI84-SS8-Y-distance	FAI85-SS4-Y-distance	FAI88-SS2-X-distance	FAI89-SS6-X-distance	FAI90-SS4-X-distance	FAI91-SS8-X-distance	FAI92-SS3-X-distance	FAI209-SS7-X-distance	FAI210-SS5-Y-distance	FAI211-SS3-Y-distance	FAI27-S1-X-distance	FAI29-S1-Y-distance	FAI30-D-diam	FAI34-L-diam	FAI35-L-position	FAI35-L-X-distance	FAI35-L-Y-distance	FAI38-R-diam	FAI39-R-position	FAI39-R-X-distance	FAI39-R-Y-distance	FAI44-S1-X-parallelism	FAI45-S1-Y-parallelism	FAI46-D-position	FAI46-D-X-distance	FAI46-D-Y-distance	FAI47-D-roundness	FAI48-L-roundness	FAI49-R-roundness	FAI101-S1-Y1-distance	FAI101-S1-Y2-distance	FAI171-SS1-X-distance	FAI172-SS1-Y-distance	FAI183-LD-Y-distance	FAI185-LD-Y-distance	FAI186-LD-X-distance	FAI187-LD-X-distance	FAI188-LD-X-distance	FAI189-LD-X-distance	FAI198-S1-Y-distance	FAI227-S1-X-distance	FAI200-S2-Y-distance	FAI201-S2-X-distance	S2-diam	S1-diam	S8-diam	S3-diam	S7-diam	S6-diam	S4-diam	S5-diam	FAI232-SE1-SE2-Y-distance	Line-B-parallelism	Line-C-parallelism	Line-D-parallelism	Line-E-parallelism	Line-F-parallelism	Line-G-parallelism";
//
#endif//575
#if 0
static QString  DefaultFullColumns ="FAI2-SE7-X-distance		FAI3-SE6-Y-distance		FAI4-SE3-SE6-Y-distance		FAI5-SE5-Y-distance		FAI6-SE1-SE9-Y-distance		FAI7-SE4-X-distance		FAI76-SS5-X-distance		FAI81-SS7-Y-distance		FAI82-SS2-Y-distance		FAI83-SS6-Y-distance		FAI84-SS8-Y-distance		FAI85-SS4-Y-distance		FAI88-SS2-X-distance		FAI89-SS6-X-distance		FAI90-SS4-X-distance		FAI91-SS8-X-distance		FAI92-SS3-X-distance		FAI209-SS7-X-distance		FAI210-SS5-Y-distance		FAI211-SS3-Y-distance		FAI27-S1-X-distance		FAI29-S1-Y-distance		FAI30-D-diam		FAI34-L-diam		FAI35-L-position		FAI35-L-X-distance		FAI35-L-Y-distance		FAI38-R-diam		FAI39-R-position		FAI39-R-X-distance		FAI39-R-Y-distance		FAI44-S1-X-parallelism		FAI45-S1-Y-parallelism		FAI46-D-position		FAI46-D-X-distance		FAI46-D-Y-distance		FAI47-D-roundness		FAI48-L-roundness		FAI49-R-roundness		FAI101-S1-Y1-distance		FAI101-S1-Y2-distance		FAI171-SS1-X-distance		FAI172-SS1-Y-distance		FAI183-LD-Y-distance		FAI185-LD-Y-distance		FAI186-LD-X-distance		FAI187-LD-X-distance		FAI188-LD-X-distance		FAI189-LD-X-distance		FAI198-S1-Y-distance		FAI227-S1-X-distance		FAI200-S2-Y-distance		FAI201-S2-X-distance		S2-diam		S1-diam		S8-diam		S3-diam		S7-diam		S6-diam		S4-diam		S5-diam		FAI232-SE1-SE2-Y-distance		Line-B-parallelism		Line-C-parallelism		Line-D-parallelism		Line-E-parallelism		Line-F-parallelism		Line-G-parallelism	";
static QString  DefaultAbbrColumns ="FAI2-SE7-X-distance		FAI3-SE6-Y-distance		FAI4-SE3-SE6-Y-distance		FAI5-SE5-Y-distance		FAI6-SE1-SE9-Y-distance		FAI7-SE4-X-distance		FAI76-SS5-X-distance		FAI81-SS7-Y-distance		FAI82-SS2-Y-distance		FAI83-SS6-Y-distance		FAI84-SS8-Y-distance		FAI85-SS4-Y-distance		FAI88-SS2-X-distance		FAI89-SS6-X-distance		FAI90-SS4-X-distance		FAI91-SS8-X-distance		FAI92-SS3-X-distance		FAI209-SS7-X-distance		FAI210-SS5-Y-distance		FAI211-SS3-Y-distance		FAI27-S1-X-distance		FAI29-S1-Y-distance		FAI30-D-diam		FAI34-L-diam		FAI35-L-position		FAI35-L-X-distance		FAI35-L-Y-distance		FAI38-R-diam		FAI39-R-position		FAI39-R-X-distance		FAI39-R-Y-distance		FAI44-S1-X-parallelism		FAI45-S1-Y-parallelism		FAI46-D-position		FAI46-D-X-distance		FAI46-D-Y-distance		FAI47-D-roundness		FAI48-L-roundness		FAI49-R-roundness		FAI101-S1-Y1-distance		FAI101-S1-Y2-distance		FAI171-SS1-X-distance		FAI172-SS1-Y-distance		FAI183-LD-Y-distance		FAI185-LD-Y-distance		FAI186-LD-X-distance		FAI187-LD-X-distance		FAI188-LD-X-distance		FAI189-LD-X-distance		FAI198-S1-Y-distance		FAI227-S1-X-distance		FAI200-S2-Y-distance		FAI201-S2-X-distance		S2-diam		S1-diam		S8-diam		S3-diam		S7-diam		S6-diam		S4-diam		S5-diam		FAI232-SE1-SE2-Y-distance		Line-B-parallelism		Line-C-parallelism		Line-D-parallelism		Line-E-parallelism		Line-F-parallelism		Line-G-parallelism	";
#endif//575

//static QString  DefaultAbbrColumns = "FAI2-SE7-X-distance	FAI3-SE6-Y-distance	FAI4-SE3-SE6-Y-distance	FAI5-SE5-Y-distance	FAI6-SE1-SE9-Y-distance	FAI7-SE4-X-distance	FAI76-SS5-X-distance	FAI81-SS7-Y-distance	FAI82-SS2-Y-distance	FAI83-SS6-Y-distance	FAI84-SS8-Y-distance	FAI85-SS4-Y-distance	FAI88-SS2-X-distance	FAI89-SS6-X-distance	FAI90-SS4-X-distance	FAI91-SS8-X-distance	FAI92-SS3-X-distance	FAI209-SS7-X-distance	FAI210-SS5-Y-distance	FAI211-SS3-Y-distance	FAI27-S1-X-distance	FAI29-S1-Y-distance	FAI30-D-diam	FAI34-L-diam	FAI35-L-position	FAI35-L-X-distance	FAI35-L-Y-distance	FAI38-R-diam	FAI39-R-position	FAI39-R-X-distance	FAI39-R-Y-distance	FAI44-S1-X-parallelism	FAI45-S1-Y-parallelism	FAI46-D-position	FAI46-D-X-distance	FAI46-D-Y-distance	FAI47-D-roundness	FAI48-L-roundness	FAI49-R-roundness	FAI101-S1-Y1-distance	FAI101-S1-Y2-distance	FAI171-SS1-X-distance	FAI172-SS1-Y-distance	FAI183-LD-Y-distance	FAI185-LD-Y-distance	FAI186-LD-X-distance	FAI187-LD-X-distance	FAI188-LD-X-distance	FAI189-LD-X-distance	FAI198-S1-Y-distance	FAI227-S1-X-distance	FAI200-S2-Y-distance	FAI201-S2-X-distance	S2-diam	S1-diam	S8-diam	S3-diam	S7-diam	S6-diam	S4-diam	S5-diam	FAI232-SE1-SE2-Y-distance	Line-B-parallelism	Line-C-parallelism	Line-D-parallelism	Line-E-parallelism	Line-F-parallelism	Line-G-parallelism";
//static QString  DefaultFullColumns = "FAI2-SE7-X-distance	FAI3-SE6-Y-distance	FAI4-SE3-SE6-Y-distance	FAI5-SE5-Y-distance	FAI6-SE1-SE9-Y-distance	FAI7-SE4-X-distance	FAI76-SS5-X-distance	FAI81-SS7-Y-distance	FAI82-SS2-Y-distance	FAI83-SS6-Y-distance	FAI84-SS8-Y-distance	FAI85-SS4-Y-distance	FAI88-SS2-X-distance	FAI89-SS6-X-distance	FAI90-SS4-X-distance	FAI91-SS8-X-distance	FAI92-SS3-X-distance	FAI209-SS7-X-distance	FAI210-SS5-Y-distance	FAI211-SS3-Y-distance	FAI27-S1-X-distance	FAI29-S1-Y-distance	FAI30-D-diam	FAI34-L-diam	FAI35-L-position	FAI35-L-X-distance	FAI35-L-Y-distance	FAI38-R-diam	FAI39-R-position	FAI39-R-X-distance	FAI39-R-Y-distance	FAI44-S1-X-parallelism	FAI45-S1-Y-parallelism	FAI46-D-position	FAI46-D-X-distance	FAI46-D-Y-distance	FAI47-D-roundness	FAI48-L-roundness	FAI49-R-roundness	FAI101-S1-Y1-distance	FAI101-S1-Y2-distance	FAI171-SS1-X-distance	FAI172-SS1-Y-distance	FAI183-LD-Y-distance	FAI185-LD-Y-distance	FAI186-LD-X-distance	FAI187-LD-X-distance	FAI188-LD-X-distance	FAI189-LD-X-distance	FAI198-S1-Y-distance	FAI227-S1-X-distance	FAI200-S2-Y-distance	FAI201-S2-X-distance	S2-diam	S1-diam	S8-diam	S3-diam	S7-diam	S6-diam	S4-diam	S5-diam	FAI232-SE1-SE2-Y-distance	Line-B-parallelism	Line-C-parallelism	Line-D-parallelism	Line-E-parallelism	Line-F-parallelism	Line-G-parallelism";

static QString  DefaultAbbrColumns = "FAI2-SE7-X-distance	FAI3-SE6-Y-distance	FAI4-SE3-SE6-Y-distance	FAI5-SE5-Y-distance	FAI6-SE1-SE9-Y-distance	FAI7-SE4-X-distance	FAI76-SS5-X-distance	FAI81-SS7-Y-distance	FAI82-SS2-Y-distance	FAI83-SS6-Y-distance	FAI84-SS8-Y-distance	FAI85-SS4-Y-distance	FAI88-SS2-X-distance	FAI89-SS6-X-distance	FAI90-SS4-X-distance	FAI91-SS8-X-distance	FAI92-SS3-X-distance	FAI209-SS7-X-distance	FAI210-SS5-Y-distance	FAI211-SS3-Y-distance	FAI27-S1-X-distance	FAI29-S1-Y-distance	FAI30-D-diam	FAI34-L-diam	FAI35-L-position	FAI35-L-X-distance	FAI35-L-Y-distance	FAI38-R-diam	FAI39-R-position	FAI39-R-X-distance	FAI39-R-Y-distance	FAI44-S1-X-parallelism	FAI45-S1-Y-parallelism	FAI46-D-position	FAI46-D-X-distance	FAI46-D-Y-distance	FAI47-D-roundness	FAI48-L-roundness	FAI49-R-roundness	FAI101-S1-Y1-distance	FAI101-S1-Y2-distance	FAI171-SS1-X-distance	FAI172-SS1-Y-distance	FAI183-LD-Y-distance	FAI185-LD-Y-distance	FAI186-LD-X-distance	FAI187-LD-X-distance	FAI188-LD-X-distance	FAI189-LD-X-distance	FAI198-S1-Y-distance	FAI227-S1-X-distance	FAI200-S2-Y-distance	FAI201-S2-X-distance	S2-diam	S1-diam	S8-diam	S3-diam	S7-diam	S6-diam	S4-diam	S5-diam	FAI232-SE1-SE2-Y-distance	Line-B-parallelism	Line-C-parallelism	Line-D-parallelism	Line-E-parallelism	Line-F-parallelism	Line-G-parallelism";
static QString  DefaultFullColumns = "FAI2-SE7-X-distance	FAI3-SE6-Y-distance	FAI4-SE3-SE6-Y-distance	FAI5-SE5-Y-distance	FAI6-SE1-SE9-Y-distance	FAI7-SE4-X-distance	FAI76-SS5-X-distance	FAI81-SS7-Y-distance	FAI82-SS2-Y-distance	FAI83-SS6-Y-distance	FAI84-SS8-Y-distance	FAI85-SS4-Y-distance	FAI88-SS2-X-distance	FAI89-SS6-X-distance	FAI90-SS4-X-distance	FAI91-SS8-X-distance	FAI92-SS3-X-distance	FAI209-SS7-X-distance	FAI210-SS5-Y-distance	FAI211-SS3-Y-distance	FAI27-S1-X-distance	FAI29-S1-Y-distance	FAI30-D-diam	FAI34-L-diam	FAI35-L-position	FAI35-L-X-distance	FAI35-L-Y-distance	FAI38-R-diam	FAI39-R-position	FAI39-R-X-distance	FAI39-R-Y-distance	FAI44-S1-X-parallelism	FAI45-S1-Y-parallelism	FAI46-D-position	FAI46-D-X-distance	FAI46-D-Y-distance	FAI47-D-roundness	FAI48-L-roundness	FAI49-R-roundness	FAI101-S1-Y1-distance	FAI101-S1-Y2-distance	FAI171-SS1-X-distance	FAI172-SS1-Y-distance	FAI183-LD-Y-distance	FAI185-LD-Y-distance	FAI186-LD-X-distance	FAI187-LD-X-distance	FAI188-LD-X-distance	FAI189-LD-X-distance	FAI198-S1-Y-distance	FAI227-S1-X-distance	FAI200-S2-Y-distance	FAI201-S2-X-distance	S2-diam	S1-diam	S8-diam	S3-diam	S7-diam	S6-diam	S4-diam	S5-diam	FAI232-SE1-SE2-Y-distance	Line-B-parallelism	Line-C-parallelism	Line-D-parallelism	Line-E-parallelism	Line-F-parallelism	Line-G-parallelism";
static const QMap<Config::FileNames ,QString> g_mapFileS = {
    std::pair<Config::FileNames ,QString>{Config::FileNames::ExcelRead,"ExcelRead"},
    std::pair<Config::FileNames ,QString>{Config::FileNames::ExcelWrite,"ExcelWrite"},
    std::pair<Config::FileNames ,QString>{Config::FileNames::XmlWrite,"XmlWrite"},
    std::pair<Config::FileNames ,QString>{Config::FileNames::TemplateExcel,"TemplateExcel"},
    std::pair<Config::FileNames ,QString>{Config::FileNames::XmlRead,"XmlRead"},
};

static const QMap<Config::PathType ,QString> g_mapPathS = {
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
    return m_setting.value(QString("%1/%2").arg(g_nodePath).arg(g_mapFileS.value(type))).toString();
}

void Config::setLastFileOpen(Config::FileNames type, const QString &filePath)
{
    auto tempCopy = filePath;
    auto dirPath = tempCopy.replace(filePath.split('/').last() ,"");
    setConfig(g_nodePath ,g_mapFileS.value(type) ,dirPath);
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

QStringList Config::measureResultHeaderTraits(const QString &measureResultPath)
{

}

void Config::setPath(Config::PathType type, const QString &path)
{
    setConfig(g_nodePath , g_mapPathS.value(type) ,path);
}

const QString Config::getPath(Config::PathType type)
{
    return getConfig(g_nodePath , g_mapPathS.value(type)).toString();
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

const QStringList Config::getShortNameColumns()
{
    auto columns = getConfig(g_nodeColumn ,g_keyAbbrColumn).toString();
    if(columns.isEmpty()){
        columns = DefaultAbbrColumns;
        setConfig(g_nodeColumn ,g_keyAbbrColumn ,DefaultAbbrColumns);
    }
    return columns.split('\t');

}

const QStringList Config::getFullNameColumns()
{
    auto columns = getConfig(g_nodeColumn ,g_keyFullColumn).toString();
    if(columns.isEmpty()){
        columns = DefaultFullColumns;
        setConfig(g_nodeColumn ,g_keyFullColumn ,DefaultFullColumns);
    }
    return columns.split('\t');

}

Config::~Config()
{

}

bool Config::isTemplateChanged(const QStringList &columnHeaders)
{

}

Config::Config() :m_setting("config.ini",QSettings::IniFormat)
{
    QStringList targetFullNameColumns ,targetShortNameColumns;
    for(auto i : getFullNameColumns())
        targetFullNameColumns<<i.trimmed();
    for(auto i : getShortNameColumns())
        targetShortNameColumns<<i.trimmed();
    if(targetFullNameColumns.size() >= targetShortNameColumns.size())
        for(int i = 0 ; i < targetShortNameColumns.size() ; i ++)
            m_codeColumnName.insert(targetFullNameColumns.at(i) , targetShortNameColumns.at(i));

}
