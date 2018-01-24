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
#include <set>

namespace KAVIGraph {

using std::vector;
using std::list;
using std::set;

typedef enum _neimode {
    IGRAPH_OUT,  // out-degree
    IGRAPH_IN,   // in-degree
    IGRAPH_ALL   // total degree (sum of the in- and out-degree)
} neimode;

class igraph_t {
public:
    igraph_t();
    void init(int vertices, bool directed);
    void deInit();
    ~igraph_t();
    void addEdge(int sVertex, int eVertex);
    void addEdges(vector<int>& edges);
    void removeEdge(int sVertex, int eVertex);
    void degree(vector<int>& res, vector<int> vertices, neimode mode);
    void neighborhood(vector<vector<int> >& res, vector<int> vertices, int order, neimode mode);
    void subcomponent(vector<int>& res, int rootVertex, neimode mode);
    void neighbors(vector<int>& res, int rootVertex, neimode mode);
    set<int> getVertices();
    bool isDirected();
    int getVertexCount();
    int getEdgeCount();
private:
    // number of vertex
    int nVertex;
    // vertex's ID set
    set<int> vertices;
    // number of edge
    int nEdge;
    // adjacency list
    vector<list<int> > adjacency;
    bool directed;
};

typedef vector<int> igraph_vector_t;
typedef vector<vector<int> > igraph_vector_ptr_t;
typedef int igraph_integer_t;
typedef neimode igraph_neimode_t;

void igraph_vector_init(igraph_vector_t& vector, igraph_integer_t size) {
    vector.resize(size);
}

igraph_integer_t igraph_vector_size(igraph_vector_t vector) {
    return vector.size();
}

igraph_integer_t igraph_vector_e(igraph_vector_t& vector, igraph_integer_t index) {
    return vector[index];
}

void igraph_vector_destroy(igraph_vector_t& vector) {
    vector.clear();
}

void igraph_vector_ptr_init(igraph_vector_ptr_t& vector, igraph_integer_t size) {
    vector.resize(size);
}

igraph_vector_t igraph_vector_ptr_e(igraph_vector_ptr_t& vector, igraph_integer_t index) {
    return vector[index];
}

void igraph_vector_ptr_destroy(igraph_vector_ptr_t& vector) {
    for (igraph_vector_t v : vector) {
        v.clear();
    }
    vector.clear();
}

void igraph_empty(igraph_t& graph, igraph_integer_t vertices,  bool directed) {
    graph.init(vertices, directed);
}

void igraph_destroy(igraph_t& graph) {
    graph.deInit();
}

void igraph_add_edge(igraph_t& graph, igraph_integer_t sVertex, igraph_integer_t eVertex) {
    graph.addEdge(sVertex, eVertex);
}

void igraph_add_edges(igraph_t& graph, igraph_vector_t& vector, void* attr) {
    graph.addEdges(vector);
}

void igraph_delete_edges(igraph_t& graph, igraph_integer_t sVertex, igraph_integer_t eVertex) {
    graph.removeEdge(sVertex, eVertex);
}

igraph_vector_t igraph_vss_1(igraph_t& graph, igraph_integer_t id) {
    igraph_vector_t res;
    set<int> tmp = graph.getVertices();
    if (tmp.find(id) != tmp.end()) {
        res.push_back(id);
    }
    return res;
}

igraph_vector_t igraph_vss_all(igraph_t& graph) {
    igraph_vector_t res;
    set<int> tmp = graph.getVertices();
    set<int>::iterator iter;
    for (iter = tmp.begin(); iter != tmp.end(); iter+++) {
        res.push_back(*iter);
    }
    return res;
}

void igraph_degree(igraph_t& graph, igraph_vector_t& res, igraph_vector_t vids, igraph_neimode_t mode, bool loops) {
    graph.degree(res, vids, mode);
}

void igraph_neighborhood(igraph_t& graph, igraph_vector_ptr_t& res, igraph_vector_t vids, igraph_integer_t order, igraph_neimode_t mode) {
    graph.neighborhood(res, vids, order, mode);
}

bool igraph_is_directed(igraph_t& graph) {
    return graph.isDirected();
}

void igraph_subcomponent(igraph_t& graph, igraph_vector_t& res, igraph_integer_t rootVertex, igraph_neimode_t mode) {
    graph.subcomponent(res, rootVertex, mode);
}

void igraph_neighbors(igraph_t& graph, igraph_vector_t& res, igraph_integer_t rootVertex, igraph_neimode_t mode) {
    graph.neighbors(res, rootVertex, mode);
}

igraph_integer_t igraph_vcount(igraph_t& graph) {
    graph.getVertexCount();
}

igraph_integer_t igraph_ecount(igraph_t& graph) {
    graph.getEdgeCount();
}

}

#endif // IGRAPH_H
