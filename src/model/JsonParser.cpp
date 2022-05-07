/**
 * @brief Definition of JsonParser functions.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file JsonParser.cpp
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#include "JsonParser.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <map>
#include "UMLClassifier.h"
#include "UMLClass.h"
#include "UMLInterface.h"
#include "UMLOperation.h"
#include "UMLAttribute.h"
#include "UMLRelation.h"
#include "UMLMessage.h"

/**
 * @brief Loads attributes from the JSON file.
 * @param qAttribute Representation of attribute object in Json file.
 * @param attribute Reference to instance of UMLAttribute.
 * @param mapOfClassifiers Map of available Classifiers in the class.
 */
void loadAttribute(QJsonObject &qAttribute, UMLAttribute &attribute,
                   const std::unordered_map<std::string, UMLClassifier *> &mapOfClassifiers) {
    // setup attribute
    if (qAttribute["_class"].toString().toStdString() != "UMLAttribute")
        throw std::invalid_argument("Expected UMLAttribute, received something else!");

    auto iter = mapOfClassifiers.find(qAttribute["type"].toString().toStdString());
    // class element not found
    if (iter == mapOfClassifiers.end())
        return;

    attribute.setName(qAttribute["name"].toString().toStdString());
    attribute.setType(iter->second);
    attribute.visibility() = qAttribute["visibility"].toInt();
}

/**
 * @brief Loads operation from the JSON file.
 * @param qOperation Representation of operation object in JSON file.
 * @param operation Reference to instance of UMLOperation.
 * @param mapOfClassifiers Map of available classifiers in the class.
 */
void loadOperation(QJsonObject &qOperation, UMLOperation &operation,
                   const std::unordered_map<std::string, UMLClassifier *> &mapOfClassifiers) {
    if (qOperation["_class"].toString().toStdString() != "UMLOperation")
        throw std::invalid_argument("Expected UMLOperation, received something else!");

    // type class element not found
    auto iterType{mapOfClassifiers.find(qOperation["type"].toString().toStdString())};
    if (iterType == mapOfClassifiers.end())
        return;
    // setup operation
    operation.setName(qOperation["name"].toString().toStdString());
    operation.setType(iterType->second);
    operation.visibility() = qOperation["visibility"].toInt();

    // add parameters
    QJsonArray qParameters = qOperation["parameters"].toArray();
    for (auto param : qParameters) {
        QJsonObject qParam = param.toObject();
        auto *parameter = new UMLAttribute("", nullptr);
        loadAttribute(qParam, *parameter, mapOfClassifiers);
        operation.addParameter(parameter);
    }
}

/**
 * @brief Loads class's attributes and operations from the JSON file.
 * @param qClassElement Representation of class object in JSON file.
 * @param mapOfClassifiers Map of available classifiers in the class.
 */
void loadClassAttributes(QJsonObject &qClassElement,
                        const std::unordered_map<std::string, UMLClassifier *> &mapOfClassifiers) {
    // setup class
    UMLClass *element = dynamic_cast<UMLClass *>(mapOfClassifiers.find(qClassElement["name"].toString().toStdString())->second);
    QJsonArray attributes = qClassElement["attributes"].toArray();

    // add attributes
    for (auto attr : attributes) {
        QJsonObject qAttr = attr.toObject();
        if (qAttr["_class"].toString().toStdString() == "UMLOperation") {
            auto *o = new UMLOperation("", nullptr);
            loadOperation(qAttr, *o, mapOfClassifiers);
            element->addAttribute(o);
        }
        else if (qAttr["_class"].toString().toStdString() == "UMLAttribute") {
            auto *a = new UMLAttribute("", nullptr);
            loadAttribute(qAttr, *a, mapOfClassifiers);
            element->addAttribute(a);
        }
    }
}

/**
 * @brief Loads interface's operations from the JSON file.
 * @param qClassElement Representation of interface object in JSON file.
 * @param mapOfClassifiers Map of available classifiers in the class.
 */
