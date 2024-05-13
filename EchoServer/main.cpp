#include <iostream>
#include <ev++.h>

#include "C_Server.h"
#include "C_Log.h"

using namespace std;

static void signal_handler(ev::sig &signal, int revents) {
    signal.loop.break_loop();
}

int main(int argc, const char* argv[])
{
    ev::sig s;
    s.set<signal_handler>();
    s.start(SIGINT);

    ev::default_loop loop;

    C_Log::getInstance().setup(C_Log::lgrBoth, "sampleLog.log");

    uint16_t port = 5000;
    std::string address = "";

    if ( argc == 2 ) {
        address = argv[1];
    }

    if ( argc == 3 ) {
        port = std::stoi(argv[2]);
    }

    tcp_conn::C_Server server;
    if ( !server.setup("127.0.0.1", 5000) ) {
        std::cout << "setup error" << std::endl;
        return 1;
    }
    if ( !server.start() ) {
        std::cout << "start error" << std::endl;
        return 1;
    }


    loop.run(0);
    return 0;
}
