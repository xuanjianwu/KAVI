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

bool igraph_t::degree(QVector<int> &res, QVector<int> vertices, neimode mode)
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

void igraph_t::calNeighborhood(int rootVertex, int currentOrder, QVector<int> &res, neimode mode, QVector<bool> &visited)
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

bool igraph_t::neighborhood(QVector<QVector<int> > &res, QVector<int> vertices, int order, neimode mode)
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

void igraph_t::calComponent(QVector<int> &res, int rootVertex, neimode mode, QVector<bool> &visited)
{
    res.push_back(rootVertex);

    QVector<int> rootNeighbors;
    neighbors(rootNeighbors, rootVertex, mode);

    for (int i = 0; i < rootNeighbors.size(); i++)
    {
        if (visited[rootNeighbors[i]] == false)
        {
            visited[rootNeighbors[i]] = true;
            calComponent(res, rootNeighbors[i], mode, visited);
        }
    }
}

bool igraph_t::subcomponent(QVector<int> &res, int rootVertex, neimode mode)
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

bool igraph_t::neighbors(QVector<int> &res, int rootVertex, neimode mode)
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

QSet<int> igraph_t::getVertices()
{
    return this->vertices;
}

bool igraph_t::isDirected()
{
    return this->directed;
}

int igraph_t::getVertexCount()
{
    return this->nVertex;
}

int igraph_t::getEdgeCount()
{
    return this->nEdge;
}




}
