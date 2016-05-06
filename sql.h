#ifndef SQL_H
#define SQL_H

#include <QtSql>

class SQL
{
public:
    SQL();
    bool conectar();
    void consulta();
    QSqlDatabase db;
};

#endif // SQL_H
