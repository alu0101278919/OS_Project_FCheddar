#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./scheduler.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , schedule(new Scheduler)
{
    ui->setupUi(this);
    setWindowTitle("FCheddar");

    QLineSeries *series = new QLineSeries();
    QLineSeries *seriesTask = new QLineSeries();
    *series << QPointF(0, 0.75) << QPointF(120, 0.75);
    *seriesTask << QPointF(0,0.75) << QPointF(5, 0.75);
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->addSeries(seriesTask);

    // Customize series
    QPen pen(QRgb(0xfdb157));
    pen.setWidth(5);
    series->setPen(pen);

    QPen penTask(QRgb(0xfdb157));
    pen.setWidth(20);
    seriesTask->setPen(penTask);

    // Customize chart title
    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
    chart->setTitleBrush(QBrush(Qt::black));
    chart->setTitle("FCheddar");

    // Customize chart background
    /*QLinearGradient backgroundGradient;
    backgroundGradient.setStart(QPointF(0, 0));
    backgroundGradient.setFinalStop(QPointF(0, 1));
    backgroundGradient.setColorAt(0.0, QRgb(0xd2d0d1));
    backgroundGradient.setColorAt(1.0, QRgb(0x4c4547));
    backgroundGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    chart->setBackgroundBrush(backgroundGradient);
    */

    // Customize plot area background
    /*QLinearGradient plotAreaGradient;
    plotAreaGradient.setStart(QPointF(0, 1));
    plotAreaGradient.setFinalStop(QPointF(1, 0));
    plotAreaGradient.setColorAt(0.0, QRgb(0x555555));
    plotAreaGradient.setColorAt(1.0, QRgb(0x55aa55));
    plotAreaGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    chart->setPlotAreaBackgroundBrush(plotAreaGradient);
    chart->setPlotAreaBackgroundVisible(true);*/

    QValueAxis *axisX = new QValueAxis;
    QCategoryAxis *axisY = new QCategoryAxis();

    // Customize axis label font
    QFont labelsFont;
    labelsFont.setPixelSize(12);
    axisX->setLabelsFont(labelsFont);
    axisY->setLabelsFont(labelsFont);

    // Customize axis colors
    QPen axisPen(QRgb(0xd18952));
    axisPen.setWidth(2);
    axisX->setLinePen(axisPen);
    axisY->setLinePen(axisPen);

    // Customize axis label colors
    QBrush axisBrush(Qt::black);
    axisX->setLabelsBrush(axisBrush);
    axisY->setLabelsBrush(axisBrush);

    // Customize grid lines and shades
    axisX->setGridLineVisible(true);
    axisY->setGridLineVisible(false);
    axisY->setShadesPen(Qt::NoPen);
    axisY->setShadesBrush(QBrush(QColor(0x99, 0xcc, 0xcc, 0x55)));
    axisY->setShadesVisible(false);

    axisX->setRange(0, 120);
    axisX->setLabelFormat("%d");
    axisY->append("T1", 1.5);
    axisY->setRange(0, 3);

    axisX->setTickCount(25);
    axisY->setTickCount(3);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    seriesTask->attachAxis(axisX);
    seriesTask->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    QVBoxLayout *layout = new QVBoxLayout(ui->plot);
    layout->addWidget(chartView);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete schedule;
}


// Open scheduler settings
void MainWindow::on_actionScheduler_Settings_triggered()
{
    schedule->setModal(true);
    schedule->setWindowTitle("Scheduler settings");
    schedule->exec();
}

