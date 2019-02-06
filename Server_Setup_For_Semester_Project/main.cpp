#include <QCoreApplication>
#include <RPiServer.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    RPiServer* Server = new RPiServer(&a);
    Server->StartServer();

    return a.exec();
}
