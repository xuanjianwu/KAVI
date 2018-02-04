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



class DataWidget;

class DefinitionClassInfo: public InfoPanel {
    Q_OBJECT

public:
    /*
    * construct function. set the content for the specified class
    * @params:
    *       data     - the dependent DataWidget
    *       classID  - the specified class's ID
    *       parent   - parent object
    * @return: N/A
    */
    DefinitionClassInfo(DataWidget* data, int classID, QWidget* parent = 0);

private:
    Ui::DefinitionClassInfo ui;
};



#endif // DEFINITIONCLASSINFO_H
