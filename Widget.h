#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <future>
#include <QMutex>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class QtAxExcelEngine;
class XmlModel;
class ModelColumnAliasConfirm;
namespace QXlsx{
class Document;
}
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btnStart_clicked();
    void on_btnRead_clicked();
    void on_btnWrite_clicked();
    void on_btnXmlPath_clicked();
    void on_btnXmlStart_clicked();
    void on_btnTempletePath_clicked();
    void on_btnConfirm_clicked();

private:
    Ui::Widget *ui;
    XmlModel* m_modelXmlConfirm;
    ModelColumnAliasConfirm* m_modelAliasConfirm;
    QVector<QPair<QString ,double>> m_valueForCalc;
    void resolveAjustedValueInTemplate(const QString);
};
#endif // WIDGET_H
