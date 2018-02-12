/*
 * @author: liYc
 * @date  : 2018/2/8
 * @brief : the basic knowledge base, offers basic functions and stable bases
*/

#ifndef KAVIKB_H
#define KAVIKB_H

#include "KAVIBase.h"

class KAVIKB {
public:
    /*
    * construct function
    * @params: N/A
    * @return: N/A
    */
    KAVIKB();

    /*
    * deconstruct function
    * @params: N/A
    * @return: N/A
    */
    ~KAVIKB();

    /*
    * create file in the specified path, if already exists return true directly
    * @params:
    *       filePath - the specified path
    *       fileName - the expected file's name
    * @return: create file success or file already exists or create failed
    */
    bool createFile(QString filePath, QString fileName);
    virtual bool loadKB() = 0;
    virtual bool saveKB() = 0;
    virtual QStringList getData() const = 0;
protected:
    // the stable class knowledge base
    QStringList classes;
    // the stable predicate knowledge base
    QStringList predicates;
};

#endif // KAVIKB_H
