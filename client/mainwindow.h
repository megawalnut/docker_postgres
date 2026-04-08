#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "../UI/authPage.h"
#include "../UI/mainPage.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    enum class Pages {
        Auth = 0,
        Main,
    };

    explicit MainWindow(Controller* controller, QWidget *parent = nullptr);

private:
    void init();
    void setupConnections();

private:
    QStackedWidget* m_stack = nullptr;
    AuthPage* m_authPage = nullptr;
    MainPage* m_mainPage = nullptr;

    Controller* m_controller = nullptr;
};
#endif // MAINWINDOW_H
