#include "clientsocket.h"

ClientSocket::ClientSocket(QObject *parent) : QObject(parent)
{
}

bool ClientSocket::startConnection()
{
    client_socket = new QTcpSocket(this);
    client_socket->setProxy(QNetworkProxy::NoProxy);

    const QHostAddress ipAddress("192.168.43.69"/*"192.168.0.1"*/);
    quint16 port = 5500;

    connect(client_socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(client_socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(client_socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(client_socket,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWritten(qint64)));

    qDebug() << "Connecting to server...";

    client_socket->connectToHost(ipAddress,port);
    client_socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    if(!client_socket->waitForConnected(4000)){
        qDebug() << "Error " << client_socket->errorString();
        return false;
    }
    return true;
}

void ClientSocket::connected()
{
    qDebug() << "Connected!";
    //QByteArray allDrinks = NewDrink::getInstance().getAllDrinks();
}

QByteArray ClientSocket::sendToServer(QByteArray msg){
    client_socket->write(msg);
    client_socket->flush();
    client_socket->waitForBytesWritten(1000);
    QByteArray response;
    if(client_socket->waitForReadyRead(1000)){
        response = client_socket->readAll();
    }
    return response;
}

QByteArray ClientSocket::addBottleToServer(QString bottle)
{
    QByteArray response;
    client_socket->write(bottle.toUtf8());
    client_socket->flush();
    client_socket->waitForBytesWritten(1000);
    if(client_socket->waitForReadyRead(1000)){
        response = client_socket->readAll();
    }
    return response;
}

QByteArray ClientSocket::addDrinkObjToServer(QByteArray drinkObj)
{
    client_socket->write(drinkObj);
    client_socket->flush();
    QByteArray response;
    if(client_socket->waitForReadyRead(1000)){
        response = client_socket->readAll();
    }

    return response;
}

QByteArray ClientSocket::getAllDrinksFromServer(QString all)
{
    //the argument needs to be GET_ALL_DRINKS
    client_socket->write(all.toUtf8());
    client_socket->flush();
    QByteArray getAllDrinks;
    if(client_socket->waitForReadyRead(1000)){
        getAllDrinks = client_socket->readAll();
    }

    return getAllDrinks;
}

QByteArray ClientSocket::getAllBottlesFromServer(QString all)
{
    //the argument needs to be GET_ALL_BOTTLES
    client_socket->write(all.toUtf8());
    client_socket->flush();
    QByteArray getAllBottles;
    if(client_socket->waitForReadyRead(1000)){
        getAllBottles = client_socket->readAll();
    }

    return getAllBottles;
}

QByteArray ClientSocket::removeDrinkFromServer(QString drinkName)
{
    QByteArray response;
    client_socket->write(drinkName.toUtf8());
    client_socket->flush();
    client_socket->waitForBytesWritten(1000);
    if(client_socket->waitForReadyRead(1000)){
        response = client_socket->readAll();
    }
    return response;
}

QByteArray ClientSocket::removeBottleFromServer(QString drinkName)
{
    QByteArray response;
    client_socket->write(drinkName.toUtf8());
    client_socket->flush();
    client_socket->waitForBytesWritten(1000);
    if(client_socket->waitForReadyRead(1000)){
        response = client_socket->readAll();
    }
    return response;
}

void ClientSocket::readyRead()
{
    //qDebug() << "Reading...";
    //qDebug() << client_socket->readAll();//<-- can be used for read test
}


void ClientSocket::bytesWritten(qint64 bytes)
{
    qDebug() << "we wrote: " << bytes;
}

void ClientSocket::disconnected()
{
    qDebug() << "Disconnected!";
    qDebug() << ++test;

    while(!startConnection()){

    }
}
