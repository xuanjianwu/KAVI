#include "PlanValidator.h"

using namespace XMLUtils;

PlanValidator::PlanValidator()
{
    KAVIRunMode = Debug;

    initEnvironment();
}

PlanValidator::PlanValidator(QDomElement chosenValidator, QString domainFile, QString problemFile, QString planFile)
{
    KAVIRunMode = Debug;

    this->chosenValidator = chosenValidator;
    this->domainFile = domainFile;
    this->problemFile = problemFile;
    this->planFile = planFile;

    initEnvironment();
}

void PlanValidator::initEnvironment()
{
    initPlan();
    stateHistory = new StateHistory();
}

void PlanValidator::initPlan()
{
    plan = new Plan();
}

void PlanValidator::matchPlanActionWithDomain(PlanAction &action, QString domainFile)
{
    QString actionName;
    QStringList actionParameters;

    QString actionFormula = action.getFormula();
    actionFormula.remove(actionFormula.indexOf("("), 1);
    actionFormula.remove(actionFormula.indexOf(")"), 1);
    actionFormula = actionFormula.simplified();
    QStringList actionItems = actionFormula.split(" ");
    for (int i = 0; i < actionItems.size(); i++)
    {
        if (i == 0)
        {
            actionName = actionItems.at(i);
        }
        else
        {
            actionParameters.append(actionItems.at(i));
        }
    }


    QFile file(domainFile);
    if ( !file.open(QFile::ReadOnly | QFile::Text ))
    {
        return;
    }
    QString content(file.readAll());
    QStringList domainActions;
    //content.replace("\n", " ");
    splitDomainActionsToString(content, domainActions);


}

void PlanValidator::splitDomainActionsToString(QString domainFile, QStringList &actions)
{
    // this function split the plain text of domain file for all the actions as string, such as
    /*
  ( :action load-truck
    :parameters
         (?obj - package
          ?truck - truck
          ?loc - location)
    :precondition
        (and 	(at ?truck ?loc)
            (at ?obj ?loc))
    :effect
        (and 	(not (at ?obj ?loc))
            (in ?obj ?truck)))
     */

    QRegExp actionChecker;
    //actionChecker.setPattern(QString("\\([\\s]*\\:action[\\s\\S]*?[\\s]*\\:parameters[\\s\\S]*?[\\s]*\\:precondition[\\s\\S]*?[\\s]*\\:effect[\\s\\S]*?"));
    actionChecker.setPattern(QString("\\([\\s]*\\:action[\\s\\S]*[\\s]*\\:parameters[\\s\\S]*[\\s]*\\:precondition[\\s\\S]*[\\s]*\\:effect[\\s\\S]*"));
    int pos = actionChecker.indexIn(domainFile);
    QString actionsString = actionChecker.cap(0);
    //QString str2 = actionChecker.cap(1);
    //QString str3 = actionChecker.cap(3);
    //actions = actionChecker.capturedTexts();
    actionsString.remove(actionsString.lastIndexOf(")"), actionsString.size()-actionsString.lastIndexOf(")"));
    QRegExp tmpChecker;
    tmpChecker.setPattern(QString("\\([\\s]*\\:action"));
    QStringList tmpList = actionsString.split(tmpChecker);

    foreach (QString action, tmpList) {
        if (!action.isEmpty())
        {
            action.prepend("(:action ");
            actions.append(action.simplified());
        }
    }
}

QString PlanValidator::getContentsAsString(QFile &file)
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

QStringList PlanValidator::getContentsAsStringList(QFile &file)
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

