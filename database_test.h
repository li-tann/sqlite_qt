#ifndef DATABASE_TEST_H
#define DATABASE_TEST_H

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QDate>
#include <vector>
#include <QDir>
#include <QList>
#include <QVector>
#include <iostream>

#include "tinyxml2.h"

using namespace std;

class vitalInfo{
public:
    vitalInfo(){};
    vitalInfo(QString _fp, QDate _dt, double _inc,QString _obt, QString _lk,double _lon, double _lat, QString _ss, QString _pol)
        :filepath(_fp),date(_dt),inc(_inc),orbit_ad(_obt),look_lr(_lk),cenlon(_lon),cenlat(_lat),sensor(_ss),polar(_pol){}

    QString to_QString(){return QString("id:%1, filepath:%2, date:%3, inc:%4, sensor:%5");}
    QString filepath;
    QDate date; /// date.toString("yyyy-MM-dd");
    double inc;
    QString orbit_ad;
    QString look_lr;
    double cenlon;
    double cenlat;
    QString sensor;
    QString polar;
};

class database_test
{
public:
    database_test();
    bool init(const char* db_path);
    void close();

    QSqlDatabase db;
    QSqlQuery query;

    QString lastError = "";

    /// 插入数据vi到数据库中
    bool insert_data(vitalInfo vi);
    /// 删除重复数据, 目前还不能用
    bool delete_distinct();
    /// 获取当前长度
    int get_length();
    /// 创建表
    bool create_table(QString table_name);
    /// 筛选符合key='value'条件的数据, 写入vis中, 该功能只支持单一条件筛选
    bool select_data(QString key, QString value, vector<vitalInfo> vis);

    /// 遍历文件夹并查询其中所有的xml文件
    void traversing_folder_record_xml_path(const char* file_folder, vector<string>& xmlpaths);
    /// 从xmlpaths找可读取的文件(landsar.xml格式), 提取关键信息写入数据库中
    void record_xmls_in_database(vector<string> xmlpaths);

};

#endif // DATABASE_TEST_H
