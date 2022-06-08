#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./scheduler.h"
#include "./database.h"
#include "./calendar.h"

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

    , graph_exists(false)
{
    ui->setupUi(this);
    setWindowTitle("FCheddar");
    ui->imgGraph->setText("<b>No image</b>");
    ui->projectTable->setContextMenuPolicy(Qt::ActionsContextMenu);
    //ui->projectTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // para que las columnas completen el View
    ui->projectTable->horizontalHeader()->setStretchLastSection(true);
    ui->imgGraph->setScaledContents(true);
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


void MainWindow::on_actionRun_Scheduler_triggered()
{
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

    graph->rms();

    chartView = new QChartView(graph->get_chart());
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout(ui->plot);
    layout->addWidget(chartView);
}


void MainWindow::on_actionRestart_Scheduler_triggered()
{
    if (graph_exists) {
        delete graph;
        delete chartView;
        delete layout;

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
    // Else, que salga un mensaje
}


void MainWindow::on_actionNew_Schedule_triggered()
{
    //Preguntar si está seguro
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


void MainWindow::on_openDatabaseButton_clicked()
{
    openDatabase(OPEN);
}



void MainWindow::on_newDatabaseButton_clicked()
{
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

}


//Insertar en base de datos
void MainWindow::on_actionSave_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Warning",
                                 "Are you sure that you want to save on the current database?",
                                 QMessageBox::Ok | QMessageBox::Cancel);

    if (reply == QMessageBox::Cancel) {
        return;
    }
    if (!database->databaseIsOpen()) {
        QMessageBox::critical(this, "Error", database->getError());
        return;
    }
    QByteArray hola;
    if (!database->insertProject(*schedule, hola)) {
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
    insertarImagen(index);
}


void MainWindow::insertarImagen(const QModelIndex &index)
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
    pixmap = pixmap.scaled(ui->imgGraph->size(), Qt::IgnoreAspectRatio);
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

