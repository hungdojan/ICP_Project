//
// Created by rebulien on 4/22/22.
//
#include <gtest/gtest.h>
#include "ClassDiagram.h"
#include "UMLClassifier.h"
#include "UMLClass.h"
#include "UMLInterface.h"
#include "UMLRelation.h"
#include "UMLOperation.h"
#include "JsonParser.h"
#include <iostream>

class ClassifierTests : public ::testing::Test {
protected:
    ClassDiagram base{"ClassDiagram"};
    UMLClass *cls1, *cls2;
    UMLInterface *inter1, *inter2;
    void SetUp() final {
        cls1 = dynamic_cast<UMLClass *>(ClassDiagram::createClassifier("cls1", ClassDiagram::CLASS));
        cls2 = dynamic_cast<UMLClass *>(ClassDiagram::createClassifier("cls2", ClassDiagram::CLASS));
        inter1 = dynamic_cast<UMLInterface *>(ClassDiagram::createClassifier("inter1", ClassDiagram::INTERFACE));
        inter2 = dynamic_cast<UMLInterface *>(ClassDiagram::createClassifier("inter2", ClassDiagram::INTERFACE));

        base.addClassifiers({cls1, cls2, inter1, inter2}, true);
    }
    void TearDown() final {

    }
};

TEST_F(ClassifierTests, NumberOfElements) {
    EXPECT_EQ(base.classElements().size(), 4);
}

TEST_F(ClassifierTests, AddAttribute) {
    UMLAttribute *attr = UMLClassifier::createAttribute(false, "Attr1", inter1, {});
    ASSERT_TRUE(cls1->addAttribute(attr));

    UMLAttribute *attr2;
    EXPECT_NE(attr2 = cls1->getAttribute("Attr1"), nullptr);
    EXPECT_EQ(attr2, attr);

    EXPECT_TRUE(base.changeClassifierName(inter1, "ahoj"));
    EXPECT_EQ(attr2->type()->name(), "ahoj");
}

TEST_F(ClassifierTests, AddMultipleAttributes) {
    UMLAttribute *attr1 = UMLClassifier::createAttribute(false, "attr1", cls2, {});
    UMLAttribute *attr2 = UMLClassifier::createAttribute(false, "attr2", inter1, {});
    UMLAttribute *attr3 = UMLClassifier::createAttribute(false, "attr3", inter2, {});
    UMLAttribute *attr4 = UMLClassifier::createAttribute(false, "attr2", cls2, {});

    cls1->addAttributes({attr1, attr2, attr3, attr4}, false);

    EXPECT_EQ(cls1->attributes().size(), 3);
    EXPECT_NE(cls1->getAttribute("attr2"), attr4);
    delete attr4;
}

TEST_F(ClassifierTests, AddOperation) {
    UMLAttribute *o1 = UMLClassifier::createAttribute(true, "operation1", cls2, {});
    UMLAttribute *o2 = UMLClassifier::createAttribute(true, "operation2", inter1, {new UMLAttribute{"param1", cls1}, new UMLAttribute{"param2", inter1}});

    cls1->addAttributes({o1, o2}, true);

    EXPECT_EQ(cls1->getOperations().size(), 2);
    EXPECT_NE(dynamic_cast<UMLOperation *>(cls1->getAttribute("operation1")), nullptr);
    UMLOperation *op2;
    EXPECT_EQ(o2, op2=dynamic_cast<UMLOperation *>(cls1->getAttribute("operation2")));
    auto classifier = new UMLClassifier("clsfr");
    EXPECT_TRUE(base.addClassifier(classifier));
    EXPECT_TRUE(dynamic_cast<UMLOperation *>(o1)->addParameter(new UMLAttribute("attr2", classifier)));
}

TEST_F(ClassifierTests, RemoveClass) {
    ASSERT_TRUE(cls1->addAttribute(UMLClassifier::createAttribute(false, "attr1", cls2, {})));
    EXPECT_EQ(cls1->getAttribute("attr1")->type(), cls2);
    ASSERT_TRUE(base.removeClassElement(cls2));
    delete cls2;
    EXPECT_EQ(cls1->getAttribute("attr1")->type(), ClassDiagram::undefClassifier_);
}

TEST_F(ClassifierTests, JsonSave) {
    auto usedClassifier = ClassDiagram::createClassifier("used", ClassDiagram::CLASSIFIER);
    auto unusedClassifier = ClassDiagram::createClassifier("unused", ClassDiagram::CLASSIFIER);

    base.addClassifiers({unusedClassifier, usedClassifier}, false);

    cls1->addAttribute(UMLClassifier::createAttribute(false, "attr1", usedClassifier, {}));
    JsonParser::saveToFile(base, "test.json");
}

TEST_F(ClassifierTests, AddRelation) {
    ASSERT_TRUE(cls1->addRelation(cls1));
    ASSERT_TRUE(cls1->addRelation(cls2));
    ASSERT_TRUE(cls1->addRelation(inter1));
    ASSERT_TRUE(cls1->addRelation(inter2));
    EXPECT_EQ(cls1->relations().size(), 4);
    EXPECT_EQ(cls2->relations().size(), 1);
    EXPECT_EQ(inter1->relations().size(), 1);
    EXPECT_EQ(inter2->relations().size(), 1);

    EXPECT_TRUE(base.removeClassElement(cls1));
    delete cls1;
    EXPECT_EQ(cls2->relations().size(), 0);
    EXPECT_EQ(inter1->relations().size(), 0);
    EXPECT_EQ(inter2->relations().size(), 0);
}
