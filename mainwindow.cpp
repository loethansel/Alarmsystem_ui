#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ctrlfile.h"
#include "qlocalsocket.h"
#include <QTimer>
#include <QDebug>
#include <QHostAddress>
#include <QNetworkInterface>

static bool connected = false;

void MainWindow::receivedMessage()
{
    qDebug() << "Message Received";
}

void testfunction()
{
    qDebug() << "Message Received";
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // main class declarations
    ctrlfile = new CtrlFile();
    ui->setupUi(this);
    // get IP-Address from eth0
    QNetworkInterface eth1Ip = QNetworkInterface::interfaceFromName("eth0");
    QList<QNetworkAddressEntry> entries = eth1Ip.addressEntries();
    if (!entries.isEmpty()) {
        QNetworkAddressEntry entry = entries.first();
        qDebug() << entry.ip();
         ui->ipaddress->setText("IP-Address: " + entry.ip().toString());
    }
    // intervalltimer for configuration update
    k_timer = new QTimer(this);
    connect(k_timer, SIGNAL(timeout()), this, SLOT(ConfigurationTimerHandler())); 
    k_timer->start(10000);
    // unix pipe to communicate to mainproc
    socket = new QLocalSocket(this);
    connect(socket, SIGNAL(connected())   , this, SLOT(SocketConnectedHandler()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(SocketDisconnectedHandler()));
    // intervalltimer for socket connection
    s_timer = new QTimer(this);
    connect(s_timer, SIGNAL(timeout()), this, SLOT(SocketTimerHandler()));
    s_timer->start(1000);

/*
    if(socket->waitForReadyRead(30000)) qDebug("Bytes Read!");
    char str[5];
    socket->read(str,5);
    qDebug(str);
    socket->write(str,5);
    if(socket->waitForBytesWritten(30000)) qDebug("Bytes Written!");
*/

    //socket->setReadBufferSize(512000);
    //socket.waitForConnected();
    //socket.open(QIODevice::ReadWrite);
    //char str[5];
    //while(1) {
    //    socket.write("test",5);
    //    socket.read(str,5);

    //};
    //socket.close();
}

MainWindow::~MainWindow()
{
    k_timer->stop();
    delete k_timer;
    s_timer->stop();
    delete s_timer;
    socket->close();
    delete ctrlfile;
    delete socket;
    delete ui;
}


void MainWindow::SocketConnectedHandler()
{
   connected = true;
   qDebug("connected event!");
}
void MainWindow::SocketDisconnectedHandler()
{
    connected = false;
    qDebug("not connected event!");
}

void MainWindow::SocketTimerHandler()
{
    if(!connected) {
        //connect(socket, SIGNAL(connected), this, SLOT(receivedMessage()));
        socket->abort();
        socket->connectToServer("/tmp/alarmpipe");
        if(socket->waitForConnected(500)) {
            connected = true;
            qDebug("connected!");
        } else {
            connected = false;
            qDebug("not connected!");
        }
    }
    if(connected) {
        // write data request to host application
        socket->write("getval",6);
        if(socket->waitForBytesWritten(3000)) qDebug("Bytes Written!");
        // wait for response from host application
        if(socket->waitForReadyRead(30000)) qDebug("Bytes Read!");
        char hchr[10];
        socket->read(hchr,10);
        string str;
        str = hchr;
        ui->temperatur->setText(QString::fromStdString(str)+"°C");
    }
}

void MainWindow::ConfigurationTimerHandler()
{
    if(ctrlfile->CheckFileExists(INIFILENAME)) {
        if(ctrlfile->ReadIniFile()) {
            ui->alarmtime->setText(QString::fromStdString(ctrlfile->ini.ALARM.alarmtime));
            ui->autoalarm->setText(QString::fromStdString(ctrlfile->ini.ALARM.autoalarm));
            ui->autocount->setText(QString::fromStdString(ctrlfile->ini.ALARM.autocnt));
            ui->autotime->setText(QString::fromStdString(ctrlfile->ini.ALARM.autotime));
            ui->infotime->setText(QString::fromStdString(ctrlfile->ini.ALARM.infotime));
            ui->linie1text->setText(QString::fromStdString(ctrlfile->ini.ALARM_LINE.linetext[0]));
            ui->linie2text->setText(QString::fromStdString(ctrlfile->ini.ALARM_LINE.linetext[1]));
            ui->linie3text->setText(QString::fromStdString(ctrlfile->ini.ALARM_LINE.linetext[2]));
            ui->linie4text->setText(QString::fromStdString(ctrlfile->ini.ALARM_LINE.linetext[3]));
            ui->creditwarnlevel->setText(QString::fromStdString(ctrlfile->ini.GSM.creditwarnlevel));
            ui->alarmnummer1->setText(QString::fromStdString(ctrlfile->ini.TEL_NUM.name[0]));
            ui->alarmnummer2->setText(QString::fromStdString(ctrlfile->ini.TEL_NUM.name[1]));
            ui->alarmnummer3->setText(QString::fromStdString(ctrlfile->ini.TEL_NUM.name[2]));
            qDebug("Read Inifile!");
        }
        else qDebug("error reading Inifile!");
    }
    else qDebug("Cannot Read Inifile!");
}
