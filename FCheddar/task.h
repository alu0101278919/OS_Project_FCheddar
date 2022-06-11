
#ifndef TASK_H
#define TASK_H

#include <QDialog>

namespace Ui {
class Task;
}

class Task : public QDialog
{
    Q_OBJECT

public:
    explicit Task(QWidget *parent = nullptr);
    ~Task();

    QString taskName() const;
    int execTime() const;
    int period() const;
    int arrivalTime() const;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::Task *ui;
};

#endif // TASK_H
