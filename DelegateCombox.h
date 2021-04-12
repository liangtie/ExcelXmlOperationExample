#ifndef DELEGATECOMBOX_H
#define DELEGATECOMBOX_H

#include <QObject>
#include <QStyledItemDelegate>

class DelegateCombox : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DelegateCombox(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex	&index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const  QModelIndex &index) const;

signals:

};

#endif // DELEGATECOMBOX_H
