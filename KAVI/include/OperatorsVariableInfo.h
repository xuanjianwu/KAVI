/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : the infomation panel for variable in operator view
*/

#ifndef OPERATORSVARIABLEINFO_H
#define OPERATORSVARIABLEINFO_H

#include "KAVIBase.h"
#include "InfoPanel.h"
#include "ui_OperatorsVariableInfo.h"

KAVI_NS_BEGIN

class OperatorsVariableInfo: public InfoPanel {
    Q_OBJECT

public:
    OperatorsVariableInfo(DataWidget* data, const DataWidget* definition, int variableID, int definitionID, QWidget* parent = 0);

private slots:

    void on_classComboBox_currentIndexChanged(QString newClass);

private:
    const DataWidget* xmlDefinition;
    QDomElement variableDef;

    void initComboBox();

    Ui::OperatorsVariableInfo ui;
}

KAVI_NS_END

#endif // OPERATORSVARIABLEINFO_H
