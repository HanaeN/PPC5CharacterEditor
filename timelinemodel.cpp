#include "timelinemodel.h"
#include <QDebug>

TimelineModel::TimelineModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    characterObject = new CharacterObject();
    characterObject->name = "Master";
    characterObject->children = QList<CharacterObject*>();
    setupParameters(characterObject->tweenList);
    CharacterObject *e = new CharacterObject();
    e = new CharacterObject();
    e->name = "testing add object";
    setupParameters(e->tweenList);
    addObject(e, characterObject);
}

TimelineModel::~TimelineModel() {
    delete characterObject;
}

int TimelineModel::columnCount(const QModelIndex &parent) const {
    return 2;
}

QModelIndex TimelineModel::parent(const QModelIndex &child) const {
//    qDebug() << dynamic_cast<CharacterObject*>(child.internalPointer());
    TimelineObject *object = (TimelineObject*)child.internalPointer();
    if (object->type == TimelineObject::CharObject) {
        if (((CharacterObject*)object)->parent == NULL) return QModelIndex();
        return createIndex(((CharacterObject*)object)->parent->children.indexOf((CharacterObject*)object) + 1, 0, ((CharacterObject*)object)->parent);
    }
    if (object->type == TimelineObject::TweenObject) {
        return createIndex(0, 0, ((TweenList*)object)->parent);
    }
    if (object->type == TimelineObject::KeyObject) {
        return createIndex(((TweenList*)(((KeyframeList*)object)->parent))->tweens.indexOf((KeyframeList*)object), 0, ((KeyframeList*)object)->parent);
    }
}

int TimelineModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        TimelineObject *t = (TimelineObject*)parent.internalPointer();
        if (t->type == TimelineObject::CharObject) {
            return 2;
            qDebug() << ((CharacterObject*)parent.internalPointer())->children.count() + 1 << "p";
            return ((CharacterObject*)parent.internalPointer())->children.count() + 1;
        }
        if (t->type == TimelineObject::TweenObject) {
            return ((TweenList*)parent.internalPointer())->tweens.count();
        }
    } else return 1;
}

QVariant TimelineModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) { // return drawing data
        TimelineObject *t = (TimelineObject*)index.internalPointer();
        if (t->type == TimelineObject::CharObject) {
            CharacterObject *object = (CharacterObject*)index.internalPointer();
            if (index.column() == 0) return object->name;
            if (index.column() == 1) return qVariantFromValue((void*)&object->tweenList->tweens);
        } else if (t->type == TimelineObject::TweenObject) {
            TweenList *object = (TweenList*)index.internalPointer();
            if (index.column() == 0) return "Tweens";
            if (index.column() == 1) return qVariantFromValue((void*)&object->tweens);
        } else {
            KeyframeList *object = (KeyframeList*)index.internalPointer();
            if (index.column() == 0) return object->propertyName;
            if (index.column() == 1) return qVariantFromValue((void*)&object->keyframes);
        }
    }
    return QVariant();
}

QModelIndex TimelineModel::index(int row, int column, const QModelIndex &parent) const {
    TimelineObject *obj = NULL, *parobj;
    if (parent.isValid()) {
        parobj = (TimelineObject*)parent.internalPointer();
        if (row == 0 && parobj->type == TimelineObject::CharObject) {
            obj = ((CharacterObject*)parobj)->tweenList;
        } else {
            if (parobj->type == TimelineObject::CharObject) {
                if (row < 1 || row >= ((CharacterObject*)parobj)->children.count() + 1) return QModelIndex();
                obj = ((CharacterObject*)parobj)->children[row - 1];
            }
        }
        if (parobj->type == TimelineObject::TweenObject) {
            if (row < 0 || row >= ((TweenList*)parobj)->tweens.count()) return QModelIndex();
            obj = ((TweenList*)parobj)->tweens[row];
        }
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
        TimelineObject *t = (TimelineObject*)parent.internalPointer();
        if (t->type == TimelineObject::CharObject) return true;
        if (t->type == TimelineObject::TweenObject) {
            if (((TweenList*)parent.internalPointer())->tweens.count() > 0) return true;
        }
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

void TimelineModel::setupParameters(TweenList* parameters) {
    parameters->tweens.clear();
    QStringList properties;
    properties << "alpha" << "posX" << "posY" << "zIndex" << "angle" << "scaleX" << "scaleY";
    for (int n = 0; n < properties.count(); n++) {
        KeyframeList *c = new KeyframeList();
        c->propertyName = properties[n];
        c->parent = parameters;
        parameters->tweens.append(c);
    }
}

void TimelineModel::insertKeyframe(CharacterObject *object, QString propertyName, int frameIndex, double value) {
    for (int n = 0; n < object->tweenList->tweens.count(); n++) {
        KeyframeList *keyframelist = object->tweenList->tweens[n];
        if (keyframelist->propertyName == propertyName) { // found;
            for (int i = 0; i < keyframelist->keyframes.count(); i++) { // update existing frame if it exists
                KeyframeObject &obj = keyframelist->keyframes[i];
                if (obj.frameIndex == frameIndex) { obj.value = value; return; }
            }
            KeyframeObject obj;
            obj.frameIndex = frameIndex;
            obj.value = value;
            if (keyframelist->keyframes.count() == 0) keyframelist->keyframes.append(obj); // no need to look for the later keyframe
            else {
                for (int i = 0; i < keyframelist->keyframes.count(); i++) { // insert before the next index
                    KeyframeObject &obj = keyframelist->keyframes[i];
                    if (obj.frameIndex > frameIndex) { keyframelist->keyframes.insert(i, obj); return; }
                }
            }
            return;
        }
    }
}
