//
// Created by darbix on 26.4.22.
//

#include "GSequenceDiagram.h"
#include <QDebug>
#include <QFrame>
#include <QVBoxLayout>
#include <QComboBox>
#include <QGraphicsLineItem>
#include <QPushButton>
#include "GTimeline.h"
#include "GMessage.h"
#include "GObject.h"
#include <QLineEdit>
#include <QLabel>

#define HORIZONTAL_GAP 350
#define NUM_INDEXES 5

GSequenceDiagram::GSequenceDiagram(GraphicsScene *scene, std::string name, ClassDiagram *classDiagram, QFrame *settings):
    QObject(), classDiagram{classDiagram}, settings{settings}, index{1}, scene{scene}{

    addSettings();
    sequanceDiagram = classDiagram->getSequenceDiagram(name);

//    for(auto item: msg2->getItems())
//        scene->removeItem(item);
//    delete msg2;
}

void GSequenceDiagram::addSettings(){
    settings->setFixedWidth(400);
    settings->setContentsMargins(0,0,0,0);

    QVBoxLayout *vLayout = new QVBoxLayout(settings);
    vLayout->setContentsMargins(0,0,0,0);

    // Add new timeline row
    QFrame *frame = new QFrame();
    vLayout->addWidget(frame);
    QHBoxLayout *qHBoxLayout = new QHBoxLayout(frame);
    vLayout->setAlignment(Qt::AlignTop);
    vLayout->setSpacing(5);

    QComboBox *comboBoxClassifiers = new QComboBox();
    qHBoxLayout->addWidget(comboBoxClassifiers);
    for(auto cls: classDiagram->classElements()){
        comboBoxClassifiers->addItem(QString::fromStdString(cls->name()));
    }
    comboBoxClassifiers->setFixedWidth(90);

    QLineEdit *lineEditInstanceName = new QLineEdit();
    lineEditInstanceName->setPlaceholderText("instance name");
    qHBoxLayout->addWidget(lineEditInstanceName);

    QPushButton *addClass = new QPushButton();
    qHBoxLayout->addWidget(addClass);
    qHBoxLayout->setContentsMargins(15,9,15,9);
    addClass->setFixedWidth(75);
    addClass->setText("➕ Class");
    connect(addClass, SIGNAL(pressed()), this, SLOT(onAddPressed()));

    // Message frame vertical panel
    QFrame *frameMsg = new QFrame();
    vLayout->addWidget(frameMsg);
    frameMsg->setStyleSheet("QFrame { background-color: #32373C; border-radius: 9px; }");
    QVBoxLayout *vLayoutMsg = new QVBoxLayout(frameMsg);
    QFrame *vFrameMsg = new QFrame();
    vLayoutMsg->addWidget(vFrameMsg);

    // Message frame horizontal 1
    QFrame *hFrame1 = new QFrame();
    vLayoutMsg->addWidget(hFrame1);
    QHBoxLayout *hLayoutMsg1 = new QHBoxLayout(hFrame1);
    // -- combo src
    QLabel *labelFrom = new QLabel("from:");
    hLayoutMsg1->addWidget(labelFrom);
    QComboBox *comboBoxSrc = new QComboBox();
    comboBoxSrc->setFixedWidth(145);
    hLayoutMsg1->addWidget(comboBoxSrc);
    for(auto cls: classDiagram->classElements()){
        comboBoxSrc->addItem(QString::fromStdString(cls->name()));
    }
    // Message frame horizontal 1 - combo dst
    QLabel *labelTo = new QLabel("  to:");
    hLayoutMsg1->addWidget(labelTo);
    QComboBox *comboBoxDst = new QComboBox();
    comboBoxDst->setFixedWidth(145);
    connect(comboBoxDst, SIGNAL(currentTextChanged(const QString &)), this, SLOT(onFuncUpdate()));

    hLayoutMsg1->addWidget(comboBoxDst);
    for(auto cls: classDiagram->classElements()){
        comboBoxDst->addItem(QString::fromStdString(cls->name()));
    }

    // Message frame horizontal 2
    QFrame *hFrame2 = new QFrame();
    vLayoutMsg->addWidget(hFrame2);
    QHBoxLayout *hLayoutMsg2 = new QHBoxLayout(hFrame2);

//    QLineEdit *lineEdit = new QLineEdit();
//    lineEdit->setPlaceholderText("message text");
//    hLayoutMsg2->addWidget(lineEdit);
    QComboBox *comboBoxFuncs = new QComboBox();
    hLayoutMsg2->addWidget(comboBoxFuncs);

    // Message frame horizontal 3
    QFrame *hFrame3 = new QFrame();
    vLayoutMsg->addWidget(hFrame3);
    QHBoxLayout *hLayoutMsg3 = new QHBoxLayout(hFrame3);

    QComboBox *comboBoxTypes = new QComboBox();
    comboBoxTypes->addItems(QStringList(QString("sync,async,response,create,delete").split(",")));
    hLayoutMsg3->addWidget(comboBoxTypes);

    QPushButton *saveMsg = new QPushButton("Save");
    saveMsg->setFixedWidth(90);
    saveMsg->setFixedHeight(24);
    saveMsg->setStyleSheet("background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"\
                           "stop:0 #6BD96A, stop:0.05 #2BA94A, stop:0.95 #0B892A, stop:1 #06690F); border-radius: 3px;");
    hLayoutMsg3->addWidget(saveMsg);
    connect(saveMsg, SIGNAL(pressed()), this, SLOT(onSaveMsg()));

}

