#include "graphNet.h"

#include <fstream>
#include <sstream>

namespace s21 {
GraphNet::GraphNet(int CountLayers, double LearningRate)
    : m_LayersCount(CountLayers), m_LearningRate(LearningRate) {
    GraphNet::initNet();
}

GraphNet::~GraphNet() {
    for (auto layer : m_Layers) {
        if (m_Layers.size() > 0 && layer != nullptr) delete layer;
    }
}

void GraphNet::initNet() {
    for (int i = 0; i < m_LayersCount; ++i) {
        Layer* p_layer;

        if (i == 0) {
            p_layer = new Layer(0, INPUT_LAYER, 0);
        } else if (i == 1) {
            p_layer = new Layer(1, HIDE_LAYER, INPUT_LAYER);
        } else if (i == m_LayersCount - 1) {
            p_layer = new Layer(m_LayersCount - 1, OUTPUT_LAYER, HIDE_LAYER);
        } else {
            p_layer = new Layer(i, HIDE_LAYER, HIDE_LAYER);
        }

        m_Layers.push_back(p_layer);
    }
}

void GraphNet::setInputLayerOutput(const std::vector<double>& outputs) {
    for (unsigned long i = 0; i < m_Layers[0]->getNeuron().size(); ++i) {
        m_Layers[0]->getNeuron()[i]->setOut(outputs[i + 1] / 255.0);
    }
}

void GraphNet::predictions() {
    for (unsigned long i = 1; i < m_Layers.size(); ++i) {
        m_Layers[i]->calculateOutput(m_Layers[i - 1]);
    }
}

void GraphNet::setLearningRate(double learningRate) { m_LearningRate = learningRate; }

double GraphNet::getLearningRate() { return m_LearningRate; }

int GraphNet::getLayersCount() { return m_LayersCount; }

void GraphNet::training(int expectedResult) {
    for (int i = (int)m_Layers.size() - 1; i >= 0; --i) {
        if (i == (int)m_Layers.size() - 1) {
            m_Layers[i]->calculateLastLayerError(expectedResult);
            m_Layers[i]->calculateDelta();
            m_Layers[i]->adjustWeight(m_Layers[i - 1], m_LearningRate);
        } else if (i > 0) {
            m_Layers[i]->calculateError(m_Layers[i + 1]);
            m_Layers[i]->calculateDelta();
            m_Layers[i]->adjustWeight(m_Layers[i - 1], m_LearningRate);
        }
    }
}

void GraphNet::saveNet(std::string path) {
    std::fstream fs;
    fs.open(path, std::fstream::out);

    if (!fs.is_open()) {
        throw std::runtime_error("The file to save the neural network is not open");
    }

    fs << getLayersCount() << "\n";

    for (auto layers : m_Layers) {
        if (layers->getLayerNumber() == 0) {
            continue;
        }

        fs << "layer"
           << "\n";

        for (auto neurons : layers->getNeuron()) {
            for (auto wieghts : neurons->getWeights()) {
                fs << wieghts << " ";
            }

            fs << "\n";
        }
    }

    fs.close();
}

void GraphNet::loadNet(std::string path) {
    std::ifstream fs;
    fs.open(path, std::fstream::in);

    if (!fs.is_open()) {
        throw std::runtime_error("The file to load the neural network is not open");
    }

    int linesCountFromFile;
    fs >> linesCountFromFile;
    if (linesCountFromFile != m_LayersCount) {
        return;
    }

    std::string line;

    int layerNumber = 0;
    int neuronCount = 0;

    while (!fs.eof()) {
        getline(fs, line);

        if (line == "layer") {
            layerNumber++;
            neuronCount = 0;
            continue;
        }

        std::istringstream st(line);
        double neuronData;
        int count = 0;

        while (st >> neuronData) {
            m_Layers.at(layerNumber)->getNeuron().at(neuronCount)->setWeight(count, neuronData);
            count++;
        }
        neuronCount++;
    }
    fs.close();
}

bool GraphNet::checkTrainingResult(int expectedResult) { return (int)getLetter() - 64 == expectedResult; }

char GraphNet::getLetter() {
    double maxRez = 0;
    int positionMax = 0;
    for (unsigned long i = 0; i < m_Layers[m_LayersCount - 1]->getNeuron().size(); ++i) {
        double outNeuron = m_Layers[m_LayersCount - 1]->getNeuron()[i]->getOut();
        if (maxRez < outNeuron) {
            maxRez = outNeuron;
            positionMax = i;
        }
    }
    return positionMax + 65;
}

std::vector<double> GraphNet::getOutLastLayer() {
    std::vector<double> rez;

    for (auto neuron : m_Layers[m_Layers.size() - 1]->getNeuron()) {
        rez.push_back(neuron->getOut());
    }

    return rez;
}

}  // namespace s21
