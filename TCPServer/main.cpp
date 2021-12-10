#include <QCoreApplication>
#include <iostream>


#include "serverclass.h"

void print_help();

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    qint16 current_port = 3334;         //Default value for port
    int thread_pool_count = 10;         // Default threads count
    int timeout_time = 30000;           // Default time for connection
    bool mt_control_flag = false;       // Default multithread work (ON)

    if (argc == 2 && !strcmp(argv[1], "-help")) {
        print_help();
        return 0;
    }

    if (argc != 5) {
        std::cout << "Standard parameters applied!" << std::endl <<
                     "Call with \"-help\" parameter for help" << std::endl;

    }
    else {
        // Set custom parameters
        current_port = atoi(argv[1]);
        thread_pool_count = atoi(argv[2]);
        timeout_time = atoi(argv[3]);
        mt_control_flag = atoi(argv[4]);
    }

    if (mt_control_flag)
        thread_pool_count = 1;

    std::cout << "Settings:" << std::endl <<
     "Server started on standard port: " << current_port << std::endl <<
     "Maximum number of threads for a pool: " << thread_pool_count << std::endl
     << "Timeout: " << timeout_time << " ms" << std::endl <<
     "Multithread work: " << (mt_control_flag ? "OFF" : "ON") << std::endl;

    ServerClass server(thread_pool_count, timeout_time, mt_control_flag);
    try {
        server.startServer(current_port);
    }
    catch (const char* e) {
        std::cerr << e << std::endl;
        return 1;
    }

    return a.exec();
}

void print_help() {
    std::cout << "Parameters:" << std::endl <<
                 "[1] - Port" << std::endl <<
                 "[2] - Maximum number of threads for a pool" << std::endl <<
                 "[3] - Timeout in milliseconds" << std::endl <<
                 "[4] - Mode of operation in one thread (0 - off, 1 - on)" <<
                 std::endl;
}
