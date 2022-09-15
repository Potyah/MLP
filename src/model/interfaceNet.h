#ifndef SRC_MODEL_INTERFACENET_H_
#define SRC_MODEL_INTERFACENET_H_

#include <string>
#include <vector>

#include "layer.h"

namespace s21 {
enum neuronsInLayerQuantity {
  INPUT_LAYER = 784,
  HIDE_LAYER = 140,
  OUTPUT_LAYER = 26
};

class InterfaceNet {
 public:
  InterfaceNet() {}
  virtual ~InterfaceNet() {}

  virtual int getLayersCount() = 0;
  virtual double getLearningRate() = 0;
  virtual void setLearningRate(double learningRate) = 0;

  virtual void predictions() = 0;
  virtual void training(int expectedResult) = 0;
  virtual void saveNet(std::string path) = 0;
  virtual void loadNet(std::string path) = 0;
  virtual bool checkTrainingResult(int expectedResult) = 0;
  virtual char getLetter() = 0;
  virtual void setInputLayerOutput(const std::vector<double> &outputs) = 0;
  virtual std::vector<double> getOutLastLayer() = 0;

 protected:
  virtual void initNet() = 0;
};
}  // namespace s21
#endif  //  SRC_MODEL_INTERFACENET_H_

