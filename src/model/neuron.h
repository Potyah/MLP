#ifndef SRC_MODEL_NEURON_H_
#define SRC_MODEL_NEURON_H_

#include <algorithm>
#include <vector>

namespace s21 {
class Neuron {
public:
    Neuron(int countLayer, int previousLayerNeuronsQuantity);
    double setRandomWeight();
    void setOut(double data);
    double getOut();
    void setError(double error);
    double getError();
    void setDelta(double delta);
    double getDelta();
    void setWeight(int position, double newWeight);
    std::vector<double> &getWeights();

private:
    double m_Out;
    double m_Error;
    double m_Delta;
    std::vector<double> m_Weights;
};
}  //  namespace s21
#endif  //  SRC_MODEL_NEURON_H_
