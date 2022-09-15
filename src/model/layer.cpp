#include "layer.h"

#include <cmath>

namespace s21 {

Layer::Layer(int layerNumber, unsigned long neuronsQuantity,
             unsigned long previousLayerNeuronsQuantity)
    : m_layerNumber(layerNumber), m_neuronsQuantity(neuronsQuantity) {
  for (unsigned long i = 0; i < neuronsQuantity; ++i) {
    Neuron* p_neuron = new Neuron(layerNumber, previousLayerNeuronsQuantity);
    m_Neurons.push_back(p_neuron);
  }
}

Layer::~Layer() {
  for (auto neuron : m_Neurons) {
    if (m_Neurons.size() > 0 && neuron != nullptr) {
      delete neuron;
    }
  }
}

std::vector<Neuron*>& Layer::getNeuron() { return m_Neurons; }

int Layer::getNeuronsQuantity() { return m_neuronsQuantity; }

int Layer::getLayerNumber() { return m_layerNumber; }

void Layer::calculateOutput(Layer* layer) {
  if (m_layerNumber > 0) {
    for (unsigned long j = 0; j < m_Neurons.size(); ++j) {
      double output = 0.0;
      for (int i = 0; i < layer->getNeuronsQuantity(); ++i) {
        double prevOut = layer->getNeuron()[i]->getOut();
        double neuronWeight = m_Neurons[j]->getWeights()[i];
        output += (prevOut * neuronWeight);
      }
      m_Neurons[j]->setOut(1.0 / (1.0 + exp(-output)));
    }
  }
}

void Layer::calculateLastLayerError(int expectedResult) {
  std::vector<double> rez;
  for (int i = 0; i < 26; ++i) {
    if (i == expectedResult - 1) {
      rez.push_back(1.0);
    } else {
      rez.push_back(0.0);
    }
  }

  for (int i = 0; i < 26; ++i) {
    double erLast = m_Neurons[i]->getOut() - rez[i];
    m_Neurons[i]->setError(erLast);
  }
}

void Layer::calculateDelta() {
  for (unsigned long i = 0; i < m_Neurons.size(); ++i) {
    double output = m_Neurons.at(i)->getOut();
    double delta = m_Neurons.at(i)->getError() * (output * (1.0 - output));
    m_Neurons.at(i)->setDelta(delta);
  }
}

void Layer::adjustWeight(Layer* prevLayer, double learningRate) {
  for (unsigned long j = 0; j < m_Neurons.size(); ++j) {
    for (unsigned long i = 0; i < m_Neurons.at(j)->getWeights().size(); ++i) {
      double prevNeuronOut = prevLayer->getNeuron()[i]->getOut();
      double newWeight =
          m_Neurons.at(j)->getWeights()[i] -
          (prevNeuronOut * m_Neurons.at(j)->getDelta() * learningRate);
      m_Neurons.at(j)->setWeight(i, newWeight);
    }
  }
}

void Layer::calculateError(Layer* previousLayer) {
  for (unsigned long i = 0; i < m_Neurons.size(); ++i) {
    double error = 0.0;

    for (unsigned long j = 0; j < previousLayer->getNeuron().size(); ++j) {
      error += previousLayer->getNeuron()[j]->getWeights()[i] *
               previousLayer->getNeuron()[j]->getDelta();
    }
    m_Neurons[i]->setError(error);
  }
}

}  // namespace s21
