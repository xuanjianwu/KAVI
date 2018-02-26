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
