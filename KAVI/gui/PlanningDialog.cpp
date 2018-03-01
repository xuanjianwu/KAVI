#include "PlanningDialog.h"
#include "ui_PlanningDialog.h"

using namespace XMLUtils;

PlanningDialog::PlanningDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanningDialog)
{
    ui->setupUi(this);
    KAVIRunMode = Debug;

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

void PlanningDialog::initEnvironment()
{
    getXMLDocument();
    loadKAVIPlanners();
    // init plannerSuggestion
    plannerSuggestion = new PlannerSuggestion();

    setDefaultDomainFile();
    setDefaultProblemFile();

    // init solutionSettingsDialog
    //solutionSettingsDialog = new SolutionSettingsDialog(getKAVIPlanners(), this);
}

void PlanningDialog::initProblemSelection()
{
    QFile pddlDomainFile(domainFile);
    if ( !pddlDomainFile.open(QFile::ReadOnly | QFile::Text ))
    {
        qDebug()<< "@Error: cannot open file: " << pddlDomainFile.fileName();
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

void PlanningDialog::on_customProblem_clicked(bool checked)
{
    if (checked)
    {
        ui->pddlDomainFile->setEnabled(true);
        ui->pddlProblemFile->setEnabled(true);
        ui->domainBrowse->setEnabled(true);
        ui->problemBrowse->setEnabled(true);
        resetDomainFile();
        resetProblemFile();
    }
    else
    {
        ui->pddlDomainFile->setEnabled(false);
        ui->pddlDomainFile->clear();
        ui->pddlProblemFile->setEnabled(false);
        ui->pddlProblemFile->clear();
        ui->domainBrowse->setEnabled(false);
        ui->problemBrowse->setEnabled(false);
        setDefaultDomainFile();
        setDefaultProblemFile();
    }
}

void PlanningDialog::on_clearOutput_clicked()
{
    ui->plannerOutput->clear();
}

void PlanningDialog::on_domainBrowse_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open PDDL Domain File"), ".", tr("Planning domains (*.pddl)"));

    if ( fileName.isEmpty() )
        return;

    ui->pddlDomainFile->setText(fileName);

    setDomainFile(fileName);
}

void PlanningDialog::on_problemBrowse_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open PDDL Problem File"), ".", tr("Planning domains (*.pddl)"));

    if ( fileName.isEmpty() )
        return;

    ui->pddlProblemFile->setText(fileName);

    setProblemFile(fileName);
}

void PlanningDialog::showPlannerOutput()
{
    ui->plannerOutput->clear();
    QString text = PlanAnalyzer::generateHTMLSinglePlanReport(getKAVIPlanners(), exe->getPlan());
    QStringList strList = exe->getTestConsoleOutput();
    QString str;
    foreach (QString line, strList) {
        str.append(line);
    }
    QFile htmlFile("test.html");

    if (!htmlFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&htmlFile);
    out << text;

    ui->plannerOutput->setText(text);
}

bool PlanningDialog::getXMLDocument()
{
    QString filePath = getXMLFilePath();
    QFile xmlFile(filePath.append(KAVIPLANNERS_FILE));

    if ( !xmlFile.open(QFile::ReadOnly | QFile::Text ))
    {
        qDebug()<< "@Error: cannot open file: " << xmlFile.fileName();
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

void PlanningDialog::setDomainFile(QString domainFile)
{
    this->domainFile = domainFile;
    initProblemSelection();
}

void PlanningDialog::setProblemFile(QString problemFile)
{
    this->problemFile = problemFile;
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
        qDebug() << "@Warning: The domain PDDL file can not be empty";
        return;
    }
    if (problemFile.simplified().isNull())
    {
        qDebug() << "@Warning: The problem PDDL file can not be empty";
        return;
    }
    if (theSingleChosenPlanner.isNull())
    {
        qDebug() << "@Warning: Please select one planner to execute planning";
        return;
    }
    solveProblemWithSinglePlanner(domainFile, problemFile, theSingleChosenPlanner);
}

QString PlanningDialog::getDomainName()
{
    QFile pddlDomainFile(domainFile);
    if ( !pddlDomainFile.open(QFile::ReadOnly | QFile::Text ))
    {
        qDebug()<< "@Error: cannot open file: " << pddlDomainFile.fileName();
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
        qDebug()<< "@Error: cannot open file: " << pddlProblemFile.fileName();
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
