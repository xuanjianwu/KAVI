#include "ExecPlanner.h"

using namespace XMLUtils;

ExecPlanner::ExecPlanner(QObject *parent)
    : QThread(parent)
{
    KAVIRunMode = Debug;
    getXMLDocument();
}

ExecPlanner::ExecPlanner(QDomElement chosenPlanner, QString domainFile, QString problemFile, bool replaning, QObject *parent)
    :QThread(parent)
{
    this->chosenPlanner = chosenPlanner;
    this->domainFile = domainFile;
    this->problemFile = problemFile;
    this->rePlaning = replaning;

    KAVIRunMode = Debug;
    getXMLDocument();
}

QString ExecPlanner::getContentsAsString(QFile &file)
{
    QString res;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str(line);
        res.append(str).append("\n");
    }
    file.seek(0);
    return res;
}

QStringList ExecPlanner::getContentsAsStringList(QFile &file)
{
    QStringList res;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str(line);
        res.append(str);
    }
    file.seek(0);
    return res;
}

void ExecPlanner::setEmptyPlan()
{
    //1. Get the default plan (empty)
    thePlan = planDocument.firstChildElement("xmlPlan");
    QDomElement domainElement = thePlan.firstChildElement("domain");
    QDomElement problemElement = thePlan.firstChildElement("problem");

    //2 set the domain and problem names
    setStrValue(domainElement, domainName);
    setStrValue(problemElement, problemName);

    //3. set the planner info
    if (!this->chosenPlanner.isNull())
    {
        QDomElement plannerElement = thePlan.firstChildElement("planner");
        //3.1 set the planner id
        //setIntAttribute(plannerElement, "id", getIntAttribute(this->chosenPlanner, "id"));

        //3.2 add planner's characteristics
        //plannerElement.appendChild(chosenPlanner.cloneNode(true));
        /*
        QDomNodeList plannerFeatures = this->chosenPlanner.childNodes();
        for (int i = 0; i < plannerFeatures.size(); i++)
        {
            QDomElement plannerFeatureElement = plannerFeatures.at(i).toElement();
            plannerElement.ownerDocument().importNode(plannerFeatureElement, true);
            plannerElement.appendChild(plannerFeatureElement);
        }
        */
        QDomNode previousNode = plannerElement.previousSibling();
        thePlan.removeChild(plannerElement);
        thePlan.insertAfter(this->chosenPlanner.cloneNode(true), previousNode);

        QDomElement newPlannerElement = thePlan.firstChildElement("planner");
        //except for setting
        QDomElement settingsElement = newPlannerElement.firstChildElement("settings");
        newPlannerElement.removeChild(settingsElement);
    }
}

void ExecPlanner::getPlanAndStatistics(QStringList output, QStringList &plan, QStringList &statistics)
{
    //Separate statistics and plan (get plan)
    if (!output.isEmpty())
    {
        foreach (QString element, output) {
            if (!element.trimmed().isEmpty())
            {
                // get statistics
                if (element.trimmed().startsWith(";"))
                {
                    statistics.append(element.trimmed().remove(0, 1).trimmed());
                }
                else
                {
                    // if it is not a standard action then check if is still an action or a statistic
                    if (!(element.indexOf(":") > -1))
                    {
                        bool isAnAction = false;

                        //check if the string can still be an action (e.g. 1 (action p1 p2 ... pn) )
                        if ((element.indexOf("(") > -1) && (element.indexOf(")") > -1))
                        {
                            //check if the first element on the string is the plan index
                            QString firstItem = "index";
                            QStringList tmpStrList = element.simplified().split(" ");
                            if (!tmpStrList.isEmpty())
                            {
                                firstItem = tmpStrList[0];
                                bool ok;
                                double theIndex = firstItem.toDouble(&ok);
                                if (ok)
                                {
                                    isAnAction = true;
                                }
                                else
                                {
                                    isAnAction = false;
                                }
                            }

                            //if it is an action, then include the ":" for standarlization
                            if (isAnAction)
                            {
                                QString actionBody = "";
                                for (int i = 1; i < tmpStrList.size(); i++) {
                                    // for each parameter, create a node
                                    actionBody.append(" ").append(tmpStrList[i]);
                                }
                                element = firstItem.append(":").append(actionBody);
                            }
                        }

                        if (isAnAction)
                        {
                            plan.append(element.trimmed());
                        }
                        else
                        {
                            statistics.append(element.trimmed());
                        }
                    }
                    else
                    {
                        //When it is really an standard action(e.g. 1: (action p1 p2 ... pn) )
                        plan.append(element.trimmed());
                    }
                }
            }
        }
    }
}

