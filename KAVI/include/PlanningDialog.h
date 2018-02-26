/*
 * @author: liYc
 * @date  : 2018/2/27
 * @brief : the dialog for the planning
*/

#ifndef PLANNINGDIALOG_H
#define PLANNINGDIALOG_H

#include <QDialog>

namespace Ui {
class PlanningDialog;
}

class PlanningDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlanningDialog(QWidget *parent = 0);
    ~PlanningDialog();

private:
    Ui::PlanningDialog *ui;
};

#endif // PLANNINGDIALOG_H
