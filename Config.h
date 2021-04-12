#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QSettings>
#include <QMap>
#include <QSqlDatabase>

#define safeDelate(ptr) { if(ptr) delete ptr; ptr = nullptr; }

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
    /**
     * @brief findAliasFullName
     * @param alias
     * @return alias or fullName if they are the same
     */
    void registerAliasModel(ModelColumnAliasConfirm* model);
    const QString findFullName(const QString alias);
    void onTemplatePathChanged(const QString& templatePath);
    void onMachineResultChanged(const QString& reusltPaht);
    bool initColumnMapping();
    /**
     * @brief getTemplateColumnSetting
     * @return there is only one template column setting everytime
     */
    const QStringList getTemplateColumnSetting();
    /**
     * @brief getPreparedResultColumnSetting
     * @return  the prepared column setting from the model
     */
    const QStringList getPreparedResultColumnSetting();
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
    char* QStringToChar(QString s);
    const QString code( const QString& fullColumnName);
    const QString deCode( const QString& shortCOlumnName );
    const QString appPath();

private:
    QSettings m_setting;
    QMap<FileNames,QString> m_mapFileNames;
    ModelColumnAliasConfirm* m_modelAlias;
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
    /**
     * @brief m_codeColumnName Mapping the alias to standard names from the resultColumns
     */
    QHash<QString, QString> m_codeColumnName;
    Config();
    bool initDb();
    void setConfig(QString qstrnodename,QString qstrkeyname,QVariant qvarvalue);
    QVariant getConfig(const QString nodeName ,const QString keyName , const QString defaultValue = "");
    void setFileName(FileNames , const QString& fileName);

};

#endif // CONFIG_H
