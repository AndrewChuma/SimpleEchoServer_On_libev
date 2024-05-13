#pragma once

#include <unordered_set>
#include <memory>

#include "C_TcpSocketServer.h"

namespace tcp_conn {

class C_Server
{
public:
    C_Server();
    ~C_Server();

    bool setup(std::string a_addr, uint16_t a_port);
    bool start();
    void onAccept(ev::io &a_watcher, int revents);
    void onRecv(ev::io &a_watcher, int revents);
    void onSend();

private:

    void processError(ev::io &a_watcher);

private:

    C_TcpSocketServer m_socket;
    ev::io m_acptWatcher;

    std::vector<char> m_buff;

    std::unordered_set<ev::io*> m_watchers;
};

} // namespace tcp_conn
