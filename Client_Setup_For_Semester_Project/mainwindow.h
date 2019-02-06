#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QNetworkProxy>
#include <QAbstractSocket>
#include <QDebug>
#include <QFile>
#include <QTimer>

#include "clientsocket.h"
#include "DrinkMachineClient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Test();
    void testFunc();

private slots:
    //void connected();
    //void disconnected();
    //void bytesWritten(qint64 bytes);
    //void readyRead();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    QByteArray receivedClient;

};

#endif // MAINWINDOW_H
