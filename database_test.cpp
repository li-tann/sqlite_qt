#include "database_test.h"

database_test::database_test()
{

}


bool database_test::init(const char *db_path)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_path);

    if(!db.open()){
        lastError = "Open database failed.";
        qDebug()<<"Open failed.";
        return false;
    }
     qDebug()<<"Open succeed.";

     if(!create_table("xml")){
         lastError = "create table xml failed.";
         qDebug()<<"create table xml failed.";
         return false;
     }

     return true;
}

void database_test::close()
{
    db.close();
}

bool database_test::insert_data(int id, QString flepath, QString date, float inc, QString sensor, QString polar)
{
    QSqlQuery query(db);
    QString qstr_insert =  QString("insert into xml(id, filepath, date, inc, sensor, polar) "
                                   "values(%1,'%2','%3',%4,'%5','%6')").arg(id).arg(flepath).arg(date).arg(inc).arg(sensor).arg(polar);
    if(!query.exec(qstr_insert)){
        lastError = "insert_data failed.";
        qDebug()<<"insert_data failed.";
        return false;
    }
    return true;
}

bool database_test::delete_distinct()
{
    QSqlQuery query(db);
    if(!create_table("Tmp")){
        lastError = "create Tmp table failed.";
        qDebug()<<"create Tmp table failed.";
        return false;
    }
    if(!query.exec("select distinct * into Tmp from xml")){
        lastError = "delete distinct step1 failed.";
        qDebug()<<"delete distinct step1 failed.";
        return false;
    }
    if(!query.exec("drop table xml")){
        lastError = "delete distinct step2 failed.";
        qDebug()<<"delete distinct step2 failed.";
        return false;
    }
    if(!query.exec("select * into xml from Tmp ")){
        lastError = "delete distinct step3 failed.";
        qDebug()<<"delete distinct step3 failed.";
        return false;
    }
    if(!query.exec("drop table Tmp")){
        lastError = "delete distinct step4 failed.";
        qDebug()<<"delete distinct step4 failed.";
        return false;
    }
    return true;
//    return query.exec("select distinct * into #Tmp from xml "
//                      "drop table xml "
//                      "select * into xml from #Tmp "
//                      "drop table #Tmp");
}

int database_test::get_length()
{
    QSqlQuery query(db);
    int length=0;
    if(!query.exec("select count(*) AS NumberOfOrders from xml"))
    {
        lastError = "get_length failed.";
        qDebug()<<"get_length failed.";
        return -1;
    }
    else{
        while(query.next()){
            length = query.value(0).toInt();
        }
    }
    return length;

}

bool database_test::create_table(QString table_name)
{
    QSqlQuery query(db);
    if(!query.exec(QString("create table if not exists %1 "
                           "(id int primary key,"
                           "filepath char(300),"
                           "date char(10),"
                           "inc real,"
                           "sensor char(20),"
                           "polar char(10))").arg(table_name)))
    {
        lastError = "create_table failed.";
        return false;
    }
    return true;
}
