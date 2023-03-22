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

    cout<<"print method(insert, traverse, select, ...):\n";
    string str;
    getline(cin,str);
    if(str == "traverse")
    {
        cout<<"print file_folder:\n";
        getline(cin,str);
        vector<string> xmlpaths;
        dbt.traversing_folder_record_xml_path(str.c_str(), xmlpaths);
        dbt.record_xmls_in_database(xmlpaths);
    }
    else if(str == "insert")
    {
        string qstr_inc;
        while(1){
            cout<<"print inc: ";
            getline(cin,qstr_inc);

            if(qstr_inc == "stop")
                break;
            int db_count = dbt.get_length();
            if(db_count < 0){
                qDebug()<<dbt.lastError;
                return -1;
            }
            vitalInfo vi(db_count+1, "filepath", QDate::currentDate(),stod(qstr_inc),"a","r",40.8, 40.8,"sensor", "polar");
            if(!dbt.insert_data(vi)){
                qDebug()<<dbt.lastError;
                return -1;
            }
        }
    }


    cout<<"over.";


//    if(!dbt.delete_distinct()){
//        qDebug()<<dbt.lastError;
//        return -1;
//    }



    cout<<"database.close().\n";
    dbt.close();
    return a.exec();
}

