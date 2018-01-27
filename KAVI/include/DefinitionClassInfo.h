/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : the infomation panel for class in definition view
*/

#ifndef DEFINITIONCLASSINFO_H
#define DEFINITIONCLASSINFO_H

#include "KAVIBase.h"
#include "InfoPanel.h"
#include "ui_DefinitionClassInfo.h"

KAVI_NS_BEGIN

class DataWidget;

class DefinitionClassInfo: public InfoPanel {
    Q_OBJECT

public:
    DefinitionClassInfo(DataWidget* data, int classID, QWidget* parent = 0);

private:
    Ui::DefinitionClassInfo ui;
}

KAVI_NS_END

#endif // DEFINITIONCLASSINFO_H