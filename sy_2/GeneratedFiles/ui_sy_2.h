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
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_sy_2Class
{
public:
    QAction *action_2;
    QAction *action_3;
    QAction *action_4;
    QAction *action_5;
    QAction *action_6;
    QWidget *centralWidget;
    QAxWidget *MapToolBarCtr1;
    QAxWidget *MapTocCtr1;
    QAxWidget *MapCtr1;
    QAxWidget *MapToolBarCtr2;
    QAxWidget *MapTocCtr2;
    QAxWidget *MapCtr2;
    QAxWidget *MapToolBarCtr4;
    QAxWidget *MapCtr4;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label;
    QLabel *label_2;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *refImageSize;
    QLineEdit *senImageSize;
    QLineEdit *ptpairsnum;
    QLineEdit *registratetime;
    QLineEdit *mse;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_3;
    QMenu *menu_2;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *sy_2Class)
    {
        if (sy_2Class->objectName().isEmpty())
            sy_2Class->setObjectName(QString::fromUtf8("sy_2Class"));
        sy_2Class->resize(1440, 967);
        action_2 = new QAction(sy_2Class);
        action_2->setObjectName(QString::fromUtf8("action_2"));
        action_3 = new QAction(sy_2Class);
        action_3->setObjectName(QString::fromUtf8("action_3"));
        action_4 = new QAction(sy_2Class);
        action_4->setObjectName(QString::fromUtf8("action_4"));
        action_5 = new QAction(sy_2Class);
        action_5->setObjectName(QString::fromUtf8("action_5"));
        action_6 = new QAction(sy_2Class);
        action_6->setObjectName(QString::fromUtf8("action_6"));
        centralWidget = new QWidget(sy_2Class);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MapToolBarCtr1 = new QAxWidget(centralWidget);
        MapToolBarCtr1->setObjectName(QString::fromUtf8("MapToolBarCtr1"));
        MapToolBarCtr1->setProperty("geometry", QVariant(QRect(0, 0, 681, 31)));
        MapTocCtr1 = new QAxWidget(centralWidget);
        MapTocCtr1->setObjectName(QString::fromUtf8("MapTocCtr1"));
        MapTocCtr1->setProperty("geometry", QVariant(QRect(0, 30, 101, 421)));
        MapCtr1 = new QAxWidget(centralWidget);
        MapCtr1->setObjectName(QString::fromUtf8("MapCtr1"));
        MapCtr1->setProperty("geometry", QVariant(QRect(100, 30, 581, 421)));
        MapToolBarCtr2 = new QAxWidget(centralWidget);
        MapToolBarCtr2->setObjectName(QString::fromUtf8("MapToolBarCtr2"));
        MapToolBarCtr2->setProperty("geometry", QVariant(QRect(750, 0, 681, 31)));
        MapTocCtr2 = new QAxWidget(centralWidget);
        MapTocCtr2->setObjectName(QString::fromUtf8("MapTocCtr2"));
        MapTocCtr2->setProperty("geometry", QVariant(QRect(750, 30, 101, 421)));
        MapCtr2 = new QAxWidget(centralWidget);
        MapCtr2->setObjectName(QString::fromUtf8("MapCtr2"));
        MapCtr2->setProperty("geometry", QVariant(QRect(850, 30, 581, 421)));
        MapToolBarCtr4 = new QAxWidget(centralWidget);
        MapToolBarCtr4->setObjectName(QString::fromUtf8("MapToolBarCtr4"));
        MapToolBarCtr4->setProperty("geometry", QVariant(QRect(0, 470, 681, 31)));
        MapCtr4 = new QAxWidget(centralWidget);
        MapCtr4->setObjectName(QString::fromUtf8("MapCtr4"));
        MapCtr4->setProperty("geometry", QVariant(QRect(0, 500, 681, 401)));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(730, 500, 251, 231));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout->addWidget(label_4);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout->addWidget(label_5);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        refImageSize = new QLineEdit(groupBox);
        refImageSize->setObjectName(QString::fromUtf8("refImageSize"));

        verticalLayout_2->addWidget(refImageSize);

        senImageSize = new QLineEdit(groupBox);
        senImageSize->setObjectName(QString::fromUtf8("senImageSize"));

        verticalLayout_2->addWidget(senImageSize);

        ptpairsnum = new QLineEdit(groupBox);
        ptpairsnum->setObjectName(QString::fromUtf8("ptpairsnum"));

        verticalLayout_2->addWidget(ptpairsnum);

        registratetime = new QLineEdit(groupBox);
        registratetime->setObjectName(QString::fromUtf8("registratetime"));

        verticalLayout_2->addWidget(registratetime);

        mse = new QLineEdit(groupBox);
        mse->setObjectName(QString::fromUtf8("mse"));

        verticalLayout_2->addWidget(mse);


        gridLayout->addLayout(verticalLayout_2, 0, 1, 1, 1);

        sy_2Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(sy_2Class);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1440, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_3 = new QMenu(menu);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        sy_2Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(sy_2Class);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        sy_2Class->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(sy_2Class);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        sy_2Class->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menu->addAction(menu_3->menuAction());

        retranslateUi(sy_2Class);

        QMetaObject::connectSlotsByName(sy_2Class);
    } // setupUi

    void retranslateUi(QMainWindow *sy_2Class)
    {
        sy_2Class->setWindowTitle(QApplication::translate("sy_2Class", "sy_2", 0, QApplication::UnicodeUTF8));
        action_2->setText(QApplication::translate("sy_2Class", "\345\210\206\345\235\227\347\256\227\346\263\225", 0, QApplication::UnicodeUTF8));
        action_3->setText(QApplication::translate("sy_2Class", "\345\277\253\351\200\237\347\256\227\346\263\225", 0, QApplication::UnicodeUTF8));
        action_4->setText(QApplication::translate("sy_2Class", "\346\211\223\345\274\200\345\217\202\350\200\203\345\233\276\345\203\217", 0, QApplication::UnicodeUTF8));
        action_5->setText(QApplication::translate("sy_2Class", "\346\211\223\345\274\200\345\276\205\351\205\215\345\207\206\345\233\276\345\203\217", 0, QApplication::UnicodeUTF8));
        action_6->setText(QApplication::translate("sy_2Class", "\346\270\205\351\231\244\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("sy_2Class", "\345\244\247\345\271\205\351\235\242\345\233\276\345\203\217\345\214\271\351\205\215", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("sy_2Class", "\345\217\202\350\200\203\345\233\276\345\203\217\345\260\272\345\257\270", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("sy_2Class", "\345\276\205\351\205\215\345\207\206\345\233\276\345\203\217\345\260\272\345\257\270", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("sy_2Class", "\345\214\271\351\205\215\347\202\271\345\257\271", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("sy_2Class", "\347\256\227\346\263\225\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("sy_2Class", "\347\256\227\346\263\225\347\262\276\345\272\246", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("sy_2Class", "\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        menu_3->setTitle(QApplication::translate("sy_2Class", "\346\211\223\345\274\200", 0, QApplication::UnicodeUTF8));
        menu_2->setTitle(QApplication::translate("sy_2Class", "\350\266\205\345\244\247\345\233\276\345\203\217\351\205\215\345\207\206", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class sy_2Class: public Ui_sy_2Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SY_2_H
