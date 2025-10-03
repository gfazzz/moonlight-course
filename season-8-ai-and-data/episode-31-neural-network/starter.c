/*
 * Episode 31: Neural Network from Scratch - STARTER CODE
 * Operation MOONLIGHT - Season 8: AI & Data Science
 * 
 * Mission: Implement neural network in pure C
 * 
 * Your tasks:
 * 1. Implement activation functions (sigmoid, ReLU, tanh)
 * 2. Create neural network structure
 * 3. Implement forward propagation
 * 4. Implement backpropagation algorithm
 * 5. Add training loop with gradient descent
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define INPUT_SIZE 3
#define HIDDEN_SIZE 5
#define OUTPUT_SIZE 1
#define LEARNING_RATE 0.01

/*
 * TODO 1: Implement activation functions
 */

double sigmoid(double x) {
    // TODO: Return 1 / (1 + e^(-x))
    return 0.0;
}

double sigmoid_derivative(double x) {
    // TODO: Return sigmoid(x) * (1 - sigmoid(x))
    return 0.0;
}

double relu(double x) {
    // TODO: Return max(0, x)
    return 0.0;
}

double relu_derivative(double x) {
    // TODO: Return 1 if x > 0, else 0
    return 0.0;
}

/*
 * TODO 2: Define neural network structure
 */

typedef struct {
    int input_size;
    int hidden_size;
    int output_size;
    
    // TODO: Add weight matrices
    // - weights_input_hidden [input_size][hidden_size]
    // - weights_hidden_output [hidden_size][output_size]
    // - bias_hidden [hidden_size]
    // - bias_output [output_size]
    
    // TODO: Add activation arrays
    // - input_activations [input_size]
    // - hidden_activations [hidden_size]
    // - output_activations [output_size]
    
    // TODO: Add gradient arrays (for backpropagation)
    // - hidden_gradients [hidden_size]
    // - output_gradients [output_size]
    
    double learning_rate;
} NeuralNetwork;

/*
 * TODO 3: Initialize network with random weights
 */

double random_weight() {
    // TODO: Return random value in range [-1, 1]
    // Hint: Use rand() / RAND_MAX
    return 0.0;
}

NeuralNetwork* create_network(int input_size, int hidden_size, int output_size, double lr) {
    // TODO:
    // 1. Allocate NeuralNetwork structure
    // 2. Allocate and initialize weight matrices
    // 3. Allocate and initialize bias arrays
    // 4. Allocate activation and gradient arrays
    // 5. Use Xavier initialization: weight / sqrt(layer_size)
    
    return NULL;
}

void free_network(NeuralNetwork *nn) {
    // TODO: Free all allocated memory
}

/*
 * TODO 4: Forward propagation
 * 
 * Steps:
 * 1. Input → Hidden:
 *    hidden[j] = sigmoid(Σ(input[i] * weight[i][j]) + bias[j])
 * 
 * 2. Hidden → Output:
 *    output[k] = sigmoid(Σ(hidden[j] * weight[j][k]) + bias[k])
 */

void forward_propagation(NeuralNetwork *nn, double *inputs) {
    // TODO: Implement forward pass through network
}

double predict(NeuralNetwork *nn, double *inputs) {
    // TODO:
    // 1. Run forward_propagation
    // 2. Return output_activations[0]
    
    return 0.0;
}

/*
 * TODO 5: Backpropagation
 * 
 * Mathematical formulas:
 * 
 * Output layer:
 *   gradient_output = (prediction - target) * sigmoid'(output)
 * 
 * Hidden layer:
 *   gradient_hidden = (Σ gradient_output * weight) * sigmoid'(hidden)
 * 
 * Weight update:
 *   weight -= learning_rate * gradient * activation
 * 
 * Bias update:
 *   bias -= learning_rate * gradient
 */

void backpropagation(NeuralNetwork *nn, double *inputs, double target) {
    // TODO: Implement backpropagation algorithm
    // Steps:
    // 1. Forward pass
    // 2. Calculate output layer gradients
    // 3. Calculate hidden layer gradients (backpropagate error)
    // 4. Update weights: hidden → output
    // 5. Update biases: output layer
    // 6. Update weights: input → hidden
    // 7. Update biases: hidden layer
}

/*
 * TODO 6: Training loop
 */

typedef struct {
    double **inputs;
    double *targets;
    int n_samples;
} Dataset;

void train_network(NeuralNetwork *nn, Dataset *data, int epochs) {
    // TODO:
    // 1. For each epoch:
    //    a. Shuffle data (optional but recommended)
    //    b. For each sample:
    //       - Run backpropagation(nn, inputs[i], targets[i])
    //    c. Calculate and print loss/accuracy every N epochs
}

/*
 * TODO 7: Calculate metrics
 */

double calculate_loss(NeuralNetwork *nn, Dataset *data) {
    // TODO: Calculate Mean Squared Error (MSE)
    // MSE = Σ(prediction - target)² / n_samples
    return 0.0;
}

double calculate_accuracy(NeuralNetwork *nn, Dataset *data) {
    // TODO: Calculate classification accuracy
    // For binary classification: prediction > 0.5 → class 1, else class 0
    return 0.0;
}

/*
 * XOR Demo (optional test)
 */

void run_xor_demo() {
    printf("🧠 XOR Problem Demo\n\n");
    
    // TODO: Create XOR dataset
    // XOR truth table:
    //   0 XOR 0 = 0
    //   0 XOR 1 = 1
    //   1 XOR 0 = 1
    //   1 XOR 1 = 0
    
    // TODO: Create network (2 inputs, 4 hidden, 1 output)
    // TODO: Train for 5000 epochs
    // TODO: Test and print results
    
    printf("⚠️  TODO: Implement XOR demo\n");
}

int main(int argc, char *argv[]) {
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║    Episode 31: Neural Network from Scratch - STARTER             ║\n");
    printf("║             Operation MOONLIGHT - Season 8                        ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("🎯 Mission: Implement neural network in pure C\n\n");
    
    srand(time(NULL));
    
    // TODO: Parse command line arguments
    // TODO: Load training data
    // TODO: Create and train network
    // TODO: Print results
    
    printf("⚠️  TODO: Implement neural network training\n");
    
    return 0;
}

