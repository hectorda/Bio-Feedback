#ifndef SQL_H
#define SQL_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include <paciente.h>

namespace Ui {
class SQL;
}

class SQL : public QDialog
{
    Q_OBJECT

public:
    explicit SQL(QWidget *parent = 0);
    ~SQL();

    bool conectar();
    void consulta();
    QSqlDatabase db;
    bool agregarPaciente(Paciente paciente);
    QStringList listarNombresPacientes();
    QStringList listarRutPacientes();
    Paciente buscarPacienteporRut(const QString rut);
    bool editarPaciente(Paciente paciente);
    void abrirTabAgregarPaciente(const QString rut);

    bool existenCamposVacios(Paciente paciente);

private:
    Ui::SQL *ui;
    void inicializar();
    void conexiones();
    void imprimirDigitoVerificador(QLineEdit *lineEditRut, QLineEdit *lineEditDigito);

public slots:
    void actualizarTablaPacientes();

private slots:
    void on_pushButtonAgregar_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_lineEditRut_editingFinished();
    void on_pushButtonEditarPaciente_clicked();
    void on_pushButtonEliminarPaciente_clicked();
    void on_pushButtonEditarActualizarDatos_clicked();
};

#endif // SQL_H
