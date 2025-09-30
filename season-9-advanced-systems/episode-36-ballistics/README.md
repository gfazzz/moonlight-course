# Episode 36: "Military Systems & Radar" 🎯
## Season 9: Advanced Systems | Episode 36/42

> *"Track. Predict. Engage."*

---

## 📋 Briefing

Военные системы: радарное отслеживание, GPS, Kalman фильтр.

**Задачи:**
1. Radar signal processing
2. Kalman filter для tracking
3. GPS координаты
4. Target prediction

---

## 📚 Теория

### Kalman Filter (Simplified)

```c
typedef struct {
    double x;      // State estimate
    double P;      // Error covariance
    double Q;      // Process noise
    double R;      // Measurement noise
} KalmanFilter;

void kalman_update(KalmanFilter *kf, double measurement) {
    // Prediction
    kf->P = kf->P + kf->Q;
    
    // Update
    double K = kf->P / (kf->P + kf->R);  // Kalman gain
    kf->x = kf->x + K * (measurement - kf->x);
    kf->P = (1 - K) * kf->P;
}
```

### GPS Coordinates

```c
typedef struct {
    double lat;
    double lon;
    double alt;
} GPSCoord;

double distance(GPSCoord *a, GPSCoord *b) {
    // Haversine formula
    double R = 6371000;  // Earth radius in meters
    double dlat = (b->lat - a->lat) * M_PI / 180.0;
    double dlon = (b->lon - a->lon) * M_PI / 180.0;
    
    double a_val = sin(dlat/2) * sin(dlat/2) +
                   cos(a->lat * M_PI / 180.0) * cos(b->lat * M_PI / 180.0) *
                   sin(dlon/2) * sin(dlon/2);
    double c = 2 * atan2(sqrt(a_val), sqrt(1-a_val));
    
    return R * c;
}
```

---

## 🛠 Практика

**Задачи:**
1. Radar simulator
2. Multi-target tracking
3. GPS navigation
4. Threat assessment

---

**Next:** [Episode 37: Quantum Computing →](../episode-37-quantum/)
