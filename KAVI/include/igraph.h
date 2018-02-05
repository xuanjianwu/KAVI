/*
 * @author: liYc
 * @date  : 2018/1/23
 * @brief : The diagrams used in KAVI are graphs that contain vertices
 *          and edges. This class is used to manipulate the graph
 *          structures and to analyze them.
*/

#ifndef IGRAPH_H
#define IGRAPH_H

#include <QSet>
#include <QMultiMap>
#include <QVector>
#include <QList>
#include <QtAlgorithms>

namespace KAVIGraph {

typedef enum _neimode {
    IGRAPH_OUT,  // out-degree
    IGRAPH_IN,   // in-degree
    IGRAPH_ALL   // total degree (sum of the in- and out-degree)
} neimode;

class igraph_t {
public:
    /*
    * construct function, default graph is undirected
    * @params: N/A
    * @return: N/A
    */
    igraph_t();

    /*
    * init the graph with vertices
    * @params:
    *       nVertex    - the number of vertices to init graph
    *       directed   - graph's directed or not
    * @return: init success or not
    */
    bool init(int nVertex, bool directed);

    /*
    * deinit the graph and reset
    * @params: N/A
    * @return: N/A
    */
    void deInit();

    /*
    * deconstruct function
    * @params: N/A
    * @return: N/A
    */
    ~igraph_t();

    /*
    * add one edge to graph
    * @params:
    *       sVertex - the start vertex's ID
    *       eVertex - the end vertex's ID
    * @return: add edge success or not
    */
    bool addEdge(int sVertex, int eVertex);

    /*
    * add edges to graph, the edges are given in a vector, the first two
    * elements define the first edge (the order is from , to for directed
    * graphs).The vector should contain even number of integer numbers.
    * @params:
    *       edges - the edges
    * @return: add edges success or not
    */
    bool addEdges(QVector<int>& edges);

    /*
    * remove one edge from graph
    * @params:
    *       sVertex - the start vertex's ID
    *       eVertex - the end vertex's ID
    * @return: remove edge success or not
    */
    bool removeEdge(int sVertex, int eVertex);

    /*
    * This function calculates the in-, out- or total degree of the specified vertices.
    * @params:
    *       res      - Vector will contain the result
    *       vertices - Vector, giving the vertex ids of which the degree will be calculated
    *       mode     - Defines the type of the degree. Valid modes are: IGRAPH_OUT, out-degree;
    *                  IGRAPH_IN, in-degree; IGRAPH_ALL, total degree (sum of the in- and out-degree).
    *                  This parameter is ignored for undirected graphs.
    * @return: success or not
    */
    bool degree(QVector<int>& res, QVector<int> vertices, neimode mode) const;

    /*
    * calculate the neighborhood of a given vertex by Depth-First-Search
    * @params:
    *       rootVertex     - The vertex for which the calculation is performed.
    *       currentOrder   - the current order decreasing from search root
    *       res            - the result of neighborhood
    *       mode           - Specifies how to use the direction of the edges if a directed graph is analyzed
    *       visited        - the flag of vertices, visited or not
    * @return: void
    */
    void calNeighborhood(int rootVertex, int currentOrder, QVector<int>& res, neimode mode, QVector<bool>& visited) const;

    /*
    * get the neighborhood of vertices. The neighborhood of a given order of a vertex
    * includes all vertices which are closer to the vertex than the order. Ie. order 0 is always
    * the vertex itself, order 1 is the vertex plus its immediate neighbors, order 2 is order 1
    * plus the immediate neighbors of the vertices in order 1, etc.
    * @params:
    *       res      - The result of the calculation will be stored here in QVector<int> objects
    *       vertices - The vertices for which the calculation is performed.
    *       order    - Integer giving the order of the neighborhood.
    *       mode     - Specifies how to use the direction of the edges if a directed graph is analyzed.
    *                  For IGRAPH_OUT only the outgoing edges are followed, so all vertices reachable
    *                  from the source vertex in at most order steps are included. For IGRAPH_IN all
    *                  vertices from which the source vertex is reachable in at most order steps are
    *                  included. IGRAPH_ALL ignores the direction of the edges. This argument is ignored
    *                  for undirected graphs.
    * @return: success or not
    */
    bool neighborhood(QVector<QVector<int> >& res, QVector<int> vertices, int order, neimode mode) const;

    /*
    * calculate the vertices in the same component of a given vertex by Depth-First-Search
    * @params:
    *       res        - The result, vector with the ids of the vertices in the same component.
    *       rootVertex - The id of the vertex of which the component is searched.
    *       mode       - Type of the component for directed graphs
    *       visited    - the flag of vertices, visited or not
    * @return: void
    */
    void calComponent(QVector<int>& res, int rootVertex, neimode mode, QVector<bool>& visited) const;

