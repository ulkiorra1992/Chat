#include "tcpchatserver.h"

#include <QtGui>
#include <iostream>
#include <QTextCodec>
#include <QApplication>

void initCodec();

int main(int argc, char *argv[])
{
/* Для адекватной регистрации приложения в операционной
 * необходимо установить название организации, домен организации,
 * а также название приложения.
 * Все настройки  будут храниться в операционной системе под этими
 * учетными данными приложения
 * */
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    QApplication app(argc, argv);
    initCodec();
    TcpChatServer server;

    server.show();

    return app.exec();
}

// ==================== Oтображение русских букв =============================//
void initCodec()
{
    const char *codecName = "UTF-8";
#ifdef Q_WS_WIN
    const char *codecForLocaleName = "CP866";
#else
    const char *codecForLocaleName = "UTF-8";
#endif

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName(codecName));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName(codecForLocaleName));
    QTextCodec::setCodecForTr(QTextCodec::codecForName(codecName));
}
