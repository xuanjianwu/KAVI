#include "PlanningDialog.h"
#include "ui_PlanningDialog.h"

using namespace XMLUtils;

PlanningDialog::PlanningDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanningDialog)
{
    ui->setupUi(this);
    KAVIRunMode = Release;

    initEnvironment();
}

PlanningDialog::~PlanningDialog()
{
    delete ui;
}

void PlanningDialog::solveProblemWithSinglePlanner(QString domain, QString problem, QDomElement chosenPlanner)
{
    exe = new ExecPlanner(chosenPlanner, domain, problem, false, this);
    exe->setDomainName(getDomainName());
    exe->setProblemName(getProblemName());

    //connect(exe, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(exe, SIGNAL(finished()), this, SLOT(showPlannerOutput()));
    connect(exe, SIGNAL(appendToLog(QString)), this, SLOT(appendToConsoleLog(QString)));

    exe->start();
}

QDomElement PlanningDialog::getKAVIPlanners()
{
    return KAVIPlanners;
}

void PlanningDialog::loadKAVIPlanners()
{
    KAVIPlanners = KAVIPlannersDocument.firstChildElement("KAVIPlanners");
}

QDomElement PlanningDialog::getKAVIValidators()
{
    return KAVIValidators;
}

void PlanningDialog::loadKAVIValidators()
{
    KAVIValidators = KAVIValidatorsDocument.firstChildElement("KAVIValidators");
}

void PlanningDialog::initChosenValidator()
{
    QDomElement validators = KAVIValidators.firstChildElement("validators");
    theSingleChosenValidator = validators.firstChildElement("validator");
}

void PlanningDialog::initEnvironment()
{
    getXMLDocument();
    loadKAVIPlanners();
    getValidatorsXMLDocument();
    loadKAVIValidators();
    initChosenValidator();

    // init plannerSuggestion
    plannerSuggestion = new PlannerSuggestion();

    setDefaultDomainFile();
    setDefaultProblemFile();
    setDefaultPlanFile();

    // init solutionSettingsDialog
    //solutionSettingsDialog = new SolutionSettingsDialog(getKAVIPlanners(), this);
}

void PlanningDialog::initProblemSelection()
{
    QFile pddlDomainFile(domainFile);
    if ( !pddlDomainFile.open(QFile::ReadOnly | QFile::Text ))
    {
        //qDebug()<< "@Error: cannot open file: " << pddlDomainFile.fileName();
        appendToConsoleLog(QString("Error: cannot open file: ").append(pddlDomainFile.fileName()));
        return;
    }
    plannerSuggestion->initialPlannerSelection(pddlDomainFile, getKAVIPlanners());
    initPlannerSelection(plannerSuggestion->getSuggestedPlanners());
}

void PlanningDialog::initPlannerSelection(QList<QDomElement> plannersList)
{
    ui->plannerSelection->clear();
    this->plannersList = plannersList;
    ui->plannerSelection->addItem("Select one planner");
    ui->plannerSelection->setCurrentIndex(0);
    for (int i = 0; i < this->plannersList.size(); i++)
    {
        QDomElement planner = this->plannersList.at(i).toElement();
        QDomElement plannerName = planner.firstChildElement("name");
        QDomElement plannerVersion = planner.firstChildElement("version");

        QString name = getStrValue(plannerName);
        QString version = getStrValue(plannerVersion);

        ui->plannerSelection->addItem(name.append(" - ").append(version));
    }
}

void PlanningDialog::appendToConsoleLog(QString text)
{
    ui->consoleLog->append(text);
}

