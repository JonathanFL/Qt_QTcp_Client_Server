#ifndef SCOPEDFILEREAD_H
#define SCOPEDFILEREAD_H

#include <QDebug>
#include <QFile>

class ScopedFileRead
{
public:
    ScopedFileRead(QFile* file) {
        filePtr = file;
        if(filePtr->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            //qDebug() << "scoped file opened";
        }else{
            qDebug() << "Failed to open file in scoped-read constructor";
        }

    }
    ~ScopedFileRead(){
        filePtr->close();
        qDebug() << "scoped file closed";
    }

    QByteArray readAll(){
        return filePtr->readAll();
    }

    qint64 size(){
        return filePtr->size();
    }

    bool isOpen(){
        return filePtr->isOpen();
    }

private:
    QFile* filePtr;
};

#endif // SCOPEDFILEREAD_H
