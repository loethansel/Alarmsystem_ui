#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qlocalsocket.h"
#include "ctrlfile.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void ConfigurationTimerHandler();
    void SocketTimerHandler();
    void SocketConnectedHandler();
    void SocketDisconnectedHandler();
private:
    Ui::MainWindow *ui;
    QTimer *k_timer;
    QTimer *s_timer;
    QLocalSocket *socket;
    CtrlFile     *ctrlfile;
protected:
    void receivedMessage();
};

#endif // MAINWINDOW_H
