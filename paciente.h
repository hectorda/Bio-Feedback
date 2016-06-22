#ifndef PACIENTE_H
#define PACIENTE_H

#include <QString>

class Paciente
{
public:
    Paciente();

    QString getRut() const;
    void setRut(const QString &value);

    QString getNombre() const;
    void setNombre(const QString &value);

    QString getApellido() const;
    void setApellido(const QString &value);

    QString getEdad() const;
    void setEdad(QString value);

    QString getAltura() const;
    void setAltura(QString value);

    QString getSexo() const;
    void setSexo(const QString &value);

    bool isEmpty();

    QString getPeso() const;
    void setPeso(const QString &value);

private:
    QString rut;
    QString nombre;
    QString apellido;
    QString edad;
    QString altura;
    QString sexo;
    QString peso;

};

#endif // PACIENTE_H
