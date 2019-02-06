#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonObject>
#include <QStringLiteral>
#include <scopedfileread.h>
#include <scopedfilewrite.h>

class SocketThread : public QThread
{
    Q_OBJECT
public:
    explicit SocketThread(int ID, QObject *parent = 0);
    void run();
    QJsonDocument localDrinksDoc;
    QJsonDocument localBottlesDoc;
    QByteArray data_json;

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();

public slots:

private:
    QTcpSocket *connectedSocket;
    int socketDescriptor;
    QJsonObject rootObjectDrinks;
    QJsonObject rootObjectBottles;
    QByteArray fileRead(QByteArray data);
    bool fileWrite(QByteArray edit);
    QByteArray byteArray_read;
};

#endif // MYTHREAD_H