void PlanningDialog::execRepair(PlanAction flawAction, QString index)
{
    if (ui->customProblem->checkState() == Qt::Checked)
    {
        QString actionStr;

        actionStr.append("        :parameters ( ");


        QString targetEffect = index;
        bool targetEffectAdvice = flawAction.getRepairAdvice().value(index);

        QString effectStr = targetEffect;

        effectStr.remove(effectStr.indexOf("("), 1);
        effectStr.remove(effectStr.indexOf(")"), 1);

        QStringList effectItems = effectStr.split(" ");
        QString predicateName = effectItems.at(0);
        QStringList argumentsList = effectItems;
        argumentsList.removeFirst();

        foreach (QString argument, argumentsList) {
            actionStr.append("?").append(argument);

            if (flawAction.getArgumentTypePair().value(argument) == "-1")
            {

            }
            else
            {
                actionStr.append(" - ").append(flawAction.getArgumentTypePair().value(argument));
            }

            actionStr.append(" ");
        }

        actionStr.append(")\n");

        actionStr.append("        :precondition (and )\n");

        actionStr.append("        :effect (and\n");

        if (targetEffectAdvice)
        {
            actionStr.append("            ( ");
        }
        else
        {
            actionStr.append("            (not (");
        }

        actionStr.append(predicateName);

        foreach (QString argument, argumentsList) {
            actionStr.append(" ?");
            actionStr.append(argument);
        }

        if (targetEffectAdvice)
        {
            actionStr.append(" )\n");
        }
        else
        {
            actionStr.append(" ))\n");
        }


        actionStr.append("        )\n");

        actionStr.append("    )\n");

        EditFileDialog* dialog = new EditFileDialog(domainFile, actionStr, this);
        dialog->exec();
    }
    else
    {

        this->accept();
        emit createNewAction(flawAction, index);
    }
}

void PlanningDialog::on_customProblem_clicked(bool checked)
{
    if (checked)
    {
        ui->domainPDDLFile->setEnabled(true);
        ui->domainPDDLFile->clear();
        ui->problemPDDLFile->setEnabled(true);
        ui->problemPDDLFile->clear();
        ui->domainBrowse->setEnabled(true);
        ui->problemBrowse->setEnabled(true);
        resetDomainFile();
        resetProblemFile();

        if (ui->planValidation->checkState() == Qt::Checked)
        {
            ui->execValidator->setEnabled(true);
            ui->planBrowse->setEnabled(true);
            ui->planFile->setEnabled(true);
            ui->planFile->clear();
            resetPlanFile();
        }
        else
        {
            ui->execValidator->setEnabled(false);
            ui->planBrowse->setEnabled(false);
            ui->planFile->setEnabled(false);
            ui->planFile->clear();
            resetPlanFile();
        }
    }
    else
    {
        ui->domainPDDLFile->setEnabled(false);
        ui->domainPDDLFile->clear();
        ui->problemPDDLFile->setEnabled(false);
        ui->problemPDDLFile->clear();
        ui->domainBrowse->setEnabled(false);
        ui->problemBrowse->setEnabled(false);
        setDefaultDomainFile();
        setDefaultProblemFile();

        if (ui->planValidation->checkState() == Qt::Checked)
        {
            ui->execValidator->setEnabled(true);
            ui->planBrowse->setEnabled(false);
            ui->planFile->setEnabled(false);
            ui->planFile->clear();
            //resetPlanFile();
            setDefaultPlanFile();
        }
        else
        {
            ui->execValidator->setEnabled(false);
            ui->planBrowse->setEnabled(false);
            ui->planFile->setEnabled(false);
            ui->planFile->clear();
            resetPlanFile();
        }
    }
}

void PlanningDialog::on_domainBrowse_clicked()
{
    QString defaultOpenFilePath = getPDDLFilePath();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open PDDL Domain File"), defaultOpenFilePath, tr("Planning domain (*.pddl)"));

    if ( fileName.isEmpty() )
        return;

    ui->domainPDDLFile->setText(fileName);

    setDomainFile(fileName);
}

void PlanningDialog::on_problemBrowse_clicked()
{
    QString defaultOpenFilePath = getPDDLFilePath();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open PDDL Problem File"), defaultOpenFilePath, tr("Planning problem (*.pddl)"));

    if ( fileName.isEmpty() )
        return;

    ui->problemPDDLFile->setText(fileName);

    setProblemFile(fileName);
}

