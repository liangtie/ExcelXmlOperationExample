#include "ModelColumnAliasConfirm.h"
#include "Config.h"

ModelColumnAliasConfirm::ModelColumnAliasConfirm(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void ModelColumnAliasConfirm::init(const QStringList &ordereTemplateNames)
{
    beginResetModel();
    m_items.clear();
    for(int i = 0 ; i < ordereTemplateNames.size() ; i ++)
        m_items.insert(i , std::make_shared<AliasItem>(ordereTemplateNames.at(i)) );
    endResetModel();
}

QStringList ModelColumnAliasConfirm::getPreparedResultColumnNames()
{
    QStringList fullNames;
    if(!isColumnMappingReady())
        return  fullNames;
    for(auto i : m_items)
        fullNames<<i->fullName();
    return fullNames;

}

QStringList ModelColumnAliasConfirm::getTemplateColumnNames()
{
    QStringList alias;
    for(auto i : m_items)
        alias<<i->alias();
    return alias;
}

bool ModelColumnAliasConfirm::isColumnMappingReady()
{
    for(auto ll = m_items.begin() ; ll != m_items.end() ; ++ll)
        if(ll.value()->isFullNameEmpty())
            return false;
    return true;
}

QVariant ModelColumnAliasConfirm::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(Qt::Horizontal == orientation){
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
        {
            switch (section) {
            case ColumnNo:
                return "模板序号";
            case ColumnAlias:
                return "模板列名";
            case ColumnFullName:
                return "机台列名";
            }
        }
        }

    }
    return QVariant();

}

int ModelColumnAliasConfirm::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_items.size();
}

int ModelColumnAliasConfirm::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return  ColumnCount;
}

QVariant ModelColumnAliasConfirm::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    switch (role) {
    case Qt::DisplayRole :
    case Qt::EditRole:
    {
        switch (index.column()) {
        case ColumnNo:
            return index.row() +1;
        case ColumnAlias:
            return m_items.value(index.row())->alias();
        case ColumnFullName:
            return m_items.value(index.row())->fullName();

        }
    }
    }

    return QVariant();
}

bool ModelColumnAliasConfirm::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags ModelColumnAliasConfirm::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    Qt::ItemFlags  flags{ Qt::ItemIsEditable,Qt::ItemIsSelectable ,Qt::ItemIsEnabled};
    return flags;
}

bool ModelColumnAliasConfirm::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();
}

bool ModelColumnAliasConfirm::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    endInsertColumns();
}

bool ModelColumnAliasConfirm::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    endRemoveRows();
}

bool ModelColumnAliasConfirm::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    endRemoveColumns();
}

AliasItem::AliasItem(const QString &aliasName):
    m_alias(aliasName)
{
    m_fullName = Config::getInstance().findFullName(m_alias);
}

AliasItem::~AliasItem()
{

}

const QString &AliasItem::alias(){
    return m_alias;
}

const QString &AliasItem::fullName()
{
    return m_fullName;
}

bool AliasItem::isFullNameEmpty()
{
    return m_fullName.isEmpty();
}

void AliasItem::onFullNameEdited(const QString &fullName)
{
    m_fullName = fullName;
    emit onAliasConfirmed(m_alias , m_fullName);
}

