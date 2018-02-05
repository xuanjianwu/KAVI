#include "igraph.h"

namespace KAVIGraph {

igraph_t::igraph_t()
{
    this->nVertex = 0;
    this->nEdge = 0;
    this->directed = false;
}

bool igraph_t::init(int nVertex, bool directed)
{
    if (nVertex < 0)
    {
        return false;
    }

    deInit();

    this->nVertex = nVertex;
    this->directed = directed;

    for(int i = 0; i < this->nVertex; i++) {
        this->vertices.insert(i);
    }
    return true;
}

void igraph_t::deInit()
{
    this->nVertex = 0;
    this->nEdge = 0;
    this->directed = false;

    this->vertices.clear();
    this->edges.clear();
}

igraph_t::~igraph_t()
{
    deInit();
}

bool igraph_t::addEdge(int sVertex, int eVertex)
{
    /*if (sVertex < 0 || sVertex >= this->nVertex
            || eVertex < 0 || eVertex >= this->nVertex)
    {
        return false;
    }*/

    if ( ( !this->vertices.contains(sVertex)) ||
         ( !this->vertices.contains(eVertex)) )
    {
        return false;
    }

    if (!this->edges.contains(sVertex, eVertex))
    {
        this->edges.insert(sVertex, eVertex);
        this->nEdge++;
        return true;
    }
    else
    {
        // the expected edge already exists, return true
        return true;
    }
}

bool igraph_t::addEdges(QVector<int> &edges)
{
    // The vector should contain even number of integer numbers between zero and
    // the number of vertices in the graph minus one (inclusive)
    if (edges.size() % 2 != 0)
    {
        return false;
    }
    for (int i = 0; i < edges.size() / 2; i++) {
        if ( !addEdge(edges[2 * i], edges[2 * i + 1]) )
        {
            return false;
        }
    }
    return true;
}

bool igraph_t::removeEdge(int sVertex, int eVertex)
{
    if ( ( !this->vertices.contains(sVertex)) ||
         ( !this->vertices.contains(eVertex)) )
    {
        return false;
    }
    if (this->edges.contains(sVertex, eVertex))
    {
        this->edges.remove(sVertex, eVertex);
        this->nEdge--;
        return true;
    }
    else
    {
        // the expected edge already not exists, return true
        return true;
    }
}

bool igraph_t::degree(QVector<int> &res, QVector<int> vertices, neimode mode) const
{
    for (int i = 0; i < vertices.size(); i++)
    {
        if ( !this->vertices.contains(vertices[i]))
        {
            return false;
        }
    }

    res.clear();
    res.resize(vertices.size());

    QList<int> startVertices = this->edges.keys();
    QList<int> endVertices = this->edges.values();

    if (this->directed)
    {
        switch (mode) {
        case IGRAPH_IN:
            for (int i = 0; i < vertices.size(); i++)
            {
                res[i] = endVertices.count(vertices[i]);
            }
            break;

        case IGRAPH_OUT:
            for (int i = 0; i < vertices.size(); i++)
            {
                res[i] = startVertices.count(vertices[i]);
            }
            break;

        case IGRAPH_ALL:
            for (int i = 0; i < vertices.size(); i++)
            {
                res[i] = startVertices.count(vertices[i]) + endVertices.count(vertices[i]);
            }

            break;

        default:
            return false;
            break;
        }
        return true;
    }
    else
    {
        for (int i = 0; i < vertices.size(); i++)
        {
            res[i] = startVertices.count(vertices[i]) + endVertices.count(vertices[i]);
        }
        return true;
    }
}

void igraph_t::calNeighborhood(int rootVertex, int currentOrder, QVector<int> &res, neimode mode, QVector<bool> &visited) const
{
    res.push_back(rootVertex);
    visited[rootVertex] = true;

    if (currentOrder == 0)
    {
        return;
    }
    QVector<int> rootNeighbors;
    neighbors(rootNeighbors, rootVertex, mode);

    for (int i = 0; i < rootNeighbors.size(); i++)
    {
        if (visited[rootNeighbors[i]] == false)
        {
            calNeighborhood(rootNeighbors[i], currentOrder-1, res, mode, visited);
        }
    }
}

bool igraph_t::neighborhood(QVector<QVector<int> > &res, QVector<int> vertices, int order, neimode mode) const
{
    for (int i = 0; i < vertices.size(); i++)
    {
        if ( !this->vertices.contains(vertices[i]))
        {
            return false;
        }
    }

    res.clear();
    res.resize(vertices.size());

    for (int i = 0; i < vertices.size(); i++)
    {
        QVector<int> tmpRes;
        QVector<bool> visited;
        visited.resize(this->nVertex);
        for (int j = 0; j < this->nVertex; j++)
            visited[j] = false;

        calNeighborhood(vertices[i], order, tmpRes, mode, visited);
        res[i] = tmpRes;
    }
    return true;
}

void igraph_t::calComponent(QVector<int> &res, int rootVertex, neimode mode, QVector<bool> &visited) const
{
    res.push_back(rootVertex);
    visited[rootVertex] = true;

    QVector<int> rootNeighbors;
    neighbors(rootNeighbors, rootVertex, mode);

    for (int i = 0; i < rootNeighbors.size(); i++)
    {
        if (visited[rootNeighbors[i]] == false)
        {
            //visited[rootNeighbors[i]] = true;
            calComponent(res, rootNeighbors[i], mode, visited);
        }
    }
}

bool igraph_t::subcomponent(QVector<int> &res, int rootVertex, neimode mode) const
{
    if ( !this->vertices.contains(rootVertex) )
    {
        return false;
    }

    res.clear();

    QVector<bool> visited;
    visited.resize(this->nVertex);
    for (int j = 0; j < this->nVertex; j++)
        visited[j] = false;

    calComponent(res, rootVertex, mode, visited);

    return true;
}

bool igraph_t::neighbors(QVector<int> &res, int rootVertex, neimode mode) const
{
    if ( !this->vertices.contains(rootVertex) )
    {
        return false;
    }

    QList<int> startVertices = this->edges.keys(rootVertex);
    QList<int> endVertices = this->edges.values(rootVertex);

    if (this->directed)
    {
        switch (mode) {
        case IGRAPH_IN:
            for (int i = 0; i < startVertices.size(); i++)
            {
                res.push_back(startVertices.at(i));
            }
            break;

        case IGRAPH_OUT:
            for (int i = 0; i < endVertices.size(); i++)
            {
                res.push_back(endVertices.at(i));
            }
            break;

        case IGRAPH_ALL:
            for (int i = 0; i < startVertices.size(); i++)
            {
                res.push_back(startVertices.at(i));
            }
            for (int i = 0; i < endVertices.size(); i++)
            {
                res.push_back(endVertices.at(i));
            }
            break;

        default:
            return false;
            break;
        }
        return true;
    }
    else
    {
        for (int i = 0; i < startVertices.size(); i++)
        {
            res.push_back(startVertices.at(i));
        }
        for (int i = 0; i < endVertices.size(); i++)
        {
            res.push_back(endVertices.at(i));
        }
        return true;
    }
}

QSet<int> igraph_t::getVertices() const
{
    return this->vertices;
}

bool igraph_t::isDirected() const
{
    return this->directed;
}

int igraph_t::getVertexCount() const
{
    return this->nVertex;
}

int igraph_t::getEdgeCount() const
{
    return this->nEdge;
}


void igraph_vector_init(igraph_vector_t &vector, igraph_integer_t size) {
    vector.resize(size);
}

igraph_integer_t igraph_vector_size(const igraph_vector_t vector) {
    return vector.size();
}

igraph_integer_t igraph_vector_e(const igraph_vector_t &vector, igraph_integer_t index) {
    return vector[index];
}

void igraph_vector_destroy(igraph_vector_t &vector) {
    vector.clear();
}

void igraph_vector_ptr_init(igraph_vector_ptr_t &vector, igraph_integer_t size) {
    vector.resize(size);
}

igraph_vector_t igraph_vector_ptr_e(const igraph_vector_ptr_t &vector, igraph_integer_t index) {
    return vector[index];
}

void igraph_vector_ptr_destroy(igraph_vector_ptr_t &vector) {
    for (igraph_vector_t v : vector) {
        v.clear();
    }
    vector.clear();
}

void igraph_empty(igraph_t &graph, igraph_integer_t vertices, bool directed) {
    graph.init(vertices, directed);
}

void igraph_destroy(igraph_t &graph) {
    graph.deInit();
}

void igraph_add_edge(igraph_t &graph, igraph_integer_t sVertex, igraph_integer_t eVertex) {
    graph.addEdge(sVertex, eVertex);
}

void igraph_add_edges(igraph_t &graph, igraph_vector_t &vector, void *attr) {
    Q_UNUSED(attr);
    graph.addEdges(vector);
}

void igraph_delete_edges(igraph_t &graph, igraph_integer_t sVertex, igraph_integer_t eVertex) {
    graph.removeEdge(sVertex, eVertex);
}

igraph_vector_t igraph_vss_1(const igraph_t &graph, igraph_integer_t id) {
    igraph_vector_t res;
    QSet<int> tmp = graph.getVertices();
    if (tmp.find(id) != tmp.end()) {
        res.push_back(id);
    }
    qSort(res.begin(), res.end());
    return res;
}

igraph_vector_t igraph_vss_all(const igraph_t &graph) {
    igraph_vector_t res;
    QSet<int> tmp = graph.getVertices();
    QSet<int>::iterator iter;
    for (iter = tmp.begin(); iter != tmp.end(); iter++) {
        res.push_back(*iter);
    }
    qSort(res.begin(), res.end());
    /*
    for (int i = 0; i < res.size(); i++)
        qWarning() << res[i];
        */
    return res;
}

void igraph_degree(const igraph_t &graph, igraph_vector_t &res, igraph_vector_t vids, igraph_neimode_t mode, bool loops) {
    Q_UNUSED(loops);
    graph.degree(res, vids, mode);
}

void igraph_neighborhood(const igraph_t &graph, igraph_vector_ptr_t &res, igraph_vector_t vids, igraph_integer_t order, igraph_neimode_t mode) {
    graph.neighborhood(res, vids, order, mode);
}

bool igraph_is_directed(const igraph_t &graph) {
    return graph.isDirected();
}

void igraph_subcomponent(const igraph_t &graph, igraph_vector_t &res, igraph_integer_t rootVertex, igraph_neimode_t mode) {
    graph.subcomponent(res, rootVertex, mode);
}

void igraph_neighbors(const igraph_t &graph, igraph_vector_t &res, igraph_integer_t rootVertex, igraph_neimode_t mode) {
    graph.neighbors(res, rootVertex, mode);
}

igraph_integer_t igraph_vcount(const igraph_t &graph) {
    return graph.getVertexCount();
}

igraph_integer_t igraph_ecount(const igraph_t &graph) {
    return graph.getEdgeCount();
}




}
