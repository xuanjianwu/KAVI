#include "GraphClass.h"
#include "XMLUtils.h"
#include "EdgeStructure.h"

KAVI_NS_BEGIN

using namespace XMLUtils;
using namespace KAVIGraph;

GraphClass::GraphClass(QDomElement diagram)
{
    graphData = diagram;
    igraph_empty(&graph, 0, false);
}

GraphClass::~GraphClass()
{
    igraph_destroy(&graph);
}

bool GraphClass::init(char nodeMask, char edgeMask, bool directed)
{
    QList<EdgeDefinition> edgeList = selectEdges(graphData, edgeMask);
    int edgeCnt = edgeList.size();
    if (edgeCnt == 0)
        qDebug() << "$GraphClass::init : no edges in diagram";

    igraph_vector_t edgeVect;
    igraph_vector_init(&edgeVect, edgeCnt * 2);

    QList<int> nodes = selectNodeIDList(graphData, nodeMask);
    if ( nodes.count() == 0 )
    {
        qDebug() << "$GraphClass::init : no nodes in diagram";
        return false;
    }

    igraph_destroy(&graph);
    igraph_empty(&graph, nodes.count(), directed);

    for (int i = 0; i < nodes.count(); ++i)
    {
        // vertex's ID in graph start from 0
        idMap.insert(nodes.at(i), i);
    }

    for (int i = 0; i < edgeCnt; ++i)
    {
        edgeVect[2*i] = idMap.value(edgeList.at(i).first);
        edgeVect[2*i+1] = idMap.value(edgeList.at(i).second);
    }

    igraph_add_edges(&graph, &edgeVect, 0);

    igraph_vector_destroy(&edgeVect);

    directedHasCycle = false;
    return true;
}

bool GraphClass::containsCycle()
{
    selectComponentRootNodes();
    return directedHasCycle;
}

int GraphClass::edgeCnt(int nodeID, igraph_neimode_t mode)
{
    igraph_vector_t res;
    igraph_vector_init(&res, 1);

    igraph_degree(&graph, &res, igraph_vss_1(graph, (igraph_integer_t)idMap.value(nodeID)), mode, IGRAPH_NO_LOOPS);

    return (int)res[0];
}

QString GraphClass::nodeLabel(int nodeID) const
{
    QDomElement node = findSubelementAttr(graphData, "node", "id", QString().setNum(nodeID));

    Q_ASSERT(!node.isNull());

    return subelementTagValue(node, "label");
}

int GraphClass::nodeParentID(int nodeID) const
{
    igraph_integer_t vertID = idMap.value(nodeID, INVALID_ID);

    Q_ASSERT(vertID != INVALID_ID);

    igraph_vector_ptr_t res;
    igraph_vector_ptr_init(&res, 1);

    igraph_neighborhood(&graph, &res, igraph_vss_1(graph, vertID), 1, IGRAPH_IN);

    //igraph_vector_t * parentVect = (igraph_vector_t*) igraph_vector_ptr_e(&res, 0);
    igraph_vector_t parentVect = (igraph_vector_t) igraph_vector_ptr_e(&res, 0);

    if ( igraph_vector_size(parentVect) > 2)
        qWarning() << "$GraphClass::nodeParentID : current node has more than one parent node";

    if (igraph_vector_size(parentVect) < 2 )
    {
        qDebug() << "$GraphClass::nodeParentID : current node has no parent";
        igraph_vector_destroy(parentVect);
        igraph_vector_ptr_destroy(&res);
        return INVALID_ID;
    }

    // the first item in parentVect is the current node itself, get the second item for parent node
    igraph_integer_t parentID = parentVect[1];
    igraph_vector_destroy(parentVect);
    igraph_vector_ptr_destroy(&res);
    return idMap.key((int)parentID);
}

void GraphClass::addEdge(int startID, int endID)
{
    igraph_integer_t startVertex = idMap.value(startID, INVALID_ID);
    igraph_integer_t endVertex = idMap.value(endID, INVALID_ID);

    Q_ASSERT( (startVertex != INVALID_ID) && (endVertex != INVALID_ID) );

    igraph_add_edge(&graph, startVertex, endVertex);
}

void GraphClass::removeEdge(int startID, int endID)
{
    igraph_integer_t startVertex = idMap.value(startID, INVALID_ID);
    igraph_integer_t endVertex = idMap.value(endID, INVALID_ID);

    Q_ASSERT( (startVertex != INVALID_ID) && (endVertex != INVALID_ID) );

    igraph_delete_edges(&graph, startVertex, endVertex);
}

QStringList GraphClass::pathToRoot(int pathStart) const
{
    Q_ASSERT(pathStart != INVALID_ID);

    QStringList result;
    while ( pathStart != INVALID_ID )
    {
        result.append(nodeLabel(pathStart));
        pathStart = nodeParentID(pathStart);
    }

    return result;
}

