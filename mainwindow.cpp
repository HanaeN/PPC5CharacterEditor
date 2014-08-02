#include "mainwindow.h"
#include "timelinedelegate.h"
#include "timelineheaderview.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QGLWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timeline = new TimelineModel(this);
    delegate = new TimelineDelegate(ui->e_Editor);
    headerView = new TimelineHeaderView(Qt::Horizontal, ui->e_Editor);
    headerView->setStretchLastSection(true);
    ui->e_Editor->setHeader(headerView);
    ui->e_Editor->setItemDelegateForColumn(1, delegate);
    ui->e_Editor->setModel(timeline);
    ui->e_Editor->expandAll();
    ui->e_Editor->setMouseTracking(true);
    ui->e_Editor->setViewport(new QGLWidget());
    headerView->resizeSection(0, 200);
    connect(delegate, SIGNAL(timelinePositionChange(int)), this, SLOT(onTimelinePositionChanged(int)));
    connect(delegate, SIGNAL(keyframeSelected(KeyframeObject*,bool,bool)), this, SLOT(onKeyframeSelected(KeyframeObject*,bool,bool)));
    connect(delegate, SIGNAL(contextMenu(CharacterObject*,QString,Keyframe::PropertyType,int,QPoint)), SLOT(onContextMenu(CharacterObject*,QString,Keyframe::PropertyType,int,QPoint)));
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Exit_triggered()
{
    this->close();
}

void MainWindow::on_e_timelineScroll_sliderMoved(int position)
{
//    delegate->scrollX = position;
//    headerView->scrollX = position;
//    ui->e_Editor->reset();
}

void MainWindow::on_e_timelineScroll_valueChanged(int value)
{
    delegate->scrollX = value;
    headerView->scrollX = value;
    headerView->viewport()->update();
    ui->e_Editor->viewport()->update();
}

void MainWindow::onTimelinePositionChanged(int x) {
    delegate->timelinePos = x;
    headerView->timelinePos = x;
    headerView->viewport()->update();
    ui->e_Editor->viewport()->update();
}

void MainWindow::onKeyframeSelected(KeyframeObject *obj, bool isAtX, bool isint) {
    isInt = isint;
    atKeyframe = isAtX;
    editingKeyframe = NULL;
    if (obj != NULL) {
        ui->e_value->setText(QString::number(obj->value));
        ui->e_easingin->setCurrentIndex(0);
        ui->grp_keyframe->setEnabled(true);
        if (obj->easeIn != Keyframe::NoEasing) {
            int id = 0;
            if (obj->easeIn == Keyframe::CubicEasing) id = 1;
            if (obj->easeIn == Keyframe::QuadraticEasing) id = 2;
            if (obj->easeIn == Keyframe::SinusoidalEasing) id = 3;
            if (obj->easeIn == Keyframe::ExponentialEasing) id = 4;
            ui->c_easingin->setChecked(true);
            ui->e_easingin->setCurrentIndex(id);
            ui->e_easingin->setEnabled(true);
        } else {
            ui->e_easingin->setCurrentIndex(0);
            ui->c_easingin->setChecked(false);
            ui->e_easingin->setEnabled(false);
        }

        if (obj->easeOut != Keyframe::NoEasing) {
            int id = 0;
            if (obj->easeOut == Keyframe::CubicEasing) id = 1;
            if (obj->easeOut == Keyframe::QuadraticEasing) id = 2;
            if (obj->easeOut == Keyframe::SinusoidalEasing) id = 3;
            if (obj->easeOut == Keyframe::ExponentialEasing) id = 4;
            ui->c_easingout->setChecked(true);
            ui->e_easingout->setCurrentIndex(id);
            ui->e_easingout->setEnabled(true);
        } else {
            ui->e_easingout->setCurrentIndex(0);
            ui->c_easingout->setChecked(false);
            ui->e_easingout->setEnabled(false);
        }
        editingKeyframe = obj;
    } else {
        ui->e_value->setText("0");
        ui->e_easingin->setCurrentIndex(0);
        ui->e_easingout->setCurrentIndex(0);
        ui->c_easingin->setChecked(false);
        ui->c_easingout->setChecked(false);
        ui->grp_keyframe->setEnabled(false);

    }
}

void MainWindow::on_e_value_textEdited(const QString &arg1)
{
    if (!arg1.endsWith(".")) {
        double number = arg1.toDouble();
        if (isInt) number = round(number);
        editingKeyframe->value = number;
        ui->e_value->setText(QString::number(number));
    }
}

void MainWindow::on_e_value_editingFinished()
{
    if (ui->e_value->text().endsWith(".")) ui->e_value->setText(ui->e_value->text() + "0");
    on_e_value_textEdited(ui->e_value->text());
}

void MainWindow::on_c_easingin_toggled(bool checked)
{
    if (editingKeyframe == NULL) return;
    if (checked) {
        ui->e_easingin->setEnabled(true);
        ui->e_easingin->setCurrentIndex(0);
        editingKeyframe->easeIn = Keyframe::LinearEasing;
    } else {
        ui->e_easingin->setCurrentIndex(0);
        ui->e_easingin->setEnabled(false);
        editingKeyframe->easeIn = Keyframe::NoEasing;
    }
}