void loadInterfaceOperations(QJsonObject &qClassElement,
                        const std::unordered_map<std::string, UMLClassifier *> &mapOfClassifiers) {
    // setup interface
    auto iter = mapOfClassifiers.find(qClassElement["name"].toString().toStdString());
    // interface not found
    if (iter == mapOfClassifiers.end())
        return;
    UMLInterface *element = dynamic_cast<UMLInterface *>(mapOfClassifiers.find(qClassElement["name"].toString().toStdString())->second);
    QJsonArray operations = qClassElement["operations"].toArray();

    // add operations
    for (auto o : operations) {
        QJsonObject qOperation = o.toObject();
        auto *op = new UMLOperation("", nullptr);
        loadOperation(qOperation, *op, mapOfClassifiers);
        element->addOperation(op);
    }
}

/**
 * @brief Loads class elements from file into class diagram.
 * @param classDiagram Reference of class diagram.
 * @param buffer Buffer of relations stored JSON file.
 */
void loadClassElements(ClassDiagram &classDiagram, QJsonArray &buffer) {
    std::unordered_map<std::string, UMLClassifier *> mapOfClassifiers;
    for (auto item : buffer) {
        // extract classifier's data
        QJsonObject qClassElement{item.toObject()};
        std::string name{qClassElement["name"].toString().toStdString()};
        bool isAbstract = qClassElement["isAbstract"].toBool();
        double x = qClassElement["x"].toDouble();
        double y = qClassElement["y"].toDouble();

        // create instance of classifier and add to class diagram
        UMLClassifier *element;
        if (qClassElement["_class"].toString().toStdString() == "UMLClass") {
            element = ClassDiagram::createClassifier(name, ClassDiagram::CLASS);
        }
        else if (qClassElement["_class"].toString().toStdString() == "UMLInterface") {
            element = ClassDiagram::createClassifier(name, ClassDiagram::INTERFACE);
        }
        else {
            element = ClassDiagram::createClassifier(name, ClassDiagram::CLASSIFIER);
        }
        element->isAbstract() = isAbstract;
        element->x() = x;
        element->y() = y;
        mapOfClassifiers.insert({name, element});
        classDiagram.addClassifier(element);
    }
    // load attributes and operations of class elements
    for (auto item : buffer) {
        QJsonObject qClassElement{item.toObject()};
        if (qClassElement["_class"].toString().toStdString() == "UMLClass") {
            loadClassAttributes(qClassElement, mapOfClassifiers);
        }
        else if (qClassElement["_class"].toString().toStdString() == "UMLInterface") {
            loadInterfaceOperations(qClassElement, mapOfClassifiers);
        }
    }
}

void loadSequenceDiagrams(ClassDiagram &classDiagram, QJsonArray &buffer) {
    for (auto sd : buffer) {
        std::map<int, UMLMessage *> messages;
        QJsonObject qSequenceDiagram = sd.toObject();
        if (qSequenceDiagram["_class"] != "SequenceDiagram")
            throw std::invalid_argument("Expected SequenceDiagram, received something else!");
        std::string name{qSequenceDiagram["name"].toString().toStdString()};
        auto sequenceDiagram = new SequenceDiagram(name, classDiagram);
        for (auto o : qSequenceDiagram["objects"].toArray()) {
            QJsonObject qObject = o.toObject();
            if (qObject["_class"] != "UMLObject")
                throw std::invalid_argument("Expected UMLObject, received something else!");
            std::string objName{qObject["name"].toString().toStdString()};
            UMLClass *baseModel;
            if (qObject["model"].toString().toStdString() == "#UNDEF")
                baseModel = SequenceDiagram::undefClass;
            else
                baseModel = dynamic_cast<UMLClass*>(classDiagram.getClassifier(qObject["model"].toString().toStdString()));

            if (baseModel == nullptr)
                continue;
            sequenceDiagram->addObject(baseModel, objName);
        }
        for (auto m : qSequenceDiagram["messages"].toArray()) {
            QJsonObject qMessage = m.toObject();
            if (qMessage["_class"] != "UMLMessage")
                throw std::invalid_argument("Expected UMLMessage, received something else!");

            std::string msgName = qMessage["name"].toString().toStdString();
            UMLObject *src = nullptr;
            UMLObject *dst = nullptr;
            UMLOperation *baseOperation = nullptr;
            std::string msgSrc = qMessage["src"].toString().toStdString();
            if (msgSrc != "#UNDEF") {
                src = sequenceDiagram->getObject(msgSrc);
            }
            std::string msgDst = qMessage["dst"].toString().toStdString();
            if (msgDst != "#UNDEF") {
                dst = sequenceDiagram->getObject(msgDst);
            }
            std::string msgBaseOperation = qMessage["baseOperation"].toString().toStdString();
            if (msgBaseOperation != "#UNDEF" && dst != nullptr) {
                baseOperation = dynamic_cast<UMLOperation *>(dst->model()->getAttribute(msgBaseOperation));
            }
            try {
                auto message = new UMLMessage(baseOperation, src, dst);
                message->setText(msgName);
                message->messageType() = qMessage["messageType"].toString().toStdString();
                messages.insert({qMessage["index"].toInt(), message});
            } catch (std::invalid_argument &e) { }
        }
        for (auto m : messages) {
            if (!sequenceDiagram->addMessage(m.second))
                delete m.second;
        }
        if (!classDiagram.addSequenceDiagram(sequenceDiagram)) {
            delete sequenceDiagram;
        }
    }
}

