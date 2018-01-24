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

KAVI_NS_BEGIN

class DataWidget;

class OperatorsPredicateInfo: public InfoPanel {
    Q_OBJECT

public:
    OperatorsPredicateInfo(DataWidget* data, const DataWidget* definition, int predicateID, QWidget* parent = 0);
    void updateContent();

private slots:
    void on_moveDown_pressed();
    void on_moveUp_pressed();

    void on_precondCheck_toggled(bool checked);
    void on_effectPosCheck_toggled(bool checked);
    void on_effectNegCheck_toggled(bool checked);

private:
    const DataWidget* xmlDefinition;
    QDomElement predicateDef;

    void fillArgumentTable();
    void setCheckBoxes();
    void moveCurrentRow(RowMoveDirection direction);
    void swapTableItems(QTableWidgetItem* first, QTableWidgetItem* second);
    void swapConnections(int firstArg, int secondArg);

    void includeInSet(QString set);
    void excludeFromSet(QString set);

    Ui::OperatorsPredicateInfo ui;
}

KAVI_NS_END

#endif // OPERATORSPREDICATEINFO_H
