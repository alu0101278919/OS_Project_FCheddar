#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>

#include "./scheduler.h"
#include "./graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionScheduler_Settings_triggered();

    void on_actionRun_Scheduler_triggered();

private:
    Ui::MainWindow *ui;
    Scheduler *schedule;
    Graph *graph;
    QChartView *chartView;
    QVBoxLayout *layout;
    bool graph_exists;
};
#endif // MAINWINDOW_H