QStringList ExecPlanner::getPlan(QStringList output)
{
    QStringList plan;
    if (!output.isEmpty())
    {
        foreach (QString element, output) {
            if (!element.trimmed().isEmpty())
            {
                if (!element.trimmed().startsWith(";"))
                {
                    plan.append(element.trimmed());
                }
            }
        }
    }
    return plan;
}

void ExecPlanner::processError(QProcess::ProcessError error)
{
    //qDebug() << "@Something wrong while running planner";
    gotError = true;
    switch(error)
    {
    case QProcess::FailedToStart:
        QMessageBox::information(0, "Tip", "FailedToStart");
        break;
    case QProcess::Crashed:
        QMessageBox::information(0, "Tip", "Crashed");
        break;
    case QProcess::Timedout:
        QMessageBox::information(0, "Tip", "Timedout");
        break;
    case QProcess::WriteError:
        QMessageBox::information(0, "Tip", "WriteError");
        break;
    case QProcess::ReadError:
        QMessageBox::information(0, "Tip", "ReadError");
        break;
    case QProcess::UnknownError:
        QMessageBox::information(0, "Tip", "UnknownError");
        break;
    default:
        QMessageBox::information(0, "Tip", "UnknownError");
        break;
    }
}

void ExecPlanner::startProcess()
{

}

void ExecPlanner::finishProcess(int exitCode, QProcess::ExitStatus exitStatus)
{
    //normalExit = true;
    if (exitStatus == QProcess::NormalExit)
    {
        normalExit = true;
    }
    else
    {
        normalExit = false;
    }
}

void ExecPlanner::startReadOutput()
{
    readyReadOutput = true;
}

void ExecPlanner::startReadError()
{

}

