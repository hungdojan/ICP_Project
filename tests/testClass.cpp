//
// Created by rebulien on 4/19/22.
//

#include <gtest/gtest.h>
#include <vector>
#include "ClassDiagram.h"
#include "UMLClassifier.h"
#include "UMLInterface.h"

class BasicTests : public ::testing::Test {
protected:
    ClassDiagram classDiagram{"Testing diagram"};
    void SetUp() final { }
    void TearDown() final { }
};

TEST_F(BasicTests, EmptyClassDiagram) {
    EXPECT_EQ(classDiagram.classElements().size(), 0);
}

TEST_F(BasicTests, AddClassifier) {
    std::vector<UMLClassifier *> lofClassifiers;
    // inserting 3 classes and 3 interfaces
    for (int i = 0; i < 3; i++) {
        std::ostringstream stream;
        stream << "Class" << i;
        auto classifier{ClassDiagram::createClassifier(stream.str(), ClassDiagram::ClassElementType::CLASS)};
        lofClassifiers.push_back(classifier);
        ASSERT_TRUE(classDiagram.addClassifier(classifier));
    }
    for (int i = 0; i < 3; i++) {
        std::ostringstream stream;
        stream << "Interface" << i;
        auto classifier{ClassDiagram::createClassifier(stream.str(), ClassDiagram::ClassElementType::INTERFACE)};
        lofClassifiers.push_back(classifier);
        ASSERT_TRUE(classDiagram.addClassifier(classifier));
    }

    // inserting already existing elements
    for (auto classifier : lofClassifiers) {
        ASSERT_FALSE(classDiagram.addClassifier(classifier));
    }
}

TEST_F(BasicTests, UMLClassifierFactory) {
    auto interface{ClassDiagram::createClassifier("interface", ClassDiagram::INTERFACE)};
    auto classifier{ClassDiagram::createClassifier("interface", ClassDiagram::CLASSIFIER)};
    auto _class{ClassDiagram::createClassifier("interface", ClassDiagram::CLASS)};

    ASSERT_NE(dynamic_cast<UMLInterface *>(interface), nullptr);
    ASSERT_NE(dynamic_cast<UMLClass *>(_class), nullptr);

    ASSERT_EQ(dynamic_cast<UMLClass *>(interface), nullptr);
    ASSERT_EQ(dynamic_cast<UMLClass *>(classifier), nullptr);

    ASSERT_EQ(dynamic_cast<UMLInterface *>(_class), nullptr);
    ASSERT_EQ(dynamic_cast<UMLInterface *>(classifier), nullptr);
    delete interface;
    delete classifier;
    delete _class;
}

TEST_F(BasicTests, GetClassifier) {
    // test of ClassDiagram::getClassifier and ClassDiagram::getClass functions
    classDiagram.addClassifier(ClassDiagram::createClassifier("classifier", ClassDiagram::CLASSIFIER));
    classDiagram.addClassifier(ClassDiagram::createClassifier("class", ClassDiagram::CLASS));

    ASSERT_NE(classDiagram.getClassifier("classifier"), nullptr);
    ASSERT_NE(classDiagram.getClassifier("class"), nullptr);
    ASSERT_NE(classDiagram.getClass("class"), nullptr);
    ASSERT_EQ(classDiagram.getClass("classifier"), nullptr);
}

TEST_F(BasicTests, MultipleClassifierInsertion) {
    UMLClassifier *cls1 = ClassDiagram::createClassifier("cls1", ClassDiagram::CLASSIFIER);
    UMLClassifier *cls2 = ClassDiagram::createClassifier("cls2", ClassDiagram::CLASSIFIER);
    UMLClass *cls3 = dynamic_cast<UMLClass *>(ClassDiagram::createClassifier("cls3", ClassDiagram::CLASS));
    // identical name as "cls2"
    UMLClass *cls4 = dynamic_cast<UMLClass *>(ClassDiagram::createClassifier("cls2", ClassDiagram::CLASS));

    classDiagram.addClassifiers({cls1, cls2, cls3, cls4});

    // class diagram should contain only cls1, cls2, cls3
    ASSERT_EQ(classDiagram.classElements().size(), 3);
    ASSERT_EQ(classDiagram.getClassifier("cls1"), cls1);
    ASSERT_EQ(classDiagram.getClassifier("cls2"), cls2);
    ASSERT_EQ(classDiagram.getClass("cls3"), cls3);
    ASSERT_EQ(classDiagram.getClass("cls4"), nullptr);
    delete cls4;
}

