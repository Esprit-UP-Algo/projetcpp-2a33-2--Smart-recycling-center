#include "pointdecollect.h"

#include "ui_mainwindow.h"
#include "connect.h"

#include <QSqlQuery>
#include <QtDebug>
#include <QSqlQueryModel>
#include <QObject>
#include <QMessageBox>

PointDeCollect::PointDeCollect()
{
   id_pc=0;
   quntiter=0;
   lieu="";
}
PointDeCollect::PointDeCollect(int a,QDate b,QString c,int d)
{
    id_pc=a;
    quntiter=d;
    lieu=c;
    dat_pc=b;

}

bool PointDeCollect::ajouter()
 {
         QSqlQuery query;
         QString id_string=QString::number(id_pc);
         QString qt_string=QString::number(quntiter);


              query.prepare("INSERT INTO points_de_collectes (id_pc,lieu,dat_pc,quntiter) "
                            "VALUES (:id_pc, :lieu, :dat_pc, :quntiter)");
              query.bindValue(0, id_string);
              query.bindValue(1, lieu);
              query.bindValue(2, dat_pc);
              query.bindValue(3, qt_string);

             return query.exec();
 }

 QSqlQueryModel* PointDeCollect::afficher()
 {   QMessageBox msgBox;
      QSqlQueryModel*  model=new QSqlQueryModel();

            model->setQuery("SELECT ID_PC,LIEU,DAT_PC,QUNTITER FROM points_de_collectes");
            model->setHeaderData(0, Qt::Horizontal, QObject::tr("Identifiant"));
            model->setHeaderData(1, Qt::Horizontal, QObject::tr("lieu de PC"));
            model->setHeaderData(2, Qt::Horizontal, QObject::tr("date pc"));
            model->setHeaderData(3, Qt::Horizontal, QObject::tr("quntite de matier"));
            return  model;
 }
 QSqlQueryModel* PointDeCollect::afficherID()
 {   QMessageBox msgBox;

           QSqlQueryModel*  model=new QSqlQueryModel();

                 model->setQuery("SELECT ID_PC FROM points_de_collectes");
                 model->setHeaderData(0, Qt::Horizontal, QObject::tr("Identifiant"));

            return  model;
 }

bool PointDeCollect::supprime(int id_PC)
{
     QSqlQuery query;
    query.prepare("Delete from points_de_collectes where id_PC=:id_PC ");
    query.bindValue(0, id_PC);
    if(query.exec())
       {
           if(query.numRowsAffected() > 0)
               return true;
           else
               return false;
       }
       else
           return false;

}

bool PointDeCollect::modifier()
{
    QSqlQuery query;
    QString id_string=QString::number(id_pc);
    QString qt_string=QString::number(quntiter);

    query.prepare("UPDATE points_de_collectes SET  dat_pc = :dat_pc,"
                  " lieu = :lieu, quntiter = :quntiter where id_pc=:id_pc" );

    query.bindValue(":id_pc", id_string);
    query.bindValue(":dat_pc", dat_pc);
    query.bindValue(":lieu", lieu);
    query.bindValue(":quntiter", qt_string);

    if(query.exec())
       {
           if(query.numRowsAffected() > 0)
               return true;
           else
               return false;
       }
       else
           return false;

}
QSqlQueryModel* PointDeCollect::tier_lieu()
{
        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery("SELECT ID_PC,LIEU,DAT_PC,QUNTITER FROM points_de_collectes ORDER BY LIEU ASC");
        return model;
}
QMap<QString, int> PointDeCollect::getquntiterStatistics()
{
    QMap<QString, int> statistics;
    QSqlQuery query;

    query.prepare("SELECT COUNT(*) AS count FROM points_de_collectes WHERE QUNTITER < 80");
    if (query.exec() && query.next()) {
        statistics["Moins de 80 QUNTITER"] = query.value("count").toInt();
    }

    query.prepare("SELECT COUNT(*) AS count FROM points_de_collectes WHERE QUNTITER >= 80 AND QUNTITER <= 100");
    if (query.exec() && query.next()) {
        statistics["80-100 QUNTITER"] = query.value("count").toInt();
    }

    query.prepare("SELECT COUNT(*) AS count FROM points_de_collectes WHERE QUNTITER > 100");
    if (query.exec() && query.next()) {
        statistics["Plus de 100 QUNTITER"] = query.value("count").toInt();
    }

    return statistics;
}











