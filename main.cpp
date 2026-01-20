#include "server.h"
#include "dbhelper.h"
#include "icommand.h"

#include <QCoreApplication>

const uint32_t portNum = 5555;

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    std::unique_ptr<DBHelper> p_dataBase = std::make_unique<DBHelper>("postgres",     // username
                                                                      "127.0.0.1",    // host
                                                                      "mydatabase",   // dataBase name
                                                                      "admin");       // password
    ICommand::addDb(std::move(p_dataBase));

    Server server(portNum);
    qDebug() << "main::start";

    return a.exec();
}
