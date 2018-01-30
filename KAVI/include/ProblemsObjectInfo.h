/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : the infomation panel for object in problem view
*/

#ifndef PROBLEMSOBJECTINFO_H
#define PROBLEMSOBJECTINFO_H

#include "KAVIBase.h"
#include "InfoPanel.h"
#include "ui_ProblemsObjectInfo.h"

KAVI_NS_BEGIN

class DataWidget;

class ProblemsObjectInfo: public InfoPanel {
    Q_OBJECT

public:
    /*
    * construct function, set the contents of InfoPanel
    * @params:
    *       data          - the dependent DataWidget
    *       definition    - the defintion DataWidget
    *       objectID      - the selected object's ID
    *       definitionID  - the selected object's associated class's ID in definition
    *       parent        - parent object
    * @return:
    */
    ProblemsObjectInfo(DataWidget* data, const DataWidget* definition, int objectID, int definitionID, QWidget* parent = 0);

private slots:

    /*
    * handle the current index changed of class combobox
    * emit madeChange(NodeClassChanged)
    * @params:
    *       newClass - the new class type for selected object
    * @return: void
    */
    void on_classComboBox_currentIndexChanged(QString newClass);

private:
    // the dependent definition DataWidget
    const DataWidget* xmlDefinition;
    // the object's associated class's element in the xmlDefinition
    QDomElement objectDef;

    /*
    * init the combobox for selected object, and set selected object as current index of
    * combobox
    * @params: N/A
    * @return: void
    */
    void initComboBox();

    Ui::ProblemsObjectInfo ui;
}

KAVI_NS_END

#endif // PROBLEMSOBJECTINFO_H
