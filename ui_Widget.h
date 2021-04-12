/********************************************************************************
** Form generated from reading UI file 'Widget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QHBoxLayout *layMain;
    QWidget *widgetLeft;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *boxOutPut;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_2;
    QPushButton *btnRead;
    QLineEdit *editRead;
    QPushButton *btnTempletePath;
    QLineEdit *editTempletePath;
    QPushButton *btnConfirm;
    QGroupBox *boxExcel;
    QVBoxLayout *verticalLayout_3;
    QWidget *widgetExcelConfig;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QSpinBox *spinRowStartRead;
    QSpinBox *spinRowEndRead;
    QLabel *label_6;
    QLabel *label_3;
    QPushButton *btnStart;
    QGroupBox *boxXml;
    QVBoxLayout *verticalLayout_5;
    QGridLayout *gridLayout_3;
    QSpinBox *spinRowReadTemp;
    QLabel *label_4;
    QLineEdit *editXmlPath;
    QPushButton *btnXmlPath;
    QPushButton *btnXmlStart;
    QSpacerItem *verticalSpacer_2;
    QLineEdit *editWrite;
    QPushButton *btnWrite;
    QTableView *tableView;
    QTreeView *treeView;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(873, 600);
        layMain = new QHBoxLayout(Widget);
        layMain->setSpacing(0);
        layMain->setObjectName(QStringLiteral("layMain"));
        widgetLeft = new QWidget(Widget);
        widgetLeft->setObjectName(QStringLiteral("widgetLeft"));
        verticalLayout_4 = new QVBoxLayout(widgetLeft);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        boxOutPut = new QGroupBox(widgetLeft);
        boxOutPut->setObjectName(QStringLiteral("boxOutPut"));
        verticalLayout = new QVBoxLayout(boxOutPut);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        btnRead = new QPushButton(boxOutPut);
        btnRead->setObjectName(QStringLiteral("btnRead"));

        gridLayout_2->addWidget(btnRead, 0, 0, 1, 1);

        editRead = new QLineEdit(boxOutPut);
        editRead->setObjectName(QStringLiteral("editRead"));

        gridLayout_2->addWidget(editRead, 0, 1, 1, 1);

        btnTempletePath = new QPushButton(boxOutPut);
        btnTempletePath->setObjectName(QStringLiteral("btnTempletePath"));

        gridLayout_2->addWidget(btnTempletePath, 1, 0, 1, 1);

        editTempletePath = new QLineEdit(boxOutPut);
        editTempletePath->setObjectName(QStringLiteral("editTempletePath"));

        gridLayout_2->addWidget(editTempletePath, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout_2);

        btnConfirm = new QPushButton(boxOutPut);
        btnConfirm->setObjectName(QStringLiteral("btnConfirm"));

        verticalLayout->addWidget(btnConfirm);


        verticalLayout_4->addWidget(boxOutPut);

        boxExcel = new QGroupBox(widgetLeft);
        boxExcel->setObjectName(QStringLiteral("boxExcel"));
        verticalLayout_3 = new QVBoxLayout(boxExcel);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        widgetExcelConfig = new QWidget(boxExcel);
        widgetExcelConfig->setObjectName(QStringLiteral("widgetExcelConfig"));
        verticalLayout_2 = new QVBoxLayout(widgetExcelConfig);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        spinRowStartRead = new QSpinBox(widgetExcelConfig);
        spinRowStartRead->setObjectName(QStringLiteral("spinRowStartRead"));
        spinRowStartRead->setValue(0);

        gridLayout->addWidget(spinRowStartRead, 0, 1, 1, 1);

        spinRowEndRead = new QSpinBox(widgetExcelConfig);
        spinRowEndRead->setObjectName(QStringLiteral("spinRowEndRead"));
        spinRowEndRead->setValue(0);

        gridLayout->addWidget(spinRowEndRead, 1, 1, 1, 1);

        label_6 = new QLabel(widgetExcelConfig);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 1, 0, 1, 1);

        label_3 = new QLabel(widgetExcelConfig);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 0, 0, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        btnStart = new QPushButton(widgetExcelConfig);
        btnStart->setObjectName(QStringLiteral("btnStart"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnStart->sizePolicy().hasHeightForWidth());
        btnStart->setSizePolicy(sizePolicy);
        btnStart->setMinimumSize(QSize(0, 0));

        verticalLayout_2->addWidget(btnStart);


        verticalLayout_3->addWidget(widgetExcelConfig);


        verticalLayout_4->addWidget(boxExcel);

        boxXml = new QGroupBox(widgetLeft);
        boxXml->setObjectName(QStringLiteral("boxXml"));
        verticalLayout_5 = new QVBoxLayout(boxXml);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        spinRowReadTemp = new QSpinBox(boxXml);
        spinRowReadTemp->setObjectName(QStringLiteral("spinRowReadTemp"));
        spinRowReadTemp->setMaximum(120);
        spinRowReadTemp->setValue(42);

        gridLayout_3->addWidget(spinRowReadTemp, 1, 1, 1, 1);

        label_4 = new QLabel(boxXml);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_3->addWidget(label_4, 1, 0, 1, 1);

        editXmlPath = new QLineEdit(boxXml);
        editXmlPath->setObjectName(QStringLiteral("editXmlPath"));

        gridLayout_3->addWidget(editXmlPath, 0, 1, 1, 1);

        btnXmlPath = new QPushButton(boxXml);
        btnXmlPath->setObjectName(QStringLiteral("btnXmlPath"));

        gridLayout_3->addWidget(btnXmlPath, 0, 0, 1, 1);


        verticalLayout_5->addLayout(gridLayout_3);

        btnXmlStart = new QPushButton(boxXml);
        btnXmlStart->setObjectName(QStringLiteral("btnXmlStart"));
        sizePolicy.setHeightForWidth(btnXmlStart->sizePolicy().hasHeightForWidth());
        btnXmlStart->setSizePolicy(sizePolicy);
        btnXmlStart->setMinimumSize(QSize(0, 0));

        verticalLayout_5->addWidget(btnXmlStart);


        verticalLayout_4->addWidget(boxXml);

        verticalSpacer_2 = new QSpacerItem(20, 103, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);

        editWrite = new QLineEdit(widgetLeft);
        editWrite->setObjectName(QStringLiteral("editWrite"));

        verticalLayout_4->addWidget(editWrite);

        btnWrite = new QPushButton(widgetLeft);
        btnWrite->setObjectName(QStringLiteral("btnWrite"));

        verticalLayout_4->addWidget(btnWrite);


        layMain->addWidget(widgetLeft);

        tableView = new QTableView(Widget);
        tableView->setObjectName(QStringLiteral("tableView"));

        layMain->addWidget(tableView);

        treeView = new QTreeView(Widget);
        treeView->setObjectName(QStringLiteral("treeView"));

        layMain->addWidget(treeView);

        layMain->setStretch(0, 1);
        layMain->setStretch(1, 2);
        layMain->setStretch(2, 2);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        boxOutPut->setTitle(QApplication::translate("Widget", "\345\277\205\345\241\253\351\205\215\347\275\256", Q_NULLPTR));
        btnRead->setText(QApplication::translate("Widget", "\351\200\211\346\213\251\346\234\272\345\217\260\347\273\223\346\236\234Excel", Q_NULLPTR));
        btnTempletePath->setText(QApplication::translate("Widget", "\351\200\211\346\213\251\350\247\206\350\247\211\346\250\241\346\235\277Excel", Q_NULLPTR));
        btnConfirm->setText(QApplication::translate("Widget", "\351\207\217\346\265\213\351\241\271\347\233\256\346\240\270\345\257\271(\345\277\205\351\241\273)", Q_NULLPTR));
        boxExcel->setTitle(QApplication::translate("Widget", "\346\234\272\345\217\260\347\273\223\346\236\234\345\257\274\345\207\272\350\256\276\347\275\256", Q_NULLPTR));
        label_6->setText(QApplication::translate("Widget", "\350\257\273\345\210\260\347\254\254\345\207\240\350\241\214\347\273\223\346\235\237?", Q_NULLPTR));
        label_3->setText(QApplication::translate("Widget", "\344\273\216\347\254\254\345\207\240\350\241\214\345\274\200\345\247\213\350\257\273\345\217\226?", Q_NULLPTR));
        btnStart->setText(QApplication::translate("Widget", "\345\274\200\345\247\213Excel\345\257\274\345\207\272", Q_NULLPTR));
        boxXml->setTitle(QApplication::translate("Widget", "Xml\351\205\215\347\275\256\345\220\214\346\255\245\350\256\276\347\275\256", Q_NULLPTR));
        label_4->setText(QApplication::translate("Widget", "\345\220\214\346\255\245\350\247\206\350\247\211\346\250\241\346\235\277\347\254\254\345\207\240\350\241\214?", Q_NULLPTR));
        btnXmlPath->setText(QApplication::translate("Widget", "\351\200\211\346\213\251Xml\351\205\215\347\275\256\346\226\207\344\273\266", Q_NULLPTR));
        btnXmlStart->setText(QApplication::translate("Widget", "\345\274\200\345\247\213Xml\345\206\231\345\205\245", Q_NULLPTR));
        btnWrite->setText(QApplication::translate("Widget", "\351\200\211\346\213\251\350\276\223\345\207\272\347\233\256\345\275\225\350\267\257\345\276\204", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
