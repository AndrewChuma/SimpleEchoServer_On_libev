#include "C_TcpSocketServer.h"
#include <iostream>

C_TcpSocketServer::C_TcpSocketServer() : iface::C_BaseAbstractSocket(SOCK_STREAM)
{

}

C_TcpSocketServer::~C_TcpSocketServer()
{

}

/*****************************************************************************
 * Установка сокета в режим прослушивания
 */
bool C_TcpSocketServer::listen()
{
    int listenResult; // Результат функции ожидания

    // Установка сокета в режим ожидания соединений
    listenResult = ::listen( m_ownSocket, 1 );

    // Обработка неудачного завершения функции ожидания
    if ( -1 == listenResult )  {

        return false;
    }

    return true;
}

/*****************************************************************************
 * Ожидание соединения
 */
bool C_TcpSocketServer::accept()
{
    m_connSocket = ::accept(m_ownSocket, 0, 0);

    if ( m_connSocket < 0 ) {
        switch(errno) {
            case EWOULDBLOCK: return true;
        default:
            LOG(C_Log::lgError, "ожидание соединения завершено с ошибкой");
            close();
            return false;
        }
    }
    setNonBlock(m_connSocket);
    return true;

}

/*****************************************************************************
 * Прием данных
 */
bool C_TcpSocketServer::recv( std::vector<char> &a_recvMessage, int a_connSock )
{
    int recvBytesQuan = 0; // Количество принятых байт

    int recvFrom_d = (a_connSock) ? a_connSock : m_connSocket;
    // Прием данных
    a_recvMessage.clear();
    recvBytesQuan = ::recv( recvFrom_d, &m_dataBuff[0], m_dataBuff.size(), MSG_NOSIGNAL );

    // Проверка результата вызова функции приема данных
    if ( recvBytesQuan == -1)
    {
        switch ( errno ) {
            case EWOULDBLOCK: return true; // В буфере сокета нет данных
            default:
                // log
                return false;
        }
        return false;
    }

    if ( recvBytesQuan == 0 ) {
        return false; // Разрыв соединения со стороны клиента
    }

    a_recvMessage.insert( a_recvMessage.begin(), m_dataBuff.begin(), m_dataBuff.begin() + recvBytesQuan );
    return true;
}

/*****************************************************************************
 * Отправка данных
 */
bool C_TcpSocketServer::send( const std::vector<char> &a_sendMessage, int a_connSock )
{
    int sendTo_d = (a_connSock) ? a_connSock : m_connSocket;

    int bytes = ::send( sendTo_d, &a_sendMessage[0], a_sendMessage.size(), MSG_NOSIGNAL );

    if ( bytes == -1 ) {
        switch ( errno ) {
            case EWOULDBLOCK:   return false; // Сокет помечен как неблокирующий, отправка не удалась
            case ETIMEDOUT:     return false; // Соединение разорвано из-за сбоя сети
            case ENETUNREACH:   return false; // Сеть недоступна
            case EHOSTUNREACH:  return false; // Удаленный узел недоступен
            default:
                //log
                return false;
        }
    }
    return true;
}

/*****************************************************************************
 * Разрыв соединения
 */
bool C_TcpSocketServer::disconnect(int a_sd)
{
    int connSock = (!a_sd) ? m_connSocket : a_sd;
    // Завершение соединения
    if ( ::shutdown( connSock, SHUT_RDWR) == -1) {

        return false;
    }

    // Закрытие сокета
    if ( ::close(connSock) == -1 ) {
        return false;
    }

    return true;
}
