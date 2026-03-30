#ifndef AUTHPAGE_H
#define AUTHPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>
#include <QCryptographicHash>


class AuthPage : public QWidget {
    Q_OBJECT
public:
    enum class TabPages {
        SignIn = 0,
        SignUp
    };

    explicit AuthPage(QWidget *parent = nullptr);

private:
    void init();
    void setupConnections();
    QString hashPassword(const QString& password) const;

signals:
    void loginRequested(QString user, QString pass);
    void registerRequested(QString user, QString pass);

private:
    QLineEdit* m_loginUser;
    QLineEdit* m_loginPass;
    QPushButton* m_loginBtn;

    QLineEdit* m_regUser;
    QLineEdit* m_regPass;
    QPushButton* m_regBtn;
};

#endif // AUTHPAGE_H
