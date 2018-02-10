#include "userauthorization.h"

#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

UserAuthorization::UserAuthorization(QWidget *pwgt) :
    QDialog(pwgt, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
    setWindowTitle("Авторизация пользователя");
    setMinimumWidth(250);
    setMaximumHeight(50);
    setMaximumHeight(250);
    userLogin_ = new QLineEdit();
    userPassword_ = new QLineEdit();

    QLabel *lblLogin = new QLabel("Логин:");
    QLabel *lblPassword = new QLabel("Пароль:");

    QPushButton *pbOk = new QPushButton ("Применить");

    connect(pbOk, SIGNAL(clicked(bool)), SLOT(accept()));

    QGridLayout *pLayout = new QGridLayout;
    pLayout->addWidget(lblLogin, 0, 0);
    pLayout->addWidget(lblPassword, 1, 0);
    pLayout->addWidget(userLogin_, 0, 1);
    pLayout->addWidget(userPassword_, 1, 1);
    pLayout->addWidget(pbOk, 3, 1);
    setLayout(pLayout);
}

QString UserAuthorization::userLogin() const
{
    return userLogin_->text();
}

QString UserAuthorization::userPassword() const
{
    return userPassword_->text();
}