void PlanningDialog::showPlannerOutput()
{
    ui->plannerOutput->clear();
    QString text = PlanAnalyzer::generateHTMLSinglePlanReport(getKAVIPlanners(), exe->getPlan());
//    QStringList strList = exe->getTestConsoleOutput();
//    QString str;
//    foreach (QString line, strList) {
//        str.append(line);
//    }
//    QFile htmlFile("test.html");

//    if (!htmlFile.open(QIODevice::WriteOnly | QIODevice::Text))
//        return;

//    QTextStream out(&htmlFile);
//    out << text;

    //ui->plannerOutput->setText(text);
    ui->plannerOutput->setHtml(text);
}

bool PlanningDialog::getXMLDocument()
{
    QString filePath = getXMLFilePath();
    QFile xmlFile(filePath.append(KAVIPLANNERS_FILE));

    if ( !xmlFile.open(QFile::ReadOnly | QFile::Text ))
    {
        //qDebug()<< "@Error: cannot open file: " << xmlFile.fileName();
        appendToConsoleLog(QString("Error: cannot open file: ").append(xmlFile.fileName()));
        return false;
    }

    QString error;
    int row = 0, column = 0;
    if (!KAVIPlannersDocument.setContent(&xmlFile, false, &error, &row, &column))
    {
        QMessageBox::information(NULL, QString("Error"), QString("Parsing xml file failed at line row and column ") + QString::number(row, 10) + QString(",") + QString::number(column, 10));
        return false;
    }
    xmlFile.close();

    return true;
}

bool PlanningDialog::getValidatorsXMLDocument()
{
    QString filePath = getValidatorsXMLFilePath();
    QFile xmlFile(filePath.append(KAVIVALIDATORS_FILE));

    if ( !xmlFile.open(QFile::ReadOnly | QFile::Text ))
    {
        //qDebug()<< "@Error: cannot open file: " << xmlFile.fileName();
        appendToConsoleLog(QString("Error: cannot open file: ").append(xmlFile.fileName()));
        return false;
    }

    QString error;
    int row = 0, column = 0;
    if (!KAVIValidatorsDocument.setContent(&xmlFile, false, &error, &row, &column))
    {
        QMessageBox::information(NULL, QString("Error"), QString("Parsing xml file failed at line row and column ") + QString::number(row, 10) + QString(",") + QString::number(column, 10));
        return false;
    }
    xmlFile.close();

    return true;
}

QString PlanningDialog::getPDDLFilePath()
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

QString PlanningDialog::getXMLFilePath()
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

QString PlanningDialog::getValidatorsXMLFilePath()
{
    QString filePath;

    QDir tmpDir;
    QString currentPath = tmpDir.currentPath();
    tmpDir.cdUp();
    QString upPath = tmpDir.path();
    tmpDir.setCurrent(currentPath);
    switch (KAVIRunMode) {
    case Debug:
        filePath.append(upPath).append(VALIDATORS_CONFIGS_DIR_DEBUG);
        break;
    case Release:
        filePath.append(currentPath).append(VALIDATORS_CONFIGS_DIR_RELEASE);
        break;
    default:
        break;
    }

    return filePath;
}

void PlanningDialog::on_plannersSettings_clicked()
{
    SolutionSettingsDialog* solutionSettingsDialog = new SolutionSettingsDialog(getKAVIPlanners(), this);
    solutionSettingsDialog->exec();
}

void PlanningDialog::setDefaultDomainFile()
{
    QString fileName = getPDDLFilePath();

    fileName.append(DEFAULT_DOMAIN_PDDL_FILE);

    setDomainFile(fileName);
}

void PlanningDialog::setDefaultProblemFile()
{
    QString fileName = getPDDLFilePath();

    fileName.append(DEFAULT_PROBLEM_PDDL_FILE);

    setProblemFile(fileName);
}

void PlanningDialog::setDefaultPlanFile()
{
    QString fileName = getPDDLFilePath();

    fileName.append(DEFAULT_PLAN_FILE);

    setPlanFile(fileName);
}

void PlanningDialog::setDomainFile(QString domainFile)
{
    this->domainFile = domainFile;
    initProblemSelection();
}

