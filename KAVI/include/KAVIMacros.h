/*
 * @author: liYc
 * @date  : 2018/1/22
 * @brief : define macros of KAVI
*/

#ifndef KAVIMACROS_H
#define KAVIMACROS_H

// namespace------------------------------

// namespace KAVI begin
#define KAVI_NS_BEGIN namespace KAVI {

// namespace KAVI end
#define KAVI_NS_END }

// namespace CheckUtils begin
#define CU_NS_BEGIN namespace CheckUtils {

// namespace CheckUtils end
#define CU_NS_END }

// namespace XMLUtils begin
#define XMLU_NS_BEGIN namespace XMLUtils {

// namespace XMLUtils end
#define XMLU_NS_END }

// KAVIMainWindow ------------------------

#define VIZ_RECT_NODE 'R'
#define VIZ_ELLIPSE_NODE 'E'

#define VIZ_LANG_DEF 'L'
#define VIZ_ACTION_DEF 'A'
#define VIZ_TASK_DEF 'T'

// common --------------------------------

#define INVALID_ID -1

// Node ----------------------------------

#define INVALID_NODE_ID -1
#define NODE_Z_VALUE 3
#define NODE_PADDING 10
#define NODE_LAYER_CNT 3
#define NODE_LAYER_WIDTH 5

// NodeStructure -------------------------

#define MAX_LABEL_LENGTH 20

// node colors
#define NC_CLASS "#F8F06F"
#define NC_VARIABLE "#B1FB3C"
#define NC_OBJECT "#4ADADC"
#define NC_PREDICATE "#F7AE22"

// state colors
#define NC_INIT_PREDICATE "#75F4AB"
#define NC_GOAL_PREDICATE "#FA8C80"
#define NC_DEFAULT "#FFFFFF"

// predicate layer colors
#define LC_PRECOND_POS "#3CDC84"
#define LC_PRECOND_NEG "#808080"
#define LC_EFFECT_POS "#0F74DC"
#define LC_EFFECT_NEG "#FF5753"
#define LC_DEFAULT "#FFFFFF"

// defaul node structure ID
#define NS_DEFAULT_ID		-1

// undefined node structure
#define NS_UNDEF		0	/*0000 0000*/

// node structure type
#define NST_CLASS		(char)1		/*0000 0001*/
#define NST_PREDICATE		(char)2		/*0000 0010*/
#define NST_VARIABLE		(char)4		/*0000 0100*/
#define NST_OBJECT		(char)8		/*0000 1000*/
#define NST_ALL_TYPES		(char)15	/*0000 1111*/
#define NST_UNKNOWN		(char)240	/*1111 0000*/

// node structure predicate set
#define NSPS_PRECOND_POS	"precond+"
#define NSPS_PRECOND_NEG	"precond-"
#define NSPS_EFFECT_POS		"effect+"
#define NSPS_EFFECT_NEG		"effect-"

// node structure task predicate set
#define NSTS_INIT		"init"
#define NSTS_GOAL		"goal"

// EdgePoint -----------------------------

#define EDGEPOINT_Z_VALUE 6
#define EDGEPOINT_SIZE 6
#define EDGEPOINT_ARROW_SIZE (EDGEPOINT_SIZE+4)
#define ARROW_SIZE 15

// Edge ----------------------------------

//const qreal Pi = 3.14159;
//const double Pi = 3.14159;
#define EDGE_Z_VALUE 5
#define EDGE_LINE_WIDTH 3

// Edge direction
#define EDGE_START 	true
#define EDGE_END	false

// EdgeStructure -------------------------

// Edge purpose
#define DEP_INHERITANCE 2	/* 0000 0010 */
#define DEP_ASSOCIATION 1	/* 0000 0001 */
#define DEP_NO_PURPOSE	0	/* 0000 0000 */
#define DEP_UNKNOWN	252	/* 1111 1100 */

// Edge colors
#define EC_ASSOCIATION "#5E5F76"
#define EC_INHERITANCE "#1386CF"

// TreeModel -----------------------------
#define LABEL_COLUMN 0
#define ID_COLUMN 1
#define COLUMN_CNT 1

// DataWidget ----------------------------

// node types
#define NT_CLASS "class"
#define NT_PREDICATE "predicate"
#define NT_VARIABLE "variable"
#define NT_OBJECT "object"

// edge purposes
#define EP_ASSOCIATION "association"
#define EP_INHERITANCE "inheritance"

// predicate set names
#define PS_PRECOND_POS "precondition+"
#define PS_PRECOND_NEG "precondition-"
#define PS_EFFECT_POS "effect+"
#define PS_EFFECT_NEG "effect-"

// DiagramWidget -------------------------

