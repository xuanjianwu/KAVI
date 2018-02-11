/*
 * @author: liYc
 * @date  : 2018/2/8
 * @brief : the specified knowledge base for classes, offers cached base
*/

#ifndef KAVICLASSKB_H
#define KAVICLASSKB_H

#include "KAVIBase.h"
#include "KAVIKB.h"

class KAVIClassKB: public KAVIKB {
public:
    /*
    * construct function
    * @params: N/A
    * @return: N/A
    */
    KAVIClassKB();

    /*
    * deconstruct function
    * @params: N/A
    * @return: N/A
    */
    ~KAVIClassKB();

    /*
    * load knowledge base from file to stable base, and write to the cached base
    * @params: N/A
    * @return: success or not
    */
    bool loadKB();

    /*
    * refresh cached base to stable base, and save stable base to file
    * @params: N/A
    * @return: success or not
    */
    bool saveKB();

    /*
    * refresh cached base to stable base
    * @params: N/A
    * @return: success or not
    */
    bool refreshCachetoBase();

    /*
    * read base from stable base to cached base
    * @params: N/A
    * @return: success or not
    */
    bool readBasetoCache();

    /*
    * get the cached class base
    * @params: N/A
    * @return: string list of classes' name
    */
    QStringList getClasses() const;

    /*
    * add class to cached base
    * @params:
    *       className - the expected new class
    * @return: success or not
    */
    bool addClass(QString className);

    /*
    * remove class from cached base
    * @params:
    *       className - the expected removed class
    * @return: success or not
    */
    bool removeClass(QString className);
private:
    // the cached class knowledge base
    QStringList cachedClasses;
};

#endif // KAVICLASSKB_H
