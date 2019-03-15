#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qlocalsocket.h"
#include "ctrlfile.h"
//#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int timerId;
    QLocalSocket *socket;
    CtrlFile     *ctrlfile;
protected:
    void timerEvent(QTimerEvent *event);
    void receivedMessage();
};

#endif // MAINWINDOW_H