QStringList ExecPlanner::getPlannerOutput(QDomElement chosenPlanner, QString domain, QString problem, QStringList &consoleOutput)
{
    QStringList output;
    QString solutionFile =  "solution.soln";

    //1.Get main planner's parameters and arguments
    QDomElement settings = chosenPlanner.firstChildElement("settings");
    QStringList commandArguments;

    //1.0 Get planner execution file
    QString plannerFilePath;
    plannerFilePath.append(getPlannersPath()).append(getStrValue(settings.firstChildElement("filePath")));
    commandArguments.append(plannerFilePath);

    QFile plannerFile(plannerFilePath);
    //plannerRunFile = plannerFile;
    bool plannerFileExists = true;
    if (!plannerFile.exists())
    {
        plannerFileExists = false;
        toolMessage += ">> Could not find selected planner '"+ getStrValue(settings.firstChildElement("filePath")) +"'. \n" +
                ">> Please download and copy it in the folder /KAVIPlanners \n";
        // should append toolMessage to planning dialog
    }

    //proceed only if planner file exists
    if (plannerFileExists)
    {
        //GET ALL ARGUMENTS (including domain and problem) in the specified ORDER
        //This makes planners have arguments before and after to the domain and problem arguments
        //1.1 Get all arguments
        QList<QDomElement> theArgs;
        QDomNodeList allArgs = settings.firstChildElement("arguments").childNodes();
        for (int i = 0; i < allArgs.size(); i++)
        {
            QDomElement arg = allArgs.at(i).toElement();
            if (getStrValue(arg.firstChildElement("enable")) == "true")
            {
                theArgs.append(arg);
            }
        }

        if (!theArgs.isEmpty())
        {
            for (int i = 0; i < theArgs.size(); i++)
            {
                QDomElement argElement = theArgs[i];
                if (!getStrAttribute(argElement, "parameter").trimmed().isEmpty())
                {
                    commandArguments.append(getStrAttribute(argElement, "parameter"));
                }
                //check if this is a domain argument
                if (argElement.tagName() == "domain")
                {
                    commandArguments.append(domain);
                }
                //check if this is a problem argument
                else if (argElement.tagName() == "problem")
                {
                    commandArguments.append(problem);
                }
                //if there is a value for the argument then add to the command
                else if (!getStrValue(argElement.firstChildElement("value")).trimmed().isEmpty())
                {
                    commandArguments.append(getStrValue(argElement.firstChildElement("value")));
                }
            }
        }

        //1.2 Get output arguments
        bool outputFile;
        QDomElement outputElement = settings.firstChildElement("output");
        if (getStrAttribute(outputElement, "hasOutputFile") == "true")
        {
            outputFile = true;
            solutionFile = getStrValue(outputElement.firstChildElement("outputFile").firstChildElement("fileName")).trimmed();
            if (getStrAttribute(outputElement.firstChildElement("outputFile").firstChildElement("argument"), "needArgument") == "true")
            {
                commandArguments.append(getStrAttribute(outputElement.firstChildElement("outputFile").firstChildElement("argument"), "parameter"));
                commandArguments.append(solutionFile);
            }
        }
        else
        {
            outputFile = false;
        }

        //Prepare the command line
        QString program = commandArguments.at(0);
        commandArguments.removeAt(0);
        QStringList arguments = commandArguments;

        this->time = 0;
        //set initial time
        QTime start_time;
        start_time.start();

        // ItSIMPLE.getInstance().appendOutputPanelText("\n>> Calling planner "+ chosenPlanner.getChildText("name")+ "\n ");
        gotError = false;
        readyReadOutput = false;
        normalExit = false;

        process = new QProcess(this);
        //process->setProcessChannelMode(QProcess::SeparateChannels);
        //process->setReadChannel(QProcess::StandardOutput);

        connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
        connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(startReadOutput()));
        connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finishProcess(int, QProcess::ExitStatus)));

        //Call the planner
        process->start(program, arguments);
        //process->execute(program, arguments);
        //process->startDetached(program, arguments);
        process->waitForFinished();

        //if (!gotError && normalExit && readyReadOutput)
        if (!gotError)
        {
            consoleOutput = QString::fromLocal8Bit(process->readAllStandardOutput()).split("\n");

            testConsoleOutput = consoleOutput;

            // ItSIMPLE.getInstance().appendOutputPanelText("\n>> Planner's output read. \n");

            this->time = start_time.elapsed();

            //The planner does provide a output file
            if (outputFile)
            {
                //Checks if the planner put some automatic string in the output file name (i.e., .SOL)
                if (!getStrValue(outputElement.firstChildElement("outputFile").firstChildElement("fileNameAutomaticIncrement")).trimmed().isEmpty())
                {
                    solutionFile.append(getStrValue(outputElement.firstChildElement("outputFile").firstChildElement("fileNameAutomaticIncrement")).trimmed());
                }

                //Get the planner answer exposed in the solution Output File

                //QString outputFilePath;
                //outputFilePath.append(getPlannersPath()).append(KAVI_PLANNERS_FOLDER).append(solutionFile);
                //QFile theOutputFile(outputFilePath);

                QString outputFilePath;
                outputFilePath.append(getPlannerGeneratedFilesPath()).append(solutionFile);
                QFile theOutputFile(outputFilePath);

                if ( !theOutputFile.open(QFile::ReadOnly | QFile::Text ))
                {
                    //qDebug()<< "@Error: cannot open file: " << theOutputFile.fileName();
                    return QStringList();
                }

                // get output
                output = getContentsAsStringList(theOutputFile);
                theOutputFile.remove();

                // delete additional generated files
                QDomNodeList generatedFiles = outputElement.firstChildElement("outputFile").firstChildElement("additionalGeneratedFiles").childNodes();
                for (int i = 0; i < generatedFiles.size(); i++) {
                    QDomElement generatedFile = generatedFiles.at(i).toElement();
                    QString generatedFilePath;
                    generatedFilePath.append(getPlannerGeneratedFilesPath()).append(getStrValue(generatedFile));
                    QFile theGeneratedFile(generatedFilePath);

                    if (theGeneratedFile.exists())
                    {
                        theGeneratedFile.remove();
                    }
                }
            }
            //The planner does not provide a output file, just the console message
            else
            {
                QString planStartIdentifier = getStrValue(outputElement.firstChildElement("consoleOutput").firstChildElement("planStartIdentifier"));
                int startsAfterNlines = getIntAttribute(outputElement.firstChildElement("consoleOutput").firstChildElement("planStartIdentifier"), "startsAfterNlines");
                QString planEndIdentifier = getStrValue(outputElement.firstChildElement("consoleOutput").firstChildElement("planEndIdentifier"));

                QStringList planList;
                QStringList statisticsList;

                bool isThePlan = false;

                for (int i = 0; i < consoleOutput.size(); i++)
                {
                    QString line = consoleOutput.at(i);
                    //Check if line contains start identifier (only if the plan was not found yet)
                    int indexPlanStart = -1;
                    if (!isThePlan)
                    {
                        indexPlanStart = line.indexOf(planStartIdentifier);
                    }
                    //The plan was found
                    if (!isThePlan && indexPlanStart > -1) {
                        isThePlan = true;

                        //Jump the necessary lines to reach the first line of the plan
                        //First action is in the same line as the idetifier.
                        if (startsAfterNlines == 0) {
                            line = line.remove(indexPlanStart, planStartIdentifier.length());
                        }
                        //Jump to the first line of the plan
                        else if (startsAfterNlines > 0)
                        {
                            for (int j = 0; j < startsAfterNlines; j++)
                            {
                                i++;
                                line = consoleOutput.at(i);
                            }
                        }
                    }
                    // the plan ended
                    else if (isThePlan && ((!planEndIdentifier.trimmed().isEmpty() && line.trimmed().indexOf(planEndIdentifier) > -1) || line.trimmed().isEmpty()))
                    {
                        isThePlan = false;
                    }

                    //capturing the plan
                    if (isThePlan)
                    {
                        if (line.trimmed().startsWith(";"))
                        {
                            statisticsList.append(line.trimmed());
                        }
                        else
                        {
                            QString mline = line;
                            //checking if it is in a pddl format about Parentheses
                            if (line.indexOf("(") == -1)
                            {
                                //if it is not in pddl format just add "(" after ":" and ")" at the end of the line
                                int indexOfDoubleDot = line.indexOf(":");
                                mline = line.insert(indexOfDoubleDot + 2, "(");
                                mline.append(")");
                            }
                            //checking if it is in a pddl format about "[" - action duration
                            if (line.indexOf("[") == -1)
                            {
                                //assume duration equals to 1
                                mline.append(" [1]");
                            }
                            line = mline;
                            planList.append(line.trimmed());
                        }
                    }
                    else if (line.trimmed().startsWith(";"))
                    {
                        statisticsList.append(line.trimmed());
                    }
                    else
                    {
                        //statisticsList.append(line.trimmed());
                    }
                }

                if (statisticsList.size() > 0 || planList.size() > 0)
                {
                    if (statisticsList.size() > 0)
                    {
                        output.append(statisticsList);
                        output.append("");
                    }
                    if (planList.size() > 0)
                    {
                        output.append(planList);
                    }
                }
            }
        }
    }
    return output;
}

