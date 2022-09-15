#include "neuraltransformations.h"

#include <time.h>

namespace s21 {
NeuralTransformations::NeuralTransformations(ModelType type, int layersCount,
                                             std::string pathTest,
                                             std::string pathTrain) {
  m_pathTestFile = pathTest;
  m_pathTrainFile = pathTrain;
  m_type = type;
  m_layersCount = layersCount;
  if (type == Graph) {
    p_net = new GraphNet(layersCount, 0.12);
  } else {
    p_net = new MatrixNet(layersCount, 0.12);
  }
}

NeuralTransformations::NeuralTransformations(ModelType type, int layersCount) {
  m_type = type;
  m_layersCount = layersCount;
  if (type == Graph) {
    p_net = new GraphNet(layersCount, 0.12);
  } else {
    p_net = new MatrixNet(layersCount, 0.12);
  }
}

std::array<double, 6> NeuralTransformations::fileTest(double sampleSize) {
  CvsReader reader;
  reader.readFile(m_pathTestFile);
  unsigned long allCount = 0, countPlus = 0, countMinus = 0, truePositive = 0;
  unsigned long trueNegative = 0, falsePositive = 0, falseNegative = 0;
  unsigned long stopTest = reader.getCountString() * sampleSize;
  clock_t start, end;
  start = clock();
  for (auto vec : reader.getVector()) {
    p_net->setInputLayerOutput(vec);
    p_net->predictions();
    double outRez = p_net->getOutLastLayer()[p_net->getLetter() - 65];
    if (p_net->checkTrainingResult(vec.at(0))) {
      ++countPlus;
      if (outRez > 0.5) {
        ++truePositive;
      } else if (outRez < 0.5) {
        ++trueNegative;
      }
    } else {
      if (outRez > 0.5) {
        ++falsePositive;
      } else if (outRez < 0.5) {
        ++falseNegative;
      }
      ++countMinus;
    }
    ++allCount;
    if (allCount == stopTest) {
      break;
    }
  }
  end = clock();
  double testError = (double)countMinus / (double)allCount;
  m_accuracy = (double)countPlus / (double)allCount;
  m_precision =
      (double)truePositive / ((double)truePositive + (double)trueNegative);
  m_recall =
      (double)truePositive / ((double)truePositive + (double)falseNegative);
  m_f_measure = 2 * (m_precision * m_recall / (m_precision + m_recall));
  m_time = double(end - start) / CLOCKS_PER_SEC;
  std::array<double, 6> rez{testError, m_accuracy,  m_precision,
                            m_recall,  m_f_measure, m_time};
  return rez;
}

std::vector<double> NeuralTransformations::train(unsigned int epoch) {
  std::vector<double> epochError;
  CvsReader reader;
  reader.readFile(m_pathTrainFile);
  for (unsigned int i = 0; i < epoch; i++) {
    for (auto vec : reader.getVector()) {
      p_net->setInputLayerOutput(vec);
      p_net->predictions();
      p_net->training((int)vec.at(0));
    }
    std::array<double, 6> rezEpoch = fileTest(1);
    epochError.push_back(rezEpoch[0]);
  }
  return epochError;
}

std::vector<double> NeuralTransformations::trainWithCrossValidation(
    unsigned long crossValidationElements) {
  CvsReader reader;
  reader.readFile(m_pathTrainFile);
  unsigned long countString = reader.getCountString();
  std::vector<double> crossValidationError;
  unsigned long jump = countString / crossValidationElements;
  for (unsigned int i = 0; i < crossValidationElements; ++i) {
    for (unsigned long j = 0; j < countString; ++j) {
      if (j == countString - (jump * (i + 1))) {
        j += jump;
      }
      if (j < countString) {
        std::vector<double> buf = reader.getVector()[j];
        p_net->setInputLayerOutput(buf);
        p_net->predictions();
        p_net->training(buf.at(0));
      }
    }
    unsigned long countMinus = 0;
    for (unsigned long k = countString - (jump * (i + 1));
         k < countString - jump * i; ++k) {
      std::vector<double> tmp = reader.getVector()[k];
      int rez = (int)singleTest(tmp) - 64;
      if (rez != (int)tmp.at(0)) ++countMinus;
    }
    crossValidationError.push_back((double)countMinus /
                                   ((double)countString - (double)jump));
  }
  return crossValidationError;
}

char NeuralTransformations::singleTest(std::vector<double> pixels) {
  p_net->setInputLayerOutput(pixels);
  p_net->predictions();
  return p_net->getLetter();
}

void NeuralTransformations::loadWeights(std::string pathWeights) {
  p_net->loadNet(pathWeights);
}

void NeuralTransformations::saveWeights(std::string pathWeights) {
  p_net->saveNet(pathWeights);
}

void NeuralTransformations::setPathTestFile(std::string pathTestFile) {
  m_pathTestFile = pathTestFile;
}

}  // namespace s21
