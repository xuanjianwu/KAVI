/*
 * @author: liYc
 * @date  : 2018/1/24
 * @brief : define the tree model of Qt's class tree view
*/

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include "KAVIBase.h"



class TreeItem;
class GraphClass;

class TreeModel: public QAbstractItemModel {
    Q_OBJECT

public:
    /*
    * construct function
    * @params:
    *       graph  - the graph to build the tree model
    *       parent - specify the tree model's parent object
    * @return: N/A
    */
    TreeModel(GraphClass* graph, QObject* parent = 0);

    /*
    * deconstruct function
    * @params: N/A
    * @return: N/A
    */
    ~TreeModel();

    /*
    * get the data at index, if role is UserRole, return the associated class's ID;
    * if role is DisplayRole or EditRole, return the associated class's label
    * @params:
    *       index - the specified index at model
    *       role  - the data role
    * @return: expected data
    */
    QVariant data(const QModelIndex& index, int role) const;

    /*
    * get the Horizontal and DisplayRole 's headerData at column section
    * @params:
    *       section     - column index
    *       orientation - object's orientation
    *       role        - the data role
    * @return: expected data
    */
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    /*
    * creates a model index for the given row and column with the No.row
    * children of parent
    * @params:
    *       row    - the specified row
    *       column - the specified column
    *       parent - the parent item's index in model
    * @return: expected model index
    */
    QModelIndex index(int row, int column,
                      const QModelIndex& parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;

    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex& index, const QVariant& value,
                 int role = Qt::EditRole);

    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant& value, int role = Qt::EditRole);
    bool insertRows(int position, int rows,
                    const QModelIndex& parent = QModelIndex());
    bool removeRows(int position, int rows,
                    const QModelIndex& parent = QModelIndex());

    bool removeRow(int id);

private:
    /*
    * init the whole classes tree with root item parent
    * @params:
    *       parent    - root item to init tree
    *       parentID  - the parent node's ID
    * @return: void
    */
    void initClasses(TreeItem* parent, int parentID);
    TreeItem* getItem(const QModelIndex& index) const;
    QModelIndex getItemIndex(int id);
    QModelIndex recursiveSearch(const QModelIndex& parent, QString label);

    // the root item of the tree
    TreeItem* rootItem;

    // the graph of definition
    GraphClass* graphData;

    // the last inserted class
    QString lastInserted;

    // map from class node's ID to class node's label
    QMap<int, QString> modelMap;
};



#endif // TREEMODEL_H
