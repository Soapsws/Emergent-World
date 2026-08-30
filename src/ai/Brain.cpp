#include "Brain.hpp"
#include "Math.hpp"


std::vector<float> Brain::ForwardPass(const std::vector<float>& input) {
    Eigen::Map<const Eigen::VectorXf> inputVec(input.data(), static_cast<Eigen::Index>(input.size()));
    
    Eigen::VectorXf hiddenLayer = (weights_input_hidden * inputVec + biases_hidden).unaryExpr([](float x) { return std::max(0.0f, x); }); // ReLU activation
    Eigen::VectorXf outputLayer = (weights_output * hiddenLayer + biases_output).unaryExpr([](float x) { return std::tanh(x); }); // tanh activation

    return std::vector<float>(outputLayer.data(), outputLayer.data() + outputLayer.size());
}

// More implementation to be added
