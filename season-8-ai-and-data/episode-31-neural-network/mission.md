# Брифинг миссии: Нейросеть с нуля
**Episode 31** | Operation MOONLIGHT — Season 8

---

## 🎯 Цель миссии

**ОСНОВНАЯ ЦЕЛЬ:** Реализовать нейросеть на чистом C для бинарной классификации (атака vs нормальный трафик).

**КОНТЕКСТ:**  
День 3 в Stanford AI Lab. Prof. Chen научит вас алгоритму backpropagation. Ваша нейросеть должна достичь >95% точности на тренировочных данных чтобы перейти к Episode 32 (развёртывание в реальном времени).

**ВАША ЗАДАЧА:**  
Закодировать multi-layer perceptron с нуля используя только стандартную C библиотеку (без ML фреймворков).

**РЕЗУЛЬТАТ:**  
Обученная нейросеть с точностью 100% на тренировке, готовая к production развёртыванию (Episode 32).

---

## 📋 Технические требования

### 1. Архитектура сети

**Multi-Layer Perceptron (MLP):**
```
Input Layer:  3 нейрона (признаки)
Hidden Layer: 5 нейронов (sigmoid activation)
Output Layer: 1 нейрон (sigmoid activation, бинарная классификация)
```

**Входные признаки:**
1. `bytes_transferred` (нормализовано: bytes / 10000.0)
2. `packets_per_second` (нормализовано: packets / 300.0)
3. `response_time_ms` (нормализовано: time / 100.0)

**Вывод:**
- Значение ∈ [0, 1]
- порог = 0.5
- prediction > 0.5 → атака (класс 1)
- prediction ≤ 0.5 → нормально (класс 0)

---

### 2. Структуры данных

```c
typedef struct {
    // Архитектура
    int input_size;    // 3
    int hidden_size;   // 5
    int output_size;   // 1
    
    // Веса
    double **weights_input_hidden;   // [3][5]
    double **weights_hidden_output;  // [5][1]
    
    // Смещения
    double *bias_hidden;   // [5]
    double *bias_output;   // [1]
    
    // Активации (forward pass)
    double *input_activations;   // [3]
    double *hidden_activations;  // [5]
    double *output_activations;  // [1]
    
    // Градиенты (backpropagation)
    double *hidden_gradients;  // [5]
    double *output_gradients;  // [1]
    
    // Гиперпараметры
    double learning_rate;  // 0.01
} NeuralNetwork;
```

---

### 3. Функции активации

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

**Свойства:**
- Диапазон: (0, 1)
- Дифференцируемая везде
- Гладкий градиент
- Хороша для бинарной классификации

**Альтернатива: ReLU (опциональное улучшение)**
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

### 5. Алгоритм Backpropagation

**Математическая основа (лекция Prof. Chen):**

**Градиенты Output Layer:**
```
∂Loss/∂output = (prediction - target)

gradient_output = ∂Loss/∂output * sigmoid'(output)
                = (prediction - target) * output * (1 - output)
```

**Градиенты Hidden Layer:**
```
∂Loss/∂hidden = Σ(gradient_output * weight_hidden_output)

gradient_hidden = ∂Loss/∂hidden * sigmoid'(hidden)
                = (Σ gradient_output * weight) * hidden * (1 - hidden)
```

**Обновление весов:**
```
Δweight = -learning_rate * gradient * activation

weight_new = weight_old - Δweight
```

