#ifndef KAVIPREDICATEKB_H
#define KAVIPREDICATEKB_H

#include "KAVIBase.h"
#include "KAVIKB.h"

class KAVIPredicateKB: public KAVIKB {
public:
    KAVIPredicateKB();
    ~KAVIPredicateKB();
    bool loadKB(QFile &baseFile);
    bool saveKB(QFile &baseFile);
    bool refreshCachetoBase();
    bool readBasetoCache();
    QStringList getPredicates() const;
    bool addPredicate(QString predicateSign);
    bool removePredicate(QString predicateSign);
private:
    QStringList cachedPredicates;
};

#endif // KAVIPREDICATEKB_H
