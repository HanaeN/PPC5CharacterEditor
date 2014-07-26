#include "timelinemodel.h"
#include <QDebug>

TimelineModel::TimelineModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    characterObject = new CharacterObject();
    characterObject->name = "Base";
    characterObject->children = QList<CharacterObject*>();
    beginInsertRows(createIndex(0, 0, characterObject), 0, 1);
    CharacterObject *e = new CharacterObject();
    e->name = "text";
    e->parent = characterObject;
    characterObject->children.append(e);
    e = new CharacterObject();
    e->name = "text 2";
    e->parent = characterObject;
    characterObject->children.append(e);
    endInsertRows();

}

TimelineModel::~TimelineModel() {
    delete characterObject;
}

int TimelineModel::columnCount(const QModelIndex &parent) const {
    return 2;
}

QModelIndex TimelineModel::parent(const QModelIndex &child) const {
    CharacterObject *object = (CharacterObject*)child.internalPointer();
    if (object->parent == NULL) return QModelIndex();
    return createIndex(object->parent->children.indexOf(object), child.column(), object->parent);
}

int TimelineModel::rowCount(const QModelIndex &parent) const {
    return calculateMaxRows();
}

QVariant TimelineModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) { // return drawing data
        CharacterObject *object = (CharacterObject*)index.internalPointer();
        if (index.column() == 0) return object->name;
        if (index.column() == 1) return qVariantFromValue((void*)&object->parameters);
    }
    return QVariant();
}

QModelIndex TimelineModel::index(int row, int column, const QModelIndex &parent) const {
    if (parent.isValid() && row >= 0 && row < ((CharacterObject*)parent.internalPointer())->children.count()) {
        return createIndex(row, column, ((CharacterObject*)parent.internalPointer())->children[row]);
    } else if (row == 0 && !parent.isValid()) return createIndex(row, column, characterObject);
    return QModelIndex();
}

int TimelineModel::calculateMaxRows() const {
    int size = 1;
    for (int n = 0; n < characterObject->children.count(); n++) {
        size += calculateRows(characterObject->children[n]);
    }
    return size;
}

int TimelineModel::calculateRows(CharacterObject *object) const {
    int size = 1;
    for (int n = 0; n < object->children.count(); n++) {
        size += calculateRows(object->children[n]);
    }
    return size;
}

void TimelineModel::addObject(CharacterObject *object, CharacterObject *parent) {

}

CharacterObject* TimelineModel::getObjectAt(int index) {
    if (index == 0) return characterObject;
    findObject = NULL;
    int currentIndex = 0;
    for (int n = 0; n < characterObject->children.count(); n++) {
        currentIndex = findRow(characterObject->children[n], index, currentIndex);
        if (findObject != NULL) return findObject;
    }
}

int TimelineModel::findRow(CharacterObject *object, int targetIndex, int currentIndex) {
    if (currentIndex == targetIndex) return currentIndex;
    currentIndex++;
    if (currentIndex == targetIndex) { findObject = object; return currentIndex; }
    for (int n = 0; n < object->children.count(); n++) {
        currentIndex = findRow(object->children[n], targetIndex, currentIndex);
        if (currentIndex == targetIndex) return currentIndex;
    }
}

CharacterObject* TimelineModel::getParentAt(int index) {
}
