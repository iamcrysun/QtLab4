#include "stamplist.h"
#include <QDebug>

StampList::StampList(QObject* parent):QAbstractListModel(parent)
{

//    add("Марка 1", "США", 2001, 500000);
//    add("Марка 2", "Россия", 2021, 500);
//    add("Марка 3", "США", 2011, 9868);
//    add("Марка 4", "Россия", 2002, 21);

     db = QSqlDatabase::addDatabase("QSQLITE");
     db.setDatabaseName("stamps.sqlite3");
         if(!db.open())
             throw std::runtime_error("Can't connect to the database");

         QSqlQuery query;
         if (query.exec("SELECT * FROM stamps;")) {
         beginInsertRows({}, 0, query.size());
         while (query.next()) {
         auto s = Stamp();
         s.setNameSt(query.value(0).toString());
         s.setCountrySt(query.value(1).toString());
          s.setCountSt(query.value(2).toInt());
          s.setYearSt(query.value(3).toInt());



         listOfStamps.append(s);
         }

         endInsertRows();
         }
}

StampList::~StampList()
{

}

int StampList::rowCount(const QModelIndex&) const
{
    return listOfStamps.size();
}


QVariant StampList::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= listOfStamps.size())
            return QVariant();
      {
        switch (role) {
                case name:
                    return QVariant(listOfStamps.at(index.row()).getNameSt());
                case year:
                    return QVariant(listOfStamps.at(index.row()).getYearSt());

                case count:
                    return QVariant(listOfStamps.at(index.row()).getCountSt());

                case country:
                    return QVariant(listOfStamps.at(index.row()).getCountrySt());


                default:
                    return QVariant();
            }

    }
}

QHash<int, QByteArray> StampList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[name] = "nameSt";
    roles[year] = "yearSt";
    roles[count] = "countSt";
    roles[country] = "countrySt";

       return roles;
}

void StampList::add(const QString& nameSt, const QString& countrySt, const int yearSt, const int countSt){
     Stamp s;
     QSqlQuery query;
         query.prepare("INSERT INTO stamps (name, country, count, year) "
                      "VALUES (:name, :country, :year, :count)");
         query.bindValue(":name", nameSt);
         query.bindValue(":country", countrySt);
         query.bindValue(":count", countSt);
         query.bindValue(":year", yearSt);

         query.exec();

         s.setNameSt(nameSt);
         s.setCountrySt(countrySt);
         s.setCountSt(countSt);
         s.setYearSt(yearSt);
         beginInsertRows(QModelIndex(), listOfStamps.size(), listOfStamps.size());
         listOfStamps.append(s);
         endInsertRows();
}

QAbstractListModel* StampList::getModel(){
    return this;
}

QString StampList::find(const int textSelArea){
    qDebug() << name << year << country<<count;
    int cnt = 0;
    for(int i = 0; i < listOfStamps.size(); i++)
        if(listOfStamps[i].getCountSt() <= textSelArea)
            cnt++;
    QString c = QString::number(cnt);
    return c;

}

void StampList::del(const int index){

    auto st = listOfStamps[index];
        QSqlQuery query;
        query.prepare("DELETE FROM stamps WHERE name = :name;");
        query.bindValue(":name", st.getNameSt());
        query.exec();

        beginRemoveRows(QModelIndex(), index, index);
        listOfStamps.remove(index);
        endRemoveRows();
    }





void StampList::edit(const QString& nameSt,const QString& countrySt, const int yearSt, const int countSt, const int index) {
     if(index >= 0 && index < listOfStamps.size() )
     {
        auto& s = listOfStamps[index];
        QSqlQuery query;
        query.prepare("UPDATE stamps "
        "SET name = :name, "
        "country = :country, "
        "count = :count, "
        "year = :year "
        "WHERE name = :name;");

        query.bindValue(":name", nameSt);
        query.bindValue(":country", countrySt);
        query.bindValue(":count", countSt);
        query.bindValue(":year", yearSt);


        query.exec();

        s.setNameSt(nameSt);
        s.setCountrySt(countrySt);
        s.setCountSt(countSt);
        s.setYearSt(yearSt);

        listOfStamps[index]=s;

        auto modelIndex = createIndex(index, 0);
        emit dataChanged(modelIndex, modelIndex);
        qDebug() << listOfStamps[index].getNameSt();


     }
      else qDebug() << "Error index";
}