TEST_F(BasicTests, CaseSensitiveInsertion) {
    UMLClassifier *cls1{ClassDiagram::createClassifier("cls1", ClassDiagram::CLASSIFIER)};
    UMLClassifier *cls2{ClassDiagram::createClassifier("CLS1", ClassDiagram::CLASSIFIER)};
    UMLClassifier *cls3{ClassDiagram::createClassifier("cLs1", ClassDiagram::CLASSIFIER)};
    UMLClassifier *cls4{ClassDiagram::createClassifier("CLs1", ClassDiagram::CLASSIFIER)};
    // identical name as cls1
    UMLClassifier *cls5{ClassDiagram::createClassifier("cls1", ClassDiagram::CLASSIFIER)};

    classDiagram.addClassifiers({cls1, cls2, cls3, cls4, cls5});

    ASSERT_EQ(classDiagram.classElements().size(), 4);
    ASSERT_EQ(classDiagram.getClassifier("cls1"), cls1);
    ASSERT_EQ(classDiagram.getClassifier("CLS1"), cls2);
    ASSERT_EQ(classDiagram.getClassifier("cLs1"), cls3);
    ASSERT_EQ(classDiagram.getClassifier("CLs1"), cls4);

    ASSERT_FALSE(classDiagram.isInClassDiagram(cls5));
    delete cls5;
}

TEST_F(BasicTests, ReplaceClassifier) {
    UMLClassifier *cls1{ClassDiagram::createClassifier("cls1", ClassDiagram::CLASSIFIER)};
    UMLClassifier *cls2{ClassDiagram::createClassifier("cls1", ClassDiagram::CLASS)};

    // inserting classifier with name that is already in use will be ignored
    ASSERT_TRUE(classDiagram.addClassifier(cls1));
    ASSERT_FALSE(classDiagram.addClassifier(cls2));
    ASSERT_EQ(classDiagram.classElements().size(), 1);

    // but when using ClassDiagram::addOrReplaceClassifier method
    // old one should be replaced by new one
    ASSERT_TRUE(classDiagram.removeClassElement(cls1));
    ASSERT_EQ(classDiagram.addOrReplaceClassifier(cls1), nullptr);
    ASSERT_EQ(classDiagram.addOrReplaceClassifier(cls2), cls1);
    ASSERT_EQ(classDiagram.classElements().size(), 1);
    delete cls1;
}

TEST_F(BasicTests, RemoveClassifierByName) {
    UMLClassifier *cls1 = ClassDiagram::createClassifier("cls1", ClassDiagram::CLASSIFIER);
    UMLClassifier *cls2 = ClassDiagram::createClassifier("cls2", ClassDiagram::CLASSIFIER);
    UMLClass *cls3 = dynamic_cast<UMLClass *>(ClassDiagram::createClassifier("cls3", ClassDiagram::CLASS));
    UMLClass *cls4 = dynamic_cast<UMLClass *>(ClassDiagram::createClassifier("cls4", ClassDiagram::CLASS));

    classDiagram.addClassifiers({cls1, cls2, cls3, cls4});

    ASSERT_EQ(classDiagram.classElements().size(), 4);

    for (int i = 0; i < 4; i++) {
        std::ostringstream stream;
        stream << "cls" << i;
        if (i == 0)
            ASSERT_FALSE(classDiagram.removeClassElement(stream.str()));
        else
            ASSERT_TRUE(classDiagram.removeClassElement(stream.str()));
    }

    ASSERT_EQ(classDiagram.classElements().size(), 1);
    ASSERT_EQ(classDiagram.getClassifier("cls4"), cls4);
    ASSERT_TRUE(classDiagram.removeClassElement(cls4));
    delete cls1; delete cls2; delete cls3; delete cls4;
}

TEST_F(BasicTests, ChangeClassName) {
    UMLClassifier *cls1 = ClassDiagram::createClassifier("cls1", ClassDiagram::CLASSIFIER);
    UMLClassifier *cls2 = ClassDiagram::createClassifier("cls2", ClassDiagram::CLASSIFIER);
    UMLClass *cls3 = dynamic_cast<UMLClass *>(ClassDiagram::createClassifier("cls3", ClassDiagram::CLASS));
    UMLClass *cls4 = dynamic_cast<UMLClass *>(ClassDiagram::createClassifier("cls4", ClassDiagram::CLASS));
    UMLClass *cls5 = dynamic_cast<UMLClass *>(ClassDiagram::createClassifier("cls5", ClassDiagram::CLASS));

    classDiagram.addClassifiers({cls1, cls2, cls3, cls4});

    ASSERT_TRUE(classDiagram.changeClassifierName("cls1", "Cls1"));
    ASSERT_TRUE(classDiagram.changeClassifierName("cls2", "cls1"));
    ASSERT_FALSE(classDiagram.changeClassifierName("cls3", "cls4"));

    ASSERT_EQ(cls3, classDiagram.getClassifier("cls3"));
    ASSERT_EQ(cls4, classDiagram.getClassifier("cls4"));

    ASSERT_FALSE(classDiagram.changeClassifierName(cls5, "cls5"));
    ASSERT_FALSE(classDiagram.changeClassifierName(cls3, "cls4"));
    ASSERT_FALSE(classDiagram.changeClassifierName(cls3, "cls3"));
    ASSERT_TRUE(classDiagram.changeClassifierName(cls4, "cls5"));

    ASSERT_EQ(classDiagram.getClassifier("cls5"), cls4);
    ASSERT_EQ(classDiagram.getClassifier("cls3"), cls3);
    ASSERT_EQ(classDiagram.getClassifier("cls1"), cls2);
    ASSERT_EQ(classDiagram.getClassifier("Cls1"), cls1);

    delete cls5;
}