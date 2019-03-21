#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QTimer>
#include "qlocalsocket.h"
#include "ctrlfile.h"


#define MAXFIELD    16
#define MINFIELDLEN  7

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    char readbuff[4096];
    char writebuff[255];
    bool prearmed;
public slots:
    void SetArmedHandler();
    void SetDisarmHandler();
    void SetClearlogHandler();
    void ActionSetHandler();
    void SetXbeeHandler();
    void ConfigurationTimerHandler();
    void SocketTimerHandler();
    void SystemTimerHandler();
    void ArmTimerHandler();
    void SocketConnectedHandler();
    void SocketDisconnectedHandler();
    void AboutBox();
private:
    Ui::MainWindow *ui;
    QDateTime current;
    QTimer *k_timer;
    QTimer *s_timer;
    QTimer *sys_timer;
    QTimer *arm_timer;
    QLocalSocket *socket;
    CtrlFile     *ctrlfile;
protected:
    void MessageInterpreter();
    void ReceivedMessage();
private slots:

};

#endif // MAINWINDOW_H
