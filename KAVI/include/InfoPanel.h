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

KAVI_NS_BEGIN

class InfoPanel: public QWidget {
    Q_OBJECT

public:
    InfoPanel(QWidget* parent = 0): QWidget(parent) {
        QHBoxLayout* layout = new QHBoxLayout;
        QLabel* hintLabel = new QLabel("Right click on some node.");
        hintLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        layout->addWidget(hintLabel);
        setLayout(layout);
    }

    InfoPanel(DataWidget* data, int nodeID, QWidget* parent = 0): QWidget(parent) {
        xmlData = data;
        selectedNode = xmlData->findNode(nodeID);
    }
    virtual void updateContent() {}

signals:
    void madeChange(int changeCode);

protected:
    DataWidget* xmlData;
    QDomElement selectedNode;
}

KAVI_NS_END

#endif // INFOPANEL_H
