#pragma once

#include <QTcpSocket>
#include <QTextEdit>

class C_TcpClient : public QObject
{
    Q_OBJECT

public:

    C_TcpClient(QTextEdit &a_log, QObject *parent = nullptr);
    ~C_TcpClient();

    void connectHost(const QHostAddress &addr, quint16 port);
    void sendMessage(const QString &msg);

private:

    QTcpSocket *m_socket;
    QTextEdit &m_log;

public slots:

    void onConnected();
    void onError(QAbstractSocket::SocketError socketError);
    void onReadyRead();
};