/**
 * @brief Loads relations between class elements into class diagram.
 * @param classDiagram Reference of class diagram.
 * @param buffer Buffer of relations stored in JSON file.
 */
void loadRelations(ClassDiagram &classDiagram, QJsonArray &buffer) {
    for (auto relation : buffer) {
        QJsonObject qRelation = relation.toObject();
        if (qRelation["_class"].toString().toStdString() != "UMLRelation")
            throw std::invalid_argument("Expected UMLRelation, received something else!");
        // extract information from the JSON file
        std::string dst{qRelation["dst"].toString().toStdString()};
        std::string src{qRelation["src"].toString().toStdString()};
        int relationType{qRelation["relationType"].toInt()};
        std::string dstMsg{qRelation["dstMsg"].toString().toStdString()};
        std::string srcMsg{qRelation["srcMsg"].toString().toStdString()};

        // create relation and set it up
        // relations between undefined class elements are ignored
        auto srcElement = classDiagram.getClassifier(src);
        if (srcElement == nullptr)
            continue;
        auto dstElement = classDiagram.getClassifier(dst);
        if (dstElement == nullptr)
            continue;
        auto rel = srcElement->addRelation(dstElement);
        if (rel == nullptr)
            continue;
        rel->setRelationType(static_cast<UMLRelation::UMLRelationType>(relationType));
        rel->dstMsg() = dstMsg;
        rel->srcMsg() = srcMsg;
    }
}

void JsonParser::initFromFile(ClassDiagram &classDiagram, const std::string &filePath) {
    // load content from file
    std::ifstream file{filePath};
    std::stringstream buffer;
    buffer << file.rdbuf();
    QJsonDocument doc{QJsonDocument::fromJson(QString::fromStdString(buffer.str()).toUtf8())};
    file.close();

    // set up class diagram
    QJsonObject qClassDiagram = doc.object();
    if (qClassDiagram["_class"].toString().toStdString() != "ClassDiagram")
        throw std::invalid_argument("Json file doesn't contain main ClassDiagram object!");
    classDiagram.setName(qClassDiagram["name"].toString().toStdString());

    // load class elements
    QJsonArray qClassElements = qClassDiagram["classElements"].toArray();
    loadClassElements(classDiagram, qClassElements);

    // load sequence diagrams
    QJsonArray qSequenceDiagrams = qClassDiagram["sequenceDiagrams"].toArray();
    loadSequenceDiagrams(classDiagram, qSequenceDiagrams);

    // load relations
    QJsonArray qRelations = qClassDiagram["relations"].toArray();
    loadRelations(classDiagram, qRelations);
}

void JsonParser::saveToFile(ClassDiagram &classDiagram, const std::string &filePath) {
    // load class diagram content
    QJsonObject mainObject;
    classDiagram.createJsonObject(mainObject);

    // write to file
    QJsonDocument doc;
    doc.setObject(mainObject);
    std::ofstream file;
    file.open(filePath);
    file << doc.toJson().toStdString();
    file.close();
}
