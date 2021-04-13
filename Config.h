#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QSettings>
#include <QMap>
#include <QSqlDatabase>

#define safeDelate(ptr) { if(ptr) delete ptr; ptr = nullptr; }

struct XmlWriteSetting{
    bool negative;
    int times;
};

class ModelColumnAliasConfirm;
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
    enum DBColumns{
        ALIAS,
        FULLNAME,
    };
    enum {
        ExcelKeyNo = 1,
    };

    ~Config();
    const XmlWriteSetting& xmlWriteConfig();
    void enableXmlWriteNegative(bool isNegative);
    void setXmlWriteTimes(int times);
    /**
     * @brief findAliasFullName
     * @param alias
     * @return alias or fullName if they are the same
     */
    void registerAliasModel(ModelColumnAliasConfirm* model);
    /**
     * @brief findFullName look up in the database
     * @param alias
     * @return
     */
    const QString findFullName(const QString alias);
    /**
     * @brief updateAlias Update the database column name mapping
     * @param alias
     * @param fullName
     * @return
     */
    bool updateAlias(const QString& alias , const QString& fullName);

    void onTemplatePathChanged(const QString& templatePath);
    void onMachineResultChanged(const QString& reusltPaht);
    bool initColumnMapping();
    /**
     * @brief getTemplateColumnSetting
     * @return there is only one template column setting everytime
     */
    const QStringList getTemplateColumnSetting();
    /**
     * @brief getOrignalResultCoumnSetting
     * @return the original column names in the result excel
     */
    const QStringList& getOrignalResultCoumnSetting();

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

    static Config& getInstance();
    const QString fullColumnNameToAlias( const QString& fullColumnName);
    const QString aliasToFullName( const QString& shortCOlumnName );
    const QString appPath();
    static QString htmlColorText(const QString content ,const QString color);

private:
    QSettings m_setting;
    QMap<FileNames,QString> m_mapFileNames;
    ModelColumnAliasConfirm* m_modelAlias;
    XmlWriteSetting m_xmlWriteConfig;
    QSqlDatabase m_db;
    const QString SqlLiteFileName = "ColumnMap.db";
    const QString ColumnMapTableName = "ColumnMapping";
    const QStringList DbColumnSetting = QStringList{"Alias" ,"FullName" };
    QStringList m_invalidHeaders;
    QString m_dbPath;
    /**
     * @brief m_templateColumns columns in the template excel
     */
    QStringList m_templateColumns;
    /**
     * @brief m_resultColumns columns in the result excel from the mechine
     */
    QStringList m_resultColumns;
    Config();
    bool initDb();
    void setConfig(QString qstrnodename,QString qstrkeyname,QVariant qvarvalue);
    QVariant getConfig(const QString nodeName ,const QString keyName , const QString defaultValue = "");
    void setFileName(FileNames , const QString& fileName);
    char* QStringToChar(QString s);


};

#endif // CONFIG_H
