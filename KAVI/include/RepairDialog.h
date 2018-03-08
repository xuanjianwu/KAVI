/*
 * @author: liYc
 * @date  : 2018/3/8
 * @brief : the repair dialog for the plan repair advice
*/

#ifndef REPAIRDIALOG_H
#define REPAIRDIALOG_H

#include "KAVIBase.h"

namespace Ui {
class RepairDialog;
}

class RepairDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RepairDialog(QStringList options, QWidget *parent = 0);
    ~RepairDialog();

    int getOption();

private slots:
    void on_ok_clicked();

    void on_cancel_clicked();

private:
    Ui::RepairDialog *ui;
};

#endif // REPAIRDIALOG_H
