#include "scheduler.h"
#include "ui_scheduler.h"
#include "task.h"
#include <algorithm>
#include <numeric>


int gcd(int a, int b)
{
    while(true)
    {
        if (a == 0) return b;
        b %= a;
        if (b == 0) return a;
        a %= b;
    }
}

int lcm(int a, int b)
{
    int temp = gcd(a, b);

    return temp ? (a / temp * b) : 0;
}

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
    delete taskTable;
}

QVector<taskInfo>* Scheduler::get_taskTable(void) const {
    return taskTable;
}

QVector<QString> Scheduler::get_taskNames(void) const {
    QVector<QString> vect;
    for(int i = 0; i < taskTable->size(); i++) {
        vect.push_back((*taskTable)[i].name);
    }
    return vect;
}

QVector<int> Scheduler::get_taskAT(void) const {
    QVector<int> vect;
    for(int i = 0; i < taskTable->size(); i++) {
        vect.push_back((*taskTable)[i].arrivalT);
    }
    return vect;
}

QVector<int> Scheduler::get_taskPeriods(void) const {
    QVector<int> vect;
    for(int i = 0; i < taskTable->size(); i++) {
        vect.push_back((*taskTable)[i].period);
    }
    return vect;
}

QVector<int> Scheduler::get_taskExecT(void) const {
    QVector<int> vect;
    for(int i = 0; i < taskTable->size(); i++) {
        vect.push_back((*taskTable)[i].execT);
    }
    return vect;
}

int Scheduler::calculate_hyperperiod(QVector<int> vect) {
    return std::accumulate(vect.begin(), vect.end(), 1, lcm);
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
    std::sort(taskTable->begin(), taskTable->end());
}

