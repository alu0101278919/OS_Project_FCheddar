#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , schedule(new Scheduler)
    , graph_exists(false)
{
    ui->setupUi(this);
    setWindowTitle("FCheddar");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete schedule;
    delete graph;
    delete chartView;
    delete layout;
}


// Open scheduler settings
void MainWindow::on_actionScheduler_Settings_triggered()
{
    schedule->setModal(true);
    schedule->setWindowTitle("Scheduler settings");
    schedule->exec();

    if (graph_exists) {
        delete graph;
        delete chartView;
        delete layout;
    }

    graph = new Graph(schedule->get_taskTable()->size(),
                      schedule->calculate_hyperperiod(schedule->get_taskPeriods()),
                      schedule->get_taskNames(),
                      schedule->get_taskAT(),
                      schedule->get_taskPeriods(),
                      schedule->get_taskExecT());
    graph_exists = true;

    chartView = new QChartView(graph->get_chart());
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout(ui->plot);
    layout->addWidget(chartView);
}

