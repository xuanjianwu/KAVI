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
    ProblemsPredicateDialog(QStringList predicates, bool radioInit, QWidget* parent = 0);

    QString predicateName();
    QString taskState();
private:
    Ui::ProblemsPredicateDialog ui;
}

KAVI_NS_END

#endif // PROBLEMSPREDICATEDIALOG_H
