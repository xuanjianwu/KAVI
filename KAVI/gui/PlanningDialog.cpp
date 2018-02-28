#include "PlanningDialog.h"
#include "ui_PlanningDialog.h"

PlanningDialog::PlanningDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanningDialog)
{
    ui->setupUi(this);
}

PlanningDialog::~PlanningDialog()
{
    delete ui;
}

void PlanningDialog::solveProblemWithSinglePlanner(QString domain, QString problem, QDomElement chosenPlanner)
{
    exe = new ExecPlanner(chosenPlanner, domain, problem, false, this);
    connect(exe, SIGNAL(finished()), this, SLOT(showPlannerOutput()));
    exe->start();
}

void PlanningDialog::on_customProblem_clicked(bool checked)
{
    if (checked)
    {
        ui->pddlDomainFile->setEnabled(true);
        ui->pddlProblemFile->setEnabled(true);
        ui->domainBrowse->setEnabled(true);
        ui->problemBrowse->setEnabled(true);
    }
    else
    {
        ui->pddlDomainFile->setEnabled(false);
        ui->pddlDomainFile->clear();
        ui->pddlProblemFile->setEnabled(false);
        ui->pddlProblemFile->clear();
        ui->domainBrowse->setEnabled(false);
        ui->problemBrowse->setEnabled(false);
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

    domainFile = fileName;
}

void PlanningDialog::on_problemBrowse_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open PDDL Problem File"), ".", tr("Planning domains (*.pddl)"));

    if ( fileName.isEmpty() )
        return;

    ui->pddlProblemFile->setText(fileName);

    problemFile = fileName;
}

void PlanningDialog::showPlannerOutput()
{
    ui->plannerOutput->clear();
    QStringList textList = exe->getTestConsoleOutput();
    foreach (QString line, textList) {
        ui->plannerOutput->append(line);
    }
}
