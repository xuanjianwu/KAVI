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
    DefinitionPredicateInfo(DataWidget* data, int predicateID, QWidget* parent = 0);
    void updateContent();

private slots:
    void on_moveDown_pressed();
    void on_moveUp_pressed();

private:
    void fillArgumentTable();
    void moveCurrentRow(RowMoveDirection direction);
    void swapTableItems(QTableWidgetItem* first, QTableWidgetItem* second);
    void swapConnections(int firstArg, int secondArg);

    Ui::DefinitionPredicateInfo ui;
}

KAVI_NS_END

#endif // DEFINITIONPREDICATEINFO_H
