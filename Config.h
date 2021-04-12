#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QSettings>
#include <QMap>
#include <unordered_map>

#define safeDelate(ptr) { if(ptr) delete ptr; ptr = nullptr; }

class Config
{
public:
    enum class FileNames{
        ExcelRead,
        ExcelWrite,
        XmlRead,
        XmlWrite,
        TemplateExcel,
    };
    enum class PathType{
        OutPutPath,
    };
    ~Config();
    bool isTemplateChanged(const QStringList& columnHeaders);
    static Config& getInstance();
    const QStringList getFullNameColumns();
    const QStringList getShortNameColumns();
    const QString getLastDirOpen(FileNames type);
    //Files
    void setLastFileOpen(FileNames type ,const QString& filePath);
    const QString getFileName(FileNames name);
    //Pathes
    void setPath(PathType ,const QString& path);
    const QString getPath(PathType type);
    //Number
    int getRowNoReadTemplateStart();
    void setRowNoReadTemplateStart(int no);

    char* QStringToChar(QString s);
    const QString code( const QString& fullColumnName);
    const QString deCode( const QString& shortCOlumnName );

private:
    Config();
    void setConfig(QString qstrnodename,QString qstrkeyname,QVariant qvarvalue);
    QVariant getConfig(const QString nodeName ,const QString keyName , const QString defaultValue = "");
    void setFileName(FileNames , const QString& fileName);
    QStringList measureResultHeaderTraits(const QString& measureResultPath);
    QSettings m_setting;
    QMap<FileNames,QString> m_mapFileNames;
    QHash<QString, QString> m_codeColumnName;
};

#endif // CONFIG_H
