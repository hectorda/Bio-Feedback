#ifndef SQL_H
#define SQL_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>

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
    bool agregarPaciente(const QString nombre, const QString apellido, const int edad);

private:
    Ui::SQL *ui;
    void inicializar();
    void conexiones();

public slots:
    void llenarTabla();

private slots:
    void on_pushButtonAgregar_clicked();
};

#endif // SQL_H
