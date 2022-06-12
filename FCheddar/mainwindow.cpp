#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./scheduler.h"
#include "./database.h"
#include "./calendar.h"
#include "./client.h"
#include "./serverwidget.h"
#include "./rtalgorithms.h"
#include "./settings.h"

#include <QCalendarWidget>
#include <QDate>


// MainWindow Constructor
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
    , settings(new Settings)
{
    ui->setupUi(this);
    readSettings();

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


// MainWindow Destructor
MainWindow::~MainWindow()
{
    writeSettings();
    delete ui;
    delete schedule;
    delete settings;
    if (projectCreated) 
    {
        delete graph;
        delete chartView;
        delete layout;
    }
}


// Saves Program Settings.
void MainWindow::writeSettings()
{
    QSettings program_settings("FCheddarSettings", "ULL_Jaime&Anabel");

    program_settings.beginGroup("MainWindow");
    program_settings.setValue("geometry", saveGeometry());
    program_settings.setValue("activePenColor", settings->get_active_color());
    program_settings.setValue("penColor", settings->get_base_color());
    program_settings.setValue("xAxisSeparation", settings->x_size());
    program_settings.endGroup();
}


// Loads Program Settings.
void MainWindow::readSettings()
{
    QSettings program_settings("FCheddarSettings", "ULL_Jaime&Anabel");

    program_settings.beginGroup("MainWindow");
    const auto geometry = program_settings.value("geometry", QByteArray()).toByteArray();  // Window Size
    if (geometry.isEmpty())
        setGeometry(200, 200, 800, 649);
    else
        restoreGeometry(geometry);

    if (!program_settings.value("activePenColor").isNull()) {
        settings->set_active_color(program_settings.value("activePenColor").value<QColor>());
    }

    if (!program_settings.value("penColor").isNull()) {
        settings->set_color(program_settings.value("penColor").value<QColor>());
    }

    if (!program_settings.value("xAxisSeparation").isNull()) {
        settings->set_x_size(program_settings.value("xAxisSeparation").toInt());
    }
    program_settings.endGroup();
}


// Function createGraph() is used to create a graph if its not created and to refresh
// an existing graph with new parameters.
// Parameter itExists is used to determine whether a graph has been created before calling
// this function.
// Parameter run is used to determine whether we want to ilustrate the simulation results
// on the graph or not.
void MainWindow::createGraph(bool itExists, bool run) {
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
                      schedule->get_taskExecT(),
                      schedule->get_hidden_tasks(),
                      settings->get_active_color(),
                      settings->get_base_color(),
                      settings->x_size(),
                      schedule->get_projectName());

    if (run) graph->rms();

    chartView = new QChartView(graph->get_chart());
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout(ui->plot);
    layout->addWidget(chartView);
}


// is_plannable returns true if the current project is plannable. It uses usability factor
// algorithm as a first aproach to determine whether it is or not plannable. This algorithm
// returns the value 1 if it is plannable, 0 if its not and -1 if plannability was not able to
// be determined. Given the last case, we use TDA (Time Demand Analysis) to determine the projects
// plannability, if this function returns true the project is plannable, false if its not.
bool MainWindow::is_plannable() {
    QVector<QString> task_names = schedule->get_taskNames();
    QVector<int> task_execT = schedule->get_taskExecT();
    RTAlgorithms rms;
    bool plannable;
    ui->listWidget->addItem("Priority algorithm: Rate monotonic scheduling.\nCalculating utilization factor...\n");
    int uf_plannable = rms.rms_utilization_factor(schedule->get_taskTable()->size(),
                                                  schedule->get_taskExecT(),
                                                  schedule->get_taskPeriods());
    float wc_rms = rms.get_worst_case();
    QVector<float> uf_rms_vect = rms.get_taskUF();

    ui->listWidget->addItem("Calculating utilization factor for each task:\n");
    for(int i = 0; i < schedule->get_taskTable()->size(); i++) {
        ui->listWidget->addItem("task '" + task_names[i] + "' = " + QString::number(uf_rms_vect[i]) + "\n");
    }
    ui->listWidget->addItem("Worst case U0: " + QString::number(wc_rms) + "\n");
    if (uf_plannable == -1) {
        ui->listWidget->addItem(schedule->get_projectName() + " might not be plannable!\n");
        ui->listWidget->addItem("warranty test result = " + QString::number(rms.get_uf()) + "\n");
        ui->listWidget->addItem("Using time demand analysis to verify plannability...\n");
        plannable = rms.rms_response_time(schedule->get_taskTable()->size(),
                                                schedule->get_taskExecT(),
                                                schedule->get_taskPeriods());
        QVector<int> td_rms_vect = rms.get_time_demand();
        QVector<bool> valid_td_vect = rms.get_is_valid();
        ui->listWidget->addItem("Calculating time demand value for each task:\n");
        for(int i = 0; i < valid_td_vect.size(); i++) {
            if(!valid_td_vect[i]) {
                ui->listWidget->addItem("task '" + task_names[i] + "' = " + QString::number(td_rms_vect[i]));
                ui->listWidget->addItem(" > " + QString::number(task_execT[i]) + " TD VALUE NOT VALID\n");
                ui->listWidget->addItem(schedule->get_projectName() + " NOT PLANNABLE!\n");
                break;
            }
            ui->listWidget->addItem("task '" + task_names[i] + "' = " + QString::number(td_rms_vect[i]) + "\n");
        }
        ui->listWidget->addItem(schedule->get_projectName() + " is plannable!\n");
    } else if (uf_plannable) {
        ui->listWidget->addItem(schedule->get_projectName() + " is plannable!\n");
        ui->listWidget->addItem("warranty test result = " + QString::number(rms.get_uf()) + "\n");
        plannable = true;
    } else {
        ui->listWidget->addItem(schedule->get_projectName() + " NOT PLANNABLE!\n");
        ui->listWidget->addItem("warranty test result = " + QString::number(rms.get_uf()) + "\n");
        plannable = false;
    }
    return plannable;
}


