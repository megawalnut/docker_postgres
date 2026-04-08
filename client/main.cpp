#include <QApplication>

#include "appContext.h"
#include "mainwindow.h"
#include "controller.h"

constexpr uint32_t PORTNUM = 5555;
const QString ADDRESS = "127.0.0.1";
const QString PATH = "/home/mikhail/projs/pet_proj_1/client";

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Client* client = new Client(PORTNUM,  // port
                                ADDRESS);    // host

    AppContext::instance().init(-1, "a", {});  //TODO

    TableModel* table = new TableModel();
    QStringList list = { "user1", "user2", "user3" };

    Controller* controller = new Controller(client, table, list);

    table->setParent(controller);

    MainWindow w(controller);

    w.show();

    delete client;
    return a.exec();
}
