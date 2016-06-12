#include "paciente.h"

Paciente::Paciente()
{

}

QString Paciente::getRut() const
{
    return rut;
}

void Paciente::setRut(const QString &value)
{
    rut = value;
}

QString Paciente::getNombre() const
{
    return nombre;
}

void Paciente::setNombre(const QString &value)
{
    nombre = value;
}

QString Paciente::getApellido() const
{
    return apellido;
}

void Paciente::setApellido(const QString &value)
{
    apellido = value;
}

QString Paciente::getAltura() const
{
    return altura;
}

void Paciente::setAltura(QString value)
{
    altura = value;
}

bool Paciente::isEmpty()
{
    if (rut.isEmpty())
        return true;
    return false;
}

QString Paciente::getSexo() const
{
    return sexo;
}

void Paciente::setSexo(const QString &value)
{
    sexo = value;
}

QString Paciente::getEdad() const
{
    return edad;
}

void Paciente::setEdad(QString value)
{
    edad = value;
}

