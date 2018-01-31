/*
 * @author: liYc
 * @date  : 2018/1/23
 * @brief : define the tree node of Qt's class tree view
*/

#ifndef TREEITEM_H
#define TREEITEM_H

#include "KAVIBase.h"

KAVI_NS_BEGIN

class TreeItem {
public:
    /*
     * construct function
     * @params:
     *      data   - tree node data
     *      parent - current tree node's parent
     * @return: N/A
    */
    TreeItem(const QVector<QVariant>& data, TreeItem* parent = 0);

    /*
     * deconstruct function, release all child items
     * @params: N/A
     * @return: N/A
    */
    ~TreeItem();

    /*
     * get the current tree node's child at index number
     * @params:
     *      number - child index at child list
     * @return: pointer to the expected child
    */
    TreeItem* child(int number);

    /*
     * get the count of child list
     * @params: N/A
     * @return: count
    */
    int childCount() const;

    /*
     * get the items' count of one tree node data
     * @params: N/A
     * @return: count
    */
    int columnCount() const;

    /*
     * get the current tree node's data at index column
     * @params:
     *      column - data index at data vector
     * @return: expected data at index column
    */
    QVariant data(int column) const;

    /*
     * insert (count childs) with (columns item)'s data at index position
     * @params:
     *      position - the index to insert
     *      count    - the number of childs to insert
     *      columns  - the number of child's data column
     * @return: bool
    */
    bool insertChildren(int position, int count, int columns);

    /*
     * insert (columns items) at data's index(position) and childs the same
     * @params:
     *      position - the index to insert
     *      columns  - the number of child's data column
     * @return: bool
    */
    bool insertColumns(int position, int columns);

    /*
     * get the parent node pointer
     * @params: N/A
     * @return: parent pointer
    */
    TreeItem* parent();

    /*
     * remove (count childs) from(include) index position
     * @params:
     *      position - index to remove from
     *      count    - number of childs to remove
     * @return: bool
    */
    bool removeChildren(int position, int count);

    /*
     * remove (columns items) from(include) index position and childs the same
     * @params:
     *      position - index to remove from
     *      columns  - number of data items to remove
     * @return: bool
    */
    bool removeColumns(int position, int columns);

    /*
     * get the idnex number of node in its parent's child list
     * @params: N/A
     * @return: index number
    */
    int childNumber() const;

    /*
     * set the data at index column with value
     * @params:
     *      column - index
     *      value  - new value
     * @return: bool
    */
    bool setData(int column, const QVariant& value);

private:
    // child tree items list
    QList<TreeItem*> childItems;
    // current tree item data
    QVector<QVariant> itemData;
    // current tree item's parent item
    TreeItem* parentItem;
};

KAVI_NS_END

#endif // TREEITEM_H