void ExecPlanner::parsePlanToXML(QDomElement planNode, QStringList plan)
{
    /*<action id="UNSTACK">
        <parameters>
            <parameter id="H1"/>
            <parameter id="A"/>
            <parameter id="B"/>
            <parameter id="TABLE1"/>
        </parameters>
        <startTime/>
        <duration/>
        <notes/>
    </action>*/

    if (thePlan.isNull())
    {
        setEmptyPlan();
    }
    if (!thePlan.isNull())
    {
        foreach (QString line, plan) {
            QDomElement action = thePlan.ownerDocument().createElement("action");

            QString actionInstance = line;
            actionInstance.remove(0, actionInstance.indexOf("(")+1);
            actionInstance.remove(actionInstance.indexOf(")"), actionInstance.size()-actionInstance.indexOf(")"));
            QStringList actionItems = actionInstance.split(" ");

            // the first item is the action name
            QString actionName = QString(actionItems[0]);
            setStrAttribute(action, "id", actionName.toUpper());

            // the other items are the parameters
            QDomElement parameters = thePlan.ownerDocument().createElement("parameters");
            for (int i = 1; i < actionItems.size(); i++)
            {
                QDomElement parameter = thePlan.ownerDocument().createElement("parameter");
                setStrAttribute(parameter, "id", QString(actionItems[i]).toUpper());
                parameters.appendChild(parameter);
                parameter.clear();
            }
            action.appendChild(parameters);

            // set the startTime name
            QString startTimeStr = line;
            startTimeStr.remove(startTimeStr.indexOf(":"), startTimeStr.size()-startTimeStr.indexOf(":"));
            QDomElement startTime = thePlan.ownerDocument().createElement("startTime");
            setStrValue(startTime, startTimeStr);
            action.appendChild(startTime);

            // set the action duration
            QString durationStr = "1";
            if (line.indexOf("[") > -1)
            {
                durationStr = line;
                durationStr.remove(0, durationStr.indexOf("[")+1);
                durationStr.remove(durationStr.indexOf("]"), durationStr.size()-durationStr.indexOf("]"));
            }
            QDomElement duration = thePlan.ownerDocument().createElement("duration");
            setStrValue(duration, durationStr);
            action.appendChild(duration);

            // add the notes node
            QDomElement notes = thePlan.ownerDocument().createElement("notes");
            action.appendChild(notes);

            // add the action to the plan node
            planNode.appendChild(action);
        }
    }
}

