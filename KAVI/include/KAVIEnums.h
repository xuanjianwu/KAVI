/*
 * @author: liYc
 * @date  : 2018/1/22
 * @brief : define enums of KAVI
*/

#ifndef KAVIENUMS_H
#define KAVIENUMS_H

/*
 * specify the shapes of Node
*/
typedef enum _NodeShape {
    Rectangle,
    Ellipse,
    Undefined
} NodeShape;

/*
 * specify content types of node structure
*/
typedef enum _ContentType {
    nodeType,           // specify the node's type
    nodeLabel,          // specify the node's label
    nodePosition,       // specify the node's position
    nodePredicateSet,   // specify the node's operator predicate set
    nodeState,          // specify the node's task state
    nodeClass           // specify the node's class
} ContentType;

/*
 * specify the shapes of EdgePoint
*/
typedef enum _EdgePointShape {
    Point,
    ArrowPos,
    ArrowNeg
} EdgePointShape;

/*
 * specify the orientations of Edge
*/
typedef enum _EdgeOrientation {
    Forward,
    Backward,
    NoOrientation
} EdgeOrientation;

#endif // KAVIENUMS_H
