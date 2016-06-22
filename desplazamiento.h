#ifndef DESPLAZAMIENTO_H
#define DESPLAZAMIENTO_H

#include <angulo.h>

class Desplazamiento
{
public:
    struct desplazamientos{
        double Desplazamiento1;
        double Desplazamiento2;
    };

private:
    double Tiempo;
    desplazamientos desplazamientoProyeccion;
    desplazamientos desplazamientoRecorridoCurvo;

public:
    Desplazamiento();
    Desplazamiento(double Tiempo,double despPro1,double despProy2,double despRec1,double despRec2);
    double getTiempo() const;
    void setTiempo(double value);
    desplazamientos getDesplazamientoProyeccion() const;
    void setDesplazamientoProyeccion(const desplazamientos &value);
    desplazamientos getDesplazamientoRecorridoCurvo() const;
    void setDesplazamientoRecorridoCurvo(const desplazamientos &value);

    void calcularDesplazamiento(Angulo *angulo, double alturaDispositivo);
};

#endif // DESPLAZAMIENTO_H