void ExecPlanner::parseStatisticsToXML(QDomElement statisticNode, QStringList statistic)
{
    /*	<statistics>
    <toolTime/>
    <time/>
    <parsingTime/>
    <nrActions/>
    <makeSpan/>
    <metricValue/>
    <planningTechnique/>
    <additional/>
    </statistics>*/

    //1. set the tool time, i. e., the total time seen by KAVI
    QDomElement toolTime = statisticNode.firstChildElement("toolTime");
    setStrValue(toolTime, QString::number(this->time/1000));

    foreach (QString line, statistic) {
        QString keyword;
        QString value;

        if (line.indexOf(" ") > -1)
        {
            // there is a value
            keyword = line;
            value = line;
            keyword = keyword.remove(keyword.indexOf(" "), keyword.size()-keyword.indexOf(" ")).trimmed();
            value = value.remove(0, value.indexOf(" ")+1).trimmed();
        }
        else
        {
            keyword = line;
            value = "";
        }

        if (keyword == "Time")
        {
            QDomElement time = statisticNode.firstChildElement("time");
            setStrValue(time, value);
        }
        else if (keyword == "Parsing")
        {
            QDomElement parsingTime = statisticNode.firstChildElement("parsingTime");
            setStrValue(parsingTime, value);
        }
        else if (keyword == "NrActions")
        {
            QDomElement nrActions = statisticNode.firstChildElement("nrActions");
            setStrValue(nrActions, value);
        }
        else if (keyword == "MakeSpan")
        {
            QDomElement makeSpan = statisticNode.firstChildElement("makeSpan");
            setStrValue(makeSpan, value);
        }
        else if (keyword == "MetricValue")
        {
            QDomElement metricValue = statisticNode.firstChildElement("metricValue");
            setStrValue(metricValue, value);
        }
        else if (keyword == "PlanningTechnique")
        {
            QDomElement planningTechnique = statisticNode.firstChildElement("planningTechnique");
            setStrValue(planningTechnique, value);
        }
        else
        {
            QDomElement additional = statisticNode.firstChildElement("additional");
            //QString text = getStrValue(additional);
            QString text = additional.text();
            setStrValue(additional, text.append(keyword).append(" ").append(value).append("\n"));
        }

    }
}

