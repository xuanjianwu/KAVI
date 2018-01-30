/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : the infomation panel for predicate in definition view
*/

#ifndef DEFINITIONPREDICATEINFO_H
#define DEFINITIONPREDICATEINFO_H

#include "KAVIBase.h"
#include "InfoPanel.h"
#include "ui_DefinitionPredicateInfo.h"

KAVI_NS_BEGIN

class DataWidget;

class DefinitionPredicateInfo: public InfoPanel {
    Q_OBJECT

public:
    /*
    * construct function, set the content for specified predicate
    * @params:
    *       data        - the dependent DataWidget
    *       predicateID - the specified predicate's ID
    *       parent      - parent object
    * @return: N/A
    */
    DefinitionPredicateInfo(DataWidget* data, int predicateID, QWidget* parent = 0);

    /*
    * update content, first clear the old content, then fill it again with new DataWidget
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

private:
    /*
    * fill the arguments table for the selected node
    * @params: N/A
    * @return: N/A
    */
    void fillArgumentTable();

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

    Ui::DefinitionPredicateInfo ui;
}

KAVI_NS_END

#endif // DEFINITIONPREDICATEINFO_H
