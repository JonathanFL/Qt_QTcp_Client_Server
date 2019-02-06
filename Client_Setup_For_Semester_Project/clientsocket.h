#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QNetworkProxy>
#include <QAbstractSocket>
#include <QDebug>
#include <QJsonDocument>
#include <QFile>

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(QObject *parent = nullptr);


private:
    qint8 test = 0;
    QTcpSocket* client_socket;

signals:

private slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

public slots:
    bool startConnection();
    QByteArray sendToServer(QByteArray msg);// WORKING
    QByteArray getAllDrinksFromServer(QString drink);//WORKING
    QByteArray getAllBottlesFromServer(QString drink);//WORKING
    QByteArray removeDrinkFromServer(QString drinkName);//WORKING
    QByteArray addBottleToServer(QString bottle);
    QByteArray addDrinkObjToServer(QByteArray drinkObj);// WORKING
    QByteArray removeBottleFromServer(QString bottleName);// WORKING
};

#endif // CLIENTSOCKET_H
