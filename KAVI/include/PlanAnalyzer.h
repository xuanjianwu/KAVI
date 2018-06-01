/*
 * @author: liYc
 * @date  : 2018/2/28
 * @brief : the class handle the analysis of xml plan
*/

#ifndef PLANANALYZER_H
#define PLANANALYZER_H

#include "KAVIBase.h"
#include "XMLUtils.h"
#include "PlanningDialog.h"

class PlanAnalyzer {
public:
    /*
     * generate html report from the plan in xml format
     * @params:
     *      planners - all planners' infomation
     *      xmlPlan  - the plan in xml format
     * @return: html in string
    */
    static QString generateHTMLSinglePlanReport(QDomElement planners, QDomElement xmlPlan);
};

#endif // PLANANALYZER_H
