#ifndef KAVIKB_H
#define KAVIKB_H

#include "KAVIBase.h"

class KAVIKB {
public:
    KAVIKB();
    ~KAVIKB();
    virtual bool loadKB(QFile &baseFile) = 0;
    virtual bool saveKB(QFile &baseFile) = 0;
protected:
    QStringList classes;
    QStringList predicates;
};

#endif // KAVIKB_H
