#ifndef SRC_CONTROLLER_CONTROLLER_H_
#define SRC_CONTROLLER_CONTROLLER_H_
#include <vector>

#include "../common.h"
#include "../model/graphNet.h"
#include "../model/interfaceNet.h"
#include "../model/matrixNet.h"
#include "../model/neuraltransformations.h"

namespace s21 {
class Controller {
 public:
  Controller();
  ~Controller();
  void setNetwork(ModelType modelType, int layersCount, std::string pathTest,
                  std::string pathTrain);
  void deleteNetwork();
  void updateNetwork(ModelType modelType, int layersCount, std::string pathTest,
                     std::string pathTrain);
  void updateNetwork(ModelType modelType, int layersCount);
  std::array<double, 6> testFile(double sampleSize);
  std::vector<double> trainingNetwork(unsigned long crossValidationElements,
                                      unsigned int epoch);
  void loadWeights(std::string pathWeights);
  void saveWeights(std::string pathWeights);
  char singleTest(std::vector<double> pixels);
  void setPathTestFile(std::string pathTestFile);

 private:
  NeuralTransformations *p_Network;
};
}  // namespace s21
#endif  // SRC_CONTROLLER_CONTROLLER_H_
