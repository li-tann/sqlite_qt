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

bool database_test::insert_data(vitalInfo vi)
{
    QSqlQuery query(db);
    QString qstr_insert =  QString("insert into xml(filepath, date, inc, orbit_ad, look_lr, cenlon, cenlat, sensor, polar) "
                                   "values('%1','%2',%3,'%4','%5','%6','%7','%8','%9')")
            .arg(vi.filepath)
            .arg(vi.date.toString("yyyy-MM-dd"))
            .arg(vi.inc)
            .arg(vi.orbit_ad)
            .arg(vi.look_lr)
            .arg(vi.cenlon)
            .arg(vi.cenlat)
            .arg(vi.sensor)
            .arg(vi.polar);
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
                           "(id INTEGER primary key AUTOINCREMENT,"
                           "filepath char(300),"
                           "date char(10),"
                           "inc real,"
                           "orbit_ad char(1),"
                           "look_lr char(1),"
                           "cenlon real,"
                           "cenlat real,"
                           "sensor char(20),"
                           "polar char(10))").arg(table_name)))
    {
        lastError = "create_table failed.";
        return false;
    }
    return true;
}

bool database_test::select_data(QString key, QString value, vector<vitalInfo> vis)
{
    QSqlQuery query(db);
    if(!query.exec(QString("select * from xml where %1='%2'").arg(key).arg(value)))
    {
        lastError = "select_data, query exec failed.";
        qDebug()<<"select_data, query exec failed.";
        return false;
    }
    else
    {
        while(query.next())
        {
            vitalInfo vi(query.value("filepath").toString(),
                         query.value("date").toDate(),
                         query.value("inc").toDouble(),
                         query.value("orbit_ad").toString(),
                         query.value("look_lr").toString(),
                         query.value("cenlon").toDouble(),
                         query.value("cenlat").toDouble(),
                         query.value("sensor").toString(),
                         query.value("polar").toString());
            vis.push_back(vi);
        }
    }
    return true;
}

void database_test::traversing_folder_record_xml_path(const char *file_folder, vector<string> &xmlpaths)
{
    QDir* dir = new QDir(file_folder);
    QStringList filter;
    QList<QFileInfo> fileInfos = dir->entryInfoList(filter);

    for(auto fileinfo : fileInfos)
    {
        if(fileinfo.fileName() == ".." || fileinfo.fileName()=="."){
            continue;
        }
        else if(fileinfo.isDir()){
            traversing_folder_record_xml_path(fileinfo.filePath().toLocal8Bit().data(),xmlpaths);
        }
        else if(fileinfo.isFile()){
            if(fileinfo.suffix()!="xml")
                continue;
            xmlpaths.push_back(fileinfo.filePath().toLocal8Bit().data());
        }
    }
}

void database_test::record_xmls_in_database(vector<string> xmlpaths)
{
    size_t total = xmlpaths.size();
    int complete = 0;
    int percent_multi_1000 = 0;
    for(auto xmlpath : xmlpaths)
    {
        vitalInfo vi;
        tinyxml2::XMLDocument doc;
        if(doc.LoadFile(xmlpath.c_str()) != 0) {
            continue ;
        }

        tinyxml2::XMLElement* root = doc.RootElement();
        if(root == nullptr) {
            continue;
        }

        tinyxml2::XMLElement* childNode_common = root->FirstChildElement("SARParametersofCommonModule");
        if(childNode_common == nullptr){
            continue;
        }
        vi.filepath = xmlpath.c_str();
        QStringList qlist_date = QString(childNode_common->FirstChildElement("acquisitionDate")->GetText()).split(" ");
        if(qlist_date.size()<3)continue;
        vi.date.setDate(qlist_date[0].toInt(),qlist_date[1].toInt(),qlist_date[2].toInt());
        vi.inc = QString(childNode_common->FirstChildElement("incidenceAngle")->GetText()).toDouble();
        vi.orbit_ad = QString(childNode_common->FirstChildElement("orbitDirection")->GetText()).left(1).toLower();
        vi.look_lr = QString(childNode_common->FirstChildElement("lookDirection")->GetText()).left(1).toLower();
        tinyxml2::XMLElement* childNode_common_centercoord = childNode_common->FirstChildElement("sceneCenterCoord");
        vi.cenlon = QString(childNode_common_centercoord->FirstChildElement("lon")->GetText()).toDouble();
        vi.cenlat = QString(childNode_common_centercoord->FirstChildElement("lat")->GetText()).toDouble();
        vi.sensor = childNode_common->FirstChildElement("mission")->GetText();
        vi.polar = childNode_common->FirstChildElement("polarisationList")->FirstChildElement("polLayer")->GetText();

        if(!this->insert_data(vi))
            continue;

        complete++;

        /// progress
        int current_percent_multi_1000 = complete * 1000 / total;
        if(current_percent_multi_1000 != percent_multi_1000){
            percent_multi_1000 = current_percent_multi_1000;
            QString qstr_percent = QString("%1.%2").arg(percent_multi_1000/10).arg(percent_multi_1000 - percent_multi_1000 / 10 * 10);
            cout<<QString("\rpercent: %1%(%2/%3)").arg(qstr_percent).arg(complete).arg(total).toLocal8Bit().data();
        }
    }
    cout<<"\n";
}


