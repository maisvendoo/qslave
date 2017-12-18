#include    "mainwindow.h"
#include    "ui_mainwindow.h"

#include    <QComboBox>
#include    <QPushButton>
#include    <QSerialPortInfo>
#include    <QPlainTextEdit>
#include    <QFileInfo>
#include    <QHeaderView>

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
enum
{
    ADDRESS_WIDTH = 59,
    DESC_WIDTH = 250,
    VALUE_WIDTH = 59
};

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
enum
{
    ADDRESS_COL = 0,
    DESC_COL = 1,
    VALUE_COL = 2
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

    loadNetworkConfig("../cfg/lastochka/lastochka.net");

    memoryTableInit(ui->twDiscreteInputs);
    memoryTableInit(ui->twCoils);
    memoryTableInit(ui->twInputRegisters);
    memoryTableInit(ui->twHoldingRedisters);

    connect(ui->lwSlavesList, &QListWidget::currentItemChanged,
            this, &MainWindow::activeSlaveChanged);
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

            logPring("Loading slave " +
                     slave->getDescription()
                     + " id = " + QString::number(slave->getID()));

            // Read slave config file name
            QString slaveCfgName = "";
            if (cfg.getString(slaveNode, "config", slaveCfgName))
            {
                slaveCfgName += ".xml";

                QFileInfo info(cfg_path);

                QString slave_cfg_path = info.path() + "/" + slaveCfgName;

                loadDiscreteValues(slave_cfg_path, COIL, slave);
                loadDiscreteValues(slave_cfg_path, DISCRETE_INPUT, slave);
                loadRegisterValues(slave_cfg_path, HOLDING_REGISTER, slave);
                loadRegisterValues(slave_cfg_path, INPUT_REGISTER, slave);
            }

            modnet->addSlave(slave);

