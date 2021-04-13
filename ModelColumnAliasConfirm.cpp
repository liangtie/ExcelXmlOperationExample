#include "ModelColumnAliasConfirm.h"
#include "Config.h"
#include <QMessageBox>

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
    isColumnMappingReady();
}

void ModelColumnAliasConfirm::onTemplateOrResultPathChanged()
{
    m_items.clear();
    isColumnMappingReady();
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

const QHash<QString, QString> &ModelColumnAliasConfirm::getColumnNameMapping()
{
    return m_mapAliasToFullName;
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
    if(!m_items.size()){
        emit updateStatus(Config::htmlColorText( "未核对量测项目" ,"#ff0000"));
        return false;
    }
    m_mapAliasToFullName.clear();
    for(auto ll = m_items.begin() ; ll != m_items.end() ; ++ll){
        m_mapAliasToFullName.insert(ll.value()->alias() , ll.value()->fullName());
        if(ll.value()->isFullNameEmpty()){
            m_mapAliasToFullName.clear();
            emit updateStatus(Config::htmlColorText( "有未映射的量测项目" ,"#ff0000"));
            return false;
        }
    }
    emit updateStatus(Config::htmlColorText( "没有未映射的量测项目" ,"#00ff00"));
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
    if(Qt::BackgroundRole == role && m_items.value(index.row())->fullName().isEmpty()){
        return QColor(255, 0, 0);
    }
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
        if(ColumnFullName == index.column())
            switch (role) {
            case Qt::DisplayRole :
            case Qt::EditRole:
                m_items.value(index.row())->onFullNameEdited(value.toString());
                emit dataChanged(index, index, QVector<int>() << role);
                auto holeRow = this->index(index.row() ,0);
                emit dataChanged(holeRow , index, QVector<int>() << Qt::BackgroundRole);
                isColumnMappingReady();
                return true;
            }
    }
    return false;
}

Qt::ItemFlags ModelColumnAliasConfirm::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    Qt::ItemFlags  flags{ Qt::ItemIsSelectable ,Qt::ItemIsEnabled};
    if(ColumnFullName == index.column())
        flags |= Qt::ItemIsEditable;
    return flags;
}

bool ModelColumnAliasConfirm::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();
    return true;
}

bool ModelColumnAliasConfirm::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    endInsertColumns();
    return true;
}

bool ModelColumnAliasConfirm::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    endRemoveRows();
    return true;
}

bool ModelColumnAliasConfirm::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    endRemoveColumns();
    return true;
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
    Config::getInstance().updateAlias(m_alias ,m_fullName);
}

