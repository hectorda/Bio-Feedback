#include "sql.h"
#include "ui_sql.h"

SQL::SQL(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SQL)
{
    inicializar();
    conexiones();
}

SQL::~SQL()
{
    delete ui;
}

void SQL::inicializar()
{
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->conectar();
    ui->tabWidget->setCurrentWidget(ui->tab_tablaPacientes);
    ui->lineEditRut->setValidator(new QIntValidator(0, 999999999) );
    ui->tabWidget->setTabEnabled(2,false);
    actualizarTablaPacientes();
}

void SQL::conexiones()
{
    connect(ui->pushButtonActualizarTabla,SIGNAL(clicked(bool)),this,SLOT(actualizarTablaPacientes()));
}

bool SQL::conectar() //Se intenta conectar y crear la base de datos en caso de que no exista.
{
    if(QSqlDatabase::isDriverAvailable("QSQLITE")) //Verificamos que el driver QSQLITE este instalado.
    {
        db = QSqlDatabase::addDatabase("QSQLITE"); //Definimos que se usara SQLITE como driver
        db.setDatabaseName(QDir::homePath()+QDir::separator()+"BaseDatosBioFeedBack.sqlite"); //Nuestra db en nuestro Home.
        bool db_ok = db.open(); //Creamos una bandera para ver si se puedo abrir la DB
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS admins (id integer primary key autoincrement, user text, passwd varchar[8])"); // Crea Tabla ADMINS solo si no existe.
        query.exec("CREATE TABLE IF NOT EXISTS pacientes (rut integer primary key, nombre text,apellido text, edad integer,sexo varchar[15])"); // Crea Tabla ADMINS solo si no existe.
        return db_ok;  //Retornamos true al metodo.

    }
    else
        qDebug() << "Driver no Disponible";
    return false;
}

