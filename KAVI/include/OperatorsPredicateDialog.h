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
    /*
    * construct function with predicates
    * @params:
    *       predicates - available predicates used to add new predicate
    *       parent     - parent object
    * @return: N/A
    */
    OperatorsPredicateDialog(QStringList predicates, QWidget* parent = 0);

    /*
    * get the selected predicate's name to init new predicate
    * @params: N/A
    * @return: predicate's name
    */
    QString predicateName();

    /*
    * get the predicate's set to add new predicate
    * @params: N/A
    * @return: predicate's set
    */
    QStringList predicateSet();

private slots:

    /*
    * handle the precondition set check for the new add predicate
    * @params:
    *       checked - check flag
    * @return: void
    */
    void on_precondCheck_toggled(bool checked);

    /*
    * handle the positive effect set check for the new add predicate
    * @params:
    *       checked - check flag
    * @return: void
    */
    void on_effectPosCheck_toggled(bool checked);

    /*
    * handle the negative effect set check for the new add predicate
    * @params:
    *       checked - check flag
    * @return: void
    */
    void on_effectNegCheck_toggled(bool checked);

private:
    Ui::OperatorsPredicateDialog ui;
};

KAVI_NS_END

#endif // OPERATORSPREDICATEDIALOG_H
