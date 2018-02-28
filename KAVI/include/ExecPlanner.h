/*
 * @author: liyc
 * @date  : 2018/2/27
 * @brief : the class handle the the execution of planner
*/

#ifndef EXECPLANNER_H
#define EXECPLANNER_H

#include "KAVIBase.h"
#include "XMLUtils.h"

class ExecPlanner: public QThread {
    Q_OBJECT

public:
    ExecPlanner();
    ExecPlanner(QDomElement chosenPlanner, QString domainFile, QString problemFile, bool replaning);

    QString getContentsAsString(QFile& file);
    QStringList getContentsAsStringList(QFile& file);

    void setEmptyPlan();

    void getPlanAndStatistics(QStringList output, QStringList plan, QStringList statistics);

    QStringList getPlan(QStringList output);

    void processError(QProcess::ProcessError error);
    void startProcess();
    void finishProcess(int exitCode, QProcess::ExitStatus exitStatus);
    void startReadOutput();
    void startReadError();

    QStringList getPlannerOutput(QDomElement chosenPlanner, QString domain, QString problem, QStringList consoleOutput);

    void parsePlanToXML(QDomElement planNode, QStringList plan);

    void parseStatisticsToXML(QDomElement statisticNode, QStringList statistic);

    QDomElement solvePlanningProblem(QDomElement chosenPlanner, QString domainFile, QString problemFile);

    QDomElement solveProblem();

    QString generateHTMLReport(QDomElement xmlPlan);

    void setXMLDomain(QDomElement domain);

    void setXMLProblem(QDomElement problem);

    void setDomainName(QString name);

    void setProblemName(QString name);

    QString getProblemName();

    void setChosenPlanner(QDomElement chosenPlanner);

    QDomElement getChosenPlanner();

    void setShowReport(bool showReport);

    QDomElement getPlan();

    void setPlan(QDomElement thePlan);

    void run();

    void destroyProcess();

    QString getOperatingSystem();

    bool getXMLDocument();

    QString getXMLFilePath();

    bool writeToXMLFile();

    QString getPlannersPath();
private:
    // planner execution time in milliseconds. Time used to solve the problem or give an answer.
    double time = 0;
    QString toolMessage = "";
    QProcess* process;
    bool gotError = false;
    bool readyReadOutput = false;
    bool normalExit = false;

    QDomElement chosenPlanner;
    QString domainFile;
    QString problemFile;
    QString domainName;
    QString problemName;

    QDomElement xmlDomain;
    QDomElement xmlProblem;


    bool rePlaning;
    bool showReport;
    QDomDocument planDocument;
    QDomElement thePlan;

    //QFile plannerRunFile;

    // the running mode of KAVI: Debug or Release
    RunMode KAVIRunMode;
};

#endif // EXECPLANNER_H
