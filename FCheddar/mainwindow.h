#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QDir>
#include <QFileDialog>

#include "./scheduler.h"
#include "./database.h"

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

    void on_actionNew_Schedule_triggered();

private:
    Ui::MainWindow *ui;
    Scheduler* schedule;
    Database* database;
};
#endif // MAINWINDOW_H
