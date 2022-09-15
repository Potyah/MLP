#include <gtest/gtest.h>

#include <array>

#include "../controller/controller.h"

using namespace s21;

Controller *controller = new Controller();

TEST(mlpTests, test1) {
    controller->setNetwork(Matrix, 7, "tests/testData.csv", "tests/testData.csv");
    controller->updateNetwork(Graph, 7, "tests/testData.csv", "tests/testData.csv");
    controller->testFile(1);
}

TEST(mlpTests, test2) {
    controller->setNetwork(Matrix, 7, "tests/testData.csv", "tests/testData.csv");
    controller->updateNetwork(Graph, 7);
    controller->deleteNetwork();
}

TEST(mlpTests, test3) {
    controller->setNetwork(Graph, 7, "tests/testData.csv", "tests/testData.csv");
    std::array<double, 6UL> testFile = controller->testFile(2);
    ASSERT_TRUE(testFile.at(0) != 0);
    controller->deleteNetwork();
}

TEST(mlpTests, test4) {
    controller->setNetwork(Matrix, 7, "tests/testData.csv", "tests/testData.csv");
    controller->trainingNetwork(5, 2);
    controller->deleteNetwork();
}

TEST(mlpTests, test5) {
    controller->setNetwork(Matrix, 7, "tests/testData.csv", "tests/testData.csv");
    controller->trainingNetwork(5, 2);
    controller->saveWeights("tests/weigths.cfg");
    controller->loadWeights("tests/weigths.cfg");
}

TEST(mlpTests, test6) {
    controller->setNetwork(Graph, 7, "tests/testData.csv", "tests/testData.csv");
    controller->trainingNetwork(0, 2);
    controller->saveWeights("tests/weigths.cfg");
    controller->loadWeights("tests/weigths.cfg");
}

TEST(mlpTests, test7) {
    controller->setNetwork(Graph, 7, "tests/testData.csv", "tests/testData.csv");
    controller->updateNetwork(Matrix, 7, "tests/testData.csv", "tests/testData.csv");
    controller->trainingNetwork(0, 1);
}

TEST(mlpTests, test8) {
    controller->setNetwork(Graph, 7, "tests/testData.csv", "tests/testData.csv");
    controller->updateNetwork(Matrix, 7);
}

TEST(mlpTests, test9) {
    CvsReader reader = CvsReader();
    reader.readFile("tests/testData.csv");
    controller->singleTest(reader.getVector().at(0));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}