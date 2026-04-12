#include <QApplication>

#include "appContext.h"
#include "mainwindow.h"
#include "controller.h"

constexpr uint32_t PORTNUM = 5555;
const QString ADDRESS = "127.0.0.1";
const QString PATH = "/home/mikhail/projs/pet_proj_1/client/app.db";

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto* client = new Client(PORTNUM, ADDRESS);

    AppContext::instance().init(-1, "", {});

    auto* table = new TableModel();
    auto* controller = new Controller(client, table, {});

    table->setParent(controller);

    MainWindow w(controller);
    w.show();

    return a.exec();
}
