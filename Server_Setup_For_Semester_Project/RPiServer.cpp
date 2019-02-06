#include "RPiServer.h"

RPiServer::RPiServer(QObject *parent) :
    QTcpServer(parent)
{
}

RPiServer::~RPiServer()
{

}

void RPiServer::StartServer()
{
    // open server and listen on given port

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if(sockfd < 0)
     {
         qDebug() << "Could not start server";
         exit(EXIT_FAILURE);
     }

     int flag = 1;
     if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) == -1)
     {
         qDebug() << "ERROR: Can't set SO_REUSEADDR";
         exit(EXIT_FAILURE);
     }

     //set Address,IFace, Port...
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(5500);


     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(sockaddr_in)) < 0)
     {
         qDebug() << "ERROR: can't bind socket";
         exit(EXIT_FAILURE);
     }

     if(::listen(sockfd,SOMAXCONN) < 0)
     {
         qDebug() << "ERROR: can't listen on port";
         exit(EXIT_FAILURE);
     }


     qDebug() << "Listening..";
     //forward our descriptor with SO_REUSEPORT to QTcpServer member
     setSocketDescriptor(sockfd);



    /*
    if(!this->listen(QHostAddress::Any,5500))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening...";
        //QThread* bgThread;

    }
    */
}

void RPiServer::incomingConnection(int socketDescriptor)
{
    qDebug() << socketDescriptor << " Connecting...";
    SocketThread *thread = new SocketThread(socketDescriptor,this);
    connect(thread, SIGNAL(finished()),thread, SLOT(deleteLater()));
    thread->start();
}
