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
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->conectar();
    ui->tabWidget->setCurrentWidget(ui->tab_tablaPacientes);
    ui->lineEditRut->setValidator(new QIntValidator(0, 999999999) );
    ui->lineEditEdad->setValidator(new QIntValidator(0, 150) );
    ui->lineEditEdad_2->setValidator(new QIntValidator(0, 150) );
    ui->lineEditAltura->setValidator(new QIntValidator(0, 299) );
    ui->lineEditAltura_2->setValidator(new QIntValidator(0, 299) );
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
        db.setDatabaseName("BaseDatosBioFeedBack.sqlite"); //Nuestra db en nuestro Home.
        bool db_ok = db.open(); //Creamos una bandera para ver si se puedo abrir la DB
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS pacientes (rut integer primary key, nombre text,apellido text, edad integer,sexo text,altura integer)"); // Crea Tabla ADMINS solo si no existe.
        return db_ok;  //Retornamos true al metodo.

    }
    else
        qDebug() << "Driver no Disponible";
    return false;
}

void SQL::actualizarTablaPacientes()
{
    db.open();
    if(db.isOpen())
    {
        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery("SELECT rut,nombre,apellido,edad,sexo,altura FROM pacientes");
        model->setHeaderData(0, Qt::Horizontal, tr("Rut"));
        model->setHeaderData(1, Qt::Horizontal, tr("Nombre"));
        model->setHeaderData(2, Qt::Horizontal, tr("Apellido"));
        model->setHeaderData(3, Qt::Horizontal, tr("Edad (años)"));
        model->setHeaderData(4, Qt::Horizontal, tr("Sexo"));
        model->setHeaderData(5, Qt::Horizontal, tr("Altura (cm)"));

        ui->tableView->setModel(model);
        ui->tableView->show();

        db.close();
    }
}

/*
 * Para el completer
 *
*/
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

Paciente SQL::buscarPacienteporRut(const QString rut)
{
    Paciente paciente;
    db.open();
    if(db.isOpen()){
        QSqlQuery query;
        query.prepare("SELECT nombre,apellido,edad,sexo,altura FROM pacientes where rut = (:rut)");
        query.bindValue(":rut",rut);
        if(query.exec()){
            while(query.next())
            {
                paciente.setRut(rut);
                paciente.setNombre(query.value(0).toString());
                paciente.setApellido(query.value(1).toString());
                paciente.setEdad(query.value(2).toString());
                paciente.setSexo(query.value(3).toString());
                paciente.setAltura(query.value(4).toString());
            }
        }
        db.close();
     }
    return paciente;
}

bool SQL::agregarPaciente(Paciente paciente)
{
    bool query_ok=false;
    db.open();
    if(db.isOpen()){
        QSqlQuery query;
        query.prepare("INSERT INTO pacientes (rut,nombre, apellido,edad,sexo,altura)"
                   "VALUES(:rut,:nombre,:apellido,:edad,:sexo,:altura)");
        query.bindValue(":rut",paciente.getRut());
        query.bindValue(":nombre", paciente.getNombre());
        query.bindValue(":apellido", paciente.getApellido());
        query.bindValue(":edad",paciente.getEdad());
        query.bindValue(":sexo",paciente.getSexo());
        query.bindValue(":altura",paciente.getAltura());
        query_ok=query.exec();
        db.close();
     }

    return query_ok;
}

bool SQL::editarPaciente(Paciente paciente)
{
    bool query_ok=false;
    db.open();
    if(db.isOpen()){
        QSqlQuery query;
        query.prepare("UPDATE pacientes SET nombre=:nombre,apellido=:apellido,edad=:edad,sexo=:sexo,altura=:altura WHERE rut=:rut");
        query.bindValue(":rut",paciente.getRut());
        query.bindValue(":nombre", paciente.getNombre());
        query.bindValue(":apellido", paciente.getApellido());
        query.bindValue(":edad",paciente.getEdad());
        query.bindValue(":sexo",paciente.getSexo());
        query.bindValue(":altura",paciente.getAltura());
        query_ok=query.exec();
        db.close();
     }
    return query_ok;
}

