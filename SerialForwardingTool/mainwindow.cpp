#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMetaEnum>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_serialPortA(new QSerialPort),
    m_serialPortB(new QSerialPort),
    m_isMonitor(false)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/res/ico/icon.ico"));
    setWindowTitle(" 串口转发工具");
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());

    // 获取本机串口名称
    for (const auto &sp : QSerialPortInfo::availablePorts())
    {
        ui->cmbCOMA->addItem(sp.portName());
        ui->cmbCOMB->addItem(sp.portName());
    }

    // 设置波特率选项
    QMetaEnum baudRate = QMetaEnum::fromType<QSerialPort::BaudRate>();
    for (int i = 0; i < baudRate.keyCount(); ++i)
    {
        ui->cmbBaudRateA->addItem(baudRate.key(i));
        ui->cmbBaudRateB->addItem(baudRate.key(i));
    }
    // 设置默认波特率为9600
    ui->cmbBaudRateA->setCurrentText(baudRate.valueToKey(QSerialPort::BaudRate::Baud9600));
    ui->cmbBaudRateB->setCurrentText(baudRate.valueToKey(QSerialPort::BaudRate::Baud9600));

    // 设置数据位选项
    QMetaEnum dataBits = QMetaEnum::fromType<QSerialPort::DataBits>();
    for (int i = 0; i < dataBits.keyCount(); ++i)
    {
        ui->cmbDataBitsA->addItem(dataBits.key(i));
        ui->cmbDataBitsB->addItem(dataBits.key(i));
    }
    // 设置默认数据为选项为8位
    ui->cmbDataBitsA->setCurrentText(dataBits.valueToKey(QSerialPort::DataBits::Data8));
    ui->cmbDataBitsB->setCurrentText(dataBits.valueToKey(QSerialPort::DataBits::Data8));

    // 设置校验位选项
    QMetaEnum parity = QMetaEnum::fromType<QSerialPort::Parity>();
    for (int i = 0; i < parity.keyCount(); ++i)
    {
        ui->cmbParityA->addItem(parity.key(i));
        ui->cmbParityB->addItem(parity.key(i));
    }

    // 设置停止位选项
    QMetaEnum stopBits = QMetaEnum::fromType<QSerialPort::StopBits>();
    for (int i = 0; i < stopBits.keyCount(); ++i)
    {
        ui->cmbStopBitsA->addItem(stopBits.key(i));
        ui->cmbStopBitsB->addItem(stopBits.key(i));
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::openSerialPort 根据用户设置打开串口，返回是否打开成功
 * @return 是否打开成功
 */
bool MainWindow::openSerialPort()
{
    ui->txtOutput->clear();
    if (ui->cmbCOMA->currentText() == ui->cmbCOMB->currentText())
    {
        ui->txtOutput->append("不能选择相同的串口！");
        return false;
    }
    if (ui->cmbCOMA->currentText() == "" || ui->cmbCOMB->currentText() == "")
    {
        ui->txtOutput->append("不能有串口为空");
        return false;
    }

    // 打开串口A
    ui->txtOutput->append("正在尝试打开串口A...");
    // 设置串口名
    m_serialPortA->setPortName(ui->cmbCOMA->currentText());
    // 设置波特率
    m_serialPortA->setBaudRate(static_cast<QSerialPort::BaudRate>(QMetaEnum::fromType<QSerialPort::BaudRate>().keyToValue(ui->cmbBaudRateA->currentText().toStdString().data())));
    // 设置数据位
    m_serialPortA->setDataBits(static_cast<QSerialPort::DataBits>(QMetaEnum::fromType<QSerialPort::DataBits>().keyToValue(ui->cmbDataBitsA->currentText().toStdString().data())));
    // 无流控制
    m_serialPortA->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    // 设置校验位
    m_serialPortA->setParity(static_cast<QSerialPort::Parity>(QMetaEnum::fromType<QSerialPort::Parity>().keyToValue(ui->cmbParityA->currentText().toStdString().data())));
    // 设置停止位
    m_serialPortA->setStopBits(static_cast<QSerialPort::StopBits>(QMetaEnum::fromType<QSerialPort::StopBits>().keyToValue(ui->cmbStopBitsA->currentText().toStdString().data())));
    // 尝试以读写方式打开
    if (!m_serialPortA->open(QIODevice::ReadWrite))
    {
        ui->txtOutput->append("串口A打开失败！错误信息：" + m_serialPortA->errorString());
        return false;
    }
    else
    {
        ui->txtOutput->append("串口A打开成功！");
    }

    // 打开串口B
    ui->txtOutput->append("正在尝试打开串口B...");
    // 设置串口名
    m_serialPortB->setPortName(ui->cmbCOMB->currentText());
    // 设置波特率
    m_serialPortB->setBaudRate(static_cast<QSerialPort::BaudRate>(QMetaEnum::fromType<QSerialPort::BaudRate>().keyToValue(ui->cmbBaudRateB->currentText().toStdString().data())));
    // 设置数据位
    m_serialPortB->setDataBits(static_cast<QSerialPort::DataBits>(QMetaEnum::fromType<QSerialPort::DataBits>().keyToValue(ui->cmbDataBitsB->currentText().toStdString().data())));
    // 无流控制
    m_serialPortB->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    // 设置校验位
    m_serialPortB->setParity(static_cast<QSerialPort::Parity>(QMetaEnum::fromType<QSerialPort::Parity>().keyToValue(ui->cmbParityB->currentText().toStdString().data())));
    // 设置停止位
    m_serialPortB->setStopBits(static_cast<QSerialPort::StopBits>(QMetaEnum::fromType<QSerialPort::StopBits>().keyToValue(ui->cmbStopBitsB->currentText().toStdString().data())));
    // 尝试以读写方式打开
    if (!m_serialPortB->open(QIODevice::ReadWrite))
    {
        ui->txtOutput->append("串口B打开失败！错误信息：" + m_serialPortB->errorString());
        if (m_serialPortA->isOpen())
        {
            m_serialPortA->close();
            ui->txtOutput->append("关闭串口A");
        }
        return false;
    }
    else
    {
        ui->txtOutput->append("串口B打开成功！");
    }
    // 连接串口信号槽
    {
        // 串口A接收数据信号
        connect(m_serialPortA,SIGNAL(readyRead()),this,SLOT(comAReceiveInfo()));
        // 串口B接收数据信号
        connect(m_serialPortB,SIGNAL(readyRead()),this,SLOT(comBReceiveInfo()));
    }
    return true;
}

/**
 * @brief MainWindow::closeSerialPort 关闭串口
 */
void MainWindow::closeSerialPort()
{
    // 断开串口信号槽连接
    {
        // 串口A接收数据信号
        disconnect(m_serialPortA,SIGNAL(readyRead()),this,SLOT(comAReceiveInfo()));
        // 串口B接收数据信号
        disconnect(m_serialPortB,SIGNAL(readyRead()),this,SLOT(comBReceiveInfo()));
    }
    m_serialPortA->close();
    m_serialPortB->close();
}

/**
 * @brief MainWindow::on_btnStart_clicked 开始转发按钮点击事件
 */
void MainWindow::on_btnStart_clicked()
{
    if (ui->btnStart->text() == "开始转发")
    {
        // 尝试打开串口
        if (this->openSerialPort())
        {
            ui->txtOutput->clear();
            ui->txtOutput->append("串口打开成功！开始转发");

            // 禁用所有选择框
            ui->cmbCOMA->setEnabled(false);
            ui->cmbCOMB->setEnabled(false);
            ui->cmbBaudRateA->setEnabled(false);
            ui->cmbBaudRateB->setEnabled(false);
            ui->cmbDataBitsA->setEnabled(false);
            ui->cmbDataBitsB->setEnabled(false);
            ui->cmbParityA->setEnabled(false);
            ui->cmbParityB->setEnabled(false);
            ui->cmbStopBitsA->setEnabled(false);
            ui->cmbStopBitsB->setEnabled(false);
            ui->btnStart->setText("停止转发");
        }
    }
    else
    {
        // 关闭串口
        this->closeSerialPort();
        // 禁用所有选择框
        ui->cmbCOMA->setEnabled(true);
        ui->cmbCOMB->setEnabled(true);
        ui->cmbBaudRateA->setEnabled(true);
        ui->cmbBaudRateB->setEnabled(true);
        ui->cmbDataBitsA->setEnabled(true);
        ui->cmbDataBitsB->setEnabled(true);
        ui->cmbParityA->setEnabled(true);
        ui->cmbParityB->setEnabled(true);
        ui->cmbStopBitsA->setEnabled(true);
        ui->cmbStopBitsB->setEnabled(true);
        ui->btnStart->setText("开始转发");
    }
}

/**
 * @brief MainWindow::comAReceiveInfo COMA接收消息事件
 */
void MainWindow::comAReceiveInfo()
{
    if (m_isMonitor)
    {
        auto data = m_serialPortA->readAll();
        ui->txtOutput->append(data);
        // 将A发的转发给B
        m_serialPortB->write(data);
    }
    else
    {
        m_serialPortB->write(m_serialPortA->readAll());
    }
}

/**
 * @brief MainWindow::comBReceiveInfo COMB接收消息事件
 */
void MainWindow::comBReceiveInfo()
{
    if (m_isMonitor)
    {
        auto data = m_serialPortB->readAll();
        ui->txtOutput->append(data);
        // 将B发的转发给A
        m_serialPortA->write(data);
    }
    else
    {
        m_serialPortA->write(m_serialPortB->readAll());
    }
}

void MainWindow::on_btnClear_clicked()
{
    ui->txtOutput->clear();
}

void MainWindow::on_checkBox_stateChanged(int isChecked)
{
    if (isChecked)
    {
        m_isMonitor = true;
        ui->txtOutput->append("开始监听");
    }
    else
    {
        m_isMonitor = false;
        ui->txtOutput->append("停止监听");
    }
}

void MainWindow::closeEvent(QCloseEvent *)
{
    this->closeSerialPort();
}
