#include "scheduler.h"
#include "ui_scheduler.h"
#include "task.h"


Scheduler::Scheduler(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Scheduler),
    taskTable(new QVector<taskInfo>)
{
    QStringList titles;
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(4);
    titles << "Task name" << "Arrival Time" << "Execution time" << "Period";
    ui->tableWidget->setHorizontalHeaderLabels(titles);
}

Scheduler::~Scheduler()
{
    delete ui;
}


QVector<taskInfo>* Scheduler::get_taskTable(void) const {
    return taskTable;
}

// Add task
void Scheduler::on_Add_button_clicked()
{
    Task task(this);
    task.setWindowTitle("Task adder");
    if (task.exec() == QDialog::Rejected)
        return;
    else if (task.period() < task.execTime()) { // NECESITAMOS PONER UN ERROR AQUÍ
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

