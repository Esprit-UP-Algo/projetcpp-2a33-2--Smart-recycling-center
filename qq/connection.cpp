#include "connection.h"

Connection::Connection()
{

}
bool Connection::createconnect()
{
bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("Source_Projet2A");
db.setUserName("system");
db.setPassword("ayahichri123");

if (db.open()) test=true;


 return test;
}