void GSequenceDiagram::onSaveMsg(){
    GTimeline *src = nullptr;
    GTimeline *dst = nullptr;

    int srcIndex = 0;
    int dstIndex = 0;

    auto boxes = settings->findChildren<QComboBox *>();
    if(!gTimelines.empty()) {
        int index = 0;
        for (auto tl: gTimelines) {
            if (tl->getName() == boxes[1]->currentText()) {
                src = tl;
                srcIndex = index;
            }
            if (tl->getName() == boxes[2]->currentText()) {
                dst = tl;
                dstIndex = index;
            }
            index++;
        }
    }
    GMessage::direction dir = GMessage::LTOL;
    if(srcIndex < dstIndex)
        dir = GMessage::LTOR_SEND;
    else if(srcIndex > dstIndex)
        dir = GMessage::RTOL_SEND;
    gMessages.push_back(src->addMsg(boxes[3]->currentText(), dst, dir, index++)); // Msg name boxes[3]
}

void GSequenceDiagram::onClassDiagramUpdated(){
    // Update ComboBox texts
    if(!settings->findChildren<QComboBox*>().empty()) {
        auto boxes = settings->findChildren<QComboBox *>();
        for(int i = 0; i < 4 && i < boxes.size(); i++) { // do not fill comboBoxTypes
            boxes[i]->clear();
            if(i == 0){
                for (auto cls: classDiagram->classElements()) {
                    boxes[i]->addItem(QString::fromStdString(cls->name()));
                }
            }
            if(i == 1 || i == 2 && !gTimelines.empty()){
                for (auto tl: gTimelines) {
                    boxes[i]->addItem(tl->getName());
                }
            }
            else if(i == 3 && !gTimelines.empty()){
                // Update fuction list of the class of the instance at boxes[2]
                auto instName = boxes[2]->currentText().toStdString();
                UMLClassifier *cls;
                for(auto tl: gTimelines){
                    if(tl->getName().toStdString() == instName)
                        cls = tl->cls;
                }
                if(cls) {
                    for (auto funcs: cls->getOperations()) { // call 'to:' obj methods
                        boxes[i]->addItem(QString::fromStdString(std::string(*funcs)));
                    }
                }
            }
        }
    }
    // todo emit
    emit classContentUpdated();
}

void GSequenceDiagram::onFuncUpdate(){
    auto boxes = settings->findChildren<QComboBox *>();
    if(classDiagram->getClassifier(boxes[2]->currentText().toStdString())) { // classifier 'to:'
        boxes[3]->clear();
        // Its methods
        for (auto funcs: classDiagram->getClassifier(boxes[2]->currentText().toStdString())->getOperations()) { // call 'to:' obj methods
            // Combo box of functions
            boxes[3]->addItem(QString::fromStdString(std::string(*funcs)));
        }
    }
}

void GSequenceDiagram::onAddPressed(){
    if(!settings->findChild<QComboBox*>()->currentText().isEmpty()) {
        auto instName = settings->findChild<QLineEdit*>()->text();
        if(!instName.isEmpty()){
            auto tl = new GTimeline(dynamic_cast<UMLClass*>(classDiagram->getClassifier(settings->findChild<QComboBox *>()->currentText().toStdString())),
                                    instName, scene, gTimelines.size() * HORIZONTAL_GAP, 0, NUM_INDEXES);
            gTimelines.push_back(tl);

            connect(tl, SIGNAL(gTimelineDeleted()), this, SLOT(onGTimelineDeleted()));
            connect(this, SIGNAL(classContentUpdated()), tl, SLOT(onClassContentUpdated()));

            sequanceDiagram->addObject(dynamic_cast<UMLClass*>(classDiagram->getClassifier(tl->getName().toStdString())), tl->getName().toStdString());
            onClassDiagramUpdated();
        }
    }
}

void GSequenceDiagram::onGTimelineDeleted() {
    auto tl = dynamic_cast<GTimeline *>(sender());
    if(!gTimelines.empty()) {
        gTimelines.erase(std::find(gTimelines.begin(), gTimelines.end(), tl));

        for(int i = gMessages.size()-1; i >= 0; i--){
            // Delete timelines messages
            if(gMessages.at(i)->src == tl || gMessages.at(i)->dst == tl) {
                auto m = gMessages.at(i);
                gMessages.erase(gMessages.begin() + i);
                delete m;
            }
        }
    }
    delete tl;
}