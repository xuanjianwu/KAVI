/*
 * @author: liYc
 * @date  : 2018/3/4
 * @brief : This class represents the state of the world during plan execution.
*/

#ifndef STATE_H
#define STATE_H

#include "KAVIBase.h"
#include "Fact.h"

class State{
public:
    State();

    QSet<QString> getIncludeFacts() const;
    void setIncludeFacts(const QSet<QString> &value);

    QSet<QString> getExcludeFacts() const;
    void setExcludeFacts(const QSet<QString> &value);

    QSet<QString> getBeforeActionFacts() const;
    void setBeforeActionFacts(const QSet<QString> &value);

    QSet<QString> getAfterActionFacts() const;
    void setAfterActionFacts(const QSet<QString> &value);

private:
    QSet<QString> beforeActionFacts;
    QSet<QString> afterActionFacts;
    QSet<QString> includeFacts;
    QSet<QString> excludeFacts;
};

#endif // STATE_H
