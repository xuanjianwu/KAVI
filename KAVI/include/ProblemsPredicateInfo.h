/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : the infomation panel for predicate in problem view
*/

#ifndef PROBLEMSPREDICATEINFO_H
#define PROBLEMSPREDICATEINFO_H

#include "KAVIBase.h"
#include "InfoPanel.h"
#include "ui_ProblemsPredicateInfo.h"

KAVI_NS_BEGIN

class DataWidget;

class ProblemsPredicateInfo: public InfoPanel {
    Q_OBJECT

public:
    ProblemsPredicateInfo(DataWidget* data, const DataWidget* definition, int predicateID, QWidget* parent = 0);
    void updateContent();

private slots:
    void on_moveDown_pressed();
    void on_moveUp_pressed();

    void on_initRadioButton_toggled(bool checked);
    void on_goalRadioButton_toggled(bool checked);

private:
    const DataWidget* xmlDefinition;
    QDomElement predicateDef;

    void fillArgumentTable();
    void setRadioButtons();
    void moveCurrentRow(RowMoveDirection direction);
    void swapTableItems(QTableWidgetItem* first, QTableWidgetItem* second);
    void swapConnections(int firstArg, int secondArg);

    void setPredicateState(QString newState);

    Ui::ProblemsPredicateInfo ui;
}

KAVI_NS_END

#endif // PROBLEMSPREDICATEINFO_H