QList<int> GraphClass::childrenNodes(int parent)
{
    // if parent <= 0, get the root vertices'ID with zero in-degree in all graph's connected components
    if (parent <= 0)
    {
        return selectComponentRootNodes();
    }
    // if parent > 0, select children neighbors
    else
    {
        return selectNeighbours(parent);
    }
}

QSet<int> GraphClass::getDescendants(int parent)
{
    QSet<int> result;

    QList<int> queue = childrenNodes(parent);

    // Breadth-First-Search
    while( !queue.isEmpty() )
    {
        int descID = queue.takeFirst();

        result << descID;

        queue << childrenNodes(descID);
    }

    return result;
}

void GraphClass::print()
{
    qDebug() << "GraphClass::print : node cnt:" << (int)igraph_vcount(&graph)
             << "edge cnt:" << (int)igraph_ecount(&graph) << '\n';
}

QList<int> GraphClass::selectComponentRootNodes()
{
    QList<int> result;
    directedHasCycle = false;

    igraph_integer_t vertexCnt = igraph_vcount(&graph);

    igraph_vector_t list;

    // contains the vertices' ID
    QStack<igraph_integer_t> vertices;

    igraph_integer_t i;

    for (i = 0; i < vertexCnt; ++i)
        vertices.push(i);

    if ( igraph_is_directed(&graph) )
    {
        qDebug() << "$GraphClass::selectComponentRootNodes : select the "
                    "root vertices'ID with zero in-degree of all graph's "
                    "connected components for directed graph";

        // contains the in-degree of vertices
        igraph_vector_t degrees;
        igraph_vector_init(&degrees, vertexCnt);
        igraph_degree(&graph, &degrees, igraph_vss_all(graph), IGRAPH_IN, IGRAPH_NO_LOOPS);

        igraph_integer_t rootVertex;

        // the minimum in-degree of current component
        igraph_integer_t minimum;

        while (!vertices.isEmpty())
        {
            rootVertex = vertices.pop();
            minimum = igraph_vector_e(&degrees, rootVertex);

            igraph_vector_init(&list, 0);
            igraph_subcomponent(&graph, &list, rootVertex, IGRAPH_ALL);

            igraph_integer_t listLength = igraph_vector_size(&list);

            for (i = 0; i < listLength; ++i)
            {
                igraph_integer_t actID = igraph_vector_e(&list, i);

                if (actID == rootVertex)
                    continue;

                igraph_integer_t inputDeg = igraph_vector_e(&degrees, actID);

                // remove the vertex in the same component with rootVertex
                vertices.remove(vertices.indexOf(actID));

                // update the minimum in-degree in current component
                if (inputDeg < minimum)
                {
                    minimum = inputDeg;
                    rootVertex = actID;
                }
            }

            if (minimum == 0)
            {
                qDebug() << "$The root vertex with zero in-degree of current component";
                result.append(idMap.key(rootVertex));
            }
            else
            {
                qWarning() << "$GraphClass::selectComponentRootNodes : graph contains a cycle";
                directedHasCycle = true;
            }

            igraph_vector_destroy(&list);
        }

        igraph_vector_destroy(&degrees);
    }
    else
    {

        qDebug() << "$GraphClass::selectComponentRootNodes : select the random"
                    "root vertices'ID of all graph's connected components for "
                    "undirected graph";
        while (!vertices.isEmpty())
        {
            igraph_integer_t someVertex = vertices.pop();
            result.append(idMap.key(someVertex));

            igraph_vector_init(&list, 0);
            igraph_subcomponent(&graph, &list, someVertex, IGRAPH_ALL);

            for (i = 0; i < igraph_vector_size(&list); ++i)
            {
                igraph_integer_t actID = igraph_vector_e(&list, i);
                if (actID == someVertex)
                    continue;

                vertices.remove(vertices.indexOf(actID));
            }

            igraph_vector_destroy(&list);
        }

    }

    return result;
}

QList<int> GraphClass::selectNeighbours(int parent)
{
    QList<int> result;
    igraph_integer_t i;

    igraph_integer_t parentVertex = idMap.value(parent);

    igraph_vector_t neighbours;
    igraph_vector_init(&neighbours, 0);

    igraph_neighbors(&graph, &neighbours, parentVertex, IGRAPH_OUT);

    for (i = 0; i < igraph_vector_size(&neighbours); ++i)
    {
        igraph_integer_t actID = igraph_vector_e(&neighbours, i);
        result.append(idMap.key(actID));
    }

    igraph_vector_destroy(&neighbours);

    return result;
}



KAVI_NS_END
