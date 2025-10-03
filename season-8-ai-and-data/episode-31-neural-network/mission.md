# Mission Brief: Neural Network from Scratch
**Episode 31** | Operation MOONLIGHT — Season 8

---

## 🎯 Mission Objective

**PRIMARY GOAL:** Implement neural network in pure C for binary classification (attack vs normal traffic).

**CONTEXT:**  
Day 3 at Stanford AI Lab. Prof. Chen will teach you backpropagation algorithm. Your neural network must achieve >95% accuracy on training data to proceed to Episode 32 (real-time deployment).

**YOUR TASK:**  
Code multi-layer perceptron from scratch using only standard C library (no ML frameworks).

**DELIVERABLE:**  
Trained neural network achieving 100% training accuracy, ready for production deployment (Episode 32).

---

## 📋 Technical Requirements

### 1. Network Architecture

**Multi-Layer Perceptron (MLP):**
```
Input Layer:  3 neurons (features)
Hidden Layer: 5 neurons (sigmoid activation)
Output Layer: 1 neuron (sigmoid activation, binary classification)
```

**Input Features:**
1. `bytes_transferred` (normalized: bytes / 10000.0)
2. `packets_per_second` (normalized: packets / 300.0)
3. `response_time_ms` (normalized: time / 100.0)

**Output:**
- Value ∈ [0, 1]
- threshold = 0.5
- prediction > 0.5 → attack (class 1)
- prediction ≤ 0.5 → normal (class 0)

---

### 2. Data Structures

```c
typedef struct {
    // Architecture
    int input_size;    // 3
    int hidden_size;   // 5
    int output_size;   // 1
    
    // Weights
    double **weights_input_hidden;   // [3][5]
    double **weights_hidden_output;  // [5][1]
    
    // Biases
    double *bias_hidden;   // [5]
    double *bias_output;   // [1]
    
    // Activations (forward pass)
    double *input_activations;   // [3]
    double *hidden_activations;  // [5]
    double *output_activations;  // [1]
    
    // Gradients (backpropagation)
    double *hidden_gradients;  // [5]
    double *output_gradients;  // [1]
    
    // Hyperparameters
    double learning_rate;  // 0.01
} NeuralNetwork;
```

---

### 3. Activation Functions

**Sigmoid:**
```c
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double sigmoid_derivative(double x) {
    double s = sigmoid(x);
    return s * (1.0 - s);
}
```

**Properties:**
- Range: (0, 1)
- Differentiable everywhere
- Smooth gradient
- Good for binary classification

**Alternative: ReLU (optional enhancement)**
```c
double relu(double x) {
    return x > 0 ? x : 0;
}

double relu_derivative(double x) {
    return x > 0 ? 1.0 : 0.0;
}
```

---

### 4. Forward Propagation

**Input → Hidden Layer:**
```c
for (int h = 0; h < hidden_size; h++) {
    double sum = bias_hidden[h];
    
    for (int i = 0; i < input_size; i++) {
        sum += input_activations[i] * weights_input_hidden[i][h];
    }
    
    hidden_activations[h] = sigmoid(sum);
}
```

**Hidden → Output Layer:**
```c
for (int o = 0; o < output_size; o++) {
    double sum = bias_output[o];
    
    for (int h = 0; h < hidden_size; h++) {
        sum += hidden_activations[h] * weights_hidden_output[h][o];
    }
    
    output_activations[o] = sigmoid(sum);
}
```

---

### 5. Backpropagation Algorithm

