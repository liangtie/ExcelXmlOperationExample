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

private:
    QtAxExcelEngine* m_readEngine;
    Ui::Widget *ui;
    std::promise<bool>* m_promise;
    std::promise<bool>* m_promiseXmlConfig;
    QMutex  m_excelMutex;
    QMutex m_xmlMutex;
    XmlModel* m_xmlModel;
    QVector<QPair<QString ,double>> m_valueForCalc;
    void resolveTemplate(const QString);
};
#endif // WIDGET_H
