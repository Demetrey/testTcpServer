#include "currenttask.h"

CurrentTask::CurrentTask(QObject *parent)
    : QObject(parent) {
    _interval = 30000;
}

int CurrentTask::getSocketDescriptor() {
    return this->_socketDescriptor;
}

void CurrentTask::setSocketDescriptor(int descriptor) {
    this->_socketDescriptor = descriptor;
}

void CurrentTask::set_interval(int intreval) {
    this->_interval = intreval;
}

void CurrentTask::set_datacounter(DataCounter *datacounter) {
    this->_datacounter = datacounter;
}

void CurrentTask::onDisconnected() {
    // set disconnextion flag and close event loop
    _datacounter->connection(0);
    std::cout << "Disconnected" << std::endl;
    _eventLoop->exit();
}

void CurrentTask::onReadyRead() {
    // restart connection timer
    _timer->stop();
    _timer->start(_interval);
    std::cout << "Update timer" << std::endl;
    std::cout << "getting data: " << _socketDescriptor << std::endl;
    // getting data from socket
    QByteArray data = _socket->readAll();
    QString qdata = QString(data);
    std::cout << "Client data: " << qdata.toStdString() << std::endl;
    // send answer
    QString serverAnswer = "Hello! This is server ansver!";
    _socket->write(serverAnswer.toUtf8());

    // set data size
    _datacounter->add_get(data.size());
    _datacounter->add_send(serverAnswer.size());
}

void CurrentTask::onTimerCompleted() {
    // connection timeout
    std::cout << "Completed" << std::endl;
    this->_socket->disconnectFromHost();
}

void CurrentTask::run() {
    // Point of entry
    _eventLoop = new QEventLoop();
    _socket = new QTcpSocket();
    _timer = new QTimer();
    if(_socket->setSocketDescriptor(_socketDescriptor)) {
        std::cout << "Client address: " << this->_socket->peerAddress()
                     .toString().toStdString() << std::endl;
        std::cout << "Client port: " << this->_socket->peerPort() << std::endl;
        connect(_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()),
                Qt::DirectConnection);
        connect(_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()),
                Qt::DirectConnection);

        connect(this->_timer, SIGNAL(timeout()), this, SLOT(onTimerCompleted()),
                Qt::DirectConnection);
        _timer->start(_interval);   //every <_interval> seconds
    }
    else {
        std::cout << "Can't set socket descriptor" << std::endl;
        return;
    }
    std::cout << "Start thread for socket with descriptor: " <<
                 _socketDescriptor << std::endl;
    // The event loop starts, the corresponding slots are executed
    _eventLoop->exec();
    _timer->stop();     // Stopping the event loop
    delete _eventLoop;
    delete _socket;
    delete _timer;
    std::cout << "Close thread on " << _socketDescriptor << std::endl;
}