// Open scheduler settings
void MainWindow::on_actionScheduler_Settings_triggered()
{
    if (projectCreated) {
        Scheduler scheduleCopy(*schedule);
        schedule->setModal(true);
        schedule->setWindowTitle("Scheduler settings");

        int result = schedule->exec();

        if (result == QDialog::Rejected) {
            delete schedule;
            schedule = new Scheduler(scheduleCopy);
            return;
        }
        createGraph(true, false);
    } else {
        QMessageBox::critical(this, "Error", "No project has been created.");
        return;
    }
}


// Run Scheduler, show results on graph and console.
void MainWindow::on_actionRun_Scheduler_triggered()
{
    ui->tabWidget->setCurrentIndex(0);
    if (projectCreated) {
        if(is_plannable()) {
          createGraph(true, true);
        }
    } else {
        QMessageBox::critical(this, "Error", "No project has been created.");
        return;
    }
}


// Restart Scheduler, show graph without running simulation.
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


// Create and configure a new project.
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
        } else {
            delete graph;
            delete chartView;
            delete layout;
            delete schedule;
            ui->listWidget->clear();
            schedule = new Scheduler;
            projectCreated = false;
        }
    }

    schedule->setModal(true);
    schedule->setWindowTitle("Scheduler settings");

    int result = schedule->exec();

    if (result == QDialog::Rejected) {
        delete schedule;
        schedule = new Scheduler;
        return;
    }
    createGraph(projectCreated, false);
    projectCreated = true;
}


// Delete current project and clear mainWindow.
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
            ui->listWidget->clear();
            schedule = new Scheduler;
            projectCreated = false;
        }
    } else {
        QMessageBox::critical(this, "Error", "No project to delete.");
        return;
    }
}


// Open existing database
void MainWindow::on_openDatabaseButton_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
    openDatabase(OPEN);
}


// Create and open a new database.
void MainWindow::on_newDatabaseButton_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
    openDatabase(NEW);
}


// Method used to open or create a database based on the parameter given (OPEN or NEW)
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


// Insert project into database (new row).
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


// Delete project from database.
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


// Inserts image in label when clicking on a row in the database.
void MainWindow::on_projectTable_clicked(const QModelIndex &index)
{
    if (!index.isValid()){
            return;
    }
    lastIndex = &index;
    insertImage(index);
}


// insertImage shows project graph as an image when clicking on any given row.
// If the row clicked has no image (is not plannable) no image is shown.
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


// Generates a query with the given filter parameters.
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


// Clears Filter options.
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


// Opens calendar widget and inserts selected date on filter.
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


// Opens program settings. Edit line colours and x-axis seperation.
void MainWindow::on_actionWindow_settings_triggered()
{
    Settings settingsCopy(*settings);
    settings->setModal(true);
    settings->setWindowTitle("Settings");

    int result = settings->exec();

    if (result == QDialog::Rejected) {
        delete settings;
        settings = new Settings(settingsCopy);
        return;
    }
    if (projectCreated) {
        createGraph(true, false);
    }
}


// Gets current project graph and converts it into PNG format to save it locally.
void MainWindow::on_actionSave_Graph_as_png_triggered()
{
    if (projectCreated) {
        QChart *chart = graph->get_chart();
        QChartView *chartView = new QChartView(chart);
        QPixmap p = chartView->grab();
        QString fileName = QFileDialog::getSaveFileName(this,
                           tr("Save Image"), "image.png",
                           tr(".png;;All Files (*)"));
        if (fileName.isEmpty())
            return;
        else {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;
            }
            p.save(fileName);
        }
    } else {
        QMessageBox::critical(this, "Error", "No project has been created.");
        return;
    }
}

