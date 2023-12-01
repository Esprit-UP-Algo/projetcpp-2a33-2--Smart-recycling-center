#ifndef CONNECT_H
#define CONNECT_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class Connection
{
public:
    Connection();
    bool createconnect();
};


#endif // CONNECT_H
