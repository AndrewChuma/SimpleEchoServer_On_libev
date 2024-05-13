#include <iostream>
#include <memory>
#include "C_Server.h"

namespace tcp_conn {

C_Server::C_Server()
{

}

C_Server::~C_Server()
{
    m_acptWatcher.stop();
    m_socket.close();
}

bool C_Server::setup(std::string a_addr, uint16_t a_port)
{
    if ( !m_socket.setup(a_addr, a_port) ) {
        return false;
    }
    if ( !m_socket.open() ) {
        LOG(C_Log::lgError, "ошибка создания сокета");
        return false;
    }
    return true;
}

bool C_Server::start()
{
    if ( !m_socket.listen() ) {
        LOG(C_Log::lgError, "установка сокета в прослушивающий режим завершена с ошибкой");
        return false;
    }
    m_acptWatcher.set<C_Server, &C_Server::onAccept>(this);
    m_acptWatcher.start(m_socket.listenSocket(), ev::READ);
    LOG(C_Log::lgInfo, "ожидание соединений");
    return true;
}

void C_Server::onAccept(ev::io &a_watcher, int revents)
{
    if ( EV_ERROR & revents ) {
        return;
    }
    if ( !m_socket.accept() ) {
        return;
    }
    LOG(C_Log::lgInfo, "установлено новое соединение");
    ev::io *new_watcher = new ev::io;
    m_watchers.insert(new_watcher);
    new_watcher->set<C_Server, &C_Server::onRecv>(this);
    new_watcher->start(m_socket.connSocket(), ev::READ);
}

void C_Server::onRecv(ev::io &a_watcher, int revents)
{
    if ( revents & EV_ERROR ) {
        processError(a_watcher);
        return;
    }
    if ( revents & EV_READ) {
        if ( !m_socket.recv(m_buff, a_watcher.fd) ) {
            LOG(C_Log::lgError, "ошибка приема данных/соединение закрыто");
            processError(a_watcher);
            return;
        }

        std::string msg{m_buff.begin(), m_buff.end()};
        LOG(C_Log::lgInfo, msg);

        if ( !m_socket.send(m_buff, a_watcher.fd) ) {
            LOG(C_Log::lgError, "ошибка отправки данных");
            processError(a_watcher);
            return;
        }
    }
}

void C_Server::processError(ev::io &a_watcher)
{
    m_socket.disconnect(a_watcher.fd);
    a_watcher.stop();
    auto watcher_ptr = m_watchers.extract(&a_watcher).value();
    delete watcher_ptr;
}

} // namespace tcp_conn
