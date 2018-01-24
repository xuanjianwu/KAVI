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
    ProblemsObjectInfo(DataWidget* data, const DataWidget* definition, int objectID, int definitionID, QWidget* parent = 0);

private slots:

    void on_classComboBox_currentIndexChanged(QString newClass);

private:
    const DataWidget* xmlDefinition;
    QDomElement objectDef;

    void initComboBox();

    Ui::ProblemsObjectInfo ui;
}

KAVI_NS_END

#endif // PROBLEMSOBJECTINFO_H
