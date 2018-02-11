#ifndef KAVIKB_H
#define KAVIKB_H

#include "KAVIBase.h"

class KAVIKB {
public:
    KAVIKB();

    /*
    * deconstruct function
    * @params: N/A
    * @return: N/A
    */
    ~KAVIKB();
    virtual bool loadKB(QFile &baseFile) = 0;
    virtual bool saveKB(QFile &baseFile) = 0;
protected:
    // the stable class knowledge base
    QStringList classes;
    // the stable predicate knowledge base
    QStringList predicates;
};

#endif // KAVIKB_H
