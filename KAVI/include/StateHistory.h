/*
 * @author: liYc
 * @date  : 2018/3/4
 * @brief : the class maintains the change history of the world state
*/

#ifndef STATEHISTORY_H
#define STATEHISTORY_H

#include "KAVIBase.h"
#include "State.h"
#include "Plan.h"
#include "PlanAction.h"

class StateHistory {
public:
    StateHistory();

    ~StateHistory();

    void initStateHistoryFromPlan(Plan* plan);

    void clearStateHistory();

    void setPlan(Plan* plan);

    void buildStateHistoryFromPlan();

    QList<State> getStatesList() const;
    void setStatesList(const QList<State> &value);

private:
    QList<State> statesList;

    Plan* plan;
};

#endif // STATEHISTORY_H
