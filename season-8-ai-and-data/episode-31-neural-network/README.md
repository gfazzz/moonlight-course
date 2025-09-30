# Episode 31: "Neural Networks from Scratch" 🧠
## Season 8: AI & Data Science | Episode 31/42

> *"Научим машину думать. На чистом C."*

---

## 📋 Briefing

Создать нейронную сеть с нуля для распознавания паттернов.

**Задачи:**
1. Perceptron implementation
2. Backpropagation
3. Activation functions
4. Training loop

---

## 📚 Теория

### Simple Perceptron

```c
typedef struct {
    double *weights;
    double bias;
    int input_size;
} Perceptron;

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double predict(Perceptron *p, double *inputs) {
    double sum = p->bias;
    for (int i = 0; i < p->input_size; i++) {
        sum += inputs[i] * p->weights[i];
    }
    return sigmoid(sum);
}

void train(Perceptron *p, double **X, double *y, int n, double lr) {
    for (int epoch = 0; epoch < 1000; epoch++) {
        for (int i = 0; i < n; i++) {
            double pred = predict(p, X[i]);
            double error = y[i] - pred;
            
            // Update weights
            for (int j = 0; j < p->input_size; j++) {
                p->weights[j] += lr * error * X[i][j];
            }
            p->bias += lr * error;
        }
    }
}
```

---

## 🛠 Практика

**Задачи:**
1. XOR problem solver
2. Multi-layer network
3. MNIST digit recognition (simplified)
4. Pattern classifier

---

**Next:** [Episode 32: Predictive Models →](../episode-32-prediction/)
