/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : This class handles all the input/output from/to files.
 *          Only output is supported for the PDDL format.
 *          In the case of XML both input and output is supported.
*/

#ifndef CONVERTOR_H
#define CONVERTOR_H

#include "KAVIBase.h"
#include "XMLUtils.h"

KAVI_NS_BEGIN

class QWidget;

class Convertor {
public:
    /*
    * construct function
    * @params:
    *       owner - specify the convertor's owner object
    * @return: N/A
    */
    Convertor(QWidget* owner = 0);

    /*
    * read QDomDocument from XML file
    * @params:
    *       input - XML file
    * @return: QDomDocument
    */
    QDomDocument readFromXML(QFile& input);

    /*
    * write QDomDocument to XML file
    * @params:
    *       doc    - target QDomDocument
    *       output - expected XML file
    * @return: void
    */
    void writeToXML(const QDomDocument& doc, QFile& output);

    /*
    * write Domain in QDomDocument to PDDL file
    * @params:
    *       doc    - target QDomDocument
    *       output - expected PDDL file
    * @return: void
    */
    void writeDomainToPDDL(const QDomDocument& doc, QFile& output);

    /*
    * write problems in QDomDocument to PDDL file
    * @params:
    *       doc    - target QDomDocument
    *       targetDir - expected PDDL file directory
    * @return: void
    */
    void writeProblemsToPDDL(const QDomDocument& doc, QString& targetDir);

private:
    // convertor's owner widget
    QWidget * ownerWidget;

    // current domain'name
    QString domainName;

    // output text stream
    QTextStream outStream;

    // current domain's document
    QDomDocument currentDoc;

    // current document's root element
    QDomElement rootElement;

    // current document's definition element
    QDomElement definitionElement;

    // current document's operators element
    QDomElement operatorsElement;

    // current document's problems element
    QDomElement problemsElement;

    /*
    * basic check for document's format
    * @params:
    *       domainDocument - target document
    * @return: bool
    */
    bool basicStructureCheck(const QDomDocument& domainDocument);

    /*
    * write a line in PDDL file
    * @params:
    *       indent - indent configure
    *       text   - content in the line
    * @return: void
    */
    void writeLine(int indent, QString text);

    /*
    * write a item in PDDL file
    * @params:
    *       indent - indent configure
    *       text   - content in the item
    *       header - content before item
    *       footer - content after item
    * @return: void
    */
    void writeItem(int indent, QString text, QString header = QString(), QString footer = QString());

    /*
    * write a action to PDDL file
    * @params:
    *       indent  - indent configure
    *       diagram - action's diagram
    *       name    - action's name
    * @return: void
    */
    void writeAction(int indent, QDomElement diagram, QString name);

    /*
    * write a task to PDDL file
    * @params:
    *       diagram    - task's diagram
    *       name       - task's name
    *       targetDir  - PDDL file's directory
    * @return: void
    */
    void writeTask(QDomElement& diagram, const QString& name, const QString& targetDir);

    QString getDefinedTypes();

    QString getDefinedPredicates();

    QString getPredicateDefinition(QDomElement diagram, QString predName);
    QString getPredicateInstance(QDomElement predicate, QDomElement diagram, bool ques = true);
    QString getObjectList(QDomElement diagram);
    QString buildPredicateString(QString& name, QStringList* argNames,
                        QStringList* argTypes = 0, bool ques = true);
    QString buildSetString(QStringList& positive, QStringList& negative);

    QStringList	generateArgumentNames(int size, char startingChar);
    QString getActionParameters(QDomElement diagram);
}

KAVI_NS_END

#endif // CONVERTOR_H
