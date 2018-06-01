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
    /*
     * construct function
     * @params:
     *      parent - parent object
     * @return: N/A
    */
    ExecPlanner(QObject *parent = 0);

    /*
     * construct function
     * @params:
     *      chosenPlanner - the chosen planner to execute plan
     *      domainFile    - the domain PDDL file
     *      problemFile   - the problem PDDL file
     *      replaning     - replaning or not
     *      parent        - parent object
     * @return: N/A
    */
    ExecPlanner(QDomElement chosenPlanner, QString domainFile, QString problemFile, bool replaning, QObject *parent = 0);

    /*
     * get the content of file as string
     * @params:
     *      file - the specified file
     * @return: content string
    */
    QString getContentsAsString(QFile& file);

    /*
     * get the content of file as string list
     * @params:
     *      file - the specified file
     * @return: content string list
    */
    QStringList getContentsAsStringList(QFile& file);

    /*
     * reset empty plan
     * @params: N/A
     * @return: N/A
    */
    void setEmptyPlan();

    /*
     * get the plan and statistics from planner's output
     * @params:
     *      output     - the planner's output
     *      plan       - the plan
     *      statistics - the statistics
     * @return: N/A
    */
    void getPlanAndStatistics(QStringList output, QStringList& plan, QStringList& statistics);

    /*
     * get the plan from output
     * @params:
     *      output - the output
     * @return: plan as string list
    */
    QStringList getPlan(QStringList output);

    /*
     * execute plan and get the planner's output
     * @params:
     *      chosenPlanner - the chosen planner to execute plan
     *      domain        - the domain file
     *      problem       - the problem file
     *      consoleOutput - the console output of planner
     * @return: output as string list
    */
    QStringList getPlannerOutput(QDomElement chosenPlanner, QString domain, QString problem, QStringList& consoleOutput);

    /*
     * parse the plan to xml format
     * @params:
     *      planNode - the xml element
     *      plan     - the specified plan
     * @return: N/A
    */
    void parsePlanToXML(QDomElement planNode, QStringList plan);

    /*
     * parse the statistic to xml format
     * @params:
     *      statisticNode - the xml element
     *      statistic     - the specified statistic
     * @return: N/A
    */
    void parseStatisticsToXML(QDomElement statisticNode, QStringList statistic);

    /*
     * solve the specified problem with the chosen planner
     * @params:
     *      chosenPlanner - the chosen planner
     *      domainFile    - the domain PDDL file
     *      problemFile   - the problem PDDL file
     * @return: the plan result in xml format
    */
    QDomElement solvePlanningProblem(QDomElement chosenPlanner, QString domainFile, QString problemFile);

    /*
     * solve problem by solvePlanningProblem
     * @params: N/A
     * @return: N/A
    */
    QDomElement solveProblem();

    QString generateHTMLReport(QDomElement xmlPlan);

    void setXMLDomain(QDomElement domain);

    void setXMLProblem(QDomElement problem);

    /*
     * set the domain name
     * @params:
     *      name - the specified domain name
     * @return: N/A
    */
    void setDomainName(QString name);

    /*
     * set the problem name
     * @params:
     *      name - the specified problem name
     * @return: N/A
    */
    void setProblemName(QString name);

    /*
     * get the problem name
     * @params: N/A
     * @return: problem name
    */
    QString getProblemName();

    /*
     * set the chosen planner
     * @params:
     *      chosenPlanner - the chosen planner in xml format
     * @return: N/A
    */
    void setChosenPlanner(QDomElement chosenPlanner);

    /*
     * get the chosen planner
     * @params: N/A
     * @return: the chosen planner in xml format
    */
    QDomElement getChosenPlanner();

    void setShowReport(bool showReport);

    /*
     * get the plan in xml format
     * @params: N/A
     * @return: plan in xml format
    */
    QDomElement getPlan();

    /*
     * set the plan
     * @params:
     *      thePlan - plan in xml format
     * @return: N/A
    */
    void setPlan(QDomElement thePlan);

    /*
     * destroy currennt process
     * @params: N/A
     * @return: N/A
    */
    void destroyProcess();

    /*
     * get the current operating system
     * @params: N/A
     * @return: system in string
    */
    QString getOperatingSystem();

    /*
     * init the xml plan document
     * @params: N/A
     * @return: success or not
    */
    bool getXMLDocument();

    /*
     * get the path of planners configs file
     * @params: N/A
     * @return: file path
    */
    QString getXMLFilePath();

    /*
     * write the plan to xml file
     * @params: N/A
     * @return: success or not
    */
    bool writeToXMLFile();

    /*
     * write the plan to txt file
     * @params:
     *      plan - actions sequence in plan
     * @return: N/A
    */
    void writePlanToDefaultPath(QStringList& plan);

    /*
     * get the PDDL file path
     * @params: N/A
     * @return: file path
    */
    QString getPDDLFilePath();

    /*
     * get the planners path
     * @params: N/A
     * @return: N/A
    */
    QString getPlannersPath();

    /*
     * remove troublesome characters of string
     * @params:
     *      inString - the specified string
     * @return: the target string
    */
    QString removeTroublesomeCharacters(QString inString);

    QStringList getTestConsoleOutput();

    /*
     * get the path of generated files of planner
     * @params: N/A
     * @return: file path
    */
    QString getPlannerGeneratedFilesPath();

signals:
    /*
     * append the log to KAVI console
     * @params:
     *      text - the specified text log
     * @return: N/A
    */
    void appendToLog(QString text);

public slots:
    void processError(QProcess::ProcessError error);
    void startProcess();
    void finishProcess(int exitCode, QProcess::ExitStatus exitStatus);
    void startReadOutput();
    void startReadError();

protected:
    void run();
private:
    // planner execution time in milliseconds. Time used to solve the problem or give an answer.
    double time = 0;

    // the log message
    QString toolMessage = "";

    // the process pointer
    QProcess* process;
    bool gotError;
    bool readyReadOutput;
    bool normalExit;

    // the chosen planner's infomation in xml format
    QDomElement chosenPlanner;

    // the domain PDDL file
    QString domainFile;

    // the problem PDDL file
    QString problemFile;

    // the domain name
    QString domainName;

    // the problem name
    QString problemName;

    // the domain in xml format
    QDomElement xmlDomain;

    // the problem in xml format
    QDomElement xmlProblem;

    QStringList testConsoleOutput;

    bool rePlaning;
    bool showReport;

    // the xml document of plan
    QDomDocument planDocument;

    // the plan in xml format
    QDomElement thePlan;

    //QFile plannerRunFile;

    // the running mode of KAVI: Debug or Release
    RunMode KAVIRunMode;
};

#endif // EXECPLANNER_H