    /*
    * get the vertices in the same component of a given vertex.
    * @params:
    *       res        - The result, vector with the ids of the vertices in the same component.
    *       rootVertex - The id of the vertex of which the component is searched.
    *       mode       - Type of the component for directed graphs, possible values:
    *                    IGRAPH_OUT: the set of vertices reachable from the vertex
    *                    IGRAPH_IN : the set of vertices from which the vertex is reachable.
    *                    IGRAPH_ALL: the graph is considered as an undirected graph. Note that
    *                                this is not the same as the union of the previous two.
    * @return: success or not
    */
    bool subcomponent(QVector<int>& res, int rootVertex, neimode mode) const;

    /*
    * get adjacent vertices to a vertex.
    * @params:
    *       res        - this vector will contain the result.
    *       rootVertex - The id of the node for which the adjacent vertices are to be searched.
    *       mode       - Defines the way adjacent vertices are searched in directed graphs. It can
    *                    have the following values: IGRAPH_OUT, vertices reachable by an edge from the
    *                    specified vertex are searched; IGRAPH_IN, vertices from which the specified vertex
    *                    is reachable are searched; IGRAPH_ALL, both kinds of vertices are searched. This
    *                    parameter is ignored for undirected graphs.
    * @return: success or not
    */
    bool neighbors(QVector<int>& res, int rootVertex, neimode mode) const;

    /*
    * get the set of vertices' ID in graph
    * @params: N/A
    * @return: set of vertices' ID
    */
    QSet<int> getVertices() const;

    /*
    * is the graph directed
    * @params: N/A
    * @return: directed or not
    */
    bool isDirected() const;

    /*
    * get the count of vertices in graph
    * @params: N/A
    * @return: count of vertices
    */
    int getVertexCount() const;

    /*
    * get the count of edges in graph
    * @params: N/A
    * @return: count of edges
    */
    int getEdgeCount() const;
private:
    // number of vertex
    int nVertex;
    // number of edge
    int nEdge;
    // the edge is directed or not, default false
    bool directed;

    // vertex's ID set, start from 0
    QSet<int> vertices;
    // edges's map <start vertex ID, end vertex ID> for directed
    QMultiMap<int, int> edges;
};


// all the following defines are used to simulate the API in
// the "igraph C library--http://igraph.org/c/", so the meanings
// of such defines can be check from the "http://igraph.org/c/doc/ix01.html"

typedef QVector<int> igraph_vector_t;
typedef QVector<QVector<int> > igraph_vector_ptr_t;
typedef int igraph_integer_t;
typedef neimode igraph_neimode_t;

#define IGRAPH_DIRECTED true
#define IGRAPH_UNDIRECTED false
#define IGRAPH_NO_LOOPS false

void igraph_vector_init(igraph_vector_t& vector, igraph_integer_t size);

igraph_integer_t igraph_vector_size(const igraph_vector_t vector);

igraph_integer_t igraph_vector_e(const igraph_vector_t& vector, igraph_integer_t index);

void igraph_vector_destroy(igraph_vector_t& vector);

void igraph_vector_ptr_init(igraph_vector_ptr_t& vector, igraph_integer_t size);

igraph_vector_t igraph_vector_ptr_e(const igraph_vector_ptr_t& vector, igraph_integer_t index);

void igraph_vector_ptr_destroy(igraph_vector_ptr_t& vector);

void igraph_empty(igraph_t& graph, igraph_integer_t vertices,  bool directed);

void igraph_destroy(igraph_t& graph);

void igraph_add_edge(igraph_t& graph, igraph_integer_t sVertex, igraph_integer_t eVertex);

void igraph_add_edges(igraph_t& graph, igraph_vector_t& vector, void* attr);

void igraph_delete_edges(igraph_t& graph, igraph_integer_t sVertex, igraph_integer_t eVertex);

igraph_vector_t igraph_vss_1(const igraph_t& graph, igraph_integer_t id);

igraph_vector_t igraph_vss_all(const igraph_t& graph);

void igraph_degree(const igraph_t& graph, igraph_vector_t& res, igraph_vector_t vids, igraph_neimode_t mode, bool loops);

void igraph_neighborhood(const igraph_t& graph, igraph_vector_ptr_t& res, igraph_vector_t vids, igraph_integer_t order, igraph_neimode_t mode);

bool igraph_is_directed(const igraph_t& graph);

void igraph_subcomponent(const igraph_t& graph, igraph_vector_t& res, igraph_integer_t rootVertex, igraph_neimode_t mode);

void igraph_neighbors(const igraph_t& graph, igraph_vector_t& res, igraph_integer_t rootVertex, igraph_neimode_t mode);

igraph_integer_t igraph_vcount(const igraph_t& graph);

igraph_integer_t igraph_ecount(const igraph_t& graph);

}

#endif // IGRAPH_H
