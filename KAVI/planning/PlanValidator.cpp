#include "PlanValidator.h"

using namespace XMLUtils;

PlanValidator::PlanValidator()
{
    KAVIRunMode = Debug;
}

PlanValidator::PlanValidator(QDomElement chosenValidator, QString domainFile, QString problemFile, QString planFile)
{
    KAVIRunMode = Debug;

    this->chosenValidator = chosenValidator;
    this->domainFile = domainFile;
    this->problemFile = problemFile;
    this->planFile = planFile;
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

        process = new QProcess(this);
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

            }
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