void SQL::abrirTabAgregarPaciente(const QString rut)
{
    ui->lineEditRut->setText(rut);
    ui->tabWidget->setCurrentWidget(ui->tab_AgregarPaciente);
    exec();
}

void SQL::on_pushButtonAgregar_clicked()
{
    Paciente paciente;
    paciente.setRut(ui->lineEditRut->text());
    paciente.setNombre(ui->lineEditNombre->text());
    paciente.setApellido(ui->lineEditApellido->text());
    paciente.setEdad(ui->lineEditEdad->text());
    paciente.setSexo(ui->comboBox->currentText());
    paciente.setAltura(ui->lineEditAltura->text());

    if (existenCamposVacios(paciente))
        QMessageBox::warning(0,"Faltan Datos por llenar","Faltan datos por completar.");
    else{

        if(buscarPacienteporRut(paciente.getRut()).isEmpty()){
            if(agregarPaciente(paciente))
                QMessageBox::information(this,"Paciente agregado","El Paciente a sido agragado exitosamente");
            else
                QMessageBox::critical(this,"Error al agregar","No se pudo agregar el Paciente");
        }
        else
            QMessageBox::warning(this,"Paciente ya existe",tr("El Paciente con rut %1 ya existe en los registros").arg(paciente.getRut()));
    }
}

bool SQL::existenCamposVacios(Paciente paciente){

    if(paciente.getRut().isEmpty()|| paciente.getNombre().isEmpty() || paciente.getApellido().isEmpty() ||
            paciente.getEdad()==0 || paciente.getAltura()==0)
        return true;

    return false;

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

            Paciente paciente=buscarPacienteporRut(rut);
            ui->tabWidget->setTabEnabled(2,true);
            ui->tabWidget->setCurrentIndex(2);

            ui->lineEditRut_2->setText(paciente.getRut());
            ui->lineEditNombre_2->setText(paciente.getNombre());
            ui->lineEditApellido_2->setText(paciente.getApellido());
            ui->lineEditEdad_2->setText(paciente.getEdad());
            ui->lineEditAltura_2->setText(paciente.getAltura());
            ui->comboBox_2->setCurrentText(paciente.getSexo());
            QTextStream stdout <<paciente.getAltura()<<endl;
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

void SQL::on_pushButtonEditarActualizarDatos_clicked()
{
    Paciente paciente;
    paciente.setRut(ui->lineEditRut_2->text());
    paciente.setNombre(ui->lineEditNombre_2->text());
    paciente.setApellido(ui->lineEditApellido_2->text());
    paciente.setEdad(ui->lineEditEdad_2->text());
    paciente.setSexo(ui->comboBox_2->currentText());
    paciente.setAltura(ui->lineEditAltura_2->text());
    if (existenCamposVacios(paciente))
        QMessageBox::warning(0,"Faltan Datos por llenar","Faltan datos por completar.");
    else{
        if(editarPaciente(paciente))
            QMessageBox::information(this,"Datos Actualizados","El los datos del Paciente han sido actualizados exitosamente");
        else
            QMessageBox::critical(this,"Error al editar datos","No se pudo editar los datos del Paciente");
    }
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

void SQL::imprimirDigitoVerificador(QLineEdit *lineEditRut, QLineEdit *lineEditDigito)
{
    QString rut;
    int suma=0;
    int digito;

    rut=lineEditRut->text();
    if(rut.isEmpty())
        lineEditDigito->setText("");

    else{
        std::reverse(rut.begin(),rut.end());
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

void SQL::on_lineEditRut_editingFinished()
{
    imprimirDigitoVerificador(ui->lineEditRut,ui->lineEditdigitoVerificador);
}

void SQL::on_tabWidget_currentChanged(int index)
{
    (void) index;
    if(ui->tabWidget->currentWidget()==ui->tab_tablaPacientes)
        actualizarTablaPacientes();
}


