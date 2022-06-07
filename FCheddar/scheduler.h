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
    QString get_projectName(void) const;

private slots:
    void on_addButton_clicked();
    void on_AcceptRejectButtons_accepted();
    void on_AcceptRejectButtons_rejected();

private:
    Ui::Scheduler *ui;
    QVector<taskInfo>* taskTable;
    QString projectName;
    enum Column
    {
        TASK_NAME, ARRIVAL_T, EXEC_T, PERIOD
    };
};

#endif // SCHEDULER_H
