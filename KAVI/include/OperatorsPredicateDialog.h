/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : the predicate dialog in operator view
*/

#ifndef OPERATORSPREDICATEDIALOG_H
#define OPERATORSPREDICATEDIALOG_H

#include "KAVIBase.h"
#include "ui_OperatorsPredicateDialog.h"

KAVI_NS_BEGIN

class OperatorsPredicateDialog: public QDialog {
    Q_OBJECT

public:
    OperatorsPredicateDialog(QStringList predicates, QWidget* parent = 0);

    QString predicateName();
    QStringList predicateSet();

private slots:

    void on_precondCheck_toggled(bool checked);
    void on_effectPosCheck_toggled(bool checked);
    void on_effectNegCheck_toggled(bool checked);

private:
    Ui::OperatorsPredicateDialog ui;
}

KAVI_NS_END

#endif // OPERATORSPREDICATEDIALOG_H
