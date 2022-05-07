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
#include <QTreeWidget>
#include <QHeaderView>
#include <QListWidget>

#define HORIZONTAL_GAP 350
#define NUM_INDEXES 15
#define START_Y 0

GSequenceDiagram::GSequenceDiagram(GraphicsScene *scene, SequenceDiagram *model, ClassDiagram *classDiagram, QFrame *settings):
    QObject(), classDiagram{classDiagram}, sequanceDiagram{model}, settings{settings}, msgList{new QListWidget()}, index{1}, scene{scene}{
    // TODO: add objects and timeline from model
    if (model != nullptr) {
        std::map<UMLObject *, GTimeline *> mapOfObjects;
        for (auto obj : model->objects()) {
            auto tl = new GTimeline(obj, scene, gTimelines.size() * HORIZONTAL_GAP, START_Y, NUM_INDEXES);
            gTimelines.push_back(tl);
            mapOfObjects.insert({obj, tl});
            connect(tl, SIGNAL(gTimelineDeleted()), this, SLOT(onGTimelineDeleted()));
            connect(this, SIGNAL(classContentUpdated()), tl, SLOT(onClassContentUpdated()));
        }

        for (auto msg : model->messages()) {
            auto iter = mapOfObjects.find(const_cast<UMLObject *>(msg->src()));
            if (iter == mapOfObjects.end())
                return;
            GTimeline *src = iter->second;
            iter = mapOfObjects.find(const_cast<UMLObject *>(msg->dst()));
            if (iter == mapOfObjects.end())
                return;
            GTimeline *dst = iter->second;
            GMessage::direction dir;
            if (src->getX() < dst->getX()) {
                dir = GMessage::direction::LTOR;
            }
            else if (src == dst) {
                dir = GMessage::LTOL;
            }
            else {
                dir = GMessage::RTOL;
            }

            auto gMsg = new GMessage(scene, msg, dir, src, dst, index++);
            gMessages.push_back(gMsg); // Msg name boxes[3]
            addMsgItem(msgList, gMsg);

            connect(this, SIGNAL(updateMsgPos()), gMsg, SLOT(onUpdateMsgPos()));

        }
    }

    addSettings();
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
    for(auto cls: classDiagram->getClasses()){
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
    vLayoutMsg->setSpacing(0);

    QFrame *hFrame0 = new QFrame();
    vLayoutMsg->addWidget(hFrame0);
    QHBoxLayout *hLayoutMsg0 = new QHBoxLayout(hFrame0);
    QLabel *labelMessage = new QLabel("New message");
    labelMessage->setFont(QFont("Ubuntu", 15, 380));
    hLayoutMsg0->addWidget(labelMessage);

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
    for(auto obj: sequanceDiagram->objects()){
        comboBoxSrc->addItem(QString::fromStdString(obj->name()));
    }
    // Message frame horizontal 1 - combo dst
    QLabel *labelTo = new QLabel("  to:");
    hLayoutMsg1->addWidget(labelTo);
    QComboBox *comboBoxDst = new QComboBox();
    comboBoxDst->setFixedWidth(145);

    hLayoutMsg1->addWidget(comboBoxDst);
    for(auto obj: sequanceDiagram->objects()){
        comboBoxDst->addItem(QString::fromStdString(obj->name()));
    }
    connect(comboBoxDst, SIGNAL(currentTextChanged(const QString &)), this, SLOT(onFuncUpdate()));

    // Message frame horizontal 2
    QFrame *hFrame2 = new QFrame();
    vLayoutMsg->addWidget(hFrame2);
    QHBoxLayout *hLayoutMsg2 = new QHBoxLayout(hFrame2);
    QComboBox *comboBoxFuncs = new QComboBox();
    // TODO: add operations??
    hLayoutMsg2->addWidget(comboBoxFuncs);

    // Message frame horizontal 3
    QFrame *hFrame3 = new QFrame();
    vLayoutMsg->addWidget(hFrame3);
    QHBoxLayout *hLayoutMsg3 = new QHBoxLayout(hFrame3);
    hLayoutMsg3->setSpacing(130);

    QComboBox *comboBoxTypes = new QComboBox();
    comboBoxTypes->addItems(QStringList(QString("sync,async,response,create,delete").split(",")));
    hLayoutMsg3->addWidget(comboBoxTypes);

    QPushButton *saveMsg = new QPushButton("Save");
    saveMsg->setFixedWidth(100);
    saveMsg->setFixedHeight(24);
    saveMsg->setStyleSheet("background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"\
                           "stop:0 #6BD96A, stop:0.05 #2BA94A, stop:0.95 #0B892A, stop:1 #06690F); border-radius: 3px;");
    hLayoutMsg3->addWidget(saveMsg);
    connect(saveMsg, SIGNAL(pressed()), this, SLOT(onSaveMsg()));


    // ---- Messages Title + List ----
    QFrame *messagesTitle = new QFrame();
    vLayout->addWidget(messagesTitle);
    QHBoxLayout *hLayoutMessages = new QHBoxLayout(messagesTitle);
    QLabel *labelMessages = new QLabel("Messages");
    labelMessages->setContentsMargins(9,0,0,0);
    labelMessages->setFont(QFont("Ubuntu", 15, 380));
    hLayoutMessages->addWidget(labelMessages);

    QFrame *frameList = new QFrame();
    frameList->setContentsMargins(14,0,14,0);
    QVBoxLayout *vLayoutTree = new QVBoxLayout(frameList);
    vLayoutTree->setContentsMargins(0,5,0,0);
    vLayout->addWidget(frameList);

    msgList->setStyleSheet("QListWidget { background-color: #32373C; border-top-left-radius: 7px; border-top-right-radius: 7px; }"
                         "QListWidget::item { height: 25px; }");
    vLayoutTree->addWidget(msgList);

    msgList->setDragDropMode(QAbstractItemView::DragDrop);
    msgList->setDefaultDropAction(Qt::MoveAction);
    connect(msgList->model(), SIGNAL(rowsMoved(const QModelIndex &, int, int, const QModelIndex &, int)), this,
            SLOT(msgDropped(const QModelIndex &, int, int, const QModelIndex &, int)));
    connect(msgList, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onMsgDoubleClick(const QModelIndex &)));
}

