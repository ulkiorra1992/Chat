#include "tcpchatclient.h"
#include <QApplication>
#include <QTextCodec>

void initCodec();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    initCodec();
    TcpChatClient client;
    client.show();

    return a.exec();
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
