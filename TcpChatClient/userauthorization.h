#pragma once

#include <QDialog>
#include <QLineEdit>

class UserAuthorization : public QDialog
{
public:
    UserAuthorization(QWidget* pwgt = 0);

    QString userLogin() const;
    QString userPassword() const;

private:
    QLineEdit *userLogin_;
    QLineEdit *userPassword_;
};

