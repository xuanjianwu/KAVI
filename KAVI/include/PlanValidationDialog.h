/*
 * @author: liYc
 * @date  : 2018/3/6
 * @brief : the dialog for the plan validation
*/

#ifndef PLANVALIDATIONDIALOG_H
#define PLANVALIDATIONDIALOG_H

#include "KAVIBase.h"
#include "PlanningDialog.h"
#include "PlanValidator.h"

namespace Ui {
class PlanValidationDialog;
}

class PlanValidationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlanValidationDialog(PlanValidator* planValidator, QWidget *parent = 0);
    ~PlanValidationDialog();

    void fillActionsTable();


private:
    Ui::PlanValidationDialog *ui;

    PlanValidator* planValidator;
};

#endif // PLANVALIDATIONDIALOG_H
