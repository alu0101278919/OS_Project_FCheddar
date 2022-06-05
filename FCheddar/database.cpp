#include "database.h"

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
    std::cout << "Paso por aqui\n" << std::flush;
    bool ok = query.exec(CREATE_TABLE_FCheddar);

    if (!ok)
        mError = query.lastError().text();
    return ok;
}

QString Database::getError() {
    return mError;
}

// int es provisional
bool Database::insertProject(const int& Project) {
    QSqlQuery query;
    QString q_str = QString("INSERT INTO %1 (%2, %3, %4, %5, %6) "
                            "VALUES (:name, :task, :hyperperiod, :scheduleable, :graph)")
                    .arg(projectTable->nameTable)
                    .arg(projectTable->nameProject)
                    .arg(projectTable->taskNumber)
                    .arg(projectTable->hyperperiod)
                    .arg(projectTable->scheduleable)
                    .arg(projectTable->graphImage);

    QByteArray graph_content;

    query.prepare(q_str);
    query.bindValue(":name", "Proyecto1");
    query.bindValue(":task", 3);
    query.bindValue(":hyperperiod", 120);
    query.bindValue(":scheduleable", "Yes");
    query.bindValue(":graph", graph_content);
    QDateTime date = QDateTime::currentDateTime();
    QString date_str = date.toString("dd.MM.yyyy hh:mm:ss");
    bool ok = query.exec();
    if (!ok)
        mError = query.lastError().text();
    return ok;
}


