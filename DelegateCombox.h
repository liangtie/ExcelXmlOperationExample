#ifndef DELEGATECOMBOX_H
#define DELEGATECOMBOX_H

#include <QObject>
#include <QItemDelegate>

class DelegateCombox : public QObject
{
    Q_OBJECT
public:
    explicit DelegateCombox(QObject *parent = nullptr);

signals:

};

#endif // DELEGATECOMBOX_H
