#include "connect.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
/*QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("Source_projet2A");
db.setUserName("hafedh");//inserer nom de l'utilisateur
db.setPassword("esprit1");//inserer mot de passe de cet utilisateur*/
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("Source_projet2A");
db.setUserName("jasser");//inserer nom de l'utilisateur
db.setPassword("esprit18");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;





    return  test;
}
