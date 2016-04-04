#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>

namespace Ui {
class MainWindow;
}

class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonStartTest_clicked();
    void on_pushButtonHome_clicked();
    void on_pushButtonResults_clicked();
    void on_pushButtonTest1_clicked();

    void openSerialPort();
    void closeSerialPort();
    void readData();
    void writeData();
    void realtimeDataSlot(double X,double Y);

signals:
    void emitdata(double,double);


private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    SettingsDialog *settings;
    QElapsedTimer timer;
    int samplesNumber;
    QString serialReaded;

    void init_Connections();
    void init_graph();
};

#endif // MAINWINDOW_H
