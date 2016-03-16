#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QTime>

class TCPServer : public QTcpServer
{
    Q_OBJECT

public:
    TCPServer(QObject * parent = 0);
    ~TCPServer();

public slots:
    void on_new_connection();
    void on_ready_read();
    void on_disconnected();

private:
    QByteArray databuf;
    QTime timer;
};

#endif // TCPSERVER_H
