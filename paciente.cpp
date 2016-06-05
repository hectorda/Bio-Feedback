#include "paciente.h"

Paciente::Paciente()
{

}

QString Paciente::getNombre() const
{
    return nombre;
}

void Paciente::setNombre(const QString &value)
{
    nombre = value;
}

QString Paciente::getRut() const
{
    return rut;
}

void Paciente::setRut(const QString &value)
{
    rut = value;
}

QString Paciente::getApellido() const
{
    return apellido;
}

void Paciente::setApellido(const QString &value)
{
    apellido = value;
}

double Paciente::getAltura() const
{
    return altura;
}

void Paciente::setAltura(double value)
{
    altura = value;
}

bool Paciente::isEmpty()
{
    if (rut.isEmpty())
        return true;
    return false;
}

int Paciente::getEdad() const
{
    return edad;
}

void Paciente::setEdad(int value)
{
    edad = value;
}
