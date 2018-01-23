/*
 * @author: liYc
 * @date  : 2018/1/23
 * @brief : The diagrams used in KAVI are graphs that contain vertices
 *          and edges. This class is used to manipulate the graph
 *          structures and to analyze them.
*/

#ifndef IGRAPH_H
#define IGRAPH_H

#include <list>
#include <vector>

namespace KAVIGraph {

using std::vector;
using std::list;

enum neimode {
    IGRAPH_OUT,  // out-degree
    IGRAPH_IN,   // in-degree
    IGRAPH_ALL   // total degree (sum of the in- and out-degree)
};

class graph {
public:
    graph();
    void init(int vertices, bool directed);
    ~graph();
    void addEdge(int sVertex, int eVertex);
    void addEdges(vector<int>& edges);
    void removeEdge(int sVertex, int eVertex);
    void degree(vector<int>& res, vector<int> vertices, neimode mode);
    void neighborhood(vector<vector<int> >& res, vector<int> vertices, int order, neimode mode);
    void subcomponent(vector<int>& res, int rootVertex, neimode mode);
    void neighbors(vector<int>& res, int rootVertex, neimode mode);
private:
    // number of vertex
    int vertices;
    // number of edge
    int edges;
    // adjacency list
    vector<list<int> > adjacency;
    bool directed;
};

}

#endif // IGRAPH_H
