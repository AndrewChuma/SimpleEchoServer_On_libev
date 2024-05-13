#include "C_BaseAbstractSocket.h"
#include <iostream>

namespace iface {

/*****************************************************************************
 * Конструктор
 *
 * @param
 *  [in] a_socketType - тип Ethternet соединения:
 *                          - SOCK_STREAM, для tcp
 *                          - SOCK_DGRAM, для udp
 */
C_BaseAbstractSocket::C_BaseAbstractSocket(int a_socketType) : m_socketType(a_socketType)
{
    m_dataBuff.resize(MAX_BUFF_SIZE);
}

C_BaseAbstractSocket::~C_BaseAbstractSocket()
{

}

/*****************************************************************************
 * Установка настроек объекта
 */
bool C_BaseAbstractSocket::setup(std::string &a_addr, uint16_t a_port)
{
    /**
     * Настройка ip адреса номера порта сокета
     */
    bzero(&m_ownAddr, sizeof(m_ownAddr));
    bzero(&m_remoteAddr, sizeof(m_remoteAddr));

    m_ownAddr.sin_family = AF_INET;
    m_ownAddr.sin_addr.s_addr = INADDR_ANY;
    if ( !a_addr.empty() ) {
        m_ownAddr.sin_addr.s_addr = inet_addr(a_addr.c_str());
    }
    m_ownAddr.sin_port = htons(a_port);

    return true;
}

/*****************************************************************************
 * Создание сокета
 *
 * Функция создает неблокирующий сокет.
 */
bool C_BaseAbstractSocket::open()
{
    m_ownSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if ( -1 == m_ownSocket ) {        
        return false;
    }
    if ( -1 == setNonBlock(m_ownSocket)) {
        return false;
    }

    const int on = 1;
    setsockopt(m_ownSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );

    if ( bind(m_ownSocket, reinterpret_cast<sockaddr*>(&m_ownAddr), sizeof(m_ownAddr)) == -1 ) {
        close();
    }

    return true;
}

/*****************************************************************************
 * Закрытие сокета
 */
bool C_BaseAbstractSocket::close()
{
    if ( -1 == shutdown(m_ownSocket, SHUT_RDWR ) ) {
        LOG(C_Log::lgError, "закрытие сокета завершено с ошибкой");
        return false;
    }
    return true;
}

/*****************************************************************************
 * Сброс параметров сокета
 */
bool C_BaseAbstractSocket::flush()
{
      memset(&m_ownAddr, 0, sizeof(m_ownAddr));
      memset(&m_remoteAddr, 0, sizeof(m_remoteAddr));
      return true;
}

/*****************************************************************************
 * Установка сокета в неблокирующий режим
 */
int C_BaseAbstractSocket::setNonBlock( int fd ) {
    int flags;
#if defined(O_NONBLOCK)
    if (-1 == (flags = fcntl(fd, F_GETFL, 0))) {
        flags = 0;
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK );
#else
    flags = 1;
    return ioctl(fd, FIONBIO, &flags);
#endif
}

} // namespace iface
