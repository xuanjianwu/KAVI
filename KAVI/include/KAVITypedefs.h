/*
 * @author: liYc
 * @date  : 2018/1/22
 * @brief : define typedefs of KAVI
*/

#ifndef KAVITYPEDEFS_H
#define KAVITYPEDEFS_H

// Node ---------------------------------

/*
 * specify the shapes of Node
*/
typedef enum _NodeShape {
    RectangleShape,
    EllipseShape,
    Undefined
} NodeShape;


// NodeStructure ------------------------

/*
 * specify content types of node structure
*/
typedef enum _ContentType {
    nodeID,             // specify the node's ID
    nodeType,           // specify the node's type
    nodeLabel,          // specify the node's label
    nodePosition,       // specify the node's position
    nodePredicateSet,   // specify the node's operator predicate set
    nodeState,          // specify the node's task state
    nodeClass           // specify the node's class
} ContentType;

// EdgePoint ----------------------------

/*
 * specify the shapes of EdgePoint
*/
typedef enum _EdgePointShape {
    Point,
    ArrowPos,
    ArrowNeg
} EdgePointShape;

// Edge ---------------------------------

/*
 * specify the orientations of Edge
*/
typedef enum _EdgeOrientation {
    Forward,
    Backward,
    NoOrientation
} EdgeOrientation;


// DataWidget ---------------------------

// <connected edge's ID, connected edgepoint's orientation>
//typedef QPair<int, bool> DataEdgepoint;

// InfoPanel ----------------------------

typedef enum _RowMoveDirection {
    MoveUp,
    MoveDown
} RowMoveDirection;

// DiagramWidget -------------------------

typedef enum _DiagramMode {
    InsertLine = 0,
    DetectClick = 1,
    MoveItems = 2
} DiagramMode;

// EditWidget ----------------------------

typedef enum _EditMode {
    Delete,
    AddEdge,
    AddEllipse,
    AddRect
} EditMode;

typedef enum _ChangeCode {
    RectNodeAdded = 1,
    EllipseNodeAdded = 2,
    AssocEdgeAdded = 3,
    InherEdgeAdded = 4,

    RectNodeDeleted = 5,
    EllipseNodeDeleted = 6,
    AssocEdgeDeleted = 7,
    InherEdgeDeleted = 8,

    AssocEdgeReconnected = 9,
    InherEdgeReconnected = 10,

    ArgumentOrderChanged = 11,

    NodeClassChanged = 12,
    NodeLabelChanged = 13,
    PredicateSetChanged = 14,
    PredicateStateChanged = 15
} ChangeCode;

// ProblemEdit ---------------------------

typedef enum _PredicateState {
    InitState,
    GoalState,
    NoState
} PredicateState;

#endif // KAVITYPEDEFS_H