void GSequenceDiagram::onMsgDoubleClick(const QModelIndex &ix){
    // Delete message
    auto msg = gMessages.at(msgList->currentRow());
    auto iter {std::find(gMessages.begin(), gMessages.end(), gMessages.at(msgList->currentRow()))};
    sequanceDiagram->removeMessage((*iter)->model());
    gMessages.erase(iter);
    auto msgWidget = msgList->currentItem();
    msgList->removeItemWidget(msgWidget);
    delete msg;
    delete msgWidget;

    index--;
    updateAfterMsgDelete();
}

void GSequenceDiagram::msgDropped(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row){
    std::vector<GMessage*> newOrder;
    for(int i = 0; i < msgList->count(); i++){
        QString textIndex = msgList->itemWidget(msgList->item(i))->findChild<QLabel*>()->text();
        int index = textIndex.remove(textIndex.size()-1, 1).toInt();

        GMessage *old = gMessages.at(index-1);
        newOrder.push_back(old); // Push msg at the old index to a new one
        old->index = i+1;
        msgList->itemWidget(msgList->item(i))->findChild<QLabel*>()->setText(QString::number(i+1)+"."); // Text index update
    }
    gMessages.erase(gMessages.begin(), gMessages.end());
    for(auto msg: newOrder){
        gMessages.push_back(msg);
    }
    emit updateMsgPos();
}

void GSequenceDiagram::updateAfterMsgDelete(){
    for(int i = 0; i < msgList->count(); i++){
        QString textIndex = msgList->itemWidget(msgList->item(i))->findChild<QLabel*>()->text();
        int index = textIndex.remove(textIndex.size()-1, 1).toInt();

        if(index > i+1){
            msgList->itemWidget(msgList->item(i))->findChild<QLabel*>()->setText(QString::number(i + 1)+".");
            gMessages.at(i)->index = i + 1;
        }
        else
            msgList->itemWidget(msgList->item(i))->findChild<QLabel *>()->setText(QString::number(index) + ".");
    }

    emit updateMsgPos();
}

