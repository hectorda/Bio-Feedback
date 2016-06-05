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

    int getEdad() const;
    void setEdad(int value);

    double getAltura() const;
    void setAltura(double value);

    QString getSexo() const;
    void setSexo(const QString &value);

    bool isEmpty();

private:
    QString rut;
    QString nombre;
    QString apellido;
    int edad;
    int altura;
    QString sexo;

};

#endif // PACIENTE_H
