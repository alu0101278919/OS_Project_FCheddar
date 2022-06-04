/********************************************************************************
** Form generated from reading UI file 'task.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TASK_H
#define UI_TASK_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Task
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *taskName;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpinBox *execTime;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QSpinBox *period;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QSpinBox *arrivalTime;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Task)
    {
        if (Task->objectName().isEmpty())
            Task->setObjectName(QString::fromUtf8("Task"));
        Task->resize(410, 178);
        verticalLayout = new QVBoxLayout(Task);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(Task);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        taskName = new QLineEdit(Task);
        taskName->setObjectName(QString::fromUtf8("taskName"));

        horizontalLayout->addWidget(taskName);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(Task);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        execTime = new QSpinBox(Task);
        execTime->setObjectName(QString::fromUtf8("execTime"));

        horizontalLayout_2->addWidget(execTime);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(Task);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        period = new QSpinBox(Task);
        period->setObjectName(QString::fromUtf8("period"));

        horizontalLayout_3->addWidget(period);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(Task);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        arrivalTime = new QSpinBox(Task);
        arrivalTime->setObjectName(QString::fromUtf8("arrivalTime"));

        horizontalLayout_4->addWidget(arrivalTime);


        verticalLayout->addLayout(horizontalLayout_4);

        buttonBox = new QDialogButtonBox(Task);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(Task);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, Task, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, Task, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(Task);
    } // setupUi

    void retranslateUi(QDialog *Task)
    {
        Task->setWindowTitle(QCoreApplication::translate("Task", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("Task", "Task name", nullptr));
        label_2->setText(QCoreApplication::translate("Task", "Execution time", nullptr));
        label_3->setText(QCoreApplication::translate("Task", "Period", nullptr));
        label_4->setText(QCoreApplication::translate("Task", "Arrival time", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Task: public Ui_Task {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TASK_H