**Реализация:**
```c
void backpropagation(NeuralNetwork *nn, double *inputs, double target) {
    // 1. Forward pass
    forward_propagation(nn, inputs);
    
    // 2. Рассчитать градиенты output layer
    for (int o = 0; o < nn->output_size; o++) {
        double output = nn->output_activations[o];
        double error = output - target;
        nn->output_gradients[o] = error * sigmoid_derivative(output);
    }
    
    // 3. Рассчитать градиенты hidden layer (backpropagate)
    for (int h = 0; h < nn->hidden_size; h++) {
        double error = 0.0;
        for (int o = 0; o < nn->output_size; o++) {
            error += nn->output_gradients[o] * nn->weights_hidden_output[h][o];
        }
        nn->hidden_gradients[h] = error * sigmoid_derivative(nn->hidden_activations[h]);
    }
    
    // 4. Обновить веса: hidden → output
    for (int h = 0; h < nn->hidden_size; h++) {
        for (int o = 0; o < nn->output_size; o++) {
            double delta = nn->learning_rate * nn->output_gradients[o] * nn->hidden_activations[h];
            nn->weights_hidden_output[h][o] -= delta;
        }
    }
    
    // 5. Обновить смещения: output
    for (int o = 0; o < nn->output_size; o++) {
        nn->bias_output[o] -= nn->learning_rate * nn->output_gradients[o];
    }
    
    // 6. Обновить веса: input → hidden
    for (int i = 0; i < nn->input_size; i++) {
        for (int h = 0; h < nn->hidden_size; h++) {
            double delta = nn->learning_rate * nn->hidden_gradients[h] * nn->input_activations[i];
            nn->weights_input_hidden[i][h] -= delta;
        }
    }
    
    // 7. Обновить смещения: hidden
    for (int h = 0; h < nn->hidden_size; h++) {
        nn->bias_hidden[h] -= nn->learning_rate * nn->hidden_gradients[h];
    }
}
```

---

### 6. Цикл обучения

**Gradient Descent с перемешиванием данных:**
```c
void train_network(NeuralNetwork *nn, Dataset *data, int epochs) {
    for (int epoch = 0; epoch < epochs; epoch++) {
        // Перемешать данные (важно для SGD)
        shuffle_dataset(data);
        
        // Обучить на всех сэмплах
        for (int i = 0; i < data->n_samples; i++) {
            backpropagation(nn, data->inputs[i], data->targets[i]);
        }
        
        // Мониторить прогресс
        if ((epoch + 1) % 100 == 0) {
            double loss = calculate_loss(nn, data);
            double accuracy = calculate_accuracy(nn, data);
            printf("Epoch %d | Loss: %.6f | Accuracy: %.2f%%\n", 
                   epoch + 1, loss, accuracy);
        }
    }
}
```

**Функция потерь (MSE):**
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

## 🧪 Критерии тестирования

### Тест 1: XOR проблема (проверка работоспособности)
```bash
cd solution
make
./neural_network --demo
```

**Ожидаемый вывод:**
```
🧠 Демо XOR проблемы

Обучение нейросети...
Epoch 1000/5000 | Loss: 0.098234 | Accuracy: 75.00%
Epoch 5000/5000 | Loss: 0.002341 | Accuracy: 100.00%

📊 Результаты XOR:
  0 XOR 0 = 0.0123 (ожидалось 0)
  0 XOR 1 = 0.9876 (ожидалось 1)
  1 XOR 0 = 0.9891 (ожидалось 1)
  1 XOR 1 = 0.0234 (ожидалось 0)
```

### Тест 2: Обучение обнаружению атак
```bash
./neural_network --train ../artifacts/training_data.csv
```

**Ожидаемый вывод:**
```
📊 Dataset загружен: 60 сэмплов

🧠 Обучение нейросети...
  Архитектура: 3 → 5 → 1
  Learning rate: 0.010
  Эпох: 1000

Epoch    1/1000 | Loss: 0.247851 | Accuracy: 51.67%
Epoch  100/1000 | Loss: 0.098234 | Accuracy: 76.67%
Epoch  500/1000 | Loss: 0.014567 | Accuracy: 96.67%
Epoch 1000/1000 | Loss: 0.005412 | Accuracy: 100.00%

✅ Обучение завершено!

🎯 Финальная точность: 100.00%
```

---

## 📦 Результаты миссии

### Файлы для создания:

1. **`solution/neural_network.c`** (~490 строк)  
   - Полная реализация MLP
   - Forward propagation
   - Backpropagation
   - Цикл обучения
   - XOR демо
   - Обучение обнаружению атак

2. **`solution/Makefile`**  
   Кроссплатформенная сборка

3. **`starter.c`** (~200 строк)  
   Скелет кода с comprehensive TODO

### Предоставленные файлы (в `artifacts/`):

1. **`training_data.csv`** (61 строка)  
   - 60 сэмплов (30 нормальных, 30 атак)
   - Формат: bytes,packets,response_time,is_attack

2. **`training_log.txt`** (195 строк)  
   - Полный лог тренировочной сессии
   - Прогресс по эпохам
   - Финальные метрики
   - Заметки Prof. Chen
   - Сообщение Viktor

