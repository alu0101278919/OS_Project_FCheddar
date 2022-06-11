#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./scheduler.h"
#include "./database.h"
#include "./calendar.h"
#include "./client.h"
#include "./serverwidget.h"

#include <QCalendarWidget>
#include <QDate>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , schedule(new Scheduler)
    , database(new Database(this))
    , mModel(nullptr)
    , lastIndex(nullptr)
    , calendar(new Calendar)
    , graph(new Graph)
    , projectCreated(false)
{
    ui->setupUi(this);
    setWindowTitle("FCheddar");
    ui->imgGraph->setText("<b>No image</b>");
    ui->projectTable->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->projectTable->horizontalHeader()->setStretchLastSection(true);
    ui->imgGraph->setScaledContents(true);

    QColor color(190,190,190);
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, color);



    ui->filterWidget->setAutoFillBackground(true);
    ui->filterWidget->setPalette(pal);
    ui->filterWidget->show();
}


MainWindow::~MainWindow()
{
    delete ui;
    delete schedule;
    if (projectCreated) {
        delete graph;
        delete chartView;
        delete layout;
    }
}


void MainWindow::createGraph(bool itExists, bool run)
{
    if (itExists) {
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

    if (run) graph->rms();

    chartView = new QChartView(graph->get_chart());
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout(ui->plot);
    layout->addWidget(chartView);
}


// Open scheduler settings
void MainWindow::on_actionScheduler_Settings_triggered()
{
    if (projectCreated) {
        schedule->setModal(true);
        schedule->setWindowTitle("Scheduler settings");
        schedule->exec();
        createGraph(true, false);
    } else {
        QMessageBox::critical(this, "Error", "No project has been created.");
        return;
    }
}


void MainWindow::on_actionRun_Scheduler_triggered()
{
    ui->tabWidget->setCurrentIndex(0);
    if (projectCreated) {
        createGraph(true, true);
    } else {
        QMessageBox::critical(this, "Error", "No project has been created.");
        return;
    }
}


void MainWindow::on_actionRestart_Scheduler_triggered()
{
    ui->tabWidget->setCurrentIndex(0);
    if (projectCreated) {
        createGraph(true, false);
    } else {
        QMessageBox::critical(this, "Error", "No project has been created.");
        return;
    }
}


void MainWindow::on_actionNew_Schedule_triggered()
{
    ui->tabWidget->setCurrentIndex(0);
    if(projectCreated) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Warning",
                                     "Are you sure you want to create a new project?\nEverything that has not been saved will be lost.",
                                     QMessageBox::Ok | QMessageBox::Cancel);
        if (reply == QMessageBox::Cancel) {
            return;
        }
    }

    schedule->setModal(true);
    schedule->setWindowTitle("Scheduler settings");
    schedule->exec();
    createGraph(projectCreated, false);
    projectCreated = true;
}


void MainWindow::on_actionDeleteCurrent_Scheduler_triggered()
{
    ui->tabWidget->setCurrentIndex(0);
    if(projectCreated) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Warning",
                                     "Are you sure you want to delete de current project?\nEverything that has not been saved will be lost.",
                                     QMessageBox::Ok | QMessageBox::Cancel);
        if (reply == QMessageBox::Cancel) {
            return;
        } else {
            delete graph;
            delete chartView;
            delete layout;
            delete schedule;
            schedule = new Scheduler;
            projectCreated = false;
        }
    } else {
        QMessageBox::critical(this, "Error", "No project to delete.");
        return;
    }
}


void MainWindow::on_openDatabaseButton_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
    openDatabase(OPEN);
}


void MainWindow::on_newDatabaseButton_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
    openDatabase(NEW);
}


