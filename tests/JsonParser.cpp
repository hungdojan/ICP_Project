//
// Created by rebulien on 4/24/22.
//

#include <gtest/gtest.h>
#include "ClassDiagram.h"
#include "SequenceDiagram.h"
#include "UMLInterface.h"
#include "JsonParser.h"

class JsonParserTest : public ::testing::Test {
protected:
    ClassDiagram classDiagram{"ClassDiagram"};
    virtual void SetUp() {
        classDiagram.addClassifier(ClassDiagram::createClassifier("void", ClassDiagram::CLASSIFIER));
        classDiagram.addClassifier(ClassDiagram::createClassifier("cls1", ClassDiagram::CLASS));
        classDiagram.addClassifier(ClassDiagram::createClassifier("cls2", ClassDiagram::CLASS));
        classDiagram.addClassifier(ClassDiagram::createClassifier("inter1", ClassDiagram::INTERFACE));
        classDiagram.addClassifier(ClassDiagram::createClassifier("inter2", ClassDiagram::INTERFACE));

        dynamic_cast<UMLClass *>(classDiagram.getClassifier("cls1"))->addAttributes({
            UMLClassifier::createAttribute(false, "attr1", classDiagram.getClassifier("cls2"), {}),
            UMLClassifier::createAttribute(false, "attr2", classDiagram.getClassifier("inter1"), {}),
            UMLClassifier::createAttribute(true, "oper1", classDiagram.getClassifier("inter1"), {
                UMLClassifier::createAttribute(false, "param1", classDiagram.getClassifier("inter2"), {}),
                UMLClassifier::createAttribute(false, "param2", classDiagram.getClassifier("inter2"), {})
            })
        }, true);
        dynamic_cast<UMLClass *>(classDiagram.getClassifier("cls2"))->addAttributes({
            UMLClassifier::createAttribute(false, "attr1", classDiagram.getClassifier("void"), {}),
            dynamic_cast<UMLOperation *>(UMLClassifier::createAttribute(true, "oper1", classDiagram.getClassifier("inter1"), {
                UMLClassifier::createAttribute(false, "param1", classDiagram.getClassifier("inter2"), {}),
                        UMLClassifier::createAttribute(false, "param2", classDiagram.getClassifier("inter1"), {})
            }))
        }, true);
        dynamic_cast<UMLInterface *>(classDiagram.getClassifier("inter1"))->addOperations({
            dynamic_cast<UMLOperation*>(UMLClassifier::createAttribute(true, "oper1", classDiagram.getClassifier("cls2"), {}))
        });
        dynamic_cast<UMLInterface *>(classDiagram.getClassifier("inter2"))->addOperations({
            dynamic_cast<UMLOperation *>(UMLClassifier::createAttribute(true, "oper1", classDiagram.getClassifier("cls2"), {})),
            dynamic_cast<UMLOperation *>(UMLClassifier::createAttribute(true, "oper2", classDiagram.getClassifier("inter2"), {
                UMLClassifier::createAttribute(false, "param1", classDiagram.getClassifier("inter2"), {}),
            })),
            dynamic_cast<UMLOperation *>(UMLClassifier::createAttribute(true, "oper3", classDiagram.getClassifier("inter1"), {
                UMLClassifier::createAttribute(false, "param1", classDiagram.getClassifier("inter2"), {}),
                UMLClassifier::createAttribute(false, "param2", classDiagram.getClassifier("inter1"), {})
            }))
        });

        auto sd1 = classDiagram.addSequenceDiagram("SequenceDiagram1");
        auto obj1 = sd1->addObject(dynamic_cast<UMLClass*>(classDiagram.getClassifier("cls1")), "obj1");
        auto obj2 = sd1->addObject(dynamic_cast<UMLClass*>(classDiagram.getClassifier("cls2")), "obj2");
        auto msg1 = sd1->addMessage(obj1, obj2);
        msg1->setOperation("oper1");
        auto msg2 = sd1->addMessage(obj2, obj1);
        msg2->setText("response");
        auto msg3 = sd1->addMessage(nullptr, obj1);
        msg3->setOperation("oper1");
        auto msg4 = sd1->addMessage(obj1, obj1);
        msg4->setOperation("oper1");
        auto sd2 = classDiagram.addSequenceDiagram("SequenceDiagram2");
        sd2->addObject(dynamic_cast<UMLClass*>(classDiagram.getClassifier("cls1")), "obj1");
        sd2->addObject(dynamic_cast<UMLClass*>(classDiagram.getClassifier("cls1")), "obj2");
    }
    virtual void TearDown() { }
};

TEST_F(JsonParserTest, GenerateJson) {
    JsonParser::saveToFile(classDiagram, "output.json");
    ClassDiagram cls{""};
    JsonParser::initFromFile(cls, "output.json");
    JsonParser::saveToFile(cls, "output2.json");
}