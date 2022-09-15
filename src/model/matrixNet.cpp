#include "matrixNet.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace s21 {
MatrixNet::MatrixNet(int layersQuantity, double learningRate) {
  m_LayersQuantity = layersQuantity;
  m_LearningRate = learningRate;
  MatrixNet::initNet();
}

void MatrixNet::initNet() {
  m_trueResult = 0;
  m_inputsVectorCount = 0;
  m_hideLayer = m_LayersQuantity - 2;
  MatrixNet::setLearningRate(0.25);
  setLayersCount(m_LayersQuantity);
  setOutput();
  setWeights();
  setError();
  setDelta();
}

void MatrixNet::setInputLayerOutput(const std::vector<double> &outputs) {
  for (int i = 0; i < m_Output[0].getColumns(); i++) {
    m_Output[0](0, i) = outputs[i + 1] / 255;
  }
}

void MatrixNet::setLearningRate(double learningRate) {
  m_LearningRate = learningRate;
}

double MatrixNet::getLearningRate() { return m_LearningRate; }

int MatrixNet::getLayersCount() { return m_LayersCount; }

std::vector<double> MatrixNet::getOutLastLayer() {
  std::vector<double> rez;
  for (unsigned int i = 0; i < OUTPUT_LAYER; ++i) {
    rez.push_back(m_Output[m_Output.size() - 1].operator()(0, i));
  }
  return rez;
}

void MatrixNet::setLayersCount(int hidenLayersQuantity) {
  m_LayersCount = hidenLayersQuantity;
}

void MatrixNet::setOutput() {
  S21Matrix inputLayer(1, INPUT_LAYER);
  m_Output.push_back(inputLayer);

  for (int i = 0; i < m_hideLayer; i++) {
    S21Matrix hidenLayer(1, HIDE_LAYER);
    m_Output.push_back(hidenLayer);
  }

  S21Matrix outputLayer(1, OUTPUT_LAYER);
  m_Output.push_back(outputLayer);
}

void MatrixNet::setWeights() {
  S21Matrix inputLayer(1, 1);
  m_Weights.push_back(inputLayer);

  for (int i = 0; i < m_hideLayer; i++) {
    if (i == 0) {
      S21Matrix hidenLayer(INPUT_LAYER, HIDE_LAYER);
      hidenLayer.fillMatrixRandomValue();
      m_Weights.push_back(hidenLayer);
    } else {
      S21Matrix hidenLayer(HIDE_LAYER, HIDE_LAYER);
      hidenLayer.fillMatrixRandomValue();
      m_Weights.push_back(hidenLayer);
    }
  }

  S21Matrix outputLayer(HIDE_LAYER, OUTPUT_LAYER);
  outputLayer.fillMatrixRandomValue();
  m_Weights.push_back(outputLayer);
}

void MatrixNet::setError() {
  S21Matrix inputLayer(1, INPUT_LAYER);
  m_Error.push_back(inputLayer);

  for (int i = 0; i < m_hideLayer; i++) {
    S21Matrix hidenLayer(1, HIDE_LAYER);
    m_Error.push_back(hidenLayer);
  }

  S21Matrix outputLayer(1, OUTPUT_LAYER);
  m_Error.push_back(outputLayer);
}

void MatrixNet::setDelta() {
  S21Matrix inputLayer(1, INPUT_LAYER);
  m_Delta.push_back(inputLayer);

  for (int i = 0; i < m_hideLayer; i++) {
    S21Matrix hidenLayer(1, HIDE_LAYER);
    m_Delta.push_back(hidenLayer);
  }

  S21Matrix outputLayer(1, OUTPUT_LAYER);
  m_Delta.push_back(outputLayer);
}

void MatrixNet::predictions() {
  for (int i = 1; i < m_LayersCount; i++) {
    m_Output[i] = m_Output[i - 1].multWithSigmoid(m_Weights[i]);
  }
}

