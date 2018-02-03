/*
 * @author: liYc
 * @date  : 2018/1/23
 * @brief : this graphclass is mainly used to offer utilities
 *          used on the graph structures in KAVI
*/

#ifndef GRAPHCLASS_H
#define GRAPHCLASS_H

#include "KAVIBase.h"
#include "igraph.h"

KAVI_NS_BEGIN

using namespace KAVIGraph;

class GraphClass {
public:
    /*
    * construct function, set the graphData and init an empty graph
    * @params:
    *       diagram - the diagarm element to construct graphclass
    * @return: N/A
    */
    GraphClass(QDomElement diagram);

    /*
    * deconstruct function, destory the graph
    * @params: N/A
    * @return: N/A
    */
    ~GraphClass();

    /*
    * init the graph with diagram element data
    * @params:
    *       nodeMask - the expected node type mask
    *       edgeMask - the expected edge type mask
    *       directed - is the graph directed?
    * @return: bool
    */
    bool init(char nodeMask, char edgeMask, bool directed);

    /*
    * does graph contains cycle?
    * @params: N/A
    * @return: bool
    */
    bool containsCycle();

    /*
    * count the expected mode's edge of node
    * @params:
    *       nodeID - the expected node's ID
    *       mode   - neimode to count
    * @return:  count
    */
    int edgeCnt(int nodeID, igraph_neimode_t mode);

    /*
    * get Node's label
    * @params:
    *       nodeID - the expected node's ID
    * @return: label
    */
    QString nodeLabel(int nodeID) const;

    /*
    * get Node's partent ID
    * @params:
    *       nodeID - the expected node's ID
    * @return: ID of partent
    */
    int nodeParentID(int nodeID) const;

    /*
    * add edge between startID and endID
    * @params:
    *       startID - start point of edge
    *       endID   - end point of edge
    * @return: void
    */
    void addEdge(int startID,int endID);

    /*
    * remove edge between startID and endID
    * @params:
    *       startID - start point of edge
    *       endID   - end point of edge
    * @return: void
    */
    void removeEdge(int startID, int endID);

    /*
    * get the path to root from current start node
    * @params:
    *       pathStart - the start node's ID
    * @return: all the node's label in the path to root
    */
    QStringList pathToRoot(int pathStart) const;

    /*
    * get children nodes of parent
    * @params:
    *       parent - parent node' ID
    * @return: childrens' ID
    */
    QList<int> childrenNodes(int parent);

    /*
    * get all the descendants of parent
    * @params:
    *       parent - parent node' ID
    * @return: descendants's ID
    */
    QSet<int> getDescendants(int parent);

    /*
    * print infomation of current graph
    * @params: N/A
    * @return: N/A
    */
    void print();
private:
    /*
    * get the root vertices'ID with zero in-degree in all graph's connected components
    * @params: N/A
    * @return: root vertices'ID of connected components
    */
    QList<int> selectComponentRootNodes();

    /*
    * get the adjacent vertices from parent
    * @params:
    *       parent - parent node's ID
    * @return: adjacent vertices's ID
    */
    QList<int> selectNeighbours(int parent);

    // the diagram of graphclass
    QDomElement graphData;

    // mapping XML Node'ID to graph vertex'ID
    QMap<int, int> idMap;

    // the graph instance of graphclass
    igraph_t graph;

    // does directed graph have cycle
    bool directedHasCycle;
};

KAVI_NS_END

#endif // GRAPHCLASS_H
