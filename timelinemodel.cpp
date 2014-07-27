#include "timelinemodel.h"
#include <QDebug>

TimelineModel::TimelineModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    characterObject = new CharacterObject();
    characterObject->name = "Master";
    characterObject->children = QList<CharacterObject*>();
    setupParameters(characterObject->parameters);
    CharacterObject *e = new CharacterObject();
    e = new CharacterObject();
    e->name = "testing add object";
    setupParameters(e->parameters);
    addObject(e, characterObject);
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
    return createIndex(object->parent->children.indexOf(object), 0, object->parent);
}

int TimelineModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return ((CharacterObject*)parent.internalPointer())->children.count();
    } else return 1;
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
    CharacterObject *obj = NULL, *parobj;
    if (parent.isValid()) {
        parobj = (CharacterObject*)parent.internalPointer();
        if (row < 0 || row >= parobj->children.count()) return QModelIndex();
        obj = parobj->children[row];
    } else if (row == 0) {
        obj = characterObject;
    }
    if (obj) return createIndex(row, column, (void*)obj);
    return QModelIndex();
}

void TimelineModel::calculateIndexes() {
    characterObject->index = 0;
    for (int n = 0; n < characterObject->children.count(); n++) {
        characterObject->children[n]->index = n;
        calculateIndex(characterObject->children[n]);
    }
}

void TimelineModel::calculateIndex(CharacterObject *object) {
    for (int n = 0; n < object->children.count(); n++) {
        object->children[n]->index = n;
        calculateIndex(object->children[n]);
    }
}

void TimelineModel::removeObject(CharacterObject *object) {
    if (object->parent != NULL) {
        qDebug() << object->parent->name << object->index << object->name;
        beginRemoveRows(createIndex(object->parent->index, 0, object->parent), object->index, object->index);
        object->parent->children.removeOne(object);
        delete object;
        endRemoveRows();
        calculateIndexes();
    }
}

// object to be added. parent to add it to. index to insert before.
void TimelineModel::addObject(CharacterObject *object, CharacterObject *parent, int index) {
    if (parent != NULL) {
        int position = index;
        if (index == -1) {
            position = parent->children.count();
        } else {
            if (position < 0) position = 0;
            if (position > parent->children.count()) position = parent->children.count();
        }
        beginInsertRows(createIndex(parent->index, 0, parent), position, position);
        object->parent = parent;
        object->index = index;
        parent->children.insert(position, object);
        endInsertRows();
        for (int n = index + 1; n < parent->children.count(); n++)
            parent->children[n]->index++;
    } else {
        qDebug() << "Could not add CharacterObject to TimelineModel: invalid parent";
    }
}

bool TimelineModel::hasChildren(const QModelIndex &parent) const {
    if (parent.isValid()) {
        if (((CharacterObject*)parent.internalPointer())->children.count() > 0) return true;
    } else return true;
    return false;
}

void TimelineModel::importFromJSONString(QString JSON) {
    for (int n = 0; n < characterObject->children.count(); n++) // clear children
        removeObject(characterObject->children[0]);
    // import from a JSON string using QtJSON
}

QString TimelineModel::exportToJSONString() {
    return "{}";
}

void TimelineModel::setupParameters(QList<KeyframeList*> &parameters) {
    parameters.clear();
    KeyframeList *c = new KeyframeList();
    c->propertyName = "alpha";
    parameters.append(c);
    c = new KeyframeList();
    c->propertyName = "posX";
    parameters.append(c);
    c = new KeyframeList();
    c->propertyName = "posY";
    parameters.append(c);
    c = new KeyframeList();
    c->propertyName = "zIndex";
    parameters.append(c);
    c = new KeyframeList();
    c->propertyName = "angle";
    parameters.append(c);
    c = new KeyframeList();
    c->propertyName = "scaleX";
    parameters.append(c);
    c = new KeyframeList();
    c->propertyName = "scaleY";
    parameters.append(c);
}
