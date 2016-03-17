#include "client.h"

#include <QCoreApplication>

Client::Client(QObject *parent) : QObject(parent)
{
    sock = new QTcpSocket(this);
    connect(sock, SIGNAL(connected()), this, SLOT(on_connected()));
    connect(sock, SIGNAL(disconnected()), this, SLOT(on_disconnected()));

    QStringList args = QCoreApplication::arguments();
    QString filename = args.at(1);
    QString address = args.at(2);
    quint16 port = args.at(3).toInt();

    qDebug() << filename << address << port;

    readFile(filename);
    sock->connectToHost(address, port);
}

Client::~Client()
{
    sock->close();
}

void Client::on_connected() {
    databuf.prepend(prefix.toLatin1());
    char *p = databuf.data();

    for (int i=0; i<databuf.size(); i+=512) {
        sock->write(p, 512);
        p += 512;
    }
    sock->close();
}

void Client::on_disconnected() {
    QCoreApplication::exit(0);
}

void Client::on_error(QAbstractSocket::SocketError socketError) {
    qDebug() << "Error:" << sock->errorString();
    QCoreApplication::exit(0);
}

void Client::readFile(QString fn) {
    QFile fd(fn);

    if (!(fd.open(QFile::ReadOnly)))
        qCritical() << "Error opening:" << fd.fileName();

    databuf = fd.readAll();
    fd.close();

    prefix = QString("$NEWF;%1;%2;%3***").
            arg(QFileInfo(fd.fileName()).fileName()).
            arg(fd.size()).
            arg(qChecksum(databuf, databuf.size()));
}

