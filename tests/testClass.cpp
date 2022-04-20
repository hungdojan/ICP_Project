//
// Created by rebulien on 4/19/22.
//

#include <gtest/gtest.h>
#include "ClassDiagram.h"

class BasicTests : public ::testing::Test {
protected:
    ClassDiagram classDiagram{"Testing diagram"};
    void SetUp() final { }
    void TearDown() final { }
};

TEST_F(BasicTests, DiagramName) {
    EXPECT_EQ(classDiagram.name(), "Testing diagram");
}
