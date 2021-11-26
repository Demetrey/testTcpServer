#include <iostream>

#include "datacounter.h"

DataCounter::DataCounter(QObject *parent) : QObject(parent)
{
    _connections = 0;
    this->_get = 0;
    this->_send = 0;
    _etimer.start();
    _create_file();
}

DataCounter::~DataCounter()
{
}

void DataCounter::print_log()
{
    QFile file(this->_file_path);
    // add to file
    if (!file.open(QIODevice::Append)) {
        throw("Can't open file for log");
    }
    QString to_write = QString("Session:\nGet:%1\nSend:%2\nCurrent session time:%3 s\n\n")
            .arg(_get).arg(_send).arg(_etimer.elapsed() / 1000.);
    file.write(to_write.toStdString().c_str());
    file.close();
}

void DataCounter::add_get(size_t get)
{
    QMutexLocker locker(&_mutex);
    this->_get += get;
}

void DataCounter::add_send(size_t send)
{
    QMutexLocker locker(&_mutex);
    this->_send += send;
}

void DataCounter::connection(bool is_connect)
{
    QMutexLocker locker(&_connection_mutex);
    if (is_connect) {
        _connections++;
        // control session count
        if (_connections == 1) {
                this->_get = 0;
                this->_send = 0;
                _etimer.restart();
            }
    }
    else {
        _connections--;
        if (_connections == 0) {
            print_log();
        }
    }
}

void DataCounter::_create_file()
{
    this->_file_path = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss") + ".log";
    QFile file(this->_file_path);
    if (!file.exists(_file_path)) {
        // create file if not xisits
        file.open(QIODevice::WriteOnly);
        file.close();
    }
}
