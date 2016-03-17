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
    float t = (float) timer.elapsed();
    QTcpSocket *s = static_cast<QTcpSocket *>(sender());

    int datastart = databuf.indexOf("***");
    QString info = databuf.left(datastart);
    QString fn   = info.section(";", 1, 1);
    //QByteArray data = databuf.right(databuf.size() - databuf.indexOf("***") - 3);

    qDebug() << "Info:" << info;
    qDebug() << "Disconnected" << s->peerAddress().toString()
             << "bytes read:" << databuf.size()/1000 << "kByte"
             << "duration:" << timer.elapsed() << "sec"
             << "speed:" << databuf.size() / t << "kByte/sec";

    char *p = databuf.data();
    p += datastart + 3;
    writeFile(fn, p, databuf.size() - datastart - 3);
}

void TCPServer::writeFile(QString fn, char *p, int nbytes) {
    QFile fd(fn);

    if (!(fd.open(QFile::WriteOnly))) {
        qCritical() << "Error opening:" << fd.fileName();
        return;
    } else
        qDebug() << "Write file:" << fd.fileName();

    fd.write(p, nbytes);
    fd.close();
}

