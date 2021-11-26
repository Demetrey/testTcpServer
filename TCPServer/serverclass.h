#ifndef SERVERCLASS_H
#define SERVERCLASS_H

#include <QTcpServer>
#include <QThreadPool>

#include "currenttask.h"
#include "datacounter.h"

class ServerClass : public QTcpServer
{
    Q_OBJECT

public:
    ServerClass(int thread_count, int timeout, QObject *parent = nullptr);
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
