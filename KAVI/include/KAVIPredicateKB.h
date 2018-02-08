#ifndef KAVIPREDICATEKB_H
#define KAVIPREDICATEKB_H

#include "KAVIBase.h"
#include "KAVIKB.h"

class KAVIPredicateKB: KAVIKB {
public:
    KAVIPredicateKB();
    ~KAVIPredicateKB();
    bool loadKB(QFile &baseFile);
    bool saveKB(QFile &baseFile);
    bool refreshCachetoBase();
    bool readBasetoCache();
    void testAddPredicates(QStringList list);
    void testDeletePredicates(QStringList list);
private:
    QStringList cachedPredicates;
}


#endif // KAVIPREDICATEKB_H