void MatrixNet::training(int expectedResult) {
  for (int i = m_LayersCount - 1; i > 0; --i) {
    if (i == m_LayersCount - 1) {
      calculateLastLayerError(expectedResult, i);
      calculateDelta(i);
      adjustWeight(i);
    } else {
      calculateError(i);
      calculateDelta(i);
      adjustWeight(i);
    }
  }
}

void MatrixNet::calculateError(int layer) {
  for (int i = 0; i < m_Error[layer].getColumns(); i++) {
    double error = 0.0;

    for (int j = 0; j < m_Output[layer + 1].getColumns(); j++) {
      error += m_Weights[layer + 1](i, j) * m_Delta[layer + 1](0, j);
    }

    m_Error[layer](0, i) = error;
  }
}

void MatrixNet::calculateLastLayerError(const int expectedResult,
                                        const int layer) {
  for (int i = 0; i < m_Output[layer].getColumns(); i++) {
    if (i == expectedResult - 1) {
      m_Error[layer](0, i) = m_Output[layer](0, i) - 1.0;
    } else {
      m_Error[layer](0, i) = m_Output[layer](0, i);
    }
  }
}

void MatrixNet::calculateDelta(const int layer) {
  for (int i = 0; i < m_Output[layer].getColumns(); i++) {
    double output = m_Output[layer](0, i);
    m_Delta[layer](0, i) = m_Error[layer](0, i) * (output * (1.0 - output));
  }
}

void MatrixNet::adjustWeight(int layer) {
  for (int i = 0; i < m_Output[layer].getColumns(); i++) {
    for (int j = 0; j < m_Output[layer - 1].getColumns(); j++) {
      double prevNeuronOut = m_Output[layer - 1](0, j);

      m_Weights[layer](j, i) =
          m_Weights[layer](j, i) -
          prevNeuronOut * m_Delta[layer](0, i) * m_LearningRate;
    }
  }
}

bool MatrixNet::checkTrainingResult(int expectedResult) {
  int prediction = getLetter() - 64;

  m_inputsVectorCount++;

  if (prediction == expectedResult) {
    m_trueResult++;
    return true;
  }

  return false;
}

char MatrixNet::getLetter() {
  double max = m_Output[m_LayersCount - 1](0, 0);
  int prediction = 0;
  double tmp;

  for (int j = 1; j < m_Output[m_LayersCount - 1].getColumns(); j++) {
    tmp = m_Output[m_LayersCount - 1](0, j);
    if (tmp > max) {
      prediction = j;
      max = tmp;
    }
  }

  return prediction + 65;
}

void MatrixNet::saveNet(std::string path) {
  std::fstream fs;
  fs.open(path, std::fstream::out);

  if (!fs.is_open()) {
    throw std::runtime_error("The file to save the neural network is not open");
  }

  fs << getLayersCount() << "\n";

  for (auto layer = m_Weights.begin() + 1; layer < m_Weights.end(); layer++) {
    fs << "layer"
       << "\n";

    for (int i = 0; i < layer->getColumns(); i++) {
      for (int j = 0; j < layer->getRows(); j++) {
        fs << layer->operator()(j, i) << " ";
      }

      fs << "\n";
    }
  }

  fs.close();
}

void MatrixNet::loadNet(std::string path) {
  std::ifstream fs;
  fs.open(path, std::fstream::in);

  if (!fs.is_open()) {
    throw std::runtime_error("The file to load the neural network is not open");
  }
  m_LayersCount = 0;

  fs >> m_LayersCount;
  m_hideLayer = m_LayersCount - 2;

  std::string line = "";
  std::getline(fs, line);

  int layerNumber = 0;
  int cols = 0;

  while (!fs.eof()) {
    std::getline(fs, line);

    if (line == "layer") {
      layerNumber++;
      cols = 0;
      continue;
    }

    std::istringstream st(line);
    double weight;
    int rows = 0;

    while (st >> weight) {
      m_Weights[layerNumber](rows, cols) = weight;
      rows++;
    }

    cols++;
  }

  fs.close();
}

}  //  namespace s21
