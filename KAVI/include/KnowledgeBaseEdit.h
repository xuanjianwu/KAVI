/*
 * @author: liYc
 * @date  : 2018/2/12
 * @brief : the dialog to view and edit knowledge base
*/

#ifndef KNOWLEDGEBASEEDIT_H
#define KNOWLEDGEBASEEDIT_H

#include "KAVIBase.h"
#include "KAVIClassKB.h"
#include "KAVIPredicateKB.h"
#include "ui_KnowledgeBaseEdit.h"

namespace Ui {
class KnowledgeBaseEdit;
}

class KnowledgeBaseEdit : public QDialog
{
    Q_OBJECT

public:
    /*
    * construct function
    * @params:
    *       classKB     - the dependent class knowledge base
    *       predicateKB - the dependent predicate knowledge base
    *       parent      - parent object
    * @return: N/A
    */
    explicit KnowledgeBaseEdit(KAVIClassKB *classKB, KAVIPredicateKB *predicateKB, QWidget *parent = 0);
    ~KnowledgeBaseEdit();

    /*
    * load data from specified knowledge base to the list widget
    * @params:
    *       base - the specified knowledge base
    *       list - the target list widget
    * @return: void
    */
    void loadDataFromBase(KAVIKB *base, QListWidget *list);

public slots:

    /*
    * handle the double click event on class item or predicate item, open the associated edit dialog
    * @params:
    *       item - the clicked item
    * @return: void
    */
    void editBase(QListWidgetItem* item);

private slots:

    /*
    * handle the edit button click event, open the associated edit dialog
    * @params: N/A
    * @return: void
    */
    void on_buttonEdit_clicked();

    /*
    * quit the knowledge base edit dialog
    * @params: N/A
    * @return: void
    */
    void on_buttonCancel_clicked();

    /*
    * handle the add button click event, open the associated edit dialog
    * @params: N/A
    * @return: void
    */
    void on_buttonAdd_clicked();

    /*
    * handle the delete button click event, delete the selected item
    * @params: N/A
    * @return: void
    */
    void on_buttonDelete_clicked();

    /*
    * hanle the index change event of tabWidget, update associated edit dialog
    * @params:
    *       index - the current index of tabWidget
    * @return: void
    */
    void on_tabWidget_currentChanged(int index);

private:
    // the dependent class knowledge base
    KAVIClassKB *classKB;
    // the dependent predicate knowledge base
    KAVIPredicateKB *predicateKB;
    // the specified name checker for class name and predicate name
    QRegExp nameChecker;
    Ui::KnowledgeBaseEdit *ui;
};

#endif // KNOWLEDGEBASEEDIT_H
