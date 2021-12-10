#include "datacounter.h"

DataCounter::DataCounter(bool mt_control_flag, QObject *parent)
    : QObject(parent) {
    _connections = 0;
    this->_get = 0;
    this->_send = 0;
    _etimer.start();
    _create_file();

    _mt_control_flag = mt_control_flag;
    _total_time = 0;
}

DataCounter::~DataCounter() {}

void DataCounter::print_log() {
    QFile file(this->_file_path);
    // add to file
    if (!file.open(QIODevice::Append)) {
        throw("Can't open file for log");
    }
    QString to_write =
            QString("Session:\nGet:%1\nSend:%2\nCurrent session time:%3 s\n\n")
            .arg(_get)
            .arg(_send)
            .arg((!_mt_control_flag ? _etimer.elapsed() : _total_time) / 1000.);
    file.write(to_write.toStdString().c_str());
    file.close();
}

void DataCounter::add_get(size_t get)
{
    QMutexLocker locker(&_mutex);
    this->_get += get;
}

void DataCounter::add_send(size_t send) {
    QMutexLocker locker(&_mutex);
    this->_send += send;
}

void DataCounter::connection(bool is_connect) {
    QMutexLocker locker(&_connection_mutex);
    if (is_connect) {
        _connections++;
        // control session counters
        if (_connections == 1 && !_mt_control_flag) {
            this->_get = 0;
            this->_send = 0;
            _etimer.restart();
        }
        // resetting the counter for a new connection in single-threaded mode
        else if (_mt_control_flag) {
            _etimer.restart();
        }
    }
    else {
        _connections--;
        if (_connections == 0) {
            if (_mt_control_flag)
                _total_time += _etimer.elapsed();
            print_log();
        }
        else if (_mt_control_flag) {
            _total_time += _etimer.elapsed();
            _etimer.restart();
            print_log();
        }
    }
}

void DataCounter::_create_file() {
    this->_file_path = QDateTime::currentDateTime()
            .toString("dd.MM.yyyy HH:mm:ss") + ".log";
    QFile file(this->_file_path);
    if (!file.exists(_file_path)) {
        // create file if not xisits
        file.open(QIODevice::WriteOnly);
        file.close();
    }
}
