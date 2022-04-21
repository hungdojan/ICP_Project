//
// Created by rebulien on 4/19/22.
//

#include <gtest/gtest.h>
#include "ClassDiagram.h"
#include "UMLClassifier.h"

class BasicTests : public ::testing::Test {
protected:
    ClassDiagram classDiagram{"Testing diagram"};
    void SetUp() final { }
    void TearDown() final { }
};

TEST_F(BasicTests, DiagramName) {
    EXPECT_EQ(classDiagram.name(), "Testing diagram");
}

TEST_F(BasicTests, ChangeName) {
   classDiagram.setName("ahoj");
   EXPECT_EQ(classDiagram.name(), "ahoj");
}

TEST_F(BasicTests, ChangeClassifierName) {
    auto classifier {classDiagram.createClassifier("ahoj", false)};
    EXPECT_EQ(classifier->name(), "ahoj");
    classifier->setName("pes");
    EXPECT_EQ(classifier->name(), "pes");

}
