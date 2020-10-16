#ifndef WEB_H
#define WEB_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QtNetwork>

class Web
{
public:
    Web();
    QTcpServer *server=nullptr;
    QTcpSocket *socketA=nullptr;
    QTcpSocket *socketB=nullptr;
    int connectNum=0;
    int Id = -1;
    enum webId{A, B, C};
};

#endif // WEB_H
