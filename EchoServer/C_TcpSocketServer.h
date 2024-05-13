/*******************************************************************************

  C_TcpSocketServer

  Класс сокета типа tcp, для реализации роли сервера в соединении


  РЕАЛИЗАЦИЯ ИНТЕРФЕЙСА

  * Класс реализует сокет для обмена данными в соответствии со спецификой работы TCP соединения.


  ИСПОЛЬЗОВАНИЕ

  * Должен быть использован для реализации роли сервера в соединении.

*******************************************************************************/

#pragma once

#include "C_BaseAbstractSocket.h"

class C_TcpSocketServer : public iface::C_BaseAbstractSocket
{
public:

    C_TcpSocketServer();
    //C_TcpSocketServer(const C_TcpSocketServer &a_other) {}
    virtual ~C_TcpSocketServer();

    /**
     * Частичная реализация интерфейса I_Connection
     */
    virtual bool recv      ( std::vector<char> &a_data, int a_connSock = 0 )       override;
    virtual bool send      ( const std::vector<char> &a_data, int a_connSock = 0 ) override;
    virtual bool listen    () override;
    virtual bool disconnect(int a_sd = 0)                          override;

    bool accept();
    int  connSocket() { return m_connSocket; }

private:

    int m_connSocket;
};
