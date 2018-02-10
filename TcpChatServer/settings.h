#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QVariant>
#include <QDir>

#include "core_global.h"

class CORESHARED_EXPORT Settings
{
public:
    /**
     * @brief getInstance синглтон.
     * @return возвращает указатель на себя.
     */
    static Settings *getInstance();

    void setUserAuthorizationLogin(QString login);
    void setUserAuthorizationPassword(QString password);

    void setUserRegistrationLogin(QString login);
    void setUserRegistrationNickName(QString nickName);
    void setUserRegistrationPassword(QString password);

    QVariant getUserAuthorizationLogin() {return authorizationLogin_; }
    QVariant getUserAuthorizationPassword() { return authorizationPassword_; }

    QVariant getUserRegistrationLogin() {return registrationLogin_; }
    QVariant getUserRegistrationNickName() {return registrationNickName_; }
    QVariant getUserRegistrationPassword() { return registrationPassword_; }

private:
    Settings() {

    };
    virtual ~Settings() {

    };

    static Settings *instance_;

    QString authorizationLogin_;
    QString authorizationPassword_;

    QString registrationLogin_;
    QString registrationNickName_;
    QString registrationPassword_;
};

#endif // SETTINGS_H
