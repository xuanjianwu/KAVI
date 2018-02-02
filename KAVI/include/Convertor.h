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
    * read document from XML file and run basicStructureCheck
    * @params:
    *       input - XML file
    * @return: the xml document that pass basicStructureCheck
    */
    QDomDocument readFromXML(QFile& input);

    /*
    * run basicStructureCheck and write docuement to XML file
    * @params:
    *       doc    - the specified document
    *       output - expected XML file
    * @return: void
    */
    void writeToXML(const QDomDocument& doc, QFile& output);

    /*
    * run basicStructureCheck and write the domain in document, including types, predicates
    * and operators to PDDL file
    * @params:
    *       doc    - the specified document
    *       output - expected PDDL file
    * @return: void
    */
    void writeDomainToPDDL(const QDomDocument& doc, QFile& output);

    /*
    * run basicStructureCheck and write problems in domain document to PDDL file
    * @params:
    *       doc    - the specified QDomDocument
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

    // current document
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
    * basic structure check for document's format like this:
    * <domain/>
    *     <definition/>
    *     <operators/>
    *     <problems/>
    * @params:
    *       domainDocument - target document
    * @return: valid structure or not
    */
    bool basicStructureCheck(const QDomDocument& domainDocument);

    /*
    * write one line in PDDL file
    * @params:
    *       indent - indent configure
    *       text   - content in the line
    * @return: void
    */
    void writeLine(int indent, QString text);

    /*
    * write one item in PDDL file
    * @params:
    *       indent - indent configure
    *       text   - content of the item
    *       header - content before item
    *       footer - content after item
    * @return: void
    */
    void writeItem(int indent, QString text, QString header = QString(), QString footer = QString());

    /*
    * write one action to PDDL file
    * @params:
    *       indent  - indent configure
    *       diagram - action's diagram
    *       name    - action's name
    * @return: void
    */
    void writeAction(int indent, QDomElement diagram, QString name);

    /*
    * write one task to PDDL file
    * @params:
    *       diagram    - task's diagram
    *       name       - task's name
    *       targetDir  - PDDL file's directory
    * @return: void
    */
    void writeTask(QDomElement& diagram, const QString& name, const QString& targetDir);

    /*
    * get the defined class types in definition
    * @params: N/A
    * @return: all clases types in string
    */
    QString getDefinedTypes();

    /*
    * get the defined predicates in definition
    * @params: N/A
    * @return: all predicates in string
    */
    QString getDefinedPredicates();

    /*
    * get the string of specified predicate definition
    * @params:
    *       diagram  - the definition diagram
    *       predName - the specified predicate name
    * @return: the string of predicate definition
    */
    QString getPredicateDefinition(QDomElement diagram, QString predName);

    /*
    * get the string of specified predicate instance
    * @params:
    *       predicate - the specified predicate element
    *       diagram   - the associated diagram element
    *       ques      - the flag for the prepend char of arguments' name
    * @return:
    */
    QString getPredicateInstance(QDomElement predicate, QDomElement diagram, bool ques = true);

    /*
    * get string of the list of objects
    * @params:
    *       diagram - the specified task diagram
    * @return: string of list of objects
    */
    QString getObjectList(QDomElement diagram);

    /*
    * build one predicate string
    * @params:
    *       name      - name of predicate
    *       argNames  - names of arguments
    *       argTypes  - class types of arguments
    *       ques      - the flag for the prepend char of arguments' name
    * @return: the string of predicate
    */
    QString buildPredicateString(QString& name, QStringList* argNames,
                        QStringList* argTypes = 0, bool ques = true);

    /*
    * build the set string, now only support the relation positive and negative: not
    * @params:
    *       positive - the positive predicate set
    *       negative - the negative predicate set
    * @return: the set string
    */
    QString buildSetString(QStringList& positive, QStringList& negative);

    /*
    * generate the names for arguments, names starting from the startingChar
    * @params:
    *       size         - the count of the name needed to generate
    *       startingChar - the name start from the startingChar
    * @return: the names's list
    */
    QStringList	generateArgumentNames(int size, char startingChar);

    /*
    * get the parameters string of action
    * @params:
    *       diagram - the specified action diagram
    * @return: parameters string
    */
    QString getActionParameters(QDomElement diagram);
};

KAVI_NS_END

#endif // CONVERTOR_H
