#include <iostream>

#include "serverclass.h"

ServerClass::ServerClass(int thread_count, int timeout, QObject *parent) : QTcpServer(parent)
{
    // Allocation of the required number of threads
    QThreadPool::globalInstance()->setMaxThreadCount(thread_count);

    this->_timeout = timeout;
    this->_datacounter = new DataCounter(this);
}

void ServerClass::startServer(qint16 port)
{
    if (this->listen(QHostAddress::Any, port)) {
        std::cout << "Server started" << std::endl;
    }
    else {
        std::cout << "Server DID NOT started" << std::endl;
    }
}

void ServerClass::incomingConnection(qintptr handle)
{
    std::cout << "Connection!" << std::endl;
    CurrentTask* task = new CurrentTask();
    task->setAutoDelete(true);
    task->setSocketDescriptor(handle);
    task->set_interval(_timeout);
    task->set_datacounter(_datacounter);
    _datacounter->connection(1);
    QThreadPool::globalInstance()->start(task);
}
