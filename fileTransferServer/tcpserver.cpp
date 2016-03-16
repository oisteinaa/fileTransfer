#include "tcpserver.h"

#include <QDebug>

TCPServer::TCPServer(QObject *parent) :
    QTcpServer(parent)
{
    listen(QHostAddress::Any, (quint16) 10000);
    setMaxPendingConnections(1);

    connect(this, SIGNAL(newConnection()), this, SLOT(on_new_connection()));

    databuf.clear();
}

TCPServer::~TCPServer()
{

}

void TCPServer::on_new_connection() {
    databuf.clear();
    QTcpSocket *sock = nextPendingConnection();

    qDebug() << "Connection:" << sock->peerAddress().toString();

    connect(sock, SIGNAL(readyRead()), this, SLOT(on_ready_read()));
    connect(sock, SIGNAL(disconnected()), this, SLOT(on_disconnected()));

    timer.start();
}

void TCPServer::on_ready_read() {
    QTcpSocket *s = static_cast<QTcpSocket *>(sender());

    while (s->bytesAvailable()) {
        databuf.append(s->readAll());
    }
}

void TCPServer::on_disconnected() {
    QTcpSocket *s = static_cast<QTcpSocket *>(sender());

    QString info = databuf.left(databuf.indexOf("***"));
    //QByteArray data = databuf.right(databuf.size() - databuf.indexOf("***") - 3);

    qDebug() << "Info:" << info;
    qDebug() << "Disconnected" << s->peerAddress().toString()
             << "bytes read:" << databuf.size()/1000 << "kByte"
             << "duration:" << timer.elapsed() << "sec"
             << "speed:" << databuf.size() / (float) timer.elapsed() << "kByte/sec";
}

