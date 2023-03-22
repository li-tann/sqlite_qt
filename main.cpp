#include <QCoreApplication>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <iostream>
#include "database_test.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    database_test dbt;
    if(!dbt.init("MyDataBase.db")){
        qDebug()<<dbt.lastError;
        return -1;
    }

//    string str;
//    cout<<"print inc: ";
//    getline(cin,str);

//    int db_count = dbt.get_length();
//    if(db_count < 0){
//        qDebug()<<dbt.lastError;
//        return -1;
//    }
//    if(!dbt.insert_data(db_count+1,"123","yyyy-mm-dd",stof(str),"sensor","polar")){
//        qDebug()<<dbt.lastError;
//        return -1;
//    }

//    qDebug()<<"insert succeed.";

    if(!dbt.delete_distinct()){
        qDebug()<<dbt.lastError;
        return -1;
    }

    dbt.close();
    return a.exec();
}

