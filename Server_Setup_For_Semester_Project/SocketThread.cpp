#include "SocketThread.h"

SocketThread::SocketThread(int ID, QObject *parent) :
    QThread(parent)
{
    QFile origDrinkFil_onConstruc("/home/root/drinks.json"), origBottlFil_onConstruc("/home/root/bottles.json");
    ScopedFileRead scopedDrinksFile_construc(&origDrinkFil_onConstruc), scopedBottleFile_construc(&origBottlFil_onConstruc);

    QByteArray *loadDrinksData = new QByteArray(scopedDrinksFile_construc.readAll()); // from QFile
    QByteArray *loadBottlesData = new QByteArray(scopedBottleFile_construc.readAll()); // from QFile

    QJsonParseError jsonErrorDrinks;
    QJsonParseError jsonErrorBottles;
    localDrinksDoc = QJsonDocument::fromJson(*loadDrinksData, &jsonErrorDrinks);
    localBottlesDoc = QJsonDocument::fromJson(*loadBottlesData, &jsonErrorBottles);

    if (jsonErrorDrinks.error != QJsonParseError::NoError)
    {
        qDebug() << "Error - DRINKS: " << jsonErrorDrinks.errorString();
    }if (jsonErrorBottles.error != QJsonParseError::NoError)
    {
        qDebug() << "Error - BOTTLE: " << jsonErrorBottles.errorString();
    }if (jsonErrorDrinks.error == QJsonParseError::NoError)
    {
        rootObjectDrinks = localDrinksDoc.object();

    }if (jsonErrorBottles.error == QJsonParseError::NoError)
    {
        rootObjectBottles = localBottlesDoc.object();
    }

    this->socketDescriptor = ID;
}

void SocketThread::run()
{
    //thread starts here
    qDebug() << socketDescriptor << " Starting thread";
    connectedSocket = new QTcpSocket();
    if (!connectedSocket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(connectedSocket->error());
        return;
    }

    connect(connectedSocket,SIGNAL(readyRead()),this,SLOT(readyRead()),Qt::DirectConnection);
    connect(connectedSocket,SIGNAL(disconnected()),this,SLOT(disconnected()),Qt::DirectConnection);

    qDebug() << socketDescriptor << " Client Connected";

    exec();//<-- similair to pthread_join
    // also the exit in the disconnect method is
    // connected to exec();
    // the thread is "deleted" when the tcp connection
    // disconnected is called, and then calls the exit(int returnCode);
    // where the return code is 0
    // from the dokumentation this means that the run(); method
    // returns the value of exit(), which is 0
}

void SocketThread::readyRead()
{
    QByteArray dataFromSocket = connectedSocket->readAll();

    /*if(data == "test"){
     * ----------USAGE OF SCOPED FILE READ---------- *
     *   QFile testFile1("/home/root/drinks.json");  *
     *   ScopedFileRead testfileRead(&testFile1);    * give address to QFile
     *   QByteArray testba = testfileRead.readAll(); * ScopedFileRead works on a pointer to the address of the QFile
     *   qDebug() << testba;                         *
    }*/

     if (dataFromSocket.startsWith("GET_ALL"))
     {
        connectedSocket->write(fileRead(dataFromSocket));
     } else {
         if (!fileWrite(dataFromSocket)){
             qDebug() << "Failed to write";
             connectedSocket->write("Failed to write to server file");
         } else {
             qDebug() << "Succesfully edited .json doc";
             connectedSocket->write("Success");
         }
     }
}

QByteArray SocketThread::fileRead(QByteArray data){
    if (data.contains("DRINKS")){
        QFile originalDrinksFile("/home/root/drinks.json");
        ScopedFileRead scopedDrinksFile(&originalDrinksFile);
        if (scopedDrinksFile.size() != 0){
            byteArray_read = scopedDrinksFile.readAll();
            QJsonParseError jsonError;

            QJsonDocument::fromJson(byteArray_read, &jsonError);
            if (jsonError.error != QJsonParseError::NoError){
                qDebug() << "Did not read valid json from doc: " << jsonError.errorString();
                return jsonError.errorString().toUtf8();
            }
        } else {
            return "no drinks";
        }
    }else if(data.contains("BOTTLES")){
        QFile bottleFile1("/home/root/bottles.json");
        ScopedFileRead bottleFile(&bottleFile1);
        if(bottleFile.size() != 0){
            byteArray_read = bottleFile.readAll();
            QJsonParseError jsonError;

            QJsonDocument::fromJson(byteArray_read, &jsonError);
            if(jsonError.error != QJsonParseError::NoError){
                qDebug() << "Did not read valid json from doc: " << jsonError.errorString();
                return jsonError.errorString().toUtf8();
            }
        } else {
            return "no bottles";
        }
    }

    return byteArray_read;
}

bool SocketThread::fileWrite(QByteArray edit){
    if(edit.startsWith("btl"))
    {
        QFile btlFile1("/home/root/bottles.json");
        ScopedFileWrite btlFile(&btlFile1);
        if(edit.contains("nw"))
        {
            edit = edit.remove(0,6);
            qDebug() << edit;
            QJsonParseError jsonError;
            QJsonDocument btlDocLoad = QJsonDocument::fromJson(edit,&jsonError);
            if(jsonError.error != QJsonParseError::NoError){
                qDebug() << "Error: did not receive JSon: " << jsonError.errorString();
                return false;
            } else {
                QString name = btlDocLoad.object().keys().at(0);
                QJsonObject subObject = btlDocLoad.object()[name].toObject();
                rootObjectBottles.insert(name,subObject);
                localBottlesDoc.setObject(rootObjectBottles);
                data_json = localBottlesDoc.toJson();
                btlFile.write(data_json);
                return true;
            }
        } else if (edit.contains("rm")){
            edit = edit.remove(0,6);
            qDebug() << edit;
            //QJsonParseError jsonError;
            rootObjectBottles.remove(edit);
            localBottlesDoc.setObject(rootObjectBottles);
            data_json = localBottlesDoc.toJson();
            btlFile.write(data_json);
            return true;
        }
    } else {//remove drink object
        QFile drinksFile1("/home/root/drinks.json");
        ScopedFileWrite scopedDrinksFile(&drinksFile1);
        if((edit.startsWith("RM_"))){// remove drink object
            edit = edit.remove(0,3);
            qDebug() << edit;
            rootObjectDrinks.remove(edit);
            localDrinksDoc.setObject(rootObjectDrinks);
            data_json = localDrinksDoc.toJson();
            scopedDrinksFile.write(data_json);
            return true;
        } else {// add drink object

            QJsonParseError jsonError;

            QJsonDocument loadDoc = QJsonDocument::fromJson(edit, &jsonError);
            if(jsonError.error != QJsonParseError::NoError){
                qDebug() << "Error: did not receive JSon: " << jsonError.errorString();
                return false;
            } else {

                QString name = loadDoc.object().keys().at(0);//get name of drink
                QJsonObject subObject = loadDoc.object()[name].toObject();// get content of received json of the 'name'

                rootObjectDrinks.insert(name,subObject);//add content with the name

                localDrinksDoc.setObject(rootObjectDrinks);//updates the jsonDoc variable

                data_json = localDrinksDoc.toJson();// put the updated jsonDoc into QByteArray
                scopedDrinksFile.write(data_json);// write the QByteArray to the drinksFile
                return true;
            }
        }
    }
    return true;
}

void SocketThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";

    connectedSocket->deleteLater();
    exit(0);//here is the exit() which is in relation to the exec() in run()
}
