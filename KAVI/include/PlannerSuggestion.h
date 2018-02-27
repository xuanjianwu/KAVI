/*
 * @author: liYc
 * @date  : 2018/2/27
 * @brief : the class handle the suggestion for the available planners to planning
*/

#ifndef PLANNERSUGGESTION_H
#define PLANNERSUGGESTION_H

#include "KAVIBase.h"



class PlannerSuggestion {
public:
    /*
    * construct function
    * @params: N/A
    * @return: N/A
    */
    PlannerSuggestion();

    /*
    * deconstruct function
    * @params: N/A
    * @return: N/A
    */
    ~PlannerSuggestion();

    /*
    * get suggested planners
    * @params: N/A
    * @return: list of planner elements
    */
    QList<QDomElement> getSuggestedPlanners();

    /*
    * set suggested planners
    * @params:
    *       planners - the specified planners
    * @return: void
    */
    void setSuggestedPlanners(QList<QDomElement> planners);

    /*
    * get discarded planners
    * @params: N/A
    * @return: list of planner elements
    */
    QList<QDomElement> getDiscardedPlanners();

    /*
    * set discarded planners
    * @params:
    *       planners - the specified planners
    * @return: void
    */
    void setDiscardedPlanners(QList<QDomElement> planners);

    /*
    * initial the planners selection according to the requirements' compatibility and operating system's compatibility
    * @params:
    *       domainPDDL      - the specified domain pddl file
    *       plannersElement - the specified planners element
    * @return: void
    */
    void initialPlannerSelection(QFile& domainPDDL, QDomElement plannersElement);

    /*
    * check that if the planner's requirements contains the pddl's requirements or not
    * @params:
    *       plannerRequirementsElements - the planner's requirements
    *       PDDL                        - the specified pddl file
    * @return: contains or not
    */
    bool containsRequirements(QList<QDomElement> plannerRequirementsElements, QFile &PDDL);

    /*
    * get the requirements of the planner Requirements Elements
    * @params:
    *       plannerRequirementsElements - the specified planner Requirements Elements
    * @return: list of requirements string
    */
    QList<QString> getPlannerRequirements(QList<QDomElement> plannerRequirementsElements);

    /*
    * get the requirements of the pddl file
    * @params:
    *       PDDL - the specified pddl file
    * @return: list of requirements string
    */
    QList<QString> getPDDLRequirements(QFile& PDDL);

private:
    /*
    * check the operating system compatibility foe the planner
    * @params:
    *       planner - the specified planner element
    * @return: compatible or not
    */
    bool checkOperatingSystemCompatibility(QDomElement planner);

    /*
    * get the current operating system where KAVI running
    * @params: N/A
    * @return: operating system's name
    */
    QString getOperatingSystem();

    // the suggested planners
    QList<QDomElement> suggestedPlanners;
    // the discarded planner
    QList<QDomElement> discardedPlanners;
};


#endif // PLANNERSUGGESTION_H
