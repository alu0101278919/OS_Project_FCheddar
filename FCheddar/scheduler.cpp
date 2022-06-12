#include "scheduler.h"
#include "ui_scheduler.h"
#include "task.h"
#include <algorithm>
#include <numeric>

#include <QMessageBox>
#include <QCheckBox>

// Calculate greatest common divisor.
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

// Calculate lowest common multiple
int lcm(int a, int b)
{
    int temp = gcd(a, b);

    return temp ? (a / temp * b) : 0;
}

// Scheduler Constructor
Scheduler::Scheduler(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Scheduler),
    taskTable(new QVector<taskInfo>)
{
    QStringList titles;
    titles << "Task name" << "Arrival Time" << "Execution time" << "Period" << "Hide";
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(titles);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // para que las columnas completen el widget
}


// Scheduler Copy Constructor
Scheduler::Scheduler(const Scheduler& otherSchedule, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Scheduler),
    taskTable(new QVector<taskInfo>),
    projectName(otherSchedule.get_projectName())
{
    *taskTable = *otherSchedule.get_taskTable();
    QStringList titles;
    titles << "Task name" << "Arrival Time" << "Execution time" << "Period" << "Hide";
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(titles);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // para que las columnas completen el widget
    ui->projectName->setText(projectName);
    for(int i = 0; i < taskTable->size(); i++) {
        QWidget *checkBoxWidget = new QWidget();
        QCheckBox *checkBox = new QCheckBox();      // We declare and initialize the checkbox
        QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget); // create a layer with reference to the widget
        layoutCheckBox->addWidget(checkBox);            // Set the checkbox in the layer
        layoutCheckBox->setAlignment(Qt::AlignCenter);  // Center the checkbox
        layoutCheckBox->setContentsMargins(0,0,0,0);
        checkBox->setChecked(!(*taskTable)[i].hide);

        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        int row = ui->tableWidget->rowCount() - 1;
        ui->tableWidget->setCellWidget(row, HIDE, checkBoxWidget);
        ui->tableWidget->setItem(row, TASK_NAME, new QTableWidgetItem((*taskTable)[i].name));
        ui->tableWidget->setItem(row, ARRIVAL_T, new QTableWidgetItem(QString::number((*taskTable)[i].arrivalT)));
        ui->tableWidget->setItem(row, EXEC_T, new QTableWidgetItem(QString::number((*taskTable)[i].execT)));
        ui->tableWidget->setItem(row, PERIOD, new QTableWidgetItem(QString::number((*taskTable)[i].period)));
    }
}

// Scheduler Destructor
Scheduler::~Scheduler()
{
    delete ui;
    delete taskTable;
}

// GETTERS
QString Scheduler::get_projectName(void) const {
    return projectName;
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

QVector<bool> Scheduler::get_hidden_tasks(void) const {
    QVector<bool> vect;
    for(int i = 0; i < taskTable->size(); i++) {
        vect.push_back((*taskTable)[i].hide);
    }
    return vect;
}

// Adds up all lowest common multiples from all tasks which gives the hyperperiod.
int Scheduler::calculate_hyperperiod(QVector<int> vect) {
    return std::accumulate(vect.begin(), vect.end(), 1, lcm);
}

// Add a task to the project and includes it in a QTableWidget
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
    current_task.hide = false;

    taskTable->push_back(current_task);

    QWidget *checkBoxWidget = new QWidget();
    QCheckBox *checkBox = new QCheckBox();      // We declare and initialize the checkbox
    QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget); // create a layer with reference to the widget
    layoutCheckBox->addWidget(checkBox);            // Set the checkbox in the layer
    layoutCheckBox->setAlignment(Qt::AlignCenter);  // Center the checkbox
    layoutCheckBox->setContentsMargins(0,0,0,0);
    checkBox->setChecked(false);

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    int row = ui->tableWidget->rowCount() - 1;
    ui->tableWidget->setCellWidget(row, HIDE, checkBoxWidget);
    ui->tableWidget->setItem(row, TASK_NAME, new QTableWidgetItem(task.taskName()));
    ui->tableWidget->setItem(row, ARRIVAL_T, new QTableWidgetItem(QString::number(task.arrivalTime())));
    ui->tableWidget->setItem(row, EXEC_T, new QTableWidgetItem(QString::number(task.execTime())));
    ui->tableWidget->setItem(row, PERIOD, new QTableWidgetItem(QString::number(task.period())));

    ui->tableWidget->sortItems(PERIOD, Qt::AscendingOrder);
    std::sort(taskTable->begin(), taskTable->end()); // Order tasks by period (bigger period means lower priority)
}

// Deletes a task from the project and reorders task by period.
void Scheduler::on_deleteButton_clicked()
{
    if (!ui->tableWidget->rowCount()) {
        QMessageBox::critical(this, "Error", "No tasks in the table.");
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Warning",
                                 "Are you sure that you want to delete the selected task?",
                                 QMessageBox::Ok | QMessageBox::Cancel);
    if (reply == QMessageBox::Cancel) {
        return;
    }
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
    taskTable->erase(taskTable->constBegin() + ui->tableWidget->currentRow() + 1);
}


void Scheduler::on_AcceptRejectButtons_accepted()
{
    if (ui->projectName->text().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Project name cannot be empty.");
        return;
    }
    projectName = ui->projectName->text();
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        QWidget *pWidget = ui->tableWidget->cellWidget(i, HIDE);
        QCheckBox *checkbox = pWidget->findChild<QCheckBox *>();
        if (checkbox && checkbox->isChecked())
            (*taskTable)[i].hide = false;
        else
            (*taskTable)[i].hide = true;
    }
    accept();
}


void Scheduler::on_AcceptRejectButtons_rejected()
{
    reject();
}

