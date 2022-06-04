/********************************************************************************
** Form generated from reading UI file 'scheduler.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCHEDULER_H
#define UI_SCHEDULER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_Scheduler
{
public:
    QDialogButtonBox *buttonBox;
    QPushButton *Add_button;
    QTableWidget *tableWidget;

    void setupUi(QDialog *Scheduler)
    {
        if (Scheduler->objectName().isEmpty())
            Scheduler->setObjectName(QString::fromUtf8("Scheduler"));
        Scheduler->resize(538, 300);
        buttonBox = new QDialogButtonBox(Scheduler);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(40, 241, 451, 31));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        Add_button = new QPushButton(Scheduler);
        Add_button->setObjectName(QString::fromUtf8("Add_button"));
        Add_button->setGeometry(QRect(40, 220, 80, 25));
        tableWidget = new QTableWidget(Scheduler);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(40, 20, 451, 192));

        retranslateUi(Scheduler);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, Scheduler, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, Scheduler, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(Scheduler);
    } // setupUi

    void retranslateUi(QDialog *Scheduler)
    {
        Scheduler->setWindowTitle(QCoreApplication::translate("Scheduler", "Dialog", nullptr));
        Add_button->setText(QCoreApplication::translate("Scheduler", "Add", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Scheduler: public Ui_Scheduler {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCHEDULER_H
