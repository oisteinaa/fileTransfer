#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QFileInfo>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();

signals:

public slots:
    void on_connected();
    void on_disconnected();
    void on_error(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *sock;
    QByteArray databuf;
    QString prefix;

    void readFile(QString fn);
};

#endif // CLIENT_H
