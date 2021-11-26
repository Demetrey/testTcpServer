#ifndef CURRENTTASK_H
#define CURRENTTASK_H

#include <QRunnable>
#include <QTcpSocket>
#include <QHostAddress>
#include <QEventLoop>
#include <QTimer>

#include "datacounter.h"

class CurrentTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    CurrentTask(QObject *parent = nullptr);

    int getSocketDescriptor();
    void setSocketDescriptor(int descriptor);

    void set_interval(int intreval);
    void set_datacounter(DataCounter *datacounter);

public slots:
    void onDisconnected();
    void onReadyRead();

signals:
    void start_proc();

private slots:
    void onTimerCompleted();

private:
    qintptr _socketDescriptor;  // Socket identification
    QTcpSocket* _socket;        // TCP socket object
    QEventLoop* _eventLoop;     // Main loop loop
    QTimer *_timer;             // Connection timer
    int _interval;              // Connection timer interval
    DataCounter *_datacounter;  // Received/transmitted data counter

protected:
    void run() override;

};

#endif // CURRENTTASK_H
