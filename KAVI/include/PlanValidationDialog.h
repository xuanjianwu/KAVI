#ifndef PLANVALIDATIONDIALOG_H
#define PLANVALIDATIONDIALOG_H

#include <QDialog>

namespace Ui {
class PlanValidationDialog;
}

class PlanValidationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlanValidationDialog(QWidget *parent = 0);
    ~PlanValidationDialog();

private:
    Ui::PlanValidationDialog *ui;
};

#endif // PLANVALIDATIONDIALOG_H