void PlanningDialog::setProblemFile(QString problemFile)
{
    this->problemFile = problemFile;
}

void PlanningDialog::setPlanFile(QString planFile)
{
    this->planFile = planFile;
}

void PlanningDialog::resetDomainFile()
{
    this->domainFile.clear();
    resetPlannerSelection();
}

void PlanningDialog::resetProblemFile()
{
    this->problemFile.clear();
}

void PlanningDialog::resetPlanFile()
{
    this->planFile.clear();
}

void PlanningDialog::resetPlannerSelection()
{
    ui->plannerSelection->clear();
    ui->plannerSelection->addItem("Select one planner");
    ui->plannerSelection->setCurrentIndex(0);
}

void PlanningDialog::on_plannerSelection_currentIndexChanged(int index)
{

    if (index < 1)
    {
        ui->execPlanner->setEnabled(false);
        theSingleChosenPlanner.clear();
        return;
    }
    ui->execPlanner->setEnabled(true);
    theSingleChosenPlanner = this->plannersList.at(index-1).toElement();
}

void PlanningDialog::on_execPlanner_clicked()
{
    if (domainFile.simplified().isNull())
    {
        //qDebug() << "@Warning: The domain PDDL file can not be empty";
        appendToConsoleLog(QString("Warning: The domain PDDL file can not be empty"));
        return;
    }
    if (problemFile.simplified().isNull())
    {
        //qDebug() << "@Warning: The problem PDDL file can not be empty";
        appendToConsoleLog(QString("Warning: The problem PDDL file can not be empty"));
        return;
    }
    if (theSingleChosenPlanner.isNull())
    {
        //qDebug() << "@Warning: Please select one planner to execute planning";
        appendToConsoleLog(QString("Warning: Please select one planner to execute planning"));
        return;
    }
    solveProblemWithSinglePlanner(domainFile, problemFile, theSingleChosenPlanner);
}

QString PlanningDialog::getDomainName()
{
    QFile pddlDomainFile(domainFile);
    if ( !pddlDomainFile.open(QFile::ReadOnly | QFile::Text ))
    {
        //qDebug()<< "@Error: cannot open file: " << pddlDomainFile.fileName();
        appendToConsoleLog(QString("Error: cannot open file: ").append(pddlDomainFile.fileName()));
        return QString();
    }

    QString domainNameLine;
    while (!pddlDomainFile.atEnd())
    {
        QByteArray line = pddlDomainFile.readLine();
        QString str(line);
        if (str.contains("define") && str.contains("domain"))
        {
            domainNameLine = str.simplified();
            break;
        }
    }
    if (domainNameLine.indexOf("(") > -1 && domainNameLine.indexOf(")") > -1)
    {
        domainNameLine.remove(0, domainNameLine.lastIndexOf("(")+1);
        domainNameLine.remove(domainNameLine.indexOf(")"), 1);
        domainNameLine = domainNameLine.simplified();
        QStringList domainItems = domainNameLine.split(" ");
        return domainItems.at(1);
    }
    pddlDomainFile.seek(0);
    return QString();
}

QString PlanningDialog::getProblemName()
{
    QFile pddlProblemFile(problemFile);
    if ( !pddlProblemFile.open(QFile::ReadOnly | QFile::Text ))
    {
        //qDebug()<< "@Error: cannot open file: " << pddlProblemFile.fileName();
        appendToConsoleLog(QString("Error: cannot open file: ").append(pddlProblemFile.fileName()));
        return QString();
    }

    QString problemNameLine;
    while (!pddlProblemFile.atEnd())
    {
        QByteArray line = pddlProblemFile.readLine();
        QString str(line);
        if (str.contains("define") && str.contains("problem"))
        {
            problemNameLine = str.simplified();
            break;
        }
    }
    if (problemNameLine.indexOf("(") > -1 && problemNameLine.indexOf(")") > -1)
    {
        problemNameLine.remove(0, problemNameLine.lastIndexOf("(")+1);
        problemNameLine.remove(problemNameLine.indexOf(")"), 1);
        problemNameLine = problemNameLine.simplified();
        QStringList problemItems = problemNameLine.split(" ");
        return problemItems.at(1);
    }
    pddlProblemFile.seek(0);
    return QString();
}

