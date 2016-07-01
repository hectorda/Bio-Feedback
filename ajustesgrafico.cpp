#include "ajustesgrafico.h"
#include "ui_ajustesgrafico.h"

AjustesGrafico::AjustesGrafico(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AjustesGrafico)
{
    inicializar();
    llenarParametrosComboBox();
    conexiones();
    aplicar();
    graficarObjetivo(ajustesActuales.RadioObjetivo,ajustesActuales.colorObjetivoSinMarcar);
    graficarRepresentacionGrafico(ajustesActuales.RadioInterior,ajustesActuales.RadioExterior);
    this->adjustSize();
}

AjustesGrafico::~AjustesGrafico()
{
    delete ui;
}

AjustesGrafico::Ajustes AjustesGrafico::getAjustes()
{
    return ajustesActuales;
}

void AjustesGrafico::inicializar()
{
    ui->setupUi(this);
    objetivo=new QCPItemEllipse(ui->qCustomPlotObjetivo);
    circuloExterior= new QCPItemEllipse(ui->qCustomRepresentacionGrafico);
    circuloInterior= new QCPItemEllipse(ui->qCustomRepresentacionGrafico);
    ui->qCustomPlotObjetivo->installEventFilter(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    // activate top and right axes, which are invisible by default:
    ui->qCustomPlotObjetivo->xAxis2->setVisible(true);
    ui->qCustomPlotObjetivo->yAxis2->setVisible(true);
    ui->qCustomRepresentacionGrafico->xAxis2->setVisible(true);
    ui->qCustomRepresentacionGrafico->yAxis2->setVisible(true);
}

void AjustesGrafico::conexiones()
{
    connect(ui->pushButtonCancelar,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->pushButtonOK,SIGNAL(clicked()),this,SLOT(aplicar()));
}

void AjustesGrafico::llenarParametrosComboBox()
{
    ui->comboBoxColorObjetivosMarcados->blockSignals(true);
    ui->comboBoxColorObjetivosSinMarcar->blockSignals(true);
    QStringList colorNames = QColor::colorNames();

    for (int i = 0; i < colorNames.size(); ++i) {
        QColor color(colorNames[i]);

        ui->comboBoxColorObjetivosSinMarcar->insertItem(i, colorNames[i]);
        ui->comboBoxColorObjetivosSinMarcar->setItemData(i, color, Qt::DecorationRole);

        ui->comboBoxColorObjetivosMarcados->insertItem(i, colorNames[i]);
        ui->comboBoxColorObjetivosMarcados->setItemData(i, color, Qt::DecorationRole);
    }
    ui->comboBoxColorObjetivosSinMarcar->setCurrentIndex(119); //Se setea el color rojo
    ui->comboBoxColorObjetivosMarcados->setCurrentIndex(54);   //Se setea el color verde

    for (int var = 0; var < 5; ++var) {
        ui->comboBoxFPS->addItem(QString::number(12*(var+1)),var);
    }
    ui->comboBoxFPS->setCurrentIndex(4); //Para 60 FPS

    ui->comboBoxColorObjetivosMarcados->blockSignals(false);
    ui->comboBoxColorObjetivosSinMarcar->blockSignals(false);

    ui->comboBoxUnidadGrafico->addItem(QString("Centímetros"),0);
    ui->comboBoxUnidadGrafico->addItem(QString("Grados"),1);

    ui->comboBoxCalculoDesplazamiento->addItem(QString("Proyeccion"),0);
    ui->comboBoxCalculoDesplazamiento->addItem(QString("Recorrido Curvo"),1);


}

void AjustesGrafico::aplicar()
{
    if(validar()){
        ajustesActuales.RadioInterior=ui->spinBoxRInterior->value();
        ajustesActuales.RadioExterior=ui->spinBoxRExterior->value();
        ajustesActuales.RadioObjetivo=ui->spinBoxRObjetivo->value();
        ajustesActuales.FPS=ui->comboBoxFPS->currentText().toDouble();
        ajustesActuales.Unidad=ui->comboBoxUnidadGrafico->currentText().trimmed().toLower();
        ajustesActuales.CalculoDesplazamiento=ui->comboBoxCalculoDesplazamiento->currentText().trimmed().toLower();

        const QColor colorSinMarcar=qvariant_cast<QColor>(ui->comboBoxColorObjetivosSinMarcar->itemData(ui->comboBoxColorObjetivosSinMarcar->currentIndex(), Qt::DecorationRole));
        const QColor colorMarcado=qvariant_cast<QColor>(ui->comboBoxColorObjetivosMarcados->itemData(ui->comboBoxColorObjetivosMarcados->currentIndex(), Qt::DecorationRole));
        ajustesActuales.colorObjetivoSinMarcar = colorSinMarcar;
        ajustesActuales.colorObjetivoMarcado = colorMarcado;
        hide();
    }
}

void AjustesGrafico::graficarObjetivo(const int rObjetivo,const QColor color){

    objetivo->topLeft->setCoords(-rObjetivo,rObjetivo);
    objetivo->bottomRight->setCoords(rObjetivo,-rObjetivo);
    objetivo->setBrush(QBrush(color));
    ui->qCustomPlotObjetivo->replot();

}

void AjustesGrafico::relacionAspecto(QCustomPlot *grafico)
{
    const int w=grafico->width();
    const int h=grafico->height();
    const QRect rect=grafico->geometry();

    if(w>h)
        grafico->setGeometry(rect.x()+((w-h)/2),rect.y(),h,h);
    else
        grafico->setGeometry(rect.x(),rect.y()+((h-w)/2),w,w);

}

void AjustesGrafico::graficarRepresentacionGrafico(const int rInterior,const int rExterior)
{
    ui->qCustomRepresentacionGrafico->clearFocus();
    ui->qCustomRepresentacionGrafico->clearGraphs();
    ui->qCustomRepresentacionGrafico->clearItems();
    ui->qCustomRepresentacionGrafico->clearPlottables();
    circuloExterior->topLeft->setCoords(-rExterior,rExterior);
    circuloExterior->bottomRight->setCoords(rExterior,-rExterior);
    circuloExterior->setBrush(QBrush(Qt::yellow));

    circuloInterior->topLeft->setCoords(-rInterior,rInterior);
    circuloInterior->bottomRight->setCoords(rInterior,-rInterior);

    const int range=rExterior+10;
    ui->qCustomRepresentacionGrafico->xAxis->setRange(-range,range);
    ui->qCustomRepresentacionGrafico->yAxis->setRange(-range,range);
    ui->qCustomRepresentacionGrafico->xAxis2->setRange(-range,range);
    ui->qCustomRepresentacionGrafico->yAxis2->setRange(-range,range);
    //Para que sea representado en los mismos rangos el grafico del objetivo
    ui->qCustomPlotObjetivo->xAxis->setRange(-range,range);
    ui->qCustomPlotObjetivo->yAxis->setRange(-range,range);
    ui->qCustomPlotObjetivo->xAxis2->setRange(-range,range);
    ui->qCustomPlotObjetivo->yAxis2->setRange(-range,range);
    ui->qCustomPlotObjetivo->replot();

    ui->qCustomRepresentacionGrafico->replot();
}

bool AjustesGrafico::event(QEvent *event)
{
    if (event->type() == QEvent::Resize) {
        relacionAspecto(ui->qCustomPlotObjetivo);
        relacionAspecto(ui->qCustomRepresentacionGrafico);
    }

    if (event->type() == QEvent::Show){
        relacionAspecto(ui->qCustomPlotObjetivo);
        relacionAspecto(ui->qCustomRepresentacionGrafico);
    }

    return QWidget::event(event);
}

bool AjustesGrafico::validar()
{
    const int RadioExterior=ui->spinBoxRExterior->value();
    const int RadioInterior=ui->spinBoxRInterior->value();
    const int RadioObjetivo=ui->spinBoxRObjetivo->value();
    bool val=true;

    if(ui->comboBoxColorObjetivosMarcados->currentText()=="white" || ui->comboBoxColorObjetivosSinMarcar->currentText()=="white")
    {
        QMessageBox::warning(this,"Color No Seleccionable","Los objetivos por defecto al parpadear son blancos");
        val=false;
    }
    if(ui->comboBoxColorObjetivosMarcados->currentText()=="yellow" || ui->comboBoxColorObjetivosSinMarcar->currentText()=="yellow")
    {
        QMessageBox::warning(this,"Color No Seleccionable","EL gráfico principal es de color amarillo.\nLos objetivos no seran demasiado visibles");
        val=false;
    }
    if(RadioExterior < RadioInterior){
        QMessageBox::warning(this,"Radio Exterior muy Pequeño","Radio Exterior debe ser debe ser mayor al Radio Interior");
        val=false;
    }
    if(RadioObjetivo>=RadioExterior){
        QMessageBox::warning(this,"Radio Objetivo muy grande","Radio Objetivo debe ser menor al Radio Exterior");
        val=false;
    }
    if(ui->comboBoxColorObjetivosMarcados->currentIndex()==ui->comboBoxColorObjetivosSinMarcar->currentIndex()){
        QMessageBox::warning(this,"Colores iguales","Se deben seleccionar colores distintos para los estados de los objetivos");
        val=false;
    }
    return val;
}

void AjustesGrafico::on_spinBoxRObjetivo_valueChanged(int arg1)
{
    if(validar())
    {
        const QColor sinMarcar=qvariant_cast<QColor>(ui->comboBoxColorObjetivosSinMarcar->itemData(ui->comboBoxColorObjetivosSinMarcar->currentIndex(), Qt::DecorationRole));
        graficarObjetivo(arg1,sinMarcar);
    }
}

void AjustesGrafico::on_spinBoxRExterior_valueChanged(int arg1)
{
    if(validar())
    {
        const int rInterior=ui->spinBoxRInterior->value();
        graficarRepresentacionGrafico(rInterior,arg1);
    }
}

void AjustesGrafico::on_spinBoxRInterior_valueChanged(int arg1)
{
    if(validar())
    {
        const int rEnterior=ui->spinBoxRExterior->value();
        graficarRepresentacionGrafico(arg1,rEnterior);
    }
}

void AjustesGrafico::on_comboBoxColorObjetivosSinMarcar_currentIndexChanged(int index)
{
    if(validar()){
        const QColor sinMarcar=qvariant_cast<QColor>(ui->comboBoxColorObjetivosSinMarcar->itemData(index, Qt::DecorationRole));
        graficarObjetivo(ui->spinBoxRObjetivo->value(),sinMarcar);
     }
}

void AjustesGrafico::on_comboBoxColorObjetivosMarcados_currentIndexChanged(int index)
{
    (void) index;
    validar();
}

bool AjustesGrafico::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseMove && obj == ui->qCustomPlotObjetivo)
    {
        QMouseEvent *mouseEvent = (QMouseEvent *)event;
        int x = this->ui->qCustomPlotObjetivo->xAxis->pixelToCoord(mouseEvent->pos().x());
        int y = this->ui->qCustomPlotObjetivo->yAxis->pixelToCoord(mouseEvent->pos().y());
        const double distancia=qSqrt(qPow(x,2)+qPow(y,2));

        if(distancia<=ui->spinBoxRObjetivo->value())
        {
            const QColor marcado=qvariant_cast<QColor>(ui->comboBoxColorObjetivosMarcados->itemData(ui->comboBoxColorObjetivosMarcados->currentIndex(), Qt::DecorationRole));
            graficarObjetivo(ui->spinBoxRObjetivo->value(),marcado);
        }
        else
        {
            const QColor sinMarcar=qvariant_cast<QColor>(ui->comboBoxColorObjetivosSinMarcar->itemData(ui->comboBoxColorObjetivosSinMarcar->currentIndex(), Qt::DecorationRole));
            graficarObjetivo(ui->spinBoxRObjetivo->value(),sinMarcar);
        }
    }
    return QWidget::eventFilter(obj, event);
}

void AjustesGrafico::on_comboBoxUnidadGrafico_currentIndexChanged(const QString &arg1)
{
    if(arg1.toLower().trimmed().contains("grados"))
        ui->comboBoxCalculoDesplazamiento->hide();
    else
        ui->comboBoxCalculoDesplazamiento->show();
}