QDomElement ExecPlanner::solvePlanningProblem(QDomElement chosenPlanner, QString domainFile, QString problemFile)
{
    toolMessage = "";

    //check if the planner file exists
    QDomElement settings = chosenPlanner.firstChildElement("settings");

    QString plannerFilePath;
    plannerFilePath.append(getPlannersPath()).append(getStrValue(settings.firstChildElement("filePath")));

    QFile plannerFile(plannerFilePath);
    //plannerRunFile = plannerFile;
    bool plannerFileExists = true;
    if (!plannerFile.exists())
    {
        plannerFileExists = false;
        toolMessage += ">> Could not find selected planner '"+ getStrValue(settings.firstChildElement("filePath")) +"'. \n" +
                ">> Please download and copy it in the folder /KAVIPlanners \n";
        // should append toolMessage to planning dialog
    }

    if (plannerFileExists)
    {
        if (thePlan.isNull())
        {
            setEmptyPlan();
        }

        if (!thePlan.isNull())
        {
            //1. get chosen planner output
            QStringList output;
            QStringList consoleOutput;

            output = getPlannerOutput(chosenPlanner, domainFile, problemFile, consoleOutput);

            //2. separates the plan and the statistics
            QStringList plan;
            QStringList statistic;
            getPlanAndStatistics(output, plan, statistic);
            writePlanToDefaultPath(plan);

            //3. set datetime
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
            QDomElement datetime = thePlan.firstChildElement("datetime");
            setStrValue(datetime, current_date);

            //4. set the planner results
            QDomElement planner = thePlan.firstChildElement("planner");

            //4.3 set the planner console output
            //4.3.1 build up the text from the string list
            QString consoleOutputStr;
            foreach (QString line, consoleOutput) {
                line = removeTroublesomeCharacters(line);
                consoleOutputStr.append(line).append("\n");
            }

            //4.3.2 set the value/output
            planner.appendChild(planner.ownerDocument().createElement("consoleOutput"));
            QDomElement consoleOutputElement = planner.firstChildElement("consoleOutput");
            setStrValue(consoleOutputElement, consoleOutputStr);

            //5. set statistics
            QDomElement statisticNode = thePlan.firstChildElement("statistics");
            parseStatisticsToXML(statisticNode, statistic);

            //6. set the plan
            QDomElement planNode = thePlan.firstChildElement("plan");
            parsePlanToXML(planNode, plan);

            if (!planNode.firstChildElement("action").isNull())
            {
                toolMessage.append("Planner generated a solution.");
            }
            else
            {
                toolMessage.append("Planner did not generate any solution!");
            }

            //7. set tool information message
            QDomElement toolInformation = thePlan.firstChildElement("toolInformation");
            QDomElement message = toolInformation.firstChildElement("message");
            setStrValue(message, toolMessage);

            //Plan Validation
            //check if automatic plan validation is enabled (if so call validation with VAL


        }
    }
    return thePlan;
}

QDomElement ExecPlanner::solveProblem()
{
    thePlan = solvePlanningProblem(chosenPlanner, domainFile, problemFile);
    return thePlan;
}

QString ExecPlanner::generateHTMLReport(QDomElement xmlPlan)
{

}

void ExecPlanner::setXMLDomain(QDomElement domain)
{
    xmlDomain = domain;
}

void ExecPlanner::setXMLProblem(QDomElement problem)
{
    xmlProblem = problem;
}

void ExecPlanner::setDomainName(QString name)
{
    domainName = name;
}

void ExecPlanner::setProblemName(QString name)
{
    problemName = name;
}

QString ExecPlanner::getProblemName()
{
    return problemName;
}

void ExecPlanner::setChosenPlanner(QDomElement chosenPlanner)
{
    this->chosenPlanner = chosenPlanner;
}

QDomElement ExecPlanner::getChosenPlanner()
{
    return chosenPlanner;
}

void ExecPlanner::setShowReport(bool showReport)
{
    this->showReport = showReport;
}

QDomElement ExecPlanner::getPlan()
{
    return thePlan;
}

void ExecPlanner::setPlan(QDomElement thePlan)
{
    this->thePlan = thePlan;
}

void ExecPlanner::run()
{
    if (!chosenPlanner.isNull() && !domainFile.isNull() && !problemFile.isNull())
    {
        QDomElement xmlPlan= solvePlanningProblem(chosenPlanner, domainFile, problemFile);
        writeToXMLFile();
    }
}

void ExecPlanner::destroyProcess()
{
    if (process != NULL)
    {
        process->destroyed();
    }
}

QString ExecPlanner::getOperatingSystem()
{
    QString operatingSystem = QSysInfo::productType().toLower();

    if (operatingSystem.contains("windows"))
    {
        operatingSystem = "windows";
    }
    else if (operatingSystem.contains("osx"))
    {
        operatingSystem = "mac";
    }
    else if (operatingSystem.contains("linux"))
    {
        operatingSystem = "linux";
    }
    return operatingSystem;
}