QStringList PlanValidator::getValidatorOutput(QDomElement chosenValidator, QString domain, QString problem, QString plan, QStringList &consoleOutput)
{
    QStringList output;
    QString solutionFile =  "solution.soln";

    // 1. get main validator's parameters and arguments
    QDomElement settings = chosenValidator.firstChildElement("settings");

    QStringList commandArguments;

    // 1.0 get validator execution file
    QString validatorFilePath;
    validatorFilePath.append(getValidatorsPath()).append(getStrValue(settings.firstChildElement("filePath")));
    commandArguments.append(validatorFilePath);

    QFile validatorFile(validatorFilePath);

    bool validatorFileExists = true;
    if (!validatorFile.exists())
    {
        validatorFileExists = false;
    }

    //proceed only if validator file exists
    if (validatorFileExists)
    {

        //GET ALL ARGUMENTS (including domain, problem, plan) in the specified ORDER

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
            for (int i = 0; i < theArgs.size();i ++)
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
                //check if this is a plan argument
                else if (argElement.tagName() == "plan")
                {
                    commandArguments.append(plan);
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

        gotError = false;
        readyReadOutput = false;
        normalExit = false;

        process = new QProcess();
        //process->setProcessChannelMode(QProcess::SeparateChannels);
        //process->setReadChannel(QProcess::StandardOutput);

        //connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
        //connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(startReadOutput()));
        //connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finishProcess(int, QProcess::ExitStatus)));

        //Call the planner
        process->start(program, arguments);
        //process->execute(program, arguments);
        //process->startDetached(program, arguments);
        process->waitForFinished();

        if (!gotError)
        {
            consoleOutput = QString::fromLocal8Bit(process->readAllStandardOutput()).split("\n");

            this->time = start_time.elapsed();

            //The validator does provide a output file
            if (outputFile)
            {

            }
            //The validator does not provide a output file, just the console message
            else
            {
                parseValidatorOutput(consoleOutput);
            }
        }
    }
}

void PlanValidator::parseValidatorOutput(QStringList &consoleOutput)
{
    bool planActioning = false;
    bool planValidationDetails = false;

    for (int i = 0; i < consoleOutput.size(); i++)
    {
        QString line = consoleOutput.at(i);

        if (line.isEmpty())
        {
            continue;
        }

        if (line.contains("Plan size"))
        {
            line = line.remove(0, line.indexOf(":")+1).simplified();
            setPlanSize(line.toInt());
            planActioning = true;
            continue;
        }
        if (line.contains("Plan Validation details"))
        {
            planActioning = false;
            planValidationDetails = true;
        }
        else if (planActioning)
        {
            double actionTime;
            QString actionFormula;

            line = line.remove(line.indexOf(":"), 1).simplified();
            actionTime = line.toDouble();

            i++;

            line = consoleOutput.at(i);
            actionFormula = line.simplified();

            PlanAction addAction;
            addAction.setTime(actionTime);
            addAction.setFormula(actionFormula);

            matchPlanActionWithDomain(addAction, domainFile);
            plan->addAction(addAction);
        }
    }
}

QString PlanValidator::getValidatorsPath()
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

QString PlanValidator::getDomainFile() const
{
    return domainFile;
}

void PlanValidator::setDomainFile(const QString &value)
{
    domainFile = value;
}

QString PlanValidator::getProblemFile() const
{
    return problemFile;
}

void PlanValidator::setProblemFile(const QString &value)
{
    problemFile = value;
}

QString PlanValidator::getPlanFile() const
{
    return planFile;
}

void PlanValidator::setPlanFile(const QString &value)
{
    planFile = value;
}

QString PlanValidator::getDomainName() const
{
    return domainName;
}

void PlanValidator::setDomainName(const QString &value)
{
    domainName = value;
}

QString PlanValidator::getProblemName() const
{
    return problemName;
}

void PlanValidator::setProblemName(const QString &value)
{
    problemName = value;
}

QDomElement PlanValidator::getChosenValidator() const
{
    return chosenValidator;
}

void PlanValidator::setChosenValidator(const QDomElement &value)
{
    chosenValidator = value;
}

void PlanValidator::setPlanSize(int value)
{
    planSize = value;
}

int PlanValidator::getPlanSize() const
{
    return planSize;
}

void PlanValidator::run()
{
    if (!chosenValidator.isNull() && !domainFile.isNull() && !problemFile.isNull() && !planFile.isNull())
    {
        QStringList consoleOutput;
        //getValidatorOutput(chosenValidator, domainFile, problemFile, planFile, consoleOutput);
        PlanAction action;
        matchPlanActionWithDomain(action, domainFile);
    }
}
