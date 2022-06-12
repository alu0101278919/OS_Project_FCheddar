#include "task.h"
#include "ui_task.h"

#include <QMessageBox>


Task::Task(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Task) {

    ui->setupUi(this);
}


Task::~Task() {
    delete ui;
}


// Accept the task
void Task::on_buttonBox_accepted() {
    if (ui->taskName->text().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Task name cannot be empty.");
        return;
    } else if (ui->execTime->value() > ui->period->value()) {
        QMessageBox::warning(this, "Warning", "Execution time must be greater than period.");
        return;
    } else if (ui->arrivalTime->value() != 0) {
        QMessageBox::warning(this, "Warning", "Arrival Time must be zero.");
        return;
    } else if (ui->period->value() == 0) {
        QMessageBox::warning(this, "Warning", "Period must not be zero.");
        return;
    } else if (ui->execTime->value() == 0) {
        QMessageBox::warning(this, "Warning", "Execution time must not be zero.");
        return;
    }
    accept();
}


// Reject the task
void Task::on_buttonBox_rejected() {
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
