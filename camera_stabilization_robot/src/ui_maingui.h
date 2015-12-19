/********************************************************************************
** Form generated from reading UI file 'maingui.ui'
**
** Created: Fri Sep 17 19:03:04 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINGUI_H
#define UI_MAINGUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainGui
{
public:
    QWidget *centralWidget;
    QDialogButtonBox *buttonBox;
    QGraphicsView *leftcamera;
    QGraphicsView *rightcamera;
    QLCDNumber *lcdNumber;
    QMenuBar *menuBar;
    QMenu *menuHOAP_Cameras;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainGui)
    {
        if (MainGui->objectName().isEmpty())
            MainGui->setObjectName(QString::fromUtf8("MainGui"));
        MainGui->resize(674, 378);
        centralWidget = new QWidget(MainGui);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        buttonBox = new QDialogButtonBox(centralWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(480, 260, 176, 27));
        buttonBox->setStandardButtons(QDialogButtonBox::Close);
        leftcamera = new QGraphicsView(centralWidget);
        leftcamera->setObjectName(QString::fromUtf8("leftcamera"));
        leftcamera->setGeometry(QRect(10, 0, 325, 245));
        rightcamera = new QGraphicsView(centralWidget);
        rightcamera->setObjectName(QString::fromUtf8("rightcamera"));
        rightcamera->setGeometry(QRect(340, 0, 325, 245));
        rightcamera->setAutoFillBackground(false);
        lcdNumber = new QLCDNumber(centralWidget);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setGeometry(QRect(30, 250, 64, 23));
        MainGui->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainGui);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 674, 23));
        menuHOAP_Cameras = new QMenu(menuBar);
        menuHOAP_Cameras->setObjectName(QString::fromUtf8("menuHOAP_Cameras"));
        MainGui->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainGui);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainGui->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainGui);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainGui->setStatusBar(statusBar);

        menuBar->addAction(menuHOAP_Cameras->menuAction());

        retranslateUi(MainGui);
        QObject::connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), MainGui, SLOT(close()));

        QMetaObject::connectSlotsByName(MainGui);
    } // setupUi

    void retranslateUi(QMainWindow *MainGui)
    {
        MainGui->setWindowTitle(QApplication::translate("MainGui", "MainGui", 0, QApplication::UnicodeUTF8));
        menuHOAP_Cameras->setTitle(QApplication::translate("MainGui", "HOAP Cameras", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainGui: public Ui_MainGui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINGUI_H
