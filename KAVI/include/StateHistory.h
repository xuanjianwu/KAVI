/*
 * @author: liYc
 * @date  : 2018/3/4
 * @brief : the class maintains the change history of the world state
*/

#ifndef STATEHISTORY_H
#define STATEHISTORY_H

#include "KAVIBase.h"
#include "State.h"

class StateHistory {
public:
    StateHistory();

    void addState(State state);

    QList<State> getStatesList() const;
    void setStatesList(const QList<State> &value);

private:
    QList<State> statesList;
};

#endif // STATEHISTORY_H
