#pragma once

#include <utility>
#include <vector>
#include <algorithm>
#include <cmath>
#include <Eigen/Dense>

// To-Do: implement predator with dumb tracking


class Brain {
    private:

        

    public:
        Brain() = default;
        virtual ~Brain() = default;

    // NN: (12) - (10) - (2)

        // for revision later: should we include root distance and angle in input or is that implicitly learnable?

    /* 
       Input layer: 12 neurons {
            distance to nearest food
            angle to nearest food (sin)
            angle to nearest food (cos)

            distance to nearest predator
            angle to nearest predator (sin)
            angle to nearest predator (cos)

            left whisker proximity
            center whisker proximity
            right whisker proximity

            health
            energy
            hunger
        }
        Activation function 1: ReLU
       Hidden layer: 10 neurons 
        Activation function 2: ReLU
        Output layer: 2 neurons { 
            thrust
            alpha
        }
        Activation function 3: tanh
            -> encoding output 1 (thrust) and output 2 (alpha) between -1 and 1 to fit tanh model
    */

    std::vector<float> ForwardPass();

    void RefreshInput(const std::vector<float>& input) {
        current_input = input;
    }

    private:

        std::vector<float> current_input;

        // weights will be fixed after headless training in python environment, and loaded into the C++ environment for inference

        Eigen::MatrixXf weights_input_hidden;
        Eigen::VectorXf biases_hidden;

        Eigen::MatrixXf weights_output;
        Eigen::VectorXf biases_output;

};
