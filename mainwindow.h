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

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    SettingsDialog *settings;
    void inicializar();
    void conexiones();
};

#endif // MAINWINDOW_H
