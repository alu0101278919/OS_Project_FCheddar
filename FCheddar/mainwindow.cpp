#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./scheduler.h"
#include "./database.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , schedule(new Scheduler)
    , database(new Database(this))
{
    ui->setupUi(this);
    setWindowTitle("FCheddar");

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


void MainWindow::on_actionNew_Schedule_triggered()
{
    QDir directory = QDir::home();
    QString nameFile;
    nameFile = QFileDialog::getSaveFileName(
                                    this,
                                    "Nueva base de datos",
                                     directory.filePath("/home/alu0101206011/Escritorio/QtSqlBLOB"),
                                    "Base de datos (*.db);; Cualquier tipo(*.*)");

    if (nameFile.isEmpty()){
        return;
    }
    if (!database->startDatabase(nameFile)) {
        QMessageBox::critical(this, "Error", database->getError());
        return;
    }
    if (!database->configureDatabase()) {
        QMessageBox::critical(this, "Error", database->getError());
        return;
    }
}

