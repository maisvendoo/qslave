#include    "mainwindow.h"
#include    "ui_mainwindow.h"

#include    <QComboBox>
#include    <QPushButton>
#include    <QSerialPortInfo>
#include    <QPlainTextEdit>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
enum
{
    PORT_LIST_UPDATE_INTERVAL = 100
};

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Modbus virtual network creation
    modnet = new ModbusNetwork(this);

    connect(modnet, &ModbusNetwork::logPrint, this, &MainWindow::logPring);

    // Timer for update ports list creation
    updateTimer = new QTimer(this);
    updateTimer->setInterval(PORT_LIST_UPDATE_INTERVAL);

    ports_count = 0;

    connect(updateTimer, &QTimer::timeout,
            this, &MainWindow::updatePortsList);

    updateTimer->start();

    // Connection button config
    connect(ui->pbConnect, &QPushButton::released,
            this, &MainWindow::onConnectionRelease);

    // Clean log button config
    connect(ui->pbCleanLog, &QPushButton::released,
            this, &MainWindow::onCleanLogRelease);

    Slave *slave = new Slave();
    slave->setID(1);

    slave->setMemoryConfig(COIL, 12);
    slave->setCoil(16, true);
    slave->setCoil(27, true);

    slave->setMemoryConfig(DISCRETE_INPUT, 12);
    slave->setDiscreteInput(8, true);

    slave->setMemoryConfig(INPUT_REGISTER, 12);
    slave->setInputRegister(5, 1024);

    slave->setMemoryConfig(HOLDING_REGISTER, 12);
    slave->setHoldingRegisters(8, 2048);

    modnet->addSlave(slave);

    slave = new Slave();
    slave->setID(4);
    slave->setMemoryConfig(COIL, 12);
    slave->setCoil(17, true);
    slave->setCoil(25, true);
    modnet->addSlave(slave);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::updatePortsList()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    if (ports.count() != ports_count)
    {
        ports_count = ports.count();
        ui->cbPortName->clear();

        for (int i = 0; i < ports_count; i++)
            ui->cbPortName->addItem(ports.at(i).portName());
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::onConnectionRelease()
{
    if (modnet->isConnected())
    {
        modnet->closeConnection();

        ui->pbConnect->setText("Connect");
        logPring("OK: Device is disconnected");
    }
    else
    {
        serial_config_t sp_config;

        sp_config.portName = ui->cbPortName->currentText();
        sp_config.baudrate = ui->cbBaubrate->currentText().toInt();
        sp_config.dataBits = ui->cbDataBits->currentText().toInt();
        sp_config.stopBits = ui->cbStopBits->currentText().toInt();
        sp_config.parity = ui->cbParity->currentText();

        modnet->init(sp_config);
        modnet->openConnection();

        ui->pbConnect->setText("Disconnect");
        logPring("OK: Device is connected");
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::onCleanLogRelease()
{
    ui->ptSystemLog->clear();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::logPring(QString msg)
{
    ui->ptSystemLog->appendPlainText(msg);
}
