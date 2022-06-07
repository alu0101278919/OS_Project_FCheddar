#include "scheduler.h"
#include "ui_scheduler.h"
#include "task.h"

#include <QMessageBox>


Scheduler::Scheduler(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Scheduler),
    taskTable(new QVector<taskInfo>)
{
    QStringList titles;
    titles << "Task name" << "Arrival Time" << "Execution time" << "Period";
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(titles);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // para que las columnas completen el widget
}

Scheduler::~Scheduler()
{
    delete ui;
}

QString Scheduler::get_projectName(void) const {
    return projectName;
}

QVector<taskInfo>* Scheduler::get_taskTable(void) const {
    return taskTable;
}



void Scheduler::on_addButton_clicked()
{
    Task task(this);
    task.setWindowTitle("Task adder");
    int result = task.exec();

    if (result == QDialog::Rejected) {
        return;
    }
    taskInfo current_task;
    current_task.name = task.taskName();
    current_task.arrivalT = task.arrivalTime();
    current_task.period = task.period();
    current_task.execT = task.execTime();

    taskTable->push_back(current_task);

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    int row = ui->tableWidget->rowCount() - 1;
    ui->tableWidget->setItem(row, TASK_NAME, new QTableWidgetItem(task.taskName()));
    ui->tableWidget->setItem(row, ARRIVAL_T, new QTableWidgetItem(QString::number(task.arrivalTime())));
    ui->tableWidget->setItem(row, EXEC_T, new QTableWidgetItem(QString::number(task.execTime())));
    ui->tableWidget->setItem(row, PERIOD, new QTableWidgetItem(QString::number(task.period())));

    ui->tableWidget->sortItems(PERIOD, Qt::AscendingOrder);
}


void Scheduler::on_AcceptRejectButtons_accepted()
{
    if (ui->projectName->text().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Project name cannot be empty.");
        return;
    }
    projectName = ui->projectName->text();
    accept();
}


void Scheduler::on_AcceptRejectButtons_rejected()
{
    reject();
}

