#ifndef SRC_MODEL_MATRIXNET_H_
#define SRC_MODEL_MATRIXNET_H_

#include "interfaceNet.h"
#include "s21_matrix_oop.h"

namespace s21 {
class MatrixNet : public InterfaceNet {
 public:
  MatrixNet(int layersQuantity, double learningRate);
  ~MatrixNet() {}

  void setInputLayerOutput(const std::vector<double> &outputs) override;
  void predictions() override;
  void training(int expectedResult) override;
  void saveNet(std::string path) override;
  void loadNet(std::string path) override;
  bool checkTrainingResult(int expectedResult) override;
  char getLetter() override;
  void setLearningRate(double learningRate) override;
  double getLearningRate() override;
  int getLayersCount() override;
  std::vector<double> getOutLastLayer() override;

 protected:
  void initNet() override;

 private:
  int m_LayersCount;
  int m_hideLayer;
  double m_LearningRate;
  int m_LayersQuantity;

  size_t m_trueResult;
  size_t m_inputsVectorCount;

  std::vector<S21Matrix> m_Weights;
  std::vector<S21Matrix> m_Output;
  std::vector<S21Matrix> m_Error;
  std::vector<S21Matrix> m_Delta;

  void setLayersCount(int hidenLayersQuantity);
  void setWeights();
  void setOutput();
  void setError();
  void setDelta();

  void calculateDelta(const int layer);
  void adjustWeight(const int layer);
  void calculateLastLayerError(int expectedResult, const int layer);
  void calculateError(int layer);
};
}  //  namespace s21
#endif  //  SRC_MODEL_MATRIXNET_H_
