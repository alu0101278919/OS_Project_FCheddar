#include "database.h"
#include "scheduler.h"

#include <iostream>

Database::Database(QObject *parent) {}


bool Database::startDatabase(const QString &nameFile) {
    mDatabase = QSqlDatabase::addDatabase("QSQLITE");
    mDatabase.setDatabaseName(nameFile);
    if (!mDatabase.open())
        mError = mDatabase.lastError().text();
    return mDatabase.open();
}

bool Database::configureDatabase() {
    // La query que crea la base de datos si no está creada
    QSqlQuery query;
    bool ok = query.exec(CREATE_TABLE_FCheddar);
    if (!ok)
        mError = query.lastError().text();
    return ok;
}

bool Database::databaseIsOpen() {
    if (!mDatabase.isOpen()) {
        mError = QString("Database is not open");
        return false;
    }
    return true;
}

QString Database::getError() {
    return mError;
}

#include <QFileDialog>
#include <QMessageBox>

// meter grafica entera
bool Database::insertProject(const Scheduler& Project, const int hyperperiod, const QByteArray& graphImage) {
    QSqlQuery query;
    QString hola = "Yes";
    query.prepare("INSERT INTO FCheddar (projectName, taskNumber, hyperperiod, scheduleable, graphImage) "
                  "VALUES (:name, :task, :hyperperiod, :scheduleable, :graph)");
    query.bindValue(":name", Project.get_projectName());
    query.bindValue(":task", Project.get_taskTable()->size());
    query.bindValue(":hyperperiod", hyperperiod);
    query.bindValue(":scheduleable", hola);
    query.bindValue(":graph", graphImage);
    bool ok = query.exec();
    if (!ok)
        mError = query.lastError().text();
    return ok;
}