#define TEMP_LINE_COLOR "#00FF00"
#define TEMP_LINE_WIDTH 3
#define TEMP_LINE_Z_VALUE 7

// EditWidget ----------------------------

#define INVALID_ARGN 0

// the size of diagram scene
#define DIAGRAM_SIZE 1000

// DrivenEditWidget ----------------------

#define NO_POSITION (-1)

// DefinitionPredicateInfo ---------------

// the column index of class
#define DE_CLASS_COLUMN 0
// the count of columns in the table
#define DE_COLUMN_COUNT 1

// OperatorsPredicateInfo ----------------

// the column index of class
#define OP_CLASS_COLUMN 0
// the column index of variable
#define OP_VARIABLE_COLUMN 1
// the count of columns in the table
#define OP_COLUMN_COUNT 2
// the default argument variable
#define OP_EMPTY_ARGUMENT "EMPTY"

// ProblemsPredicateInfo -----------------

// the column index of class
#define PR_CLASS_COLUMN 0
// the column index of object
#define PR_OBJECT_COLUMN 1
// the count of columns in the table
#define PR_COLUMN_COUNT 2
// the default argument object
#define PR_EMPTY_ARGUMENT "EMPTY"

// DomainDescDialog ----------------------

// the max length of domain's name
#define MAX_DOMAIN_NAME_LENGTH (int)50

// Convertor -----------------------------

// the output indent configure
#define OUTPUT_INDENT 4

// KAVIMainWindow ------------------------

#define MAX_NAME_LENGTH 30

#define KAVI_EXAMPLES_FOLDER_RELEASE "/KAVIExamples/"
#define KAVI_EXAMPLES_FOLDER_DEBUG "/KAVI/KAVIExamples/"

#define KAVI_PNG_FOLDER_RELEASE "/KAVIPNG/"
#define KAVI_PNG_FOLDER_DEBUG "/KAVI/KAVIPNG/"

// the margin for saving PNG
#define IMAGE_MARGIN 20

// the default indexs of tabWidget
#define TAB_INDEX_DEFINITION 0
#define TAB_INDEX_OPERATORS	 1
#define TAB_INDEX_PROBLEMS	 2

// main ----------------------------------

// the prefix of message that would not append to logTextEdit
#define PREFIX '$'

// the prefix of message that would append to logTextEdit
#define KAVIPREFIX '@'

// KAVIKB --------------------------------

#define KB_DIR_RELEASE "/KAVIKBFile/"
#define KB_DIR_DEBUG "/KAVI/KAVIKBFile/"
#define CLASSKB_FILE "classKB.xml"
#define PREDICATEKB_FILE "predicateKB.xml"

// KnowledgeBaseEdit ---------------------

// the default index of tabWidget
#define TAB_INDEX_CLASS 0
#define TAB_INDEX_PREDICATE	 1

// SolutionSettingsDialog ----------------

#define PLANNERS_CONFIGS_DIR_RELEASE "/KAVIConfigs/planners/"
#define PLANNERS_CONFIGS_DIR_DEBUG "/KAVI/KAVIConfigs/planners/"
#define KAVIPLANNERS_FILE "KAVIPlanners.xml"

#define PLANNER_PARAMETERS_TABLE_COLUMN_COUNT 5

// ExecPlanner ---------------------------

#define KAVI_PLANNERS_FOLDER "KAVIPlanners/"
#define PLANNERS_DIR_RELEASE "/"
#define PLANNERS_DIR_DEBUG "/KAVI/"
#define DEFAULTPLAN_FILE "DefaultPlan.xml"
#define TEMPPLAN_FILE "TempPlan.xml"
#define PLANNER_GENERATED_FILE_DIR "/"

// PlanningDialog ------------------------

#define KAVI_PDDL_DIR_RELEASE "/KAVIPDDL/"
#define KAVI_PDDL_DIR_DEBUG "/KAVI/KAVIPDDL/"
#define DEFAULT_DOMAIN_PDDL_FILE "DefaultDomain.pddl"
#define DEFAULT_PROBLEM_PDDL_FILE "DefaultProblem.pddl"
#define DEFAULT_PLAN_FILE "DefaultPlan.txt"

#define KAVI_PARSERS_FOLDER "KAVIParsers/"
#define KAVI_VALIDATORS_FOLDER "KAVIValidators/"
#define VALIDATORS_CONFIGS_DIR_RELEASE "/KAVIConfigs/validators/"
#define VALIDATORS_CONFIGS_DIR_DEBUG "/KAVI/KAVIConfigs/validators/"
#define KAVIVALIDATORS_FILE "KAVIValidators.xml"

#endif // KAVIMACROS_H
