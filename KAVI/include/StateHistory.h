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

    /*
     * init state history from plan
     * @params:
     *      plan - the associated plan
     * @return: N/A
    */
    void initStateHistoryFromPlan(Plan* plan);

    void clearStateHistory();

    void setPlan(Plan* plan);

    /*
     * build state history from plan
     * @params: N/A
     * @return: N/A
    */
    void buildStateHistoryFromPlan();

    QList<State> getStatesList() const;
    void setStatesList(const QList<State> &value);

private:
    // list of states
    QList<State> statesList;

    // the associated plan
    Plan* plan;
};

#endif // STATEHISTORY_H
