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
    State(QSet<QString> facts);

    void changeState();

    QSet<QString> getFacts() const;
    void setFacts(const QSet<QString> &value);

    QSet<QString> getIncludeFacts() const;
    void setIncludeFacts(const QSet<QString> &value);

    QSet<QString> getExcludeFacts() const;
    void setExcludeFacts(const QSet<QString> &value);

private:
    QSet<QString> facts;
    QSet<QString> includeFacts;
    QSet<QString> excludeFacts;
};

#endif // STATE_H