            slaveNode = cfg.getNextSection();
            default_id++;
        }

        logPring("OK: network complete");

        updateSlavesList();
    }
    else
        logPring("ERROR: file " + cfg_path + " is't found");
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::loadDiscreteValues(QString cfg_path,
                                    DataType type,
                                    Slave *slave)
{
    QString sectionName = "";
    quint16 default_address = 0;

    switch (type)
    {
    case COIL:

        sectionName = "Coil";
        default_address = CL_INIT_ADDRESS;
        break;

    case DISCRETE_INPUT:

        sectionName = "DiscreteInput";
        default_address = DI_INIT_ADDRESS;
        break;

    default:

        return;
    }

    CfgReader cfg;

    if (cfg.load(cfg_path))
    {
        QDomNode sec_node = cfg.getFirstSection(sectionName);

        while (!sec_node.isNull())
        {
            data_unit_t<bool> dv;

            int tmp = 0;
            if (cfg.getInt(sec_node, "address", tmp))
                dv.address = static_cast<quint16>(tmp);
            else
                dv.address = default_address;

            if (cfg.getInt(sec_node, "value", tmp))
                dv.value = static_cast<bool>(tmp);
            else
                dv.value = false;

            if (!cfg.getString(sec_node, "description", dv.description))
                dv.description = sectionName + " #" + QString::number(dv.address);

            slave->addDiscreteValue(type, dv);

            logPring(sectionName + " " + dv.description +
                     " address = " + QString::number(dv.address) + " is configered");

            sec_node = cfg.getNextSection();
            default_address++;
        }
    }
    else
        logPring("ERROR: file " + cfg_path + " is't found");
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::loadRegisterValues(QString cfg_path,
                                    DataType type,
                                    Slave *slave)
{
    QString sectionName = "";
    quint16 default_address = 0;

    switch (type)
    {
    case HOLDING_REGISTER:

        sectionName = "HoldingRegister";
        default_address = HL_INIT_ADDRESS;
        break;

    case INPUT_REGISTER:

        sectionName = "InputRegister";
        default_address = IT_INIT_ADDRESS;
        break;

    default:

        return;
    }

    CfgReader cfg;

    if (cfg.load(cfg_path))
    {
        QDomNode sec_node = cfg.getFirstSection(sectionName);

        while (!sec_node.isNull())
        {
            data_unit_t<quint16> rv;

            int tmp = 0;
            if (cfg.getInt(sec_node, "address", tmp))
                rv.address = static_cast<quint16>(tmp);
            else
                rv.address = default_address;

            if (cfg.getInt(sec_node, "value", tmp))
                rv.value = static_cast<quint16>(tmp);
            else
                rv.value = 0;

            if (!cfg.getString(sec_node, "description", rv.description))
                rv.description = sectionName + " #" + QString::number(rv.address);

            slave->addRegisterValue(type, rv);

            logPring(sectionName + " " + rv.description +
                     " address = " + QString::number(rv.address) + " is configered");

            sec_node = cfg.getNextSection();
            default_address++;
        }
    }
    else
        logPring("ERROR: file " + cfg_path + " is't found");
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::memoryTableInit(QTableWidget *tw)
{
    tw->setColumnCount(3);
    tw->setColumnWidth(ADDRESS_COL, ADDRESS_WIDTH);
    tw->setColumnWidth(DESC_COL, DESC_WIDTH);
    tw->setColumnWidth(VALUE_COL, VALUE_WIDTH);

    tw->setHorizontalHeaderLabels(QStringList() <<
                                  tr("Addr") <<
                                  tr("Description") <<
                                  tr("Value"));

    tw->verticalHeader()->setVisible(false);
    tw->verticalHeader()->setDefaultSectionSize(18);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Slave *MainWindow::getSlaveByIndex(int idx)
{
    QMap<int, Slave *>::iterator it = modnet->getSlaves().begin();
    int i = 0;

    for (i = 0; it != modnet->getSlaves().end(); ++it, ++i)
    {
        if (i == idx)
            return it.value();
    }

    return nullptr;
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

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::updateSlavesList()
{
    QMap<int, Slave *> slaves = modnet->getSlaves();
    QMap<int, Slave *>::iterator it;

    for (it = slaves.begin(); it != slaves.end(); ++it)
    {
        QString item = "ID = " + QString::number(it.key()) + " " +
                it.value()->getDescription();

        ui->lwSlavesList->insertItem(it.key(), item);
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::activeSlaveChanged(QListWidgetItem *cur, QListWidgetItem *prev)
{
    Q_UNUSED(prev)

    int idx = cur->listWidget()->row(cur);

    Slave *slave = getSlaveByIndex(idx);

    if (slave == nullptr)
        return;

    updateSlaveOutputValues(slave->getID());
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::updateSlaveOutputValues(quint8 id)
{
    Slave *slave = modnet->getSlaves()[id];

    ui->twCoils->setRowCount(0);
    ui->twHoldingRedisters->setRowCount(0);

    for (int i = 0; i < slave->getCoilsCount(); i++)
    {
        ui->twCoils->insertRow(i);

        ui->twCoils->setItem(i,
                             ADDRESS_COL,
                             new QTableWidgetItem(QString::number(CL_INIT_ADDRESS + i)));

        ui->twCoils->setItem(i,
                             DESC_COL,
                             new QTableWidgetItem(slave->getCoilDescription(CL_INIT_ADDRESS + i)));

        int value = static_cast<int>(slave->getCoil(CL_INIT_ADDRESS + i));

        ui->twCoils->setItem(i,
                             VALUE_COL,
                             new QTableWidgetItem(QString::number(value)));
    }

    for (int i = 0; i < slave->getHoldingRegistersCount(); i++)
    {
        ui->twHoldingRedisters->insertRow(i);

        ui->twHoldingRedisters->setItem(i,
                             ADDRESS_COL,
                             new QTableWidgetItem(QString::number(HL_INIT_ADDRESS + i)));

        ui->twHoldingRedisters->setItem(i,
                             DESC_COL,
                             new QTableWidgetItem(slave->getHoldingRegisterDescription(HL_INIT_ADDRESS + i)));

        int value = static_cast<int>(slave->getHoldingRegister(HL_INIT_ADDRESS + i));

        ui->twHoldingRedisters->setItem(i,
                             VALUE_COL,
                             new QTableWidgetItem(QString::number(value)));
    }
}
