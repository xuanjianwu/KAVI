/*
 * @author: liYc
 * @date  : 2018/3/4
 * @brief : This class implements a plan. For each action we store a unique
 *          action ID - implemented as HashMap<Integer, PlanAction>
*/

#ifndef PLAN_H
#define PLAN_H

#include "KAVIBase.h"
#include "PlanAction.h"

class Plan {
public:
    Plan();

    void addAction(PlanAction action);
    void insertAction(int actionId, PlanAction action);
    bool removeAction(int actionId);

    int getActionId(PlanAction action) const;
    PlanAction getAction(int actionId) const;

    int getActionId() const;
    void setActionId(int value);

    // get the actions' count
    int getPlanSize() const;

    QMap<int, PlanAction> getActions() const;
    void setActions(const QMap<int, PlanAction> &value);

    int getInterruptActionId() const;
    void setInterruptActionId(int value);

private:
    // the new actions's id to add
    int actionId;

    // sequence of actions
    QMap<int, PlanAction> actions;

    // the flawed action's id
    int interruptActionId;
};

#endif // PLAN_H
