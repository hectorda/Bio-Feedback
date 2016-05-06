#include "sql.h"

SQL::SQL()
{

}

bool SQL::conectar()
{
    if(QSqlDatabase::isDriverAvailable("QSQLITE")) //Verificamos que el driver QSQLITE este instalado.
     {
         db = QSqlDatabase::addDatabase("QSQLITE"); //Definimos que se usara SQLITE como driver
         db.setDatabaseName(QDir::homePath()+QDir::separator()+"BaseDatosBioFeedBack.sqlite"); //Nuestra db en nuestro Home.
         bool db_ok = db.open(); //Creamos una bandera para ver si se puedo abrir la DB
         return db_ok;  //Retornamos true al metodo.
     }
     else
         qDebug() << "Driver no Disponible";
         return false;

}

void SQL::consulta()
{
    this->conectar();
    if(db.isOpen())
    {

        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS admins (id integer primary key autoincrement, user text, passwd varchar[8])"); // Crea Tabla ADMINS solo si no existe.
        query.exec("CREATE TABLE IF NOT EXISTS pacientes (id integer primary key autoincrement, nombre text,apellido text, edad integer,sexo varchar[15])"); // Crea Tabla ADMINS solo si no existe.
        QString user="admin";
        query.prepare("SELECT id FROM admins WHERE user= (:user)");
        query.bindValue(":user", user);
        query.exec();

        if(!query.next()){
            QTextStream stdout << "Agregando el admin"<<endl;
            query.exec("INSERT INTO admins (user, passwd)" "VALUES('admin','12345')");
        }

        QTextStream stdout <<"Consultando a la Pseudo-Base de Datos"<<endl;

        if(query.exec("SELECT * FROM admins"))
        {
        while(query.next())
            {
                QTextStream stdout  << "id: " << query.value(0).toString();
                QTextStream stdout  << " user: " << query.value(1).toString();
                QTextStream stdout  << " paswd: " << query.value(2).toString()<<endl;
            }
        }

        db.close();
    }
}

