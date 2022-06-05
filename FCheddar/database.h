#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QVariant>

#define CREATE_TABLE_FCheddar \
    "CREATE TABLE IF NOT EXISTS FCheddar"\
    "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL"\
    ", projectName TEXT UNIQUE NOT NULL"\
    ", task_num INTEGER NOT NULL"\
    ", hyperperiod INTEGER NOT NULL"\
    ", scheduleable TEXT NOT NULL CHECK (scheduleable IN (\'Yes\', \'No\'))"\
    ", personImage BLOB NOT NULL)"

class Database : public QObject {
  Q_OBJECT
public:

    struct FCheddarTable {
        QString nameTable;

        QString nameProject;
        int taskNumber;
        int hyperperiod;
        bool scheduleable;
        QByteArray graphImage;
    };

    explicit Database(QObject *parent);
    bool startDatabase(const QString &nameFile);
    bool configureDatabase();

    bool openDatabase(QObject *parent);
    bool insertProject(const int &);

    QString getError();

private:
    QSqlDatabase mDatabase;
    FCheddarTable* projectTable;
    QString mError;

};

#endif // DATABASE_H
