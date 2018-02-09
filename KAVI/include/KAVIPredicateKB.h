#ifndef KAVIPREDICATEKB_H
#define KAVIPREDICATEKB_H

#include "KAVIBase.h"
#include "KAVIKB.h"

class KAVIPredicateKB: public KAVIKB {
public:
    /*
    * construct function
    * @params: N/A
    * @return: N/A
    */
    KAVIPredicateKB();

    /*
    * deconstruct function
    * @params: N/A
    * @return: N/A
    */
    ~KAVIPredicateKB();

    /*
    * load knowledge base from file to stable base, and write to the cached base
    * @params:
    *       baseFile - the specified file
    * @return: success or not
    */
    bool loadKB(QFile &baseFile);

    /*
    * refresh cached base to stable base, and save stable base to file
    * @params:
    *       baseFile - the specified file
    * @return: success or not
    */
    bool saveKB(QFile &baseFile);

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
    * get the cached predicate base
    * @params: N/A
    * @return: string list of predicates' sign
    */
    QStringList getPredicates() const;

    /*
    * add predicate sign to cached base
    * @params:
    *       predicateSign - the expected new predicate sign
    * @return: success or not
    */
    bool addPredicate(QString predicateSign);

    /*
    * remove predicate sign from cached base
    * @params:
    *       predicateSign - the expected removed predicate sign
    * @return: success or not
    */
    bool removePredicate(QString predicateSign);
private:
    // the cached predicate knowledge base
    QStringList cachedPredicates;
};

#endif // KAVIPREDICATEKB_H
