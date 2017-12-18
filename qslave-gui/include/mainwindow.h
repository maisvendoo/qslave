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
#include    <QListWidget>
#include    <QTableWidget>

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

    void loadNetworkConfig(QString cfg_path);

    void loadDiscreteValues(QString cfg_path, DataType type, Slave *slave);

    void loadRegisterValues(QString cfg_path, DataType type, Slave *slave);

    void memoryTableInit(QTableWidget *tw);

    Slave *getSlaveByIndex(int idx);

private slots:

    void updatePortsList();

    void onConnectionRelease();

    void onCleanLogRelease();

    void logPring(QString msg);

    void updateSlavesList();

    void activeSlaveChanged(QListWidgetItem *cur, QListWidgetItem *prev);

    void updateSlaveOutputValues(quint8 id);
};

#endif // MAINWINDOW_H