void GSequenceDiagram::addMsgItem(QListWidget *qList, GMessage *gMsg){
    QListWidgetItem *item = new QListWidgetItem();
    QFrame *itemFrame = new QFrame();
    itemFrame->setStyleSheet("QFrame { background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"\
                             "stop:0 #383C41, stop:0.92 #2A2F35, stop:1 #383C41);}");
    QHBoxLayout *itemLayout = new QHBoxLayout(itemFrame);
    itemLayout->setContentsMargins(0,0,0,0);
    itemLayout->setAlignment(Qt::AlignLeft);

    QLabel *indexLabel = new QLabel();
    indexLabel->setText(QString::number(gMsg->index)+"."); //gMsg->index
    indexLabel->setFixedWidth(23);
    indexLabel->setStyleSheet("QLabel { color: #F2804A;}");
    itemLayout->addWidget(indexLabel);

    QLabel *srcLabel = new QLabel();
    srcLabel->setText(gMsg->src->getName());
    srcLabel->setFixedWidth(70);
    itemLayout->addWidget(srcLabel);

    QLabel *arrowLabel = new QLabel();
    arrowLabel->setText(" -➤ ");
    arrowLabel->setFixedWidth(20);
    itemLayout->addWidget(arrowLabel);

    QLabel *dstLabel = new QLabel();
    dstLabel->setText(gMsg->dst->getName());
    dstLabel->setFixedWidth(70);
    itemLayout->addWidget(dstLabel);

    QLabel *funcLabel = new QLabel();
    funcLabel->setText(gMsg->getFuncName());
    funcLabel->setStyleSheet("QLabel { color: #4585CF;}");
    itemLayout->addWidget(funcLabel);

    qList->addItem(item);
    qList->setItemWidget(item, itemFrame);
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
    if(!src || !dst)
        return;

    GMessage::direction dir = GMessage::LTOL;
    if(srcIndex < dstIndex)
        dir = GMessage::LTOR;
    else if(srcIndex > dstIndex)
        dir = GMessage::RTOL;//todo

    // Add the message to gMessages vector and to a QListWidget
    // extract function name from the combobox
    auto funcName = boxes[3]->currentText().split(" ").value(1).split("(").value(0);
    if((!funcName.isEmpty() && boxes[4]->currentText() != "create" && boxes[4]->currentText() != "delete") ||
        (boxes[4]->currentText() == "create" || boxes[4]->currentText() == "delete")) {
        auto operation = dynamic_cast<UMLOperation*>(dst->model()->model()->getAttribute(funcName.toStdString()));
        UMLMessage *messageModel;
        if (operation != nullptr)
            messageModel = new UMLMessage(operation, src->model(), dst->model());
        else
            messageModel = new UMLMessage(funcName.toStdString(), src->model(), dst->model());
        messageModel->messageType() = boxes[4]->currentText().toStdString();
        sequanceDiagram->addMessage(messageModel);
//        GMessage gMessage = new GMessage()
//        auto gMsg = src->addMsg(funcName, dst, dir, boxes[4]->currentText(), index++);
        auto gMsg = new GMessage(scene, messageModel, dir, src, dst, index++);
        gMessages.push_back(gMsg); // Msg name boxes[3]
        addMsgItem(msgList, gMsg);

        connect(this, SIGNAL(updateMsgPos()), gMsg, SLOT(onUpdateMsgPos()));
    }
    else if(boxes[4]->currentText() != "create" || boxes[3]->currentText() != "delete"){
        // TODO:
    }
}

void GSequenceDiagram::onClassDiagramUpdated(){
    // Update ComboBox texts
    if(!settings->findChildren<QComboBox*>().empty()) {
        auto boxes = settings->findChildren<QComboBox *>();
        for(int i = 0; i < 4 && i < boxes.size(); i++) { // do not fill comboBoxTypes
            boxes[i]->clear();
            if(i == 0){
                for (auto cls: classDiagram->getClasses()) {
                    boxes[i]->addItem(QString::fromStdString(cls->name()));
                }
            }
            if(i == 1 || i == 2 && !gTimelines.empty()){
                for (auto tl: gTimelines) {
                    boxes[i]->addItem(QString::fromStdString(tl->model()->name()));
                }
            }
            else if(i == 3 && !gTimelines.empty()){
                // Update fuction list of the class of the instance at boxes[2]
                UMLClassifier *cls = getClassifByInst(boxes[2]->currentText());
                if(cls) {
                    for (auto funcs: cls->getOperations()) { // call 'to:' obj methods
                        boxes[i]->addItem(QString::fromStdString(std::string(*funcs)));
                    }
                }
            }
        }
    }
    emit classContentUpdated();

    // If a class function changed, text goes red
    for(auto msg: gMessages){
        bool containsFunc = false;
        for(auto op: getClassifByInst(msg->src->getName())->getOperations()){
            if(std::string(*op) == msg->getFuncName().toStdString())
                containsFunc = true;
        }
        if(!containsFunc && msg->getFuncName() != "DELETE" && msg->getFuncName() != "CREATE")
            msg->warn();
    }
}

