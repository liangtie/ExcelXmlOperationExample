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
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *editWrite;
    QPushButton *btnWrite;
    QSpacerItem *verticalSpacer_3;
    QGroupBox *boxExcel;
    QHBoxLayout *horizontalLayout;
    QWidget *widgetExcelConfig;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QSpinBox *spinRowEndRead;
    QLineEdit *editRead;
    QSpinBox *spinRowStartRead;
    QPushButton *btnRead;
    QLabel *label_6;
    QLabel *label_3;
    QPushButton *btnStart;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *boxXml;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout_3;
    QLineEdit *editXmlPath;
    QPushButton *btnTempletePath;
    QPushButton *btnXmlPath;
    QLineEdit *editTempletePath;
    QLabel *label_4;
    QSpinBox *spinRowReadTemp;
    QPushButton *btnXmlStart;
    QTreeView *treeView;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(689, 541);
        layMain = new QHBoxLayout(Widget);
        layMain->setObjectName(QStringLiteral("layMain"));
        widgetLeft = new QWidget(Widget);
        widgetLeft->setObjectName(QStringLiteral("widgetLeft"));
        verticalLayout_4 = new QVBoxLayout(widgetLeft);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        boxOutPut = new QGroupBox(widgetLeft);
        boxOutPut->setObjectName(QStringLiteral("boxOutPut"));
        horizontalLayout_3 = new QHBoxLayout(boxOutPut);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        editWrite = new QLineEdit(boxOutPut);
        editWrite->setObjectName(QStringLiteral("editWrite"));

        horizontalLayout_3->addWidget(editWrite);

        btnWrite = new QPushButton(boxOutPut);
        btnWrite->setObjectName(QStringLiteral("btnWrite"));

        horizontalLayout_3->addWidget(btnWrite);


        verticalLayout_4->addWidget(boxOutPut);

        verticalSpacer_3 = new QSpacerItem(20, 103, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_3);

        boxExcel = new QGroupBox(widgetLeft);
        boxExcel->setObjectName(QStringLiteral("boxExcel"));
        horizontalLayout = new QHBoxLayout(boxExcel);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        widgetExcelConfig = new QWidget(boxExcel);
        widgetExcelConfig->setObjectName(QStringLiteral("widgetExcelConfig"));
        verticalLayout_2 = new QVBoxLayout(widgetExcelConfig);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        spinRowEndRead = new QSpinBox(widgetExcelConfig);
        spinRowEndRead->setObjectName(QStringLiteral("spinRowEndRead"));
        spinRowEndRead->setValue(0);

        gridLayout->addWidget(spinRowEndRead, 2, 1, 1, 1);

        editRead = new QLineEdit(widgetExcelConfig);
        editRead->setObjectName(QStringLiteral("editRead"));

        gridLayout->addWidget(editRead, 0, 1, 1, 1);

        spinRowStartRead = new QSpinBox(widgetExcelConfig);
        spinRowStartRead->setObjectName(QStringLiteral("spinRowStartRead"));
        spinRowStartRead->setValue(0);

        gridLayout->addWidget(spinRowStartRead, 1, 1, 1, 1);

        btnRead = new QPushButton(widgetExcelConfig);
        btnRead->setObjectName(QStringLiteral("btnRead"));

        gridLayout->addWidget(btnRead, 0, 0, 1, 1);

        label_6 = new QLabel(widgetExcelConfig);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 2, 0, 1, 1);

        label_3 = new QLabel(widgetExcelConfig);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);


        verticalLayout_2->addLayout(gridLayout);


        horizontalLayout->addWidget(widgetExcelConfig);

        btnStart = new QPushButton(boxExcel);
        btnStart->setObjectName(QStringLiteral("btnStart"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnStart->sizePolicy().hasHeightForWidth());
        btnStart->setSizePolicy(sizePolicy);
        btnStart->setMinimumSize(QSize(120, 120));

        horizontalLayout->addWidget(btnStart);


        verticalLayout_4->addWidget(boxExcel);

        verticalSpacer_2 = new QSpacerItem(20, 103, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);

        boxXml = new QGroupBox(widgetLeft);
        boxXml->setObjectName(QStringLiteral("boxXml"));
        horizontalLayout_2 = new QHBoxLayout(boxXml);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        editXmlPath = new QLineEdit(boxXml);
        editXmlPath->setObjectName(QStringLiteral("editXmlPath"));

        gridLayout_3->addWidget(editXmlPath, 0, 1, 1, 1);

        btnTempletePath = new QPushButton(boxXml);
        btnTempletePath->setObjectName(QStringLiteral("btnTempletePath"));

        gridLayout_3->addWidget(btnTempletePath, 2, 0, 1, 1);

        btnXmlPath = new QPushButton(boxXml);
        btnXmlPath->setObjectName(QStringLiteral("btnXmlPath"));

        gridLayout_3->addWidget(btnXmlPath, 0, 0, 1, 1);

        editTempletePath = new QLineEdit(boxXml);
        editTempletePath->setObjectName(QStringLiteral("editTempletePath"));

        gridLayout_3->addWidget(editTempletePath, 2, 1, 1, 1);

        label_4 = new QLabel(boxXml);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_3->addWidget(label_4, 1, 0, 1, 1);

        spinRowReadTemp = new QSpinBox(boxXml);
        spinRowReadTemp->setObjectName(QStringLiteral("spinRowReadTemp"));
        spinRowReadTemp->setMaximum(120);
        spinRowReadTemp->setValue(42);

        gridLayout_3->addWidget(spinRowReadTemp, 1, 1, 1, 1);


        horizontalLayout_2->addLayout(gridLayout_3);

        btnXmlStart = new QPushButton(boxXml);
        btnXmlStart->setObjectName(QStringLiteral("btnXmlStart"));
        sizePolicy.setHeightForWidth(btnXmlStart->sizePolicy().hasHeightForWidth());
        btnXmlStart->setSizePolicy(sizePolicy);
        btnXmlStart->setMinimumSize(QSize(120, 120));

        horizontalLayout_2->addWidget(btnXmlStart);


        verticalLayout_4->addWidget(boxXml);


        layMain->addWidget(widgetLeft);

        treeView = new QTreeView(Widget);
        treeView->setObjectName(QStringLiteral("treeView"));

        layMain->addWidget(treeView);

        layMain->setStretch(0, 1);
        layMain->setStretch(1, 2);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        boxOutPut->setTitle(QApplication::translate("Widget", "\350\256\276\347\275\256\350\276\223\345\207\272\347\233\256\345\275\225", Q_NULLPTR));
        btnWrite->setText(QApplication::translate("Widget", "\351\200\211\346\213\251\350\276\223\345\207\272\347\233\256\345\275\225", Q_NULLPTR));
        boxExcel->setTitle(QApplication::translate("Widget", "Excel\351\205\215\347\275\256", Q_NULLPTR));
        btnRead->setText(QApplication::translate("Widget", "\351\200\211\346\213\251\350\246\201\345\257\274\345\205\245\347\232\204Excel", Q_NULLPTR));
        label_6->setText(QApplication::translate("Widget", "\350\257\273\345\210\260\347\254\254\345\207\240\350\241\214\347\273\223\346\235\237?", Q_NULLPTR));
        label_3->setText(QApplication::translate("Widget", "\344\273\216\347\254\254\345\207\240\350\241\214\345\274\200\345\247\213\350\257\273\345\217\226?", Q_NULLPTR));
        btnStart->setText(QApplication::translate("Widget", "\345\274\200\345\247\213Excel\345\257\274\345\207\272", Q_NULLPTR));
        boxXml->setTitle(QApplication::translate("Widget", "Xml\351\205\215\347\275\256", Q_NULLPTR));
        btnTempletePath->setText(QApplication::translate("Widget", "\351\200\211\346\213\251\350\247\206\350\247\211\346\250\241\346\235\277", Q_NULLPTR));
        btnXmlPath->setText(QApplication::translate("Widget", "\351\200\211\346\213\251Xml\351\205\215\347\275\256\346\226\207\344\273\266", Q_NULLPTR));
        label_4->setText(QApplication::translate("Widget", "\345\220\214\346\255\245\350\247\206\350\247\211\346\250\241\346\235\277\347\254\254\345\207\240\350\241\214?", Q_NULLPTR));
        btnXmlStart->setText(QApplication::translate("Widget", "\345\274\200\345\247\213Xml\345\206\231\345\205\245", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
