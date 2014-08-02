#ifndef TIMELINEMODEL_H
#define TIMELINEMODEL_H

#include <QAbstractItemModel>
#include <QList>

namespace Keyframe {
    enum PropertyType{INT, FLOAT};
    enum EasingMode {NoEasing, LinearEasing, CubicEasing, QuadraticEasing, SinusoidalEasing, ExponentialEasing};
}
struct TimelineObject {
    enum Type {CharObject, KeyObject, TweenObject};
    TimelineObject(Type type) : type(type) {}
    Type type;
};

struct KeyframeObject {
    Keyframe::EasingMode easeIn = Keyframe::NoEasing;
    Keyframe::EasingMode easeOut = Keyframe::NoEasing;
    double value = 0;
    short frameIndex = 0;
};

struct TweenList;
struct KeyframeList : public TimelineObject {
    KeyframeList() : TimelineObject(KeyObject) {}
    QString propertyName;
    double currentValue = 0;
    Keyframe::PropertyType propertyType = Keyframe::INT;
    QList<KeyframeObject> keyframes;
    TweenList *parent = NULL;
};

struct CharacterObject;
struct TweenList : public TimelineObject {
    TweenList(): TimelineObject(TweenObject) {}
    QList<KeyframeList*> tweens;
    CharacterObject *parent = NULL;
};

struct CharacterObject : public TimelineObject {
    CharacterObject() : TimelineObject(CharObject) { tweenList = new TweenList(); tweenList->parent = this; }
    ~CharacterObject() { delete tweenList; }
    int index = 0;
    QString name;
    QList<CharacterObject*> children;
    TweenList *tweenList;
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
    KeyframeObject& getKeyframe(CharacterObject* object, QString propertyName, int frameIndex);
    void removeKeyframe(CharacterObject* object, QString propertyName, int frameIndex);
    KeyframeObject& insertKeyframe(CharacterObject* object, QString propertyName, int frameIndex, double value,
                        Keyframe::EasingMode easeIn, Keyframe::EasingMode easeOut);
    QString exportToJSONString();
    void setupParameters(TweenList *parameters);
    bool hasChildren(const QModelIndex &parent) const;
    ~TimelineModel();
    CharacterObject *characterObject;
    QString easeToString(Keyframe::EasingMode value);
    Keyframe::EasingMode easeToEnum(QString value);
private:
    void calculateIndexes();
    void calculateIndex(CharacterObject *object);

signals:

public slots:

};

#endif // TIMELINEMODEL_H