UMLClassifier *GSequenceDiagram::getClassifByInst(QString instName){
    // UMLClassifier *cls = nullptr;
    // if(!gTimelines.empty()) {
    //     for (auto tl: gTimelines) {
    //         if (tl->model()->name() == instName.toStdString())
    //             cls = tl->cls;
    //     }
    // }
//    return cls;
    auto iter {std::find_if(gTimelines.begin(), gTimelines.end(),
        [instName](GTimeline *gt){ return gt->model()->name() == instName.toStdString(); })};
    if (iter == gTimelines.end())
        return nullptr;
    return (*iter)->model()->model();
}

void GSequenceDiagram::onFuncUpdate(){
    auto boxes = settings->findChildren<QComboBox *>();
    UMLClassifier *cls = getClassifByInst(boxes[2]->currentText());
    if(cls) { // classifier 'to:'
        boxes[3]->clear();
        // Its methods
        for (auto func: cls->getOperations()) { // call 'to:' obj methods
            // Combo box of functions
            boxes[3]->addItem(QString::fromStdString(std::string(*func)));
        }
    }
}
void GSequenceDiagram::onAddPressed(){
    if(!settings->findChild<QComboBox*>()->currentText().isEmpty()) {
        auto instName = settings->findChild<QLineEdit*>()->text();
        if(!instName.isEmpty() && sequanceDiagram->getObject(instName) == nullptr){
            auto instClass = dynamic_cast<UMLClass*>(classDiagram->getClassifier(settings->findChild<QComboBox *>()->currentText().toStdString()));

            auto objectModel = sequanceDiagram->addObject(instClass, instName.toStdString());
            // newObj->setModel(instClass);
            // TODO: add instance of gobject
            auto tl = new GTimeline(objectModel, scene, gTimelines.size() * HORIZONTAL_GAP, START_Y, NUM_INDEXES);
            gTimelines.push_back(tl);
            connect(tl, SIGNAL(gTimelineDeleted()), this, SLOT(onGTimelineDeleted()));
            connect(this, SIGNAL(classContentUpdated()), tl, SLOT(onClassContentUpdated()));

            onClassDiagramUpdated();
        }
    }
    settings->findChild<QLineEdit*>()->setText(""); // Clear instance name QLineEdit content
}

void GSequenceDiagram::onGTimelineDeleted() {
    auto tl = dynamic_cast<GTimeline *>(sender());

    deleteTimeline(tl);
}

void GSequenceDiagram::deleteBeforeClass(UMLClassifier *classifier){
    // Delete all gTimelines depending on the deleting class
    std::vector<GTimeline*> toDelete;
    for(auto tl: gTimelines) {
        if(tl->model()->model()->name() == classifier->name())
            toDelete.push_back(tl);
    }
    for(auto td: toDelete){
        deleteTimeline(td);
    }
}

void GSequenceDiagram::deleteTimeline(GTimeline *tl){
    if(!gTimelines.empty()) {
        gTimelines.erase(std::find(gTimelines.begin(), gTimelines.end(), tl));

        for(int i = gMessages.size()-1; i >= 0; i--){
            // Delete timelines messages
            if(gMessages.at(i)->src == tl || gMessages.at(i)->dst == tl) {
                auto m = gMessages.at(i);

                for(int j = 0; j < msgList->count(); j++){
                    QString textIndex = msgList->itemWidget(msgList->item(j))->findChild<QLabel*>()->text();
                    int msgIndex = textIndex.remove(textIndex.size()-1, 1).toInt(); // Remove dot
                    if(msgIndex == m->index){
                        auto msgWidget = msgList->item(j);
                        msgList->removeItemWidget(msgWidget);
                        delete msgWidget;
                        break;
                    }
                }
                gMessages.erase(gMessages.begin() + i);
                delete m;
                index--; // Lower index for new messages
            }
        }
    }
    sequanceDiagram->removeObject(tl->model()->name());
    delete tl;

    for(int i = 0; i < gTimelines.size(); i++){
        // Change positions of timelines, when some of them is removed
        gTimelines.at(i)->changePos(i*HORIZONTAL_GAP, 0);
    }

    // Recalculate message indexes
    updateAfterMsgDelete();
}
