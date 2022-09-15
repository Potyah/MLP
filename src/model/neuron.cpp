#include "neuron.h"

#include <random>

namespace s21 {
Neuron::Neuron(int countLayer, int previousLayerNeuronsQuantity) {
  if (countLayer > 0) {
    for (int i = 0; i < previousLayerNeuronsQuantity; ++i) {
      m_Weights.push_back(setRandomWeight());
    }
  }
}

double Neuron::setRandomWeight() {
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_real_distribution<double> distr(-1, 1);

  return distr(eng);
}

void Neuron::setOut(double data) { m_Out = data; }
double Neuron::getOut() { return m_Out; }

void Neuron::setError(double error) { m_Error = error; }
double Neuron::getError() { return m_Error; }

void Neuron::setDelta(double delta) { m_Delta = delta; }
double Neuron::getDelta() { return m_Delta; }

void Neuron::setWeight(int position, double newWeight) {
  m_Weights[position] = newWeight;
}

std::vector<double> &Neuron::getWeights() { return m_Weights; }

}  //  namespace s21
