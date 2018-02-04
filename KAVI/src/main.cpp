#include <QApplication>
#include "KAVIMainWindow.h"

#include <iostream>

using namespace std;

KAVIMainWindow *msgCatcher;

void msgHandler(QtMsgType type, const QMessageLogContext &context, const QString & msg)
{
    QString text(msg);
    bool prefixed = (text.at(0) == PREFIX);
    if (prefixed)
        text = text.remove(0, 1);

    switch (type)
    {
    case QtDebugMsg:
        if (prefixed)
            msgCatcher->appendToLog(prefixed, text, Qt::cyan);
        else
            msgCatcher->appendToLog(prefixed, text, Qt::green);
        break;
    case QtWarningMsg:
        if (prefixed)
            msgCatcher->appendToLog(prefixed, text, Qt::red);
        else
            msgCatcher->appendToLog(prefixed, text, Qt::blue);
        break;
    case QtCriticalMsg:
        if (prefixed)
            msgCatcher->appendToLog(prefixed, text, Qt::yellow);
        else
            msgCatcher->appendToLog(prefixed, text, Qt::darkYellow);
        break;
    case QtFatalMsg:
        msgCatcher->appendToLog(false, text.prepend("FATAL "));
        msgCatcher->crash();
    }
}

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    KAVIMainWindow mainWindow;

    msgCatcher = &mainWindow;

    //qInstallMsgHandler(msgHandler);
    qInstallMessageHandler(msgHandler);

    mainWindow.show();
    return app.exec();
}
