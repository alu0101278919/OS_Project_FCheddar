#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QDir>
#include <QFileDialog>
#include <QSqlTableModel>

#include "./scheduler.h"
#include "./database.h"
#include "./calendar.h"
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
    void on_actionNew_Schedule_triggered();
    void on_openDatabaseButton_clicked();
    void on_newDatabaseButton_clicked();
    void on_connectToDatabase_clicked();
    void on_actionSave_triggered();
    void on_deleteButton_clicked();
    void on_projectTable_clicked(const QModelIndex &index);
    void on_filterButton_clicked();
    void on_cleanButton_clicked();
    void on_calendarButton_clicked();
    void on_actionRun_Scheduler_triggered();
    void on_actionRestart_Scheduler_triggered();

    void on_actionDeleteCurrent_Scheduler_triggered();

private:
    enum typeAction {OPEN, NEW};

    void openDatabase(typeAction);
    void insertImage(const QModelIndex &index);
    void createGraph(bool, bool);

    Ui::MainWindow *ui;
    Scheduler* schedule;
    Database* database;
    QSqlTableModel *mModel;
    const QModelIndex *lastIndex;
    Calendar* calendar;

    Graph *graph;
    QChartView *chartView;
    QVBoxLayout *layout;
    bool projectCreated;
};
#endif // MAINWINDOW_H
