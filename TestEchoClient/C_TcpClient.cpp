#include "C_TcpClient.h"

#include <QDebug>
#include <QDataStream>

C_TcpClient::C_TcpClient(QTextEdit &a_log, QObject *parent) : QObject(parent), m_log(a_log)
{

    m_socket = new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::connected, this, &C_TcpClient::onConnected);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &C_TcpClient::onError);
    connect(m_socket, &QTcpSocket::readyRead, this, &C_TcpClient::onReadyRead);
}

C_TcpClient::~C_TcpClient()
{

}

void C_TcpClient::connectHost(const QHostAddress &addr, quint16 port)
{
    m_socket->connectToHost(addr, port);
}

void C_TcpClient::onConnected()
{
    m_log.append("Info: connect success");
}

void C_TcpClient::onError(QAbstractSocket::SocketError socketError)
{
    m_log.append(QString("Error: error occured %1").arg(socketError));

}

void C_TcpClient::onReadyRead()
{

     QByteArray recvMsg;
     recvMsg = m_socket->readAll();

     m_log.append(recvMsg);
}

void C_TcpClient::sendMessage(const QString &msg)
{
    m_socket->write(msg.toUtf8(), msg.length());
}
