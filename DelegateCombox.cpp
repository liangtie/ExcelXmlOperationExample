#include "DelegateCombox.h"
#include "Config.h"
#include <QComboBox>
#include <QStyleOptionComboBox>
#include <QTableView>
#include "ModelColumnAliasConfirm.h"

DelegateCombox::DelegateCombox(QObject *parent) : QStyledItemDelegate(parent)
{

}

QWidget *DelegateCombox::createEditor(QWidget *parent,const QStyleOptionViewItem &option ,const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->addItems(Config::getInstance().getOrignalResultCoumnSetting());
    editor->installEventFilter(const_cast<DelegateCombox*>(this));
    return editor;
}

void DelegateCombox::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //TODO
    return QStyledItemDelegate::paint(painter ,option , index);
}

void DelegateCombox::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    QString str =index.model()->data(index).toString();
    QComboBox *box = static_cast<QComboBox*>(editor);
    int i=box->findText(str);
    box->setCurrentIndex(i);
}

void DelegateCombox::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *box = static_cast<QComboBox*>(editor);
    QString str = box->currentText();
    model->setData(index,str);
}

void DelegateCombox::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}

