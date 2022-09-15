#ifndef SRC_MODEL_LAYER_H_
#define SRC_MODEL_LAYER_H_

#include "neuron.h"
namespace s21 {

class Layer {
public:
    Layer(int layerNumber, unsigned long neuronsQuantity, unsigned long previousLayerNeuronsQuantity);
    ~Layer();

    std::vector<Neuron*>& getNeuron();

    int getNeuronsQuantity();
    int getLayerNumber();
    void calculateOutput(Layer* layer);
    void calculateLastLayerError(int expectedResult);
    void calculateError(Layer* previousLayer);
    void calculateDelta();
    void adjustWeight(Layer* layer, double learningRate);

private:
    std::vector<Neuron*> m_Neurons;
    int m_layerNumber;
    int m_neuronsQuantity;
};
}  // namespace s21
#endif  //  SRC_MODEL_LAYER_H_
