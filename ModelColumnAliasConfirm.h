#ifndef MODELCOLUMNALIASCONFIRM_H
#define MODELCOLUMNALIASCONFIRM_H

#include <QAbstractTableModel>
#include <QLineEdit>
#include <memory>

class AliasItem : public QObject{
    Q_OBJECT
public:
    AliasItem(const QString& aliasName);
    ~AliasItem();
    const QString& alias();
    const QString& fullName();
    bool isFullNameEmpty();
public slots:
    void onFullNameEdited(const QString&);
private:
    QString m_alias;
    QString m_fullName;

};

Q_DECLARE_METATYPE(std::shared_ptr<AliasItem>);

class ModelColumnAliasConfirm : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum CoumnSetting{
        ColumnNo,
        ColumnAlias,
        ColumnFullName,
        ColumnCount,
    };
    enum DataRoles{
        Role_AliasPtr = Qt::UserRole +1 ,
    };

    explicit ModelColumnAliasConfirm(QObject *parent = nullptr);

    void init(const QStringList& ordereTemplateNames );

    bool isColumnMappingReady();


    QStringList getPreparedResultColumnNames();

    const QHash<QString ,QString>& getColumnNameMapping();

    /**
     * @brief getTemplateColumnNames  , note : template's meaning is the same to the alias
     * @return
     */
    QStringList getTemplateColumnNames();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    QMap<int , std::shared_ptr<AliasItem>> m_items;
    QHash<QString ,QString> m_mapAliasToFullName;
signals:
    void updateStatus(const QString&);


};

#endif // MODELCOLUMNALIASCONFIRM_H
