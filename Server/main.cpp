#include <QCoreApplication>

#include "server.h"
#include "dbhelper.h"


const uint32_t PORTSERVER = 5555;    //the base server port
const uint32_t PORTDB = 5432;    //the base PostgreSQL port

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    auto db = std::make_shared<DBHelper>("admin",     // username
                                        "127.0.0.1",    // host
                                        "test_database",// dataBase name
                                        "root");       // password

    if(!db->connect()) {
        qFatal("Main::main: Failed to connect test_database");
    }
    qDebug() << "Main::main: Success to connect test_database";

    Server server(db);
    qDebug() << "Main::main: Start";

    return a.exec();
}
