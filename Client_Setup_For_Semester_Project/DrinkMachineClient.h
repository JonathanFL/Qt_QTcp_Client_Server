#ifndef NEWDRINK_H
#define NEWDRINK_H
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QVariant>
#include <QStringList>
#include <QDebug>
//#include <QTextCodec>

#include "clientsocket.h"

class DrinkMachineClient
{
public:
    static DrinkMachineClient& getInstance()
    {
        static DrinkMachineClient nwDrnk;
        return nwDrnk;
    }

    /*void startConnection(){
        clntSock.startConnection();
    }*/

    QByteArray makeDrinkObj(QString name,QString ingr1, int ingr1_cl, QString ingr2, int ingr2_cl, QString ingr3, int ingr3_cl)
    {

        Drinks* drinks = new Drinks(ingr1,ingr1_cl,ingr2,ingr2_cl,ingr3,ingr3_cl);

        drinksRootObj.insert(name,mergeObjs(drinks));

        drinksDoc.setObject(drinksRootObj);
        drinksDataJson = drinksDoc.toJson();

        QByteArray returnVal;
        returnVal = clntSock.addDrinkObjToServer(drinksDataJson);

        delete drinks;

        return returnVal;
    }
    QByteArray removeDrinkObj(QString drinkName){
        drinkName = "RM_" + drinkName;
        return clntSock.removeDrinkFromServer(drinkName);
    }
    QByteArray getAllDrinks()
    {
        QByteArray allDrinks = clntSock.getAllDrinksFromServer("GET_ALL_DRINKS");
        if(allDrinks == "no drinks"){
            qDebug() << "add a drink to the system";
        }
        return allDrinks;
    }
    QByteArray getAllBottles()
    {
        return clntSock.getAllBottlesFromServer("GET_ALL_BOTTLES");;
    }
    QByteArray addBottle(QString bottleName, int bottleCl, int position)
    {
        QJsonObject clObj;
        clObj.insert("cl",bottleCl);
        clObj.insert("position",position);

        bottlesRootObj.insert(bottleName,clObj);

        bottlesDoc.setObject(bottlesRootObj);
        bottlesDataJson = "btl_nw" + bottlesDoc.toJson();
        //qDebug() << bottlesDoc.toJson();

        return clntSock.sendToServer(bottlesDataJson);
    }


    QByteArray rmBottle(QString bottleName)
    {
        bottleName = "btl_rm" + bottleName;
        return clntSock.removeBottleFromServer(bottleName);
    }

    /*bool restartConnection(){
        if(clntSock.startConnection()){
            return true;
        }
        return false;
    }*/

private:

    ClientSocket clntSock;

    struct Drinks{//drinks struct to pass to private methods from makeDrinkObj
        Drinks(QString ingr1, int ingr1_cl,
               QString ingr2, int ingr2_cl,
               QString ingr3, int ingr3_cl) :
            ingr1_(ingr1), ingr1_cl_(ingr1_cl),
            ingr2_(ingr2), ingr2_cl_(ingr2_cl),
            ingr3_(ingr3), ingr3_cl_(ingr3_cl)
        {}
        QString ingr1_;
        int ingr1_cl_;
        QString ingr2_;
        int ingr2_cl_;
        QString ingr3_;
        int ingr3_cl_;
    };

    DrinkMachineClient()
    {
        clntSock.startConnection();
    }

    //NewDrink::
    QJsonObject addIngrObjs(Drinks* drinks_)
    {
        QJsonObject obj;

        if(drinks_->ingr1_ != ""){
            obj.insert(drinks_->ingr1_,drinks_->ingr1_cl_);
        }
        if(drinks_->ingr2_ != ""){
            obj.insert(drinks_->ingr2_,drinks_->ingr2_cl_);
        }
        if(drinks_->ingr3_ != ""){
            obj.insert(drinks_->ingr3_,drinks_->ingr3_cl_);
        }

        return obj;
    }

    QJsonObject mergeObjs(Drinks* drinks_)
    {
        QJsonObject ingrObj;

        ingrObj.insert("ingredienser",addIngrObjs(drinks_));
                                 // using the return value from
                                 // addIngrObjs(returns QJsonObj) as second arg to insert
        return ingrObj;
    }

    //DRINKS
    QByteArray drinksDataJson;
    QJsonObject drinksRootObj;
    QJsonDocument drinksDoc;

    //BOTTLES
    QByteArray bottlesDataJson;
    QJsonObject bottlesRootObj;
    QJsonDocument bottlesDoc;

};

#endif // NEWDRINK_H
