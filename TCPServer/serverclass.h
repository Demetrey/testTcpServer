#ifndef SERVERCLASS_H
#define SERVERCLASS_H

#include <QTcpServer>
#include <QThreadPool>
#include <iostream>


#include "currenttask.h"
#include "datacounter.h"

// The server accepts connections and redirects
// data processing to a separate thread from a pool of threads of a given size
class ServerClass : public QTcpServer
{
    Q_OBJECT

public:
    ServerClass(int thread_count, int timeout, bool mt_control_flag = false,
                QObject *parent = nullptr);
    void startServer(qint16 port);

signals:

public slots:


private:
    int _timeout;
    DataCounter *_datacounter;

    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle) override;
};

#endif // SERVERCLASS_H