void MainWindow::on_c_easingout_toggled(bool checked)
{
    if (editingKeyframe == NULL) return;
    if (checked) {
        ui->e_easingout->setEnabled(true);
        ui->e_easingin->setCurrentIndex(0);
        editingKeyframe->easeOut = Keyframe::LinearEasing;
        qDebug() <<editingKeyframe->easeOut;
    } else {
        ui->e_easingout->setCurrentIndex(0);
        ui->e_easingout->setEnabled(false);
        editingKeyframe->easeOut = Keyframe::NoEasing;
    }
}

void MainWindow::on_e_easingout_currentIndexChanged(int index)
{
    if (editingKeyframe == NULL) return;
    if (ui->c_easingout->isEnabled()) {
        if (index == 0) editingKeyframe->easeOut = Keyframe::LinearEasing;
        if (index == 1) editingKeyframe->easeOut = Keyframe::CubicEasing;
        if (index == 2) editingKeyframe->easeOut = Keyframe::QuadraticEasing;
        if (index == 3) editingKeyframe->easeOut = Keyframe::SinusoidalEasing;
        if (index == 4) editingKeyframe->easeOut = Keyframe::ExponentialEasing;
    }
}

void MainWindow::on_e_easingin_currentIndexChanged(int index)
{
    if (editingKeyframe == NULL) return;
    if (ui->c_easingin->isEnabled()) {
        if (index == 0) editingKeyframe->easeIn = Keyframe::LinearEasing;
        if (index == 1) editingKeyframe->easeIn = Keyframe::CubicEasing;
        if (index == 2) editingKeyframe->easeIn = Keyframe::QuadraticEasing;
        if (index == 3) editingKeyframe->easeIn = Keyframe::SinusoidalEasing;
        if (index == 4) editingKeyframe->easeIn = Keyframe::ExponentialEasing;
    }

}

void MainWindow::exportJSON() {
    QJsonObject master;
    QJsonObject characterInfo;
    characterInfo["Name"] = ui->e_name->text();
    characterInfo["Dropset"] = "2222222222222222";
    characterInfo["AITrait"] = ui->e_trait->currentText();
    characterInfo["AIDifficulty"] = ui->e_aidifficulty->currentText();
    QJsonObject obj;
    obj["CharacterInfo"] = characterInfo;
    master["Keyframes"] = exportKeyframeJSON(timeline->characterObject->tweenList);
    master["Children"] = exportChildJSON(timeline->characterObject->children);
    obj["Master"] = master;
    QJsonDocument doc(obj);
    qDebug() << doc.toJson().data();
}

void MainWindow::on_action_Save_triggered()
{
    exportJSON();
}

QJsonObject MainWindow::exportKeyframeJSON(TweenList *tweenList) {
    QJsonObject r;
    for (int n = 0; n < tweenList->tweens.count(); n++){
        KeyframeList *list = tweenList->tweens[n];
        QString paramName = list->propertyName;
        QJsonArray keyframes;
        for (int i = 0; i < list->keyframes.count(); i++) {
            KeyframeObject &obj = list->keyframes[i];
            QJsonObject o;
            o["index"] = obj.frameIndex;
            o["value"] = obj.value;
            o["easeIn"] = timeline->easeToString(obj.easeIn);
            o["easeOut"] = timeline->easeToString(obj.easeOut);
            keyframes.append(o);
        }
        r[paramName] = keyframes;
    }
    return r;
}

QJsonArray MainWindow::exportChildJSON(QList<CharacterObject *> &children) {
    QJsonArray a;
    for (int n = 0; n < children.count(); n++) {
        QJsonObject child;
        CharacterObject *obj = children[n];
        child["Name"] = obj->name;
        child["Children"] = exportChildJSON(obj->children);
        child["Keyframes"] = exportKeyframeJSON(obj->tweenList);
        a.append(child);
    }
    return a;
}

void MainWindow::onContextMenu(CharacterObject *obj, QString propertyName, Keyframe::PropertyType propertyType, int index, QPoint mPos) {
    if (atKeyframe) { // offer remove options here
        QMenu menu(this);
        QAction *a = new QAction("Remove Keyframe", this);
        connect(a, &QAction::triggered, [=]() {
            timeline->removeKeyframe(obj, propertyName, index);
            this->onKeyframeSelected(NULL, false, false);
        });
        menu.addAction(a);
        menu.exec(mPos);
    } else {
        QMenu menu(this);
        QAction *a = new QAction("Insert Keyframe", this);
        connect(a, &QAction::triggered, [=]() {
            if (editingKeyframe != NULL) {
                KeyframeObject &o = timeline->insertKeyframe(obj, propertyName, index, editingKeyframe->value, editingKeyframe->easeIn, editingKeyframe->easeOut);
                onKeyframeSelected(&o, true, propertyType == Keyframe::INT ? true : false);
            } else {
                KeyframeObject &o = timeline->insertKeyframe(obj, propertyName, index, 0, Keyframe::NoEasing, Keyframe::LinearEasing);
                onKeyframeSelected(&o, true, propertyType == Keyframe::INT ? true : false);
            }
        });
        menu.addAction(a);
        menu.exec(mPos);
    }
}

void MainWindow::insertKeyframe() {
    qDebug() << "inserting because pie";
}
