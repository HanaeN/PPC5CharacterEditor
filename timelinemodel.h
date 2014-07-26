#ifndef TIMELINEMODEL_H
#define TIMELINEMODEL_H

#include <QAbstractItemModel>
#include <QList>

namespace Keyframe {
    enum PropertyType{INT, FLOAT};
}
struct KeyframeObject {
    double value = 0;
    short frameIndex = 0;
};
struct KeyframeList {
    QString propertyName;
    Keyframe::PropertyType propertyType = Keyframe::INT;
    QList<KeyframeObject> keyframes;
};
struct CharacterObject {
    QString name;
    QList<CharacterObject*> children;
    QList<KeyframeList> parameters;
    CharacterObject *parent = NULL;
};

class TimelineModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TimelineModel(QObject *parent = 0);
    int columnCount(const QModelIndex&) const;
    int rowCount(const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    void addObject(CharacterObject* object, CharacterObject* parent);
    CharacterObject* getObjectAt(int index);
    CharacterObject* getParentAt(int index);
    int calculateMaxRows() const;
    int calculateRows(CharacterObject *object) const;
    int findRow(CharacterObject *object, int targetIndex, int currentIndex);
    ~TimelineModel();
private:
    CharacterObject *findObject;
    CharacterObject *characterObject;

signals:

public slots:

};

#endif // TIMELINEMODEL_H
