#ifndef KAVICLASSKB_H
#define KAVICLASSKB_H

#include "KAVIBase.h"
#include "KAVIKB.h"

class KAVIClassKB: public KAVIKB {
public:
    KAVIClassKB();
    ~KAVIClassKB();
    bool loadKB(QFile &baseFile);
    bool saveKB(QFile &baseFile);
    bool refreshCachetoBase();
    bool readBasetoCache();
    QStringList getClasses() const;
    bool addClass(QString className);
    bool removeClass(QString className);
private:    
    QStringList cachedClasses;
};

#endif // KAVICLASSKB_H
