#ifndef SRC_MODEL_GRAPHNET_H_
#define SRC_MODEL_GRAPHNET_H_
#include "interfaceNet.h"
#include "cvsReader.h"


namespace s21 {
//  Класс создания нейронной сети на графах. Наследуется от интерфейса
class GraphNet : public InterfaceNet {
 public:
  //  Конструктор. Принимет колличество слоёв и шаг обучения
  GraphNet(int CountLayers, double LearningRate);
  ~GraphNet();

  int getLayersCount() override;
  double getLearningRate() override;
  void setLearningRate(double learningRate) override;
  void setInputLayerOutput(const std::vector<double> &outputs) override;
  void predictions() override;
  void training(int expectedResult) override;
  void saveNet(std::string path) override;
  void loadNet(std::string path) override;
  bool checkTrainingResult(int expectedResult) override;
  char getLetter() override;
  std::vector<double> getOutLastLayer() override;

 protected:
  void initNet() override;

 private:
  int m_LayersCount;
  double m_LearningRate;
  std::vector<Layer*> m_Layers;
};
}  // namespace s21
#endif  //  SRC_MODEL_GRAPHNET_H_
