#ifndef SCOPEDFILEWRITE_H
#define SCOPEDFILEWRITE_H

#include <QFile>

class ScopedFileWrite
{
public:
    ScopedFileWrite(QFile* file) {
        filePtr = file;
        filePtr->open(QIODevice::WriteOnly | QIODevice::Text);
    }
    ~ScopedFileWrite(){
        filePtr->close();
    }

    void write(QByteArray contentToWrite){
        filePtr->write(contentToWrite);
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

#endif // SCOPEDFILEWRITE_H
