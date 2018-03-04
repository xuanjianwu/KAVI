/*
 * @author: liYc
 * @date  : 2018/3/4
 * @brief : the class handle the basic fact of planning world such as predicate
*/

#ifndef FACT_H
#define FACT_H

#include "KAVIBase.h"

class Fact {
public:
    Fact(QString formula, int id);

    QString getFormula() const;
    void setFormula(const QString &value);

    int getId() const;
    void setId(int value);

private:
    QString formula;
    int id;
};


#endif // FACT_H
