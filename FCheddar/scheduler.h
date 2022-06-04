#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <QDialog>

namespace Ui {
class Scheduler;
}

struct taskInfo {
    QString name;
    int arrivalT;
    int period;
    int execT;
};

class Scheduler : public QDialog
{
    Q_OBJECT

public:
    explicit Scheduler(QWidget *parent = nullptr);
    ~Scheduler();

    QVector<taskInfo>* get_taskTable(void) const;

private slots:
    void on_Add_button_clicked();

private:
    Ui::Scheduler *ui;
    QVector<taskInfo>* taskTable;
    enum Column
    {
        TASK_NAME, ARRIVAL_T, EXEC_T, PERIOD
    };
};

#endif // SCHEDULER_H
