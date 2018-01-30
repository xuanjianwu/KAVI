#include "InfoPanel.h"

KAVI_NS_BEGIN



InfoPanel::InfoPanel(QWidget *parent): QWidget(parent) {
    QHBoxLayout* layout = new QHBoxLayout;
    QLabel* hintLabel = new QLabel("Right click on some node.");
    hintLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    layout->addWidget(hintLabel);
    setLayout(layout);
}

InfoPanel::InfoPanel(DataWidget *data, int nodeID, QWidget *parent): QWidget(parent) {
    xmlData = data;
    selectedNode = xmlData->findNode(nodeID);
}



KAVI_NS_END
