/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : the widget shows infomation about any objects
*/

#ifndef INFOPANEL_H
#define INFOPANEL_H

#include "KAVIBase.h"
#include "XMLUtils.h"
#include "DataWidget.h"



class InfoPanel: public QWidget {
    Q_OBJECT

public:
    /*
    * construct function, init the InfoPanel's UI
    * @params:
    *       parent - parent object
    * @return: N/A
    */
    InfoPanel(QWidget* parent = 0);

    /*
    * construct function, init the InfoPanel's Data
    * @params:
    *       data   - set the dependent DataWidget
    *       nodeID - set the selected node to show Info for
    *       parent - parent object
    * @return:
    */
    InfoPanel(DataWidget* data, int nodeID, QWidget* parent = 0);

    /*
    * update the content of InfoPanel
    * @params: N/A
    * @return: N/A
    */
    virtual void updateContent() {}

signals:
    /*
    * this signal is occur by InfoPanel's inheritor,
    * this signal is handled by the EditWidget's handleExternChange
    * @params:
    *       changeCode - the changeCode carried by signal
    * @return: void
    */
    void madeChange(int changeCode);

protected:
    // the dependent DataWidget for the InfoPanel
    DataWidget* xmlData;

    // the selected Node to show InfoPanel
    QDomElement selectedNode;
};



#endif // INFOPANEL_H
