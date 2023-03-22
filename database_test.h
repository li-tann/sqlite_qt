#ifndef DATABASE_TEST_H
#define DATABASE_TEST_H

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>


class database_test
{
public:
    database_test();
    bool init(const char* db_path);
    void close();

    QSqlDatabase db;
    QSqlQuery query;

    QString lastError = "";

    bool insert_data(int id, QString flepath, QString date, float inc, QString sensor, QString polar);
    bool delete_distinct();
    int get_length();
    bool create_table(QString table_name);

};

#endif // DATABASE_TEST_H