void PlanningDialog::on_planBrowse_clicked()
{
    QString defaultOpenFilePath = getPDDLFilePath();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Plan File in txt format"), defaultOpenFilePath, tr("Plan txt (*.txt)"));

    if ( fileName.isEmpty() )
        return;

    ui->planFile->setText(fileName);

    setPlanFile(fileName);
}

void PlanningDialog::on_planValidation_clicked(bool checked)
{
    if (checked)
    {
        ui->execValidator->setEnabled(true);
        if (ui->customProblem->checkState() == Qt::Checked)
        {
            ui->planBrowse->setEnabled(true);
            ui->planFile->setEnabled(true);
            ui->planFile->clear();
            resetPlanFile();
        }
        else
        {
            ui->planBrowse->setEnabled(false);
            ui->planFile->setEnabled(false);
            ui->planFile->clear();
            setDefaultPlanFile();
        }
    }
    else {
        ui->execValidator->setEnabled(false);
        ui->planBrowse->setEnabled(false);
        ui->planFile->setEnabled(false);
        ui->planFile->clear();
        resetPlanFile();
    }
}

void PlanningDialog::on_execValidator_clicked()
{
    if (domainFile.simplified().isNull())
    {
        //qDebug() << "@Warning: The domain PDDL file can not be empty";
        appendToConsoleLog(QString("Warning: The domain PDDL file can not be empty"));
        return;
    }
    if (problemFile.simplified().isNull())
    {
        //qDebug() << "@Warning: The problem PDDL file can not be empty";
        appendToConsoleLog(QString("Warning: The problem PDDL file can not be empty"));
        return;
    }
    if (planFile.simplified().isNull())
    {
        //qDebug() << "@Warning: The plan file can not be empty";
        appendToConsoleLog(QString("Warning: The plan file can not be empty"));
        return;
    }
    if (theSingleChosenValidator.isNull())
    {
        //qDebug() << "@Warning: Please select one validator to execute validation";
        appendToConsoleLog(QString("Warning: Please select one validator to execute validation"));
        return;
    }
    planValidator = new PlanValidator(theSingleChosenValidator, domainFile, problemFile, planFile);
    planValidator->run();
    PlanValidationDialog* dialog = new PlanValidationDialog(planValidator, this);

    connect(dialog, SIGNAL(createNewAction(PlanAction, QString)), this, SLOT(execRepair(PlanAction, QString)));

    dialog->exec();
}

void PlanningDialog::on_domainPDDLFile_textChanged(const QString &arg1)
{
    if (arg1.isEmpty())
    {
        ui->editDomainPDDL->setEnabled(false);
    }
    else
    {
        ui->editDomainPDDL->setEnabled(true);
    }
}

void PlanningDialog::on_problemPDDLFile_textChanged(const QString &arg1)
{
    if (arg1.isEmpty())
    {
        ui->editProblemPDDL->setEnabled(false);
    }
    else
    {
        ui->editProblemPDDL->setEnabled(true);
    }
}

void PlanningDialog::on_planFile_textChanged(const QString &arg1)
{
    if (arg1.isEmpty())
    {
        ui->editPlanFile->setEnabled(false);
    }
    else
    {
        ui->editPlanFile->setEnabled(true);
    }
}

void PlanningDialog::on_editDomainPDDL_clicked()
{
    EditFileDialog* dialog = new EditFileDialog(domainFile, this);
    dialog->exec();
}

void PlanningDialog::on_editProblemPDDL_clicked()
{
    EditFileDialog* dialog = new EditFileDialog(problemFile, this);
    dialog->exec();
}

void PlanningDialog::on_editPlanFile_clicked()
{
    EditFileDialog* dialog = new EditFileDialog(planFile, this);
    dialog->exec();
}

void PlanningDialog::on_clearConsoleLog_clicked()
{
    ui->consoleLog->clear();
}
