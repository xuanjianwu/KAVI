#ifndef DEFINITIONPREDICATEDIALOG_H
#define DEFINITIONPREDICATEDIALOG_H

#include "KAVIBase.h"
#include "KAVIPredicateKB.h"
#include "ui_DefinitionPredicateDialog.h"

namespace Ui {
class DefinitionPredicateDialog;
}

class DefinitionPredicateDialog : public QDialog
{
    Q_OBJECT

public:

    /*
    * construct function
    * @params:
    *       predicateKB - the dependent predicate sign knowledge base
    *       parent      - parent object
    * @return: N/A
    */
    explicit DefinitionPredicateDialog(KAVIPredicateKB *predicateKB, QWidget *parent = 0);

    /*
    * deconstruct function
    * @params: N/A
    * @return: N/A
    */
    ~DefinitionPredicateDialog();

    /*
    * get the current select predicate sign
    * @params: N/A
    * @return: predicate sign
    */
    QString predicateSign() const;

public slots:

    /*
    * after complete the edit, add the selected predicate sign to cached base
    * @params: N/A
    * @return: void
    */
    void editComplete();

private:
    // the string list model for the completer
    QStringListModel *listModel;
    // the dependent predicate sign knowledge base
    KAVIPredicateKB *predicateKB;
    Ui::DefinitionPredicateDialog *ui;
};

#endif // DEFINITIONPREDICATEDIALOG_H