void MainWindow::openDatabase(typeAction type_action) {
    QString nameFile;
    QDir directory = QDir::home();
    if (type_action == NEW) {
        nameFile = QFileDialog::getSaveFileName(this,  // no se borra la base de datos anterior y no hay manera de borrarlo en sqlite
                                                "New Database",
                                                 directory.absolutePath() + "/SQLDatabase.db",
                                                "Database (*.db);; Any type(*.*)");

    } else {
        nameFile = QFileDialog::getOpenFileName(this,
                                                "Open Database",
                                                directory.absolutePath() + "/SQLDatabase.db",
                                                "Database (*.db);; Any type(*.*)");
    }
    if (nameFile.isEmpty()) {
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
    delete mModel;
    mModel = new QSqlTableModel(this);
    mModel->setTable("FCheddar");    // Pasamos la tabla
    mModel->select();
    ui->projectTable->setModel(mModel);
    ui->projectTable->hideColumn(0);
    ui->projectTable->hideColumn(6);
    mModel->setHeaderData(1, Qt::Horizontal, "Project name");
    mModel->setHeaderData(2, Qt::Horizontal, "Task number");
    mModel->setHeaderData(3, Qt::Horizontal, "Hyperperiod");
    mModel->setHeaderData(4, Qt::Horizontal, "Scheduleable");
    mModel->setHeaderData(5, Qt::Horizontal, "Date");
}


// hacer de cliente
void MainWindow::on_connectToDatabase_clicked()
{
    Client client(this);
    client.setWindowTitle("Client tcp");
    client.exec();
}


void MainWindow::on_actionServer_Database_triggered()
{
    if (!database->databaseIsOpen()) {
        QMessageBox::critical(this, "Error", database->getError());
        return;
    }
    ui->tabWidget->setCurrentIndex(1);
    ServerWidget server(this);
    server.setWindowTitle("Server tcp");
    server.exec();
}


//Insertar en base de datos
void MainWindow::on_actionSave_triggered()
{
    ui->tabWidget->setCurrentIndex(1);
    if (!database->databaseIsOpen()) {
        QMessageBox::critical(this, "Error", database->getError());
        return;
    } else if (!projectCreated) {
        QMessageBox::critical(this, "Error", "No project created.");
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Warning",
                                 "Are you sure you want to save on the current database?",
                                 QMessageBox::Ok | QMessageBox::Cancel);
    if (reply == QMessageBox::Cancel) {
        return;
    } else if (!database->insertProject(*schedule, graph->get_hyperperiod(), graph->get_chart_img())) {
        QMessageBox::critical(this, "Error", database->getError());
        return;
    }
    if (mModel) {
        mModel->select();
    }
}


void MainWindow::on_deleteButton_clicked()
{
    if (!database->databaseIsOpen()) {
        QMessageBox::critical(this, "Error", database->getError());
        return;
    }
    if (!mModel){
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Warning",
                                 "Are you sure that you want to delete the selected project?",
                                 QMessageBox::Ok | QMessageBox::Cancel);
    if (reply == QMessageBox::Cancel) {
        return;
    }
    mModel->removeRows(ui->projectTable->currentIndex().row(),1);
    mModel->select();
    ui->imgGraph->setText("<b>No image</b>");
}


void MainWindow::on_projectTable_clicked(const QModelIndex &index)
{
    if (!index.isValid()){
            return;
    }
    lastIndex = &index;
    insertImage(index);
}


void MainWindow::insertImage(const QModelIndex &index)
{
    const int id = mModel->index(lastIndex->row(), 0).data().toInt(); // Obtenemos índice
    QSqlQuery query;
    query.exec(QString("SELECT scheduleable FROM FCheddar WHERE id=%1").arg(id));
    query.next();
    if (query.value(0).toString() == "No") {
        ui->imgGraph->setText("<b>No image</b>");
        return;
    }
    query.exec(QString("SELECT graphImage FROM FCheddar WHERE id=%1").arg(id));
    query.next();
    QPixmap pixmap;
    if (!pixmap.loadFromData(query.value(0).toByteArray())){
        ui->imgGraph->setText("<b>Error de imagen</b>");
        return;
    }
    pixmap = pixmap.scaled(pixmap.size(),  Qt::KeepAspectRatio);
    ui->imgGraph->setPixmap(pixmap);
}


void MainWindow::on_filterButton_clicked()
{
    if (!database->databaseIsOpen()) {
        QMessageBox::critical(this, "Error", database->getError());
        return;
    }
    QString filter;
    bool previous = false;
    if (!ui->projectNameEdit->text().isEmpty()) {
        filter.append("projectName='" + ui->projectNameEdit->text() + "'");
        previous = true;
    }
    if (!ui->taskNumberEdit->text().isEmpty()) {
        if (previous) filter.append(" AND ");
        filter.append("taskNumber=" + ui->taskNumberEdit->text());
        previous = true;
    }
    if (!ui->hyperperiodEdit->text().isEmpty()) {
        if (previous) filter.append(" AND ");
        filter.append("hyperperiod=" + ui->hyperperiodEdit->text());
        previous = true;
    }
    if (!ui->scheduleableEdit->text().isEmpty()) {
        if (previous) filter.append(" AND ");
        filter.append("scheduleable='" + ui->scheduleableEdit->text() + "'");
        previous = true;
    }
    if (!ui->calendarEdit->text().isEmpty()) {
        if (previous) filter.append(" AND ");
        filter.append("timestamp='" + ui->calendarEdit->text() + "'");
    }
    if (mModel) {
        mModel->setFilter(filter);
    }
}


void MainWindow::on_cleanButton_clicked()
{
    if (mModel) {
        mModel->setFilter("");
    }
    ui->imgGraph->setText("<b>No image</b>");
    ui->projectNameEdit->clear();
    ui->taskNumberEdit->clear();
    ui->hyperperiodEdit->clear();
    ui->scheduleableEdit->clear();
    ui->calendarEdit->clear();
}


void MainWindow::on_calendarButton_clicked()
{
    if (!database->databaseIsOpen()) {
        QMessageBox::critical(this, "Error", database->getError());
        return;
    }
    calendar->setModal(true);
    calendar->setWindowTitle("Calendar settings");
    if (calendar->exec() == QDialog::Rejected) {
        return;
    }
    ui->calendarEdit->clear();
    ui->calendarEdit->setText(calendar->get_selectedDate());
}


