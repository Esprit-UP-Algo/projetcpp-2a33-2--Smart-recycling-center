#ifndef POINTDECOLLECT_H
#define POINTDECOLLECT_H
#include <QString>
#include <QDate>
#include <QSqlQueryModel>
class PointDeCollect
{
private:
    int id_pc,quntiter;
    QString lieu;
    QDate dat_pc;

public:
    PointDeCollect();
    PointDeCollect(int ,QDate,QString,int);


    int getid(){return id_pc ;};
    QString getlieu(){return lieu;};
    QDate getdate(){return dat_pc;};
    int getquntite(){return quntiter;};

    void setid(int a){id_pc = a;};
    void setlieu(QString a){lieu = a;};
    void setdate(QDate a){dat_pc = a;};
    void setquntite(int a){quntiter = a;};

    bool modifier();
    bool ajouter();
    bool supprime(int id);
    QSqlQueryModel* afficher();
    QSqlQueryModel* afficherID();
    QSqlQueryModel* tier_lieu();

    QMap<QString, int> getquntiterStatistics();


    ~PointDeCollect(){};
};

#endif // POINTDECOLLECT_H
