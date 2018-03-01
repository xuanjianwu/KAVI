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
    static QString generateHTMLSinglePlanReport(QDomElement planners, QDomElement xmlPlan);
};

#endif // PLANANALYZER_H
