/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : the predicate dialog in problem view
*/

#ifndef PROBLEMSPREDICATEDIALOG_H
#define PROBLEMSPREDICATEDIALOG_H

#include "KAVIBase.h"
#include "ui_ProblemsPredicateDialog.h"

KAVI_NS_BEGIN

class ProblemsPredicateDialog: public QDialog {
    Q_OBJECT

public:
    /*
    * construct function with predicates
    * @params:
    *       predicates - available predicates used to add new predicate
    *       radioInit  - predicates's initial state
    *       parent     - parent object
    * @return: N/A
    */
    ProblemsPredicateDialog(QStringList predicates, bool radioInit, QWidget* parent = 0);

    /*
    * get the selected predicate's name to init new predicate
    * @params: N/A
    * @return: predicate's name
    */
    QString predicateName();

    /*
    * get the predicate's state to add new predicate
    * @params: N/A
    * @return: predicate's state
    */
    QString taskState();
private:
    Ui::ProblemsPredicateDialog ui;
};

KAVI_NS_END

#endif // PROBLEMSPREDICATEDIALOG_H
