#ifndef KAVIENUMS_H
#define KAVIENUMS_H

/*
 * specify the shapes of Node
*/
enum NodeShape {
    Rectangle,
    Ellipse,
    Undefined
};

/*
 * specify content types of node structure
*/
enum ContentType {
    nodeType,           // specify the node's type
    nodeLabel,          // specify the node's label
    nodePosition,       // specify the node's position
    nodePredicateSet,   // specify the node's operator predicate set
    nodeState,          // specify the node's task state
    nodeClass           // specify the node's class
};

/*
 * specify the shapes of EdgePoint
*/
enum EdgePointShape {
    Point,
    ArrowPos,
    ArrowNeg
};

#endif // KAVIENUMS_H
