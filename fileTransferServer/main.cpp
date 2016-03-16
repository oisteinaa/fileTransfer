#include <QCoreApplication>

#include "tcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TCPServer s(&a);

    a.exec();
}
