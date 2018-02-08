#-------------------------------------------------
#
# Project created by QtCreator 2018-01-18T17:30:54
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KAVI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += . include

SOURCES += \
    view/EdgePoint.cpp \
    view/DiagramElement.cpp \
    view/Edge.cpp \
    view/DiagramWidget.cpp \
    model/DataWidget.cpp \
    view/Node.cpp \
    view/EdgeStructure.cpp \
    view/NodeStructure.cpp \
    gui/DefinitionClassInfo.cpp \
    gui/DefinitionPredicateInfo.cpp \
    gui/DomainDescDialog.cpp \
    gui/InfoPanel.cpp \
    gui/OperatorsPredicateDialog.cpp \
    gui/OperatorsPredicateInfo.cpp \
    gui/OperatorsVariableDialog.cpp \
    gui/OperatorsVariableInfo.cpp \
    gui/ProblemsObjectDialog.cpp \
    gui/ProblemsObjectInfo.cpp \
    gui/ProblemsPredicateDialog.cpp \
    gui/ProblemsPredicateInfo.cpp \
    controller/EditWidget.cpp \
    controller/DrivenEditWidget.cpp \
    controller/DefinitionEdit.cpp \
    controller/OperatorsEdit.cpp \
    controller/ProblemsEdit.cpp \
    gui/TreeItem.cpp \
    gui/TreeModel.cpp \
    utils/CheckUtils.cpp \
    utils/Convertor.cpp \
    utils/XMLUtils.cpp \
    utils/igraph.cpp \
    utils/GraphClass.cpp \
    view/DiagramEvents.cpp \
    src/main.cpp \
    gui/KAVIMainWindow.cpp \
    knowledgeBase/KAVIKB.cpp \
    knowledgeBase/KAVIClassKB.cpp \
    knowledgeBase/KAVIPredicateKB.cpp \
    include/DefinitionClassDialog.cpp

HEADERS += \
    include/KAVIBase.h \
    include/KAVIMacros.h \
    include/DiagramElement.h \
    include/CheckUtils.h \
    include/Convertor.h \
    include/DiagramEvents.h \
    include/DataWidget.h \
    include/DiagramWidget.h \
    include/DomainDescDialog.h \
    include/DrivenEditWidget.h \
    include/Edge.h \
    include/DefinitionEdit.h \
    include/DefinitionClassInfo.h \
    include/OperatorsEdit.h \
    include/OperatorsPredicateDialog.h \
    include/OperatorsVariableDialog.h \
    include/EdgePoint.h \
    include/EditWidget.h \
    include/GraphClass.h \
    include/InfoPanel.h \
    include/Node.h \
    include/NodeStructure.h \
    include/ProblemsObjectInfo.h \
    include/OperatorsPredicateInfo.h \
    include/DefinitionPredicateInfo.h \
    include/ProblemsPredicateInfo.h \
    include/ProblemsEdit.h \
    include/ProblemsObjectDialog.h \
    include/ProblemsPredicateDialog.h \
    include/TreeItem.h \
    include/TreeModel.h \
    include/OperatorsVariableInfo.h \
    include/KAVIMainWindow.h \
    include/XMLUtils.h \
    include/igraph.h \
    include/EdgeStructure.h \
    include/KAVITypedefs.h \
    include/KAVIKB.h \
    include/KAVIPredicateKB.h \
    include/KAVIClassKB.h \
    include/DefinitionClassDialog.h

FORMS += \
    forms/KAVIMainWindow.ui \
    forms/DefinitionClassInfo.ui \
    forms/DefinitionPredicateInfo.ui \
    forms/DomainDescDialog.ui \
    forms/OperatorsPredicateDialog.ui \
    forms/OperatorsPredicateInfo.ui \
    forms/OperatorsVariableDialog.ui \
    forms/OperatorsVariableInfo.ui \
    forms/ProblemsObjectDialog.ui \
    forms/ProblemsObjectInfo.ui \
    forms/ProblemsPredicateDialog.ui \
    forms/ProblemsPredicateInfo.ui

RESOURCES += \
    res/KAVIIcons.qrc \
    res/KAVIImages.qrc
