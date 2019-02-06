#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QByteArray allDrinks = DrinkMachineClient::getInstance().getAllDrinks();
    QJsonParseError jsonError;
    QJsonDocument loadDoc = QJsonDocument::fromJson(allDrinks, &jsonError);
    if(jsonError.error != QJsonParseError::NoError){
        qDebug() << "Did not read valid json from doc: " << jsonError.errorString();
        // jsonError.errorString().toUtf8();
    }
    qDebug() << loadDoc;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()//add drink WORKING
{
    QByteArray returnVal = DrinkMachineClient::getInstance().makeDrinkObj("Roed sodavand og rom","rom",4,"roed sodavand",20,"",0);
    if(returnVal == "Success"){
        qDebug() << "FEDT!";
    }
}


void MainWindow::on_pushButton_2_clicked()//remove drink WORKING
{
    qDebug() << DrinkMachineClient::getInstance().removeDrinkObj("Roed sodavand og rom");
}


void MainWindow::on_pushButton_3_clicked() // add bottle WORKING
{
    qDebug() << DrinkMachineClient::getInstance().addBottle("lort",75, 2);
}

void MainWindow::on_pushButton_4_clicked()//get bottles WORKING
{
    qDebug() << DrinkMachineClient::getInstance().getAllBottles();
}

void MainWindow::on_pushButton_5_clicked()//remove bottle WORKING
{
qDebug() << DrinkMachineClient::getInstance().rmBottle("lort");
}
