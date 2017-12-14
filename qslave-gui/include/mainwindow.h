//------------------------------------------------------------------------------
//
//
//
//
//
//------------------------------------------------------------------------------

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include    <QMainWindow>
#include    <QTimer>

#include    "modbus.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
namespace Ui
{
    class MainWindow;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    Ui::MainWindow  *ui;

    ModbusNetwork   *modnet;

    QTimer          *updateTimer;

    int             ports_count;

private slots:

    void updatePortsList();

    void onConnectionRelease();

    void onCleanLogRelease();

    void logPring(QString msg);
};

#endif // MAINWINDOW_H