bool ExecPlanner::getXMLDocument()
{
    QString filePath = getXMLFilePath();
    QFile xmlFile(filePath.append(DEFAULTPLAN_FILE));

    if ( !xmlFile.open(QFile::ReadOnly | QFile::Text ))
    {
        qDebug()<< "@Error: cannot open file: " << xmlFile.fileName();
        return false;
    }

    QString error;
    int row = 0, column = 0;
    if (!planDocument.setContent(&xmlFile, false, &error, &row, &column))
    {
        QMessageBox::information(NULL, QString("Error"), QString("Parsing xml file failed at line row and column ") + QString::number(row, 10) + QString(",") + QString::number(column, 10));
        return false;
    }
    xmlFile.close();

    return true;
}

QString ExecPlanner::getXMLFilePath()
{
    QString filePath;

    QDir tmpDir;
    QString currentPath = tmpDir.currentPath();
    tmpDir.cdUp();
    QString upPath = tmpDir.path();
    tmpDir.setCurrent(currentPath);
    switch (KAVIRunMode) {
    case Debug:
        filePath.append(upPath).append(PLANNERS_CONFIGS_DIR_DEBUG);
        break;
    case Release:
        filePath.append(currentPath).append(PLANNERS_CONFIGS_DIR_RELEASE);
        break;
    default:
        break;
    }

    return filePath;
}


bool ExecPlanner::writeToXMLFile()
{
    QString filePath = getXMLFilePath();
    QFile xmlFile(filePath.append(TEMPPLAN_FILE));

    QString fileName = xmlFile.fileName();
    if (QFile::exists(fileName))
    {
        QFile::remove(fileName);
    }
    QFile newXmlFile(fileName);

    if ( !newXmlFile.open(QFile::WriteOnly | QFile::Text ))
    {
        qDebug()<< "@Error: cannot open file: " << newXmlFile.fileName();
        return false;
    }
    QTextStream saveStream(&newXmlFile);
    planDocument.save(saveStream, OUTPUT_INDENT);
    newXmlFile.close();
}

void ExecPlanner::writePlanToDefaultPath(QStringList &plan)
{
    QString fileName = getPDDLFilePath();

    fileName.append(DEFAULT_PLAN_FILE);

    if (QFile::exists(fileName))
    {
        QFile::remove(fileName);

    }
    QFile file(fileName);

    if ( !file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text) )
    {
        //QMessageBox::critical(this, tr("Error"), tr("Can't open file %1").arg(fileName));
        return;
    }

    QTextStream outStream(&file);
    foreach (QString line, plan) {
        outStream << line << "\n";
    }
    file.close();
}

QString ExecPlanner::getPDDLFilePath()
{
    QString filePath;

    QDir tmpDir;
    QString currentPath = tmpDir.currentPath();
    tmpDir.cdUp();
    QString upPath = tmpDir.path();
    tmpDir.setCurrent(currentPath);
    switch (KAVIRunMode) {
    case Debug:
        filePath.append(upPath).append(KAVI_PDDL_DIR_DEBUG);
        break;
    case Release:
        filePath.append(currentPath).append(KAVI_PDDL_DIR_RELEASE);
        break;
    default:
        break;
    }

    return filePath;
}

QString ExecPlanner::getPlannersPath()
{
    QString filePath;

    QDir tmpDir;
    QString currentPath = tmpDir.currentPath();
    tmpDir.cdUp();
    QString upPath = tmpDir.path();
    tmpDir.setCurrent(currentPath);
    switch (KAVIRunMode) {
    case Debug:
        filePath.append(upPath).append(PLANNERS_DIR_DEBUG);
        break;
    case Release:
        filePath.append(currentPath).append(PLANNERS_DIR_RELEASE);
        break;
    default:
        break;
    }

    return filePath;
}

QString ExecPlanner::removeTroublesomeCharacters(QString inString)
{
    if (inString.isNull())
        return QString();

    QString newString;

    for (int i = 0; i < inString.size(); i++)
    {
        QChar ch = inString.at(i);
        if ((ch < 0x00FD && ch > 0x001F) || ch == '\t' || ch == '\n' || ch == '\r')
        {
            newString.append(ch);
        }
    }
    return newString;
}

QStringList ExecPlanner::getTestConsoleOutput()
{
    return testConsoleOutput;
}

QString ExecPlanner::getPlannerGeneratedFilesPath()
{
    QString filePath;

    QDir tmpDir;
    filePath = tmpDir.currentPath();

    filePath.append(PLANNER_GENERATED_FILE_DIR);

    return filePath;
}
