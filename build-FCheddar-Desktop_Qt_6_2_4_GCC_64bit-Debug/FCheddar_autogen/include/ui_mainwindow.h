/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew_Schedule;
    QAction *actionOpen_Schedule;
    QAction *actionSave;
    QAction *actionSave_as;
    QAction *actionClose_Schedule;
    QAction *actionRestart_Scheduler;
    QAction *actionScheduler_Settings;
    QAction *actionRun_Scheduler;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_3;
    QFrame *plot;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_6;
    QLabel *label;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuTools;
    QMenu *menuHelp;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 613);
        actionNew_Schedule = new QAction(MainWindow);
        actionNew_Schedule->setObjectName(QString::fromUtf8("actionNew_Schedule"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/icons8-add-file-80.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_Schedule->setIcon(icon);
        actionOpen_Schedule = new QAction(MainWindow);
        actionOpen_Schedule->setObjectName(QString::fromUtf8("actionOpen_Schedule"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/img/icons8-folder-480.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_Schedule->setIcon(icon1);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/img/icons8-save-80.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon2);
        actionSave_as = new QAction(MainWindow);
        actionSave_as->setObjectName(QString::fromUtf8("actionSave_as"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/img/icons8-save-as-80.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_as->setIcon(icon3);
        actionClose_Schedule = new QAction(MainWindow);
        actionClose_Schedule->setObjectName(QString::fromUtf8("actionClose_Schedule"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/img/icons8-close-480.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClose_Schedule->setIcon(icon4);
        actionRestart_Scheduler = new QAction(MainWindow);
        actionRestart_Scheduler->setObjectName(QString::fromUtf8("actionRestart_Scheduler"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/img/icons8-restart-480.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRestart_Scheduler->setIcon(icon5);
        actionScheduler_Settings = new QAction(MainWindow);
        actionScheduler_Settings->setObjectName(QString::fromUtf8("actionScheduler_Settings"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/img/icons8-services-480.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionScheduler_Settings->setIcon(icon6);
        actionRun_Scheduler = new QAction(MainWindow);
        actionRun_Scheduler->setObjectName(QString::fromUtf8("actionRun_Scheduler"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/img/icons8-next-80.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRun_Scheduler->setIcon(icon7);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_3 = new QVBoxLayout(centralwidget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        plot = new QFrame(centralwidget);
        plot->setObjectName(QString::fromUtf8("plot"));
        plot->setMinimumSize(QSize(778, 245));
        plot->setFrameShape(QFrame::StyledPanel);
        plot->setFrameShadow(QFrame::Raised);
        verticalLayoutWidget = new QWidget(plot);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 781, 251));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);

        verticalLayout_3->addWidget(plot);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(776, 242));
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-color: rgb(122, 122, 122);\n"
"font: 9pt \"Consolas\";\n"
"color: rgb(0, 0, 0);"));

        verticalLayout_6->addWidget(label);


        verticalLayout_3->addLayout(verticalLayout_6);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuTools = new QMenu(menubar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuTools->menuAction());
        menubar->addAction(menuHelp->menuAction());
        toolBar->addAction(actionNew_Schedule);
        toolBar->addAction(actionOpen_Schedule);
        toolBar->addAction(actionSave);
        toolBar->addAction(actionSave_as);
        toolBar->addAction(actionClose_Schedule);
        toolBar->addSeparator();
        toolBar->addAction(actionRestart_Scheduler);
        toolBar->addAction(actionScheduler_Settings);
        toolBar->addAction(actionRun_Scheduler);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionNew_Schedule->setText(QCoreApplication::translate("MainWindow", "New Schedule", nullptr));
        actionOpen_Schedule->setText(QCoreApplication::translate("MainWindow", "Open Schedule", nullptr));
        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        actionSave_as->setText(QCoreApplication::translate("MainWindow", "Save as", nullptr));
        actionClose_Schedule->setText(QCoreApplication::translate("MainWindow", "Close Schedule", nullptr));
        actionRestart_Scheduler->setText(QCoreApplication::translate("MainWindow", "Restart Scheduler", nullptr));
        actionScheduler_Settings->setText(QCoreApplication::translate("MainWindow", "Scheduler Settings", nullptr));
        actionRun_Scheduler->setText(QCoreApplication::translate("MainWindow", "Run Scheduler", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        menuTools->setTitle(QCoreApplication::translate("MainWindow", "Tools", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
