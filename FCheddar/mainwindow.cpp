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

    ui->tabWidget->setTabText(0, "Simulation");
    ui->tabWidget->setTabText(1, "Database");

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
        mPlannable = is_plannable();
        if(mPlannable) {
            createGraph(true, true);
            QMessageBox::information(this, "Plannable info", "Current project is plannable");
        } else {
            QMessageBox::information(this, "Plannable info", "Current project is not plannable");
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
    ui->listWidget->clear();
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
    } else if (!database->insertProject(*schedule, graph->get_hyperperiod(), mPlannable, graph->get_chart_img())) {
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


// TCP server help
void MainWindow::on_actionServer_Database_help_triggered()
{
    QMessageBox::information(this,"About TCP Server",
                             "<h1>TCP Server</h1>"
                             "<p>To access the server options select: <b>Tools > "
                             "Server Database</b>.</p>"
                             "<p>It should be noted that it is necessary to have an"
                             " open database to be able to access this option.</p>"
                             "<p>Once inside, there is a default server address and"
                             " a server port that needs to be changed.</p>"
                             "<p>For example, you could put port 3000.</p>"
                             "<p>When you give the <b>\"Start\"</b> the server will"
                             " start working.</p>"
                             "<p>If <b>\"Disconnect\"</b> is given, the server will"
                             " be off, so it will stop listening to the selected port.</p>");
}


// TCP client help
void MainWindow::on_actionClient_Database_help_triggered()
{
    QMessageBox::information(this,"About TCP Client",
                             "<h1>TCP Client</h1>"
                             "<p>To access the client options, select the button:"
                             " <b>Connect to Database</b> in the database tab.</p>"
                             "<h2>How to connect?</h2>"
                             "<p>To connect, you first need to know the port that a"
                             " server is already listening on.</p>"
                             "<p>Having the port, it must be placed in the port spin"
                             " box, since we are making a local connection,"
                             " it is not necessary to change the server address.</p>"
                             "<p>To finish we select the button to connect.</p>"
                             "<h2>How to disconnect?</h2>"
                             "<p>Simply select the disconnect button.</p>"
                             "<h2>Types of requests to the server</h2>"
                             "<p>There are 2 different types of request:</p>"
                             "<ul><li type=\"disc\">Text: the information stored in "
                             "the database of the requested project will be requested.</li>"
                             "<li type=\"disc\">Image: the image stored in the database "
                             "of the requested project will be requested.</li></ul>"
                             "<p>To use the requests it is necessary to know that the server"
                             " contains the requested project.</p>");
}


// General information on the use of the simulator help
void MainWindow::on_actionGeneral_Usage_Information_Simulation_triggered()
{
    QMessageBox::information(this, "General Usage Information.",
                             "<h1>General information on the use of the simulator</h1>"
                             "<p>The simulation corresponds to the entire simulation tab.</p>"
                             "<p>To access the simulation part it is necessary to have "
                             "a project created</p>"
                             "<h2>How to create a project?</h2>"
                             "<p>To create a project it is necessary to select the new schedule "
                             "button, inside there the planner options will appear where tasks "
                             "can be added and they will be ordered by period according to the "
                             "Rate Monotonic planning algorithm.<p>"
                             "<h2>How to edit a project?</h2>"
                             "<p>To edit the project select the <b>Scheduler Settings</b> option. "
                             "The options will appear again with the tasks added with your "
                             "information.</p>"
                             "<h2>How to delete a project?</h2>"
                             "<p>To delete the project you just have to select the <b>Close "
                             "Schedule option</b>.</p>"
                             "<h2>How to refresh the graph?</h2>"
                             "<p>To refresh the graph we just select the Restart Scheduler option.</p>"
                             "<h2>Why is the graph sometimes not painted?</h2>"
                             "<p>Sometimes the graph does not paint because the tasks "
                             "you are trying to schedule are not scheduleable.</p>"
                             );

}


// Edit appearance of the graph help
void MainWindow::on_actionEdit_appearance_Simulation_triggered()
{
    QMessageBox::information(this, "Edit appearance",
                             "<h1>Edit the appearance of the graph</h1>"
                             "<p>To edit the appearance of the graph select: <b>Edit > Window Settings</b>.</p>"
                             "<p>3 options will appear in the window.</p> <p>The first is used to split the "
                             "X-axis differently with the hyperperiod, which will allow you to "
                             "adjust if you have too large a hyperperiod. By default this option "
                             "is 5.</p><p>The second and third have to do with colors on the graph.<br>The "
                             "second edits the color of the line that always appears and the third "
                             "is the color of the line when a task is running.</p>"
                             );
}


// Database help
void MainWindow::on_actionGeneral_Usage_Information_Database_triggered()
{
    QMessageBox::information(this, "About Database",
                             "<h1>About Database</h1>"
                             "<p>The database corresponds to the entire database tab.</p>"
                             "<h2>New Database</h2>"
                             "<p>To create a new database select the new database button "
                             "and select where you want to save the database and its name</p>"
                             "<h2>Open Database</h2>"
                             "<p>to open the database select the open database "
                             "button and select the database that you have saved locally</p>"
                             "<h2>Delete row</h2>"
                             "<p>To delete rows select the row to delete "
                             "in the table and press the delete button</p>");
}


// Filter help
void MainWindow::on_actionFilter_triggered()
{
    QMessageBox::information(this, "About Filter",
                             "<h1>About Filter</h1>"
                             "<p>The filter is located in the database tab."
                             "The filter is very easy and intuitive. "
                             "Just write in the section what you want "
                             "to filter from the database table and it "
                             "will be filtered.</p>"
                             "<p>It is necessary to have an open database.</p>"
                             "<p>It is not necessary to fill in all the fields.</p>"
                             "<p>For the date you just have to open the calendar "
                             "and search for the specific day you want to filter. "
                             "Although you can also write the date directly if you want.</p>");
}


// FCheddar help
void MainWindow::on_actionInfo_FCheddar_help_triggered()
{
    QMessageBox::information(this, "About FCheddar",
                             "<h1>About FCheddar</h1>"
                             "<p>Made by Jaime Pablo Pérez Moro "
                             "and Anabel Díaz Labrador.</p>"
                             "<h2>Description</h2>"
                             "<p>In this project we have programmed a real-time scheduler simulator inspired by Cheddar."
                             "We have used QT creator to make this project."
                             "This simulation will be done using Rate-monotonic scheduling.</p>"
                             "<h2>QT Creator</h2>"
                             "<p>We have worked on version 6.0.2 although it can possibly run on version 5.</p>"
                             "<h2>Version FCheddar v0.1</h2>"
                             "<p>Fcheddar is a simple schedule simulator "
                             "that provides plannability information on a "
                             "given project created by the user.</p>");
}







