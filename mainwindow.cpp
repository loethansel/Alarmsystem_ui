#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ctrlfile.h"
#include "qlocalsocket.h"
#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QMessageBox>

static bool connected = false;

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
    // get Date and Time
    current = QDateTime::currentDateTime();
    ui->datetime->setText(current.toString("yyyy-MM-dd hh:mm:ss t"));
    // intervalltimer for configuration update
    k_timer = new QTimer(this);
    connect(k_timer, SIGNAL(timeout()), this, SLOT(ConfigurationTimerHandler())); 
    k_timer->start(1000);
    // unix pipe to communicate to mainproc
    socket = new QLocalSocket(this);
    connect(socket, SIGNAL(connected())   , this, SLOT(SocketConnectedHandler()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(SocketDisconnectedHandler()));
    // intervalltimer for socket connection
    s_timer = new QTimer(this);
    connect(s_timer, SIGNAL(timeout()), this, SLOT(SocketTimerHandler()));
    s_timer->start(1000);
    // intervalltimer for system
    sys_timer = new QTimer(this);
    connect(sys_timer, SIGNAL(timeout()), this, SLOT(SystemTimerHandler()));
    sys_timer->start(1000);
    // Armtimer for Alarm
    arm_timer = new QTimer(this);
    connect(arm_timer, SIGNAL(timeout()), this, SLOT(ArmTimerHandler()));
    prearmed = false;
    // push buttons
    connect(ui->aktiv     ,SIGNAL(clicked()), this, SLOT (SetArmedHandler()));
    connect(ui->inaktiv   ,SIGNAL(clicked()), this, SLOT (SetDisarmHandler()));
    connect(ui->clearlog  ,SIGNAL(clicked()), this, SLOT (SetClearlogHandler()));
    connect(ui->xbee      ,SIGNAL(clicked()), this, SLOT (SetXbeeHandler()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(AboutBox()));
    connect(ui->actionSet_Button, SIGNAL(triggered()), this, SLOT(ActionSetHandler()));
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

void MainWindow::AboutBox()
{
    QMessageBox msgBox;
    msgBox.setText("Alarmsystem by Flugsport-Berlin\n        Author: Ralf Pandel");
    msgBox.exec();
}

void MainWindow::SystemTimerHandler()
{
    if(!prearmed) {
        ctrlfile->ReadSystemArmed();
        if(ctrlfile->armed_from_file) { ui->aktiv->setStyleSheet("background-color: red"); qDebug("read armed!"); }
        else                          { ui->aktiv->setStyleSheet("background-color: ");    qDebug("read disarmed!"); }
    }
    // get Date and Time
    current = QDateTime::currentDateTime();
    ui->datetime->setText(current.toString("yyyy-MM-dd hh:mm:ss t"));
}

void MainWindow::ArmTimerHandler()
{
    prearmed = false;
    ctrlfile->WriteSystemArmed(true);
    ui->aktiv->setStyleSheet("background-color: red");
    arm_timer->stop();
}

void MainWindow::SetArmedHandler()
{
    ui->aktiv->setStyleSheet("background-color: yellow");
    arm_timer->start(60000);
    prearmed = true;
    qDebug("set armed!");
}
void MainWindow::SetDisarmHandler()
{
    ui->aktiv->setStyleSheet("background-color: ");
    ctrlfile->WriteSystemArmed(false);
    ui->inaktiv->setEnabled(false);
    ui->clearlog->setEnabled(false);
    ui->xbee->setEnabled(false);
    arm_timer->stop();
    qDebug("set disarmed!");
}
void MainWindow::SetClearlogHandler()
{
fstream ofs;

    ofs.open(LOGFILENAME, ios::out | ios::trunc);
    ofs.close();
    qDebug("clear logfile!");
}
void MainWindow::SetXbeeHandler()
{
    if(connected) {
        // write data request to host application
        socket->flush();
        socket->write("xbeetest\0",9);
        if(socket->waitForBytesWritten(1000)) qDebug("XBeetest Written!");
    }
    qDebug("set xbeetest!");
}

void MainWindow::ActionSetHandler()
{
   ui->inaktiv->setEnabled(true);
   ui->clearlog->setEnabled(true);
   ui->xbee->setEnabled(true);
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

void MainWindow::MessageInterpreter()
{
string buffstr(readbuff);
string substring[16];
int posa, posb, i;

    for(i=0;i<MAXFIELD;i++) {
        if(buffstr.length() < MINFIELDLEN) break;
        posa = buffstr.find("=");
        posb = buffstr.find("&");
        substring[i] = buffstr.substr(posa+1,posb-posa-1);
        buffstr.erase(0,posb+1);
        qDebug(substring[i].c_str());
    }
    //     qDebug(substring.c_str());
    ui->systemprogramm->setText(QString::fromStdString(substring[0]));
    ui->gsmmodul->setText(QString::fromStdString(substring[1]));
    ui->bme680modul->setText(QString::fromStdString(substring[2]));
    ui->socketclient->setText(QString::fromStdString(substring[3]));
    ui->serialrelais->setText(QString::fromStdString(substring[4]));
    ui->xbeemodul->setText(QString::fromStdString(substring[5]));
    ui->alarmstatus->setText(QString::fromStdString(substring[6]));
    ui->guthaben->setText(QString::fromStdString(substring[7]));
    ui->rssipegel->setText(QString::fromStdString(substring[8]));
    ui->temperatur->setText(QString::fromStdString(substring[9]));
    ui->luftdruck->setText(QString::fromStdString(substring[10]));
    ui->luftfeuchtigkeit->setText(QString::fromStdString(substring[11]));
    ui->linie1val->display(QString::fromStdString(substring[12]));
    ui->linie2val->display(QString::fromStdString(substring[13]));
    ui->linie3val->display(QString::fromStdString(substring[14]));
    ui->linie4val->display(QString::fromStdString(substring[15]));
}

void MainWindow::SocketTimerHandler()
{
    s_timer->stop();
    if(!connected) {
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
        socket->flush();
        socket->write("getvalue\0",9);
        if(socket->waitForBytesWritten(100)) qDebug("getvalue Written!");
        // wait for response from host application
        if(socket->waitForReadyRead(100)) qDebug("Bytes Read!");
        int len = socket->bytesAvailable();
        socket->read(readbuff,len);
        MessageInterpreter();
    }
    s_timer->start(1000);
}

void MainWindow::ConfigurationTimerHandler()
{
stringstream ss;

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
            ss.str(""); ss.clear();
            ss << ctrlfile->ini.OUT_ACTIVE.out[0] << ","
               << ctrlfile->ini.OUT_ACTIVE.out[1] << ","
               << ctrlfile->ini.OUT_ACTIVE.out[2] << ","
               << ctrlfile->ini.OUT_ACTIVE.out[3];
            ui->relaisausgaenge->setText(QString::fromStdString(ss.str()));
            ss.str(""); ss.clear();
            ss << ctrlfile->ini.ALARM_LINE.lineactv[0] << ","
               << ctrlfile->ini.ALARM_LINE.lineactv[1] << ","
               << ctrlfile->ini.ALARM_LINE.lineactv[2] << ","
               << ctrlfile->ini.ALARM_LINE.lineactv[3];
            ui->linieaktiv->setText(QString::fromStdString(ss.str()));
            ui->alarmmail1->setText(QString::fromStdString(ctrlfile->ini.EMAIL.alarmmail[0]));
            ui->alarmmail2->setText(QString::fromStdString(ctrlfile->ini.EMAIL.alarmmail[1]));
            ui->servicemail->setText(QString::fromStdString(ctrlfile->ini.EMAIL.servicemail));
            ui->socketclienthost->setText(QString::fromStdString(ctrlfile->ini.TSPEAK.hostname));
            qDebug("Read Inifile!");
        }
        else qDebug("error reading Inifile!");
    }
    else qDebug("Cannot Read Inifile!");
}

