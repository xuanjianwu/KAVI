#include "PlanValidationDialog.h"
#include "ui_PlanValidationDialog.h"

PlanValidationDialog::PlanValidationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanValidationDialog)
{
    ui->setupUi(this);
}

PlanValidationDialog::~PlanValidationDialog()
{
    delete ui;
}