void SQL::consulta()
{
    db.open();
    if(db.isOpen())
    {
        QSqlQuery query;
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

void SQL::actualizarTablaPacientes()
{
    db.open();
    if(db.isOpen())
    {
        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery("SELECT rut,nombre,apellido,edad FROM pacientes");
        model->setHeaderData(0, Qt::Horizontal, tr("Rut"));
        model->setHeaderData(1, Qt::Horizontal, tr("Nombre"));
        model->setHeaderData(2, Qt::Horizontal, tr("Apellido"));
        model->setHeaderData(3, Qt::Horizontal, tr("Edad"));

        ui->tableView->setModel(model);
        ui->tableView->show();

        db.close();
    }
}

QStringList SQL::listarNombresPacientes()
{
    QStringList nombres;
    db.open();
    if(db.isOpen()){
        QSqlQuery query;
        query.prepare("SELECT nombre FROM pacientes");

        if(query.exec()){
            while(query.next())
            {
                nombres.append(query.value(0).toString());
            }
        }
        db.close();
     }
    return nombres;
}

QStringList SQL::listarRutPacientes()
{
    QStringList ruts;
    db.open();
    if(db.isOpen()){
        QSqlQuery query;
        query.prepare("SELECT rut FROM pacientes");
        if(query.exec()){
            while(query.next())
            {
                ruts.append(query.value(0).toString());
            }
        }
        db.close();
     }
    return ruts;
}

QStringList SQL::buscarPacienteporRut(const QString rut)
{
    QStringList datos;
    db.open();
    if(db.isOpen()){
        QSqlQuery query;
        query.prepare("SELECT nombre,apellido,edad FROM pacientes where rut = (:rut)");
        query.bindValue(":rut",rut);
        if(query.exec()){
            while(query.next())
            {
                datos.append(query.value(0).toString());
                datos.append(query.value(1).toString());
                datos.append(query.value(2).toString());
            }
        }
        db.close();
     }
    return datos;

}

bool SQL::agregarPaciente(const QString rut,const QString nombre, const QString apellido, const int edad)
{
    bool query_ok=false;
    db.open();
    if(db.isOpen()){
        QSqlQuery query;
        query.prepare("INSERT INTO pacientes (rut,nombre, apellido,edad)"
                   "VALUES(:rut,:nombre,:apellido,:edad)");
        query.bindValue(":rut", rut.toInt());
        query.bindValue(":nombre", nombre);
        query.bindValue(":apellido", apellido);
        query.bindValue(":edad",edad);
        if(query.exec())
            query_ok=true;
        else
            query_ok=false;
        db.close();
     }

    return query_ok;
}

void SQL::on_pushButtonAgregar_clicked()
{
    const QString rut=ui->lineEditRut->text();
    const QString nombre=ui->lineEditNombre->text();
    const QString apellido=ui->lineEditApellido->text();
    const int edad=ui->spinBoxEdad->value();
    if (rut.isEmpty()|| nombre.isEmpty() || apellido.isEmpty() || edad==0)
        QMessageBox::warning(0,"Faltan Ddtos por llenar","Faltan datos por completar.");
    else{

        if(buscarPacienteporRut(rut).isEmpty()){

            if(agregarPaciente(rut,nombre,apellido,edad))
                QMessageBox::information(this,"Paciente agregado","El Paciente a sido agragado exitosamente");
            else
                QMessageBox::critical(this,"Error al agregar","No se pudo agregar el Paciente");
        }
        else
            QMessageBox::warning(this,"Paciente ya existe",tr("El Paciente con rut %1 ya existe en los registros").arg(rut));
    }

}

void SQL::on_tabWidget_currentChanged(int index)
{
    (void) index;
    if(ui->tabWidget->currentWidget()==ui->tab_tablaPacientes)
        actualizarTablaPacientes();
}

void SQL::on_lineEditRut_editingFinished()
{
    imprimirDigitoVerificador(ui->lineEditRut,ui->lineEditdigitoVerificador);
}

void SQL::imprimirDigitoVerificador(QLineEdit *lineEditRut, QLineEdit *lineEditDigito)
{
    QString rut;
    int suma=0;
    int digito;

    rut=lineEditRut->text();
    QTextStream stdout <<rut<<endl;
    if(rut.isEmpty())
        lineEditDigito->setText("");

    else{
        std::reverse(rut.begin(),rut.end());
        QTextStream stdout <<"reverse: "<<rut.size()<<endl;

        for (int var = 0; var <rut.size(); ++var) {
            if(var<6)
                suma+=rut.at(var).digitValue()*(var+2);
            else
                suma+=rut.at(var).digitValue()*(var-4);
        }
        digito = 11 - suma%11;
        if (digito<10)
            lineEditDigito->setText(QString::number(digito));

        else{
            if(digito==11)
                lineEditDigito->setText("0");
            if (digito==10)
                lineEditDigito->setText("K");
        }
    }
}

void SQL::on_pushButtonEditarPaciente_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    QModelIndex index = select->currentIndex();

    int row = index.row();
    QString rut = index.sibling(row, 0).data().toString();
    db.open();
    if(db.isOpen()){
        if(select->hasSelection()){

            QStringList datos=buscarPacienteporRut(rut);
            ui->tabWidget->setTabEnabled(2,true);
            ui->tabWidget->setCurrentIndex(2);

            ui->lineEditRut_2->setText(rut);
            ui->lineEditNombre_2->setText(datos.at(0));
            ui->lineEditApellido_2->setText(datos.at(1));
            ui->lineEditAltura_2->setText(datos.at(2));
            imprimirDigitoVerificador(ui->lineEditRut_2,ui->lineEditdigitoVerificador_2);
        }
        else
            QMessageBox::critical(this, "No ha seleccionado una fila!",
                                  "Primero debes seleccionar una \nfila de la tabla.",
                                  QMessageBox::Ok);
    }
    else
        QMessageBox::critical(this, "Error al Conexion!",
                               "No se pudo conectar con la Base de Datos.",
                               QMessageBox::Ok);

}

void SQL::on_pushButtonEliminarPaciente_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    QModelIndex index = select->currentIndex();

    int row = index.row();
    QString rut = index.sibling(row, 0).data().toString();
    db.open();
    if(db.isOpen()){
        if(select->hasSelection()){
            QMessageBox messageBox(QMessageBox::Question,
                                   "Confirmar Eliminacion",
                                   "Está a punto de eliminar un Paciente!\nDesea continuar?",
                                    QMessageBox::Yes|QMessageBox::No);
            messageBox.setButtonText(QMessageBox::Yes, tr("Eliminar"));
            messageBox.setButtonText(QMessageBox::No, tr("No"));
            if (messageBox.exec() == QMessageBox::Yes) {

                QSqlQuery *query = new QSqlQuery(db);
                query->prepare("DELETE FROM pacientes WHERE rut= (:rut)");
                query->bindValue(":rut",rut);

                if(query->exec()){
                    QMessageBox::information(this, "Aviso",
                                          "Paciente eliminado Exitosamente!",
                                          QMessageBox::Ok);
                    actualizarTablaPacientes();

                }
                else
                    QMessageBox::critical(this, "Error! :(",
                                      "Ocurrió un error al procesar su solicitud.\nVuelva a intentar más tarde.",
                                      QMessageBox::Ok);
            }
        }
        else
            QMessageBox::critical(this, "No ha seleccionado una fila!",
                                  "Primero debes seleccionar una \nfila de la tabla.",
                                  QMessageBox::Ok);
    }
    else
        QMessageBox::critical(this, "Error al Conexion!",
                               "No se pudo conectar con la Base de Datos.",
                               QMessageBox::Ok);
}
