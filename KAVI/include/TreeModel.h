/*
 * @author: liYc
 * @date  : 2018/1/24
 * @brief : define the tree model of Qt's class tree view
*/

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include "KAVIBase.h"

KAVI_NS_BEGIN

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

    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

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
    void initClasses(TreeItem* parent, int parentID);
    TreeItem* getItem(const QModelIndex& index) const;
    QModelIndex getItemIndex(int id);
    QModelIndex recursiveSearch(const QModelIndex& parent, QString label);

    TreeItem* rootItem;
    GraphClass* graphData;

    QString lastInserted;
    QMap<int, QString> modelMap;
}

KAVI_NS_END

#endif // TREEMODEL_H
