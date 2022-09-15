#include "controller.h"

#include <vector>

namespace s21 {
Controller::Controller() { p_Network = nullptr; }

Controller::~Controller() { deleteNetwork(); }

void Controller::setNetwork(ModelType modelType, int layersCount,
                            std::string pathTest, std::string pathTrain) {
  p_Network =
      new NeuralTransformations(modelType, layersCount, pathTest, pathTrain);
}

void Controller::deleteNetwork() {
  if (p_Network != nullptr) {
    delete p_Network;
    p_Network = nullptr;
  }
}

void Controller::updateNetwork(ModelType modelType, int layersCount,
                               std::string pathTest, std::string pathTrain) {
  deleteNetwork();
  setNetwork(modelType, layersCount, pathTest, pathTrain);
}

void Controller::updateNetwork(ModelType modelType, int layersCount) {
  deleteNetwork();
  p_Network = new NeuralTransformations(modelType, layersCount);
}

std::array<double, 6> Controller::testFile(double sampleSize) {
  return p_Network->fileTest(sampleSize);
}

std::vector<double> Controller::trainingNetwork(
    unsigned long crossValidationElements, unsigned int epoch) {
  if (crossValidationElements > 0) {
    return p_Network->trainWithCrossValidation(crossValidationElements);
  } else {
    return p_Network->train(epoch);
  }
}

void Controller::loadWeights(std::string pathWeights) {
  p_Network->loadWeights(pathWeights);
}

void Controller::saveWeights(std::string pathWeights) {
  p_Network->saveWeights(pathWeights);
}

char Controller::singleTest(std::vector<double> pixels) {
  return p_Network->singleTest(pixels);
}

void Controller::setPathTestFile(std::string pathTestFile) {
  p_Network->setPathTestFile(pathTestFile);
}

}  // namespace s21
