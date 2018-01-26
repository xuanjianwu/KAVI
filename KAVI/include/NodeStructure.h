/*
 * @author: liYc
 * @date  : 2018/1/22
 * @brief : define Node structure base on Node, adding
 *          the user application's infomation
*/

#ifndef NODESTRUCTURE_H
#define NODESTRUCTURE_H

#include "KAVIBase.h"
#include "Node.h"

KAVI_NS_BEGIN

struct NodeStructure {
    /*
     * construct function
     * @params: N/A
     * @return: N/A
    */
    NodeStructure();

    /*
     * copy construct function
     * @params:
     *      original - target to copy from
     * @return: N/A
    */
    NodeStructure(const NodeStructure& original);

    /*
     * get the content data of node structure
     * @params:
     *      type - the content type filter to read data
     * @return: the specified type data
    */
    QVariant readData(ContentType type) const;

    /*
     * set the specified type data of node structure
     * @params:
     *      type - the content type filter to set data
     *      data - target data
     * @return: void
    */
    void setData(ContentType type, QVariant data);

    /*
     * get the node's shape
     * @params: N/A
     * @return: NodeShape
    */
    NodeShape shape() const;

    /*
     * get the node's color
     * @params: N/A
     * @return: QColor
    */
    QColor color() const;

private:
    // specify the content list of node structure
    QHash<ContentType, QVariant> content;
};

KAVI_NS_END

#endif // NODESTRUCTURE_H
