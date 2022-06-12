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
    bool hide;

    bool operator<(const taskInfo &a) {
        return period < a.period;
    }
};

class Scheduler : public QDialog
{
    Q_OBJECT

public:
    explicit Scheduler(QWidget *parent = nullptr);
    explicit Scheduler(const Scheduler&, QWidget *parent = nullptr);
    ~Scheduler();

    QVector<taskInfo>* get_taskTable(void) const;
    QString get_projectName(void) const;
    QVector<QString> get_taskNames(void) const;
    QVector<int> get_taskAT(void) const;
    QVector<int> get_taskPeriods(void) const;
    QVector<int> get_taskExecT(void) const;
    QVector<bool> get_hidden_tasks(void) const;
    int calculate_hyperperiod(QVector<int>);

private slots:
    void on_addButton_clicked();
    void on_AcceptRejectButtons_accepted();
    void on_AcceptRejectButtons_rejected();
    void on_deleteButton_clicked();

private:
    Ui::Scheduler *ui;
    QVector<taskInfo>* taskTable;
    QString projectName;


    enum Column
    {
        TASK_NAME, ARRIVAL_T, EXEC_T, PERIOD, HIDE
    };
};

#endif // SCHEDULER_H
