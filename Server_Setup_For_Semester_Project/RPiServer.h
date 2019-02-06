#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QDebug>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "SocketThread.h"
#include "hwbutton.h"

class RPiServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit RPiServer(QObject *parent = 0);
    ~RPiServer();
    void StartServer();

signals:

public slots:

protected:
    void incomingConnection(int socketDescriptor);

private:
    int sockfd = 0;//socket filedescriptor
    struct sockaddr_in serv_addr;

};

#endif // MYSERVER_H
