#ifndef SRC_MODEL_NEURALTRANSFORMATIONS_H_
#define SRC_MODEL_NEURALTRANSFORMATIONS_H_

#include <array>

#include "../common.h"
#include "cvsReader.h"
#include "graphNet.h"
#include "interfaceNet.h"
#include "matrixNet.h"

namespace s21 {
class NeuralTransformations {
private:
    int m_layersCount;
    double m_accuracy;
    double m_precision;
    double m_recall;
    double m_f_measure;
    double m_time;
    InterfaceNet *p_net;
    ModelType m_type;
    std::string m_pathTestFile;
    std::string m_pathTrainFile;

public:
    NeuralTransformations(ModelType type, int layersCount, std::string pathTest, std::string pathTrain);
    NeuralTransformations(ModelType type, int layersCount);
    std::array<double, 6> fileTest(double sampleSize);
    std::vector<double> train(unsigned int epoch);
    std::vector<double> trainWithCrossValidation(unsigned long crossValidationElements);
    char singleTest(std::vector<double> pixels);
    void loadWeights(std::string pathWeights);
    void saveWeights(std::string pathWeights);
    void setPathTestFile(std::string pathTestFile);
};
}  //  namespace s21

#endif  //  SRC_MODEL_NEURALTRANSFORMATIONS_H_
