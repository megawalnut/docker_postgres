#ifndef AUTHPAGE_H
#define AUTHPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>
#include <QCryptographicHash>

#include "controller.h"

class AuthPage : public QWidget {
    Q_OBJECT
public:
    enum class TabPages {
        SignIn = 0,
        SignUp
    };

    explicit AuthPage(Controller* controller, QWidget *parent = nullptr);

private:
    void init();
    void setupConnections();

private:
    QLineEdit* m_loginUser = nullptr;
    QLineEdit* m_loginPass = nullptr;
    QPushButton* m_loginBtn = nullptr;

    QLineEdit* m_regUser = nullptr;
    QLineEdit* m_regPass = nullptr;
    QPushButton* m_regBtn = nullptr;

    //controller
    Controller* m_controller = nullptr;
};

#endif // AUTHPAGE_H
