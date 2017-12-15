#include    "mainwindow.h"
#include    "ui_mainwindow.h"

#include    <QComboBox>
#include    <QPushButton>
#include    <QSerialPortInfo>
#include    <QPlainTextEdit>

#include    "CfgReader.h"

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

    loadNetworkConfig("../cfg/lastochka/lastochka.xml");
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
void MainWindow::loadNetworkConfig(QString cfg_path)
{
    CfgReader cfg;

    if (cfg.load(cfg_path))
    {
        QString sectionName = "Slave";

        QDomNode slaveNode = cfg.getFirstSection(sectionName);

        quint8 default_id = 1;

        while (!slaveNode.isNull())
        {
            Slave *slave = new Slave();

            // Read slave ID
            int tmp = 0;
            if (cfg.getInt(slaveNode, "id", tmp))
                slave->setID(static_cast<quint8>(tmp));
            else
                slave->setID(default_id);

            // Read description
            QString description = "";

            if (cfg.getString(slaveNode, "Description", description))
                slave->setDescription(description);
            else
                slave->setDescription("Slave #" + QString::number(slave->getID()));

            // Read slave config file name
            QString slaveCfgName = "";
            if (cfg.getString(slaveNode, "config", slaveCfgName))
            {
                slaveCfgName += ".xml";
            }

            modnet->addSlave(slave);

            slaveNode = cfg.getNextSection();
            default_id++;
        }
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::loadDiscreteValues(QString cfg_path,
                                    DataType type,
                                    Slave *slave)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::loadRegisterValues(QString cfg_path,
                                    DataType type,
                                    Slave *slave)
{

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
