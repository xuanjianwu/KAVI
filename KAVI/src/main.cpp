#include <QApplication>
#include "KAVIMainWindow.h"

#include <iostream>
#include "igraph.h"
#include "KAVIClassKB.h"

using namespace KAVIGraph;

using namespace std;

KAVIMainWindow *msgCatcher;

void msgHandler(QtMsgType type, const QMessageLogContext &context, const QString & msg)
{
    QString text(msg);
    //bool prefixed = (text.at(0) == PREFIX);
    bool prefixed = (text.at(0) == KAVIPREFIX);
    if (prefixed)
        text = text.remove(0, 1);

    switch (type)
    {
    case QtDebugMsg:
        if (!prefixed)
            msgCatcher->appendToLog(prefixed, text, Qt::cyan);
        else
            msgCatcher->appendToLog(prefixed, text, Qt::green);
        break;
    case QtWarningMsg:
        if (!prefixed)
            msgCatcher->appendToLog(prefixed, text, Qt::red);
        else
            msgCatcher->appendToLog(prefixed, text, Qt::blue);
        break;
    case QtCriticalMsg:
        if (!prefixed)
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
    //mainWindow.setWindowFlags(mainWindow.windowFlags() & Qt::WindowMaximizeButtonHint& Qt::WindowMinimizeButtonHint);

    msgCatcher = &mainWindow;

    //qInstallMsgHandler(msgHandler);
    qInstallMessageHandler(msgHandler);

    mainWindow.show();
    return app.exec();
}


/*
int main3()
{
    igraph_t graph;
    graph.init(7, IGRAPH_DIRECTED);
    QVector<int> edges;
    edges << 1 << 2 << 2 << 3 << 4 << 2 << 2 << 5 << 5 << 7 << 5 << 6
          << 6 << 7;
    graph.addEdges(edges);
    QSet<int> vertices = graph.getVertices();
    cout << graph.getVertexCount() << endl;
    cout << graph.getEdgeCount() << endl;

    return 0;
}
*/

