#include "userregistration.h"

#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

UserRegistration::UserRegistration(QWidget *pwgt):
    QDialog(pwgt, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
    setWindowTitle("Регистрация пользователя");
    setMinimumWidth(250);
    setMaximumHeight(50);
    setMaximumHeight(250);

    userLogin_ = new QLineEdit();
    userNickName_ = new QLineEdit();
    userPassword_ = new QLineEdit();

    QLabel *lblLogin = new QLabel("Логин:");
    QLabel *lblNickName = new QLabel("Имя в системе");
    QLabel *lblPassword = new QLabel("Пароль:");

    QPushButton *pbOk = new QPushButton ("Применить");

    connect(pbOk, SIGNAL(clicked(bool)), SLOT(accept()));

    QGridLayout *pLayout = new QGridLayout;
    pLayout->addWidget(lblLogin, 0, 0);
    pLayout->addWidget(lblNickName, 1 , 0);
    pLayout->addWidget(lblPassword, 2, 0);
    pLayout->addWidget(userLogin_, 0, 1);
    pLayout->addWidget(userNickName_, 1, 1);
    pLayout->addWidget(userPassword_, 2, 1);
    pLayout->addWidget(pbOk, 3, 1);
    setLayout(pLayout);
}

QString UserRegistration::userLogin() const
{
    return userLogin_->text();
}

QString UserRegistration::userNickName() const
{
    return userNickName_->text();
}

QString UserRegistration::userPassword() const
{
    return userPassword_->text();
}
