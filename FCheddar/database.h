#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QVariant>

#include "scheduler.h"

#define CREATE_TABLE_FCheddar \
    "CREATE TABLE IF NOT EXISTS FCheddar"\
    "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL"\
    ", projectName TEXT UNIQUE NOT NULL"\
    ", taskNumber INTEGER NOT NULL"\
    ", hyperperiod INTEGER"\
    ", scheduleable TEXT CHECK (scheduleable IN (\'Yes\', \'No\'))"\
    ", timestamp TEXT DEFAULT(date('now','localtime'))"\
    ", graphImage BLOB)"



class Database : public QObject {
  Q_OBJECT
public:
    explicit Database(QObject *parent);
    bool startDatabase(const QString &nameFile);
    bool configureDatabase();
    bool databaseIsOpen();
    bool insertProject(const Scheduler& Project, const int hyperperiod, const bool plannable, const QByteArray& graphImage);

    QString getError();

private:
    QSqlDatabase mDatabase;
    QString mError;

};

#endif // DATABASE_H
