#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ctrlfile.h"
#include "qlocalsocket.h"
#include <QTimer>
#include <QDebug>



void MainWindow::receivedMessage()
{

    qDebug() << "Message Received";
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ctrlfile = new CtrlFile();
    ui->setupUi(this);


    timerId = startTimer(10000);

    socket = new QLocalSocket(this);
    //connect(socket, SIGNAL(connected), this, SLOT(receivedMessage()));
    socket->abort();
    socket->connectToServer("/tmp/alarmpipe");
    if(socket->waitForConnected(1000))  qDebug("Connected!");
    if(socket->waitForReadyRead(30000)) qDebug("Bytes Read!");
    char str[5];
    socket->read(str,5);
    qDebug(str);
    socket->write(str,5);
    if(socket->waitForBytesWritten(30000)) qDebug("Bytes Written!");

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
    killTimer(timerId);
    delete ctrlfile;
    delete socket;
    delete ui;
}



void MainWindow::timerEvent(QTimerEvent *event)
{
    if(ctrlfile->CheckFileExists(INIFILENAME)) {
        ctrlfile->ReadIniFile();
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
        qDebug("Read Inifile!");
    }
    else {
        qDebug("Cannot Read Inifile!");
    }
}
