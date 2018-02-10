#pragma once

#include <QDialog>
#include <QLineEdit>

class UserRegistration : public QDialog
{
public:
    UserRegistration(QWidget* pwgt = 0);

    QString userLogin() const;
    QString userNickName() const;
    QString userPassword() const;

private:
    QLineEdit *userLogin_;
    QLineEdit *userNickName_;
    QLineEdit *userPassword_;
};

