/********************************************************************************
** Form generated from reading UI file 'sy_2.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SY_2_H
#define UI_SY_2_H

#include <ActiveQt/QAxWidget>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_sy_2Class
{
public:
    QWidget *centralWidget;
    QAxWidget *MapToolBarCtr1;
    QAxWidget *MapTocCtr1;
    QAxWidget *MapCtr1;
    QAxWidget *MapToolBarCtr2;
    QAxWidget *MapTocCtr2;
    QAxWidget *MapCtr2;
    QAxWidget *MapToolBarCtr4;
    QAxWidget *MapCtr4;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *sy_2Class)
    {
        if (sy_2Class->objectName().isEmpty())
            sy_2Class->setObjectName(QString::fromUtf8("sy_2Class"));
        sy_2Class->resize(1440, 967);
        centralWidget = new QWidget(sy_2Class);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MapToolBarCtr1 = new QAxWidget(centralWidget);
        MapToolBarCtr1->setObjectName(QString::fromUtf8("MapToolBarCtr1"));
        MapToolBarCtr1->setProperty("geometry", QVariant(QRect(0, 0, 681, 31)));
        MapTocCtr1 = new QAxWidget(centralWidget);
        MapTocCtr1->setObjectName(QString::fromUtf8("MapTocCtr1"));
        MapTocCtr1->setProperty("geometry", QVariant(QRect(0, 30, 101, 401)));
        MapCtr1 = new QAxWidget(centralWidget);
        MapCtr1->setObjectName(QString::fromUtf8("MapCtr1"));
        MapCtr1->setProperty("geometry", QVariant(QRect(100, 30, 581, 401)));
        MapToolBarCtr2 = new QAxWidget(centralWidget);
        MapToolBarCtr2->setObjectName(QString::fromUtf8("MapToolBarCtr2"));
        MapToolBarCtr2->setProperty("geometry", QVariant(QRect(750, 0, 681, 31)));
        MapTocCtr2 = new QAxWidget(centralWidget);
        MapTocCtr2->setObjectName(QString::fromUtf8("MapTocCtr2"));
        MapTocCtr2->setProperty("geometry", QVariant(QRect(750, 30, 101, 411)));
        MapCtr2 = new QAxWidget(centralWidget);
        MapCtr2->setObjectName(QString::fromUtf8("MapCtr2"));
        MapCtr2->setProperty("geometry", QVariant(QRect(850, 30, 581, 411)));
        MapToolBarCtr4 = new QAxWidget(centralWidget);
        MapToolBarCtr4->setObjectName(QString::fromUtf8("MapToolBarCtr4"));
        MapToolBarCtr4->setProperty("geometry", QVariant(QRect(0, 440, 671, 31)));
        MapCtr4 = new QAxWidget(centralWidget);
        MapCtr4->setObjectName(QString::fromUtf8("MapCtr4"));
        MapCtr4->setProperty("geometry", QVariant(QRect(0, 470, 671, 421)));
        sy_2Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(sy_2Class);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1440, 23));
        sy_2Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(sy_2Class);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        sy_2Class->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(sy_2Class);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        sy_2Class->setStatusBar(statusBar);

        retranslateUi(sy_2Class);

        QMetaObject::connectSlotsByName(sy_2Class);
    } // setupUi

    void retranslateUi(QMainWindow *sy_2Class)
    {
        sy_2Class->setWindowTitle(QApplication::translate("sy_2Class", "sy_2", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class sy_2Class: public Ui_sy_2Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SY_2_H
