#ifndef DATACOUNTER_H
#define DATACOUNTER_H

#include <QObject>
#include <QFile>
#include <QIODevice>
#include <QMutex>
#include <QMutexLocker>
#include <QDateTime>
#include <QElapsedTimer>

class DataCounter : public QObject
{
    Q_OBJECT
public:
    explicit DataCounter(QObject *parent = nullptr);
    ~DataCounter();

    void print_log();                   // \_(^-_-^)_/
    void add_get(size_t get);           // adding the number of bytes received
    void add_send(size_t send);         // adding the number of bytes sent
    void start_timer();                 // start the session duration timer

    void connection(bool is_connect);   //to exit the session

private:
    QString _file_path;         // path to log
    QElapsedTimer _etimer;      // get time of connetion
    QMutex _mutex;              // for write to file
    QMutex _connection_mutex;   // for counter?
    size_t _get;                // bytes get
    size_t _send;               // bytes send
    size_t _connections;        // number of connections

    void _create_file();        // create file if not exists

signals:

};

#endif // DATACOUNTER_H
