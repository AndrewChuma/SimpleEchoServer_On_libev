/***************************************************************************

  С_BaseAbstractSocket

  Реализация базовой функциональности сокетов типа TCP и UDP


  ОПИСАНИЕ

  * Промежуточная реализация между интерфейсом и классами сокетов.

  * Переопределяет методы iface::I_Connection, общие для TCP и UDP соединений.

****************************************************************************/

#pragma once

#include "I_Connection.h"

namespace iface {

class C_BaseAbstractSocket : public I_Connection
{

public:

    C_BaseAbstractSocket(int a_socketType);
    virtual ~C_BaseAbstractSocket();

    /**
     * Частичная реализация интерфейса I_Connection
     */
    virtual bool setup(std::string &a_addr, uint16_t a_port) override;
    virtual bool open () override;
    virtual bool close() override;
    virtual bool flush() override;

    int listenSocket() { return m_ownSocket; }

protected:

    int setNonBlock( int fd );

protected:

    std::vector<char> m_dataBuff;

    int m_ownSocket;
    sockaddr_in m_ownAddr;
    sockaddr_in m_remoteAddr;

private:

    int m_socketType;
    const size_t MAX_BUFF_SIZE = 80;
};

} // namespace iface

