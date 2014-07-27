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
    int index = 0;
    QString name;
    QList<CharacterObject*> children;
    QList<KeyframeList*> parameters;
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
    void addObject(CharacterObject* object, CharacterObject* parent, int index = -1);
    void removeObject(CharacterObject* object);
    void importFromJSONString(QString JSON);
    QString exportToJSONString();
    void setupParameters(QList<KeyframeList *> &parameters);
    bool hasChildren(const QModelIndex &parent) const;
    ~TimelineModel();
private:
    void calculateIndexes();
    void calculateIndex(CharacterObject *object);
    CharacterObject *characterObject;

signals:

public slots:

};

#endif // TIMELINEMODEL_H
