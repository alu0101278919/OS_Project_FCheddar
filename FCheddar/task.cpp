#include "task.h"
#include "ui_task.h"

Task::Task(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Task)
{
    ui->setupUi(this);
}

Task::~Task()
{
    delete ui;
}

// Accept the task
void Task::on_buttonBox_accepted()
{
    accept();
}


// Reject the task
void Task::on_buttonBox_rejected()
{
    reject();
}


QString Task::taskName() const {
    return ui->taskName->text();
}

int Task::execTime() const {
    return ui->execTime->value();
}

int Task::period() const {
    return ui->period->value();
}

int Task::arrivalTime() const {
    return ui->arrivalTime->value();
}
