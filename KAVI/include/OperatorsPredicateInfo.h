/*
 * @author: liYc
 * @date  : 2018/1/
 * @brief : the infomation panel for predicate in operator view
*/

#ifndef OPERATORSPREDICATEINFO_H
#define OPERATORSPREDICATEINFO_H

#include "KAVIBase.h"
#include "InfoPanel.h"
#include "ui_OperatorsPredicateInfo.h"



class DataWidget;

class OperatorsPredicateInfo: public InfoPanel {
    Q_OBJECT

public:
    /*
    * construct function, set the content for specified predicate
    * @params:
    *       data        - the dependent DataWidget
    *       definition  - the definition DataWidget
    *       predicateID - the selected predicate's ID
    *       parent      - parent object
    * @return: N/A
    */
    OperatorsPredicateInfo(DataWidget* data, const DataWidget* definition, int predicateID, QWidget* parent = 0);

    /*
    * update content, first clear the old content, and reset predicateDef (Because
    * the selected predicate may have changed due to the change from InfoPanel),
    * then fill it again with new DataWidget
    * @params: N/A
    * @return: N/A
    */
    void updateContent();

private slots:
    /*
    * handle the press on moveDown button
    * emit madeChange(ArgumentOrderChanged)
    * @params: N/A
    * @return: N/A
    */
    void on_moveDown_pressed();

    /*
    * handle the press on moveUp button
    * emit madeChange(ArgumentOrderChanged)
    * @params: N/A
    * @return: N/A
    */
    void on_moveUp_pressed();

    /*
    * handle precondition check toggle, update predicate's set in DataWidget
    * and refresh the predicate node in DiagramWidget
    * emit madeChange(PredicateSetChanged)
    * @params:
    *       checked - check flag
    * @return: void
    */
    void on_precondPosCheck_toggled(bool checked);

    /*
    * handle effect positive check toggle, update predicate's set in DataWidget
    * and refresh the predicate node in DiagramWidget
    * emit madeChange(PredicateSetChanged)
    * @params:
    *       checked - check flag
    * @return: void
    */
    void on_effectPosCheck_toggled(bool checked);

    /*
    * handle effect negative check toggle, update predicate's set in DataWidget
    * and refresh the predicate node in DiagramWidget
    * emit madeChange(PredicateSetChanged)
    * @params:
    *       checked - check flag
    * @return: void
    */
    void on_effectNegCheck_toggled(bool checked);

    /*
    * handle precondition check toggle, update predicate's set in DataWidget
    * and refresh the predicate node in DiagramWidget
    * emit madeChange(PredicateSetChanged)
    * @params:
    *       checked - check flag
    * @return: void
    */
    void on_precondNegCheck_toggled(bool checked);

private:
    // the dependent definition DataWidget
    const DataWidget* xmlDefinition;
    // the selected predicate's associated predicate in definition
    QDomElement predicateDef;

    /*
    * fill the arguments table for the selected node
    * @params: N/A
    * @return: N/A
    */
    void fillArgumentTable();

    /*
    * set the predicate's set check boxes
    * @params: N/A
    * @return: N/A
    */
    void setCheckBoxes();

    /*
    * move the currenr row in the table, including update the DataWidget and table's content
    * @params:
    *       direction - the direction of move
    * @return: void
    */
    void moveCurrentRow(RowMoveDirection direction);

    /*
    * swap the table items's content in the table, and set the current table
    * item to the target table index
    * @params:
    *       first   - the table item of positive move
    *       second  - the table item of negative move
    * @return: void
    */
    void swapTableItems(QTableWidgetItem* first, QTableWidgetItem* second);

    /*
    * swap the arguments' order in the DataWidget
    * @params:
    *       firstArg  - the first argument's index of positive move in the table
    *       secondArg - the second argument's index of negative move in the table
    * @return: void
    */
    void swapConnections(int firstArg, int secondArg);

    /*
    * add the set to the selected predicate
    * @params:
    *       set - the added set
    * @return: void
    */
    void includeInSet(QString set);

    /*
    * remove set from the selected predicate
    * @params:
    *       set - the removed set
    * @return: void
    */
    void excludeFromSet(QString set);

    Ui::OperatorsPredicateInfo ui;
};



#endif // OPERATORSPREDICATEINFO_H