**Mathematical Foundation (Prof. Chen's lecture):**

**Output Layer Gradients:**
```
∂Loss/∂output = (prediction - target)

gradient_output = ∂Loss/∂output * sigmoid'(output)
                = (prediction - target) * output * (1 - output)
```

**Hidden Layer Gradients:**
```
∂Loss/∂hidden = Σ(gradient_output * weight_hidden_output)

gradient_hidden = ∂Loss/∂hidden * sigmoid'(hidden)
                = (Σ gradient_output * weight) * hidden * (1 - hidden)
```

**Weight Updates:**
```
Δweight = -learning_rate * gradient * activation

weight_new = weight_old - Δweight
```

**Implementation:**
```c
void backpropagation(NeuralNetwork *nn, double *inputs, double target) {
    // 1. Forward pass
    forward_propagation(nn, inputs);
    
    // 2. Calculate output layer gradients
    for (int o = 0; o < nn->output_size; o++) {
        double output = nn->output_activations[o];
        double error = output - target;
        nn->output_gradients[o] = error * sigmoid_derivative(output);
    }
    
    // 3. Calculate hidden layer gradients (backpropagate)
    for (int h = 0; h < nn->hidden_size; h++) {
        double error = 0.0;
        for (int o = 0; o; o < nn->output_size; o++) {
            error += nn->output_gradients[o] * nn->weights_hidden_output[h][o];
        }
        nn->hidden_gradients[h] = error * sigmoid_derivative(nn->hidden_activations[h]);
    }
    
    // 4. Update weights: hidden → output
    for (int h = 0; h < nn->hidden_size; h++) {
        for (int o = 0; o < nn->output_size; o++) {
            double delta = nn->learning_rate * nn->output_gradients[o] * nn->hidden_activations[h];
            nn->weights_hidden_output[h][o] -= delta;
        }
    }
    
    // 5. Update biases: output
    for (int o = 0; o < nn->output_size; o++) {
        nn->bias_output[o] -= nn->learning_rate * nn->output_gradients[o];
    }
    
    // 6. Update weights: input → hidden
    for (int i = 0; i < nn->input_size; i++) {
        for (int h = 0; h < nn->hidden_size; h++) {
            double delta = nn->learning_rate * nn->hidden_gradients[h] * nn->input_activations[i];
            nn->weights_input_hidden[i][h] -= delta;
        }
    }
    
    // 7. Update biases: hidden
    for (int h = 0; h < nn->hidden_size; h++) {
        nn->bias_hidden[h] -= nn->learning_rate * nn->hidden_gradients[h];
    }
}
```

---

### 6. Training Loop

**Gradient Descent with Data Shuffling:**
```c
void train_network(NeuralNetwork *nn, Dataset *data, int epochs) {
    for (int epoch = 0; epoch < epochs; epoch++) {
        // Shuffle data (important for SGD)
        shuffle_dataset(data);
        
        // Train on all samples
        for (int i = 0; i < data->n_samples; i++) {
            backpropagation(nn, data->inputs[i], data->targets[i]);
        }
        
        // Monitor progress
        if ((epoch + 1) % 100 == 0) {
            double loss = calculate_loss(nn, data);
            double accuracy = calculate_accuracy(nn, data);
            printf("Epoch %d | Loss: %.6f | Accuracy: %.2f%%\n", 
                   epoch + 1, loss, accuracy);
        }
    }
}
```

**Loss Function (MSE):**
```c
double calculate_loss(NeuralNetwork *nn, Dataset *data) {
    double total_loss = 0.0;
    
    for (int i = 0; i < data->n_samples; i++) {
        double prediction = predict(nn, data->inputs[i]);
        double error = prediction - data->targets[i];
        total_loss += error * error;
    }
    
    return total_loss / data->n_samples;
}
```

---

## 🧪 Testing Criteria

### Test 1: XOR Problem (Sanity Check)
```bash
cd solution
make
./neural_network --demo
```

**Expected Output:**
```
🧠 XOR Problem Demo

Training Neural Network...
Epoch 1000/5000 | Loss: 0.098234 | Accuracy: 75.00%
Epoch 5000/5000 | Loss: 0.002341 | Accuracy: 100.00%

📊 XOR Results:
  0 XOR 0 = 0.0123 (expected 0)
  0 XOR 1 = 0.9876 (expected 1)
  1 XOR 0 = 0.9891 (expected 1)
  1 XOR 1 = 0.0234 (expected 0)
```

### Test 2: Attack Detection Training
```bash
./neural_network --train ../artifacts/training_data.csv
```

**Expected Output:**
```
📊 Dataset loaded: 60 samples

🧠 Training Neural Network...
  Architecture: 3 → 5 → 1
  Learning rate: 0.010
  Epochs: 1000

Epoch    1/1000 | Loss: 0.247851 | Accuracy: 51.67%
Epoch  100/1000 | Loss: 0.098234 | Accuracy: 76.67%
Epoch  500/1000 | Loss: 0.014567 | Accuracy: 96.67%
Epoch 1000/1000 | Loss: 0.005412 | Accuracy: 100.00%

✅ Training complete!

🎯 Final Accuracy: 100.00%
```

---

## 📦 Deliverables

### Files to Create:

1. **`solution/neural_network.c`** (~490 lines)  
   - Complete MLP implementation
   - Forward propagation
   - Backpropagation
   - Training loop
   - XOR demo
   - Attack detection training

2. **`solution/Makefile`**  
   Cross-platform build

3. **`starter.c`** (~200 lines)  
   Skeleton code with comprehensive TODOs

### Files Provided (in `artifacts/`):

1. **`training_data.csv`** (61 lines)  
   - 60 samples (30 normal, 30 attack)
   - Format: bytes,packets,response_time,is_attack

2. **`training_log.txt`** (195 lines)  
   - Complete training session log
   - Epoch-by-epoch progress
   - Final metrics
   - Prof. Chen's notes
   - Viktor's message

3. **`test_results.json`** (179 lines)  
   - Network architecture details
   - Training configuration
   - Performance metrics (100% accuracy)
   - Sample predictions
   - Learned weights

---

## 🎯 Success Criteria

✅ **Training Performance:**  
- Final loss < 0.01
- Training accuracy > 95% (target: 100%)
- Convergence within 1000 epochs

✅ **Classification Metrics:**  
- Precision > 95%
- Recall > 95%
- F1-score > 95%
- Zero false negatives (all attacks detected)

✅ **Code Quality:**  
- No memory leaks (valgrind clean)
- Cross-platform (Linux/macOS/FreeBSD)
- Training time < 5 seconds
- Prediction time < 1ms per sample

✅ **XOR Test:**  
- All 4 XOR cases correct (threshold 0.5)
- Proof that network can learn non-linear functions

---

## 💡 Hints

1. **Weight Initialization:**  
   - Use Xavier initialization: `weight / sqrt(layer_size)`
   - Prevents vanishing/exploding gradients
   - Random range: [-1/√n, 1/√n]

2. **Data Normalization:**  
   - CRITICAL: Normalize all inputs to [0, 1]
   - bytes: divide by 10000
   - packets: divide by 300
   - response_time: divide by 100

3. **Learning Rate:**  
   - Too high (>0.1): unstable training, oscillations
   - Too low (<0.001): slow convergence
   - Sweet spot: 0.01

4. **Debugging Backpropagation:**  
   - Check: loss should DECREASE
   - If loss increases: bug in gradient calculation
   - If loss plateaus: learning rate too low or stuck in local minimum

5. **Memory Management:**  
   - Allocate weights as 2D arrays: `double **weights`
   - Free in reverse order of allocation
   - Use valgrind to detect leaks

---

## 📊 Prof. Chen's Approval

**Upon completion, Prof. Chen says:**
```
"Excellent work, Agent! 🎉

Your neural network achieved:
- 100% training accuracy
- Perfect convergence (700 epochs)
- Clean backpropagation implementation
- Production-ready code

This is PhD-level work, completed in 4 hours.

Tomorrow (Episode 32): Real-time deployment.
We'll test your model on 1000 LIVE traffic samples.

Block rate > 95% = mission success.
Block rate < 95% = enemy breakthrough.

Get some rest. Final battle tomorrow at 10:00 AM.

Remember: training accuracy ≠ real-world performance.
Be prepared for adversarial attacks.

- Prof. Chen"
```

---

**Good luck, Agent!** 🚀  
**Remember:** Teach the machine to think. Pure C. No frameworks.