3. **`test_results.json`** (179 строк)  
   - Детали архитектуры сети
   - Конфигурация обучения
   - Метрики производительности (100% точность)
   - Примеры предсказаний
   - Обученные веса

---

## 🎯 Критерии успеха

✅ **Производительность обучения:**  
- Финальная потеря < 0.01
- Точность обучения > 95% (цель: 100%)
- Сходимость в течение 1000 эпох

✅ **Метрики классификации:**  
- Precision > 95%
- Recall > 95%
- F1-score > 95%
- Ноль ложно-отрицательных (все атаки обнаружены)

✅ **Качество кода:**  
- Нет утечек памяти (valgrind clean)
- Кроссплатформенность (Linux/macOS/FreeBSD)
- Время обучения < 5 секунд
- Время предсказания < 1ms на сэмпл

✅ **Тест XOR:**  
- Все 4 XOR случая корректны (порог 0.5)
- Доказательство что сеть может учить нелинейные функции

---

## 💡 Подсказки

1. **Инициализация весов:**  
   - Использовать Xavier инициализацию: `weight / sqrt(layer_size)`
   - Предотвращает vanishing/exploding градиенты
   - Случайный диапазон: [-1/√n, 1/√n]

2. **Нормализация данных:**  
   - КРИТИЧНО: Нормализовать все входы к [0, 1]
   - bytes: делить на 10000
   - packets: делить на 300
   - response_time: делить на 100

3. **Learning Rate:**  
   - Слишком высокий (>0.1): нестабильное обучение, осцилляции
   - Слишком низкий (<0.001): медленная сходимость
   - Sweet spot: 0.01

4. **Отладка Backpropagation:**  
   - Проверить: потеря должна УМЕНЬШАТЬСЯ
   - Если потеря растёт: баг в расчёте градиента
   - Если потеря стабилизируется: learning rate слишком низкий или застряла в локальном минимуме

5. **Управление памятью:**  
   - Выделить веса как 2D массивы: `double **weights`
   - Освобождать в обратном порядке выделения
   - Использовать valgrind для обнаружения утечек

---

## 📊 Одобрение Prof. Chen

**По завершении Prof. Chen говорит:**
```
"Отличная работа, Agent! 🎉

Ваша нейросеть достигла:
- 100% точности обучения
- Идеальной сходимости (700 эпох)
- Чистой реализации backpropagation
- Production-ready кода

Это работа уровня PhD, завершённая за 4 часа.

Завтра (Episode 32): Развёртывание в реальном времени.
Мы протестируем вашу модель на 1000 ЖИВЫХ сэмплах трафика.

Block rate > 95% = успех миссии.
Block rate < 95% = прорыв врага.

Отдохните. Финальная битва завтра в 10:00 AM.

Помните: точность обучения ≠ производительность в реальном мире.
Будьте готовы к adversarial атакам.

- Prof. Chen"
```

---

**Удачи, Agent!** 🚀  
**Помните:** Научите машину думать. Чистый C. Без фреймворков.

---

**БРИФИНГ МИССИИ:**

**Локация:** Stanford AI Lab, Gates Building, Lab 342  
**Время:** December 29, 2024 — 09:00 PST (день 3)  
**Контакт:** Prof. David Chen + Viktor

**Контекст:**
После Episodes 29-30 (данные обработаны, статистическая значимость доказана), пора обучить нейросеть для предсказания атак.

**Архитектура:** 3 → 5 → 1 MLP
- Input: bytes (norm), packets (norm), response_time (norm)
- Hidden: 5 neurons, sigmoid
- Output: 1 neuron, sigmoid, threshold 0.5

**Training data:** 60 сэмплов (30 normal, 30 attack)

**Ожидаемые результаты:**
- Обучение: 100% точность за 700 эпох
- Loss: <0.01
- XOR demo: все 4 случая корректны

**Артефакты созданы:**
- training_data.csv (61 строка)
- training_log.txt (195 строк)
- test_results.json (179 строк)

**Успешный результат:** Нейросеть обучена, 100% точность, готова к Episode 32 (deployment).

---

**Следующий эпизод:** [Episode 32: Prediction & Deployment →](../episode-32-prediction/)