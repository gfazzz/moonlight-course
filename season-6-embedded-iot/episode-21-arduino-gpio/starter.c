/*
 * Episode 21: Arduino & GPIO Fundamentals
 * STARTER CODE
 * 
 * TODO: Implement GPIO system for surveillance
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Pin definitions (Arduino Uno compatible)
#define PIR_SENSOR_PIN     2
#define DOOR_CONTACT_PIN   3
#define TEMP_SENSOR_PIN    A0
#define LED_PIN            13
#define RELAY_PIN          8

// Threshold values
#define TEMP_THRESHOLD     50.0  // °C
#define ALERT_INTERVAL     1000  // ms

// TODO: Initialize GPIO pins
void gpio_init(void) {
    // Configure inputs
    // pinMode(PIR_SENSOR_PIN, INPUT);
    // pinMode(DOOR_CONTACT_PIN, INPUT);
    
    // Configure outputs
    // pinMode(LED_PIN, OUTPUT);
    // pinMode(RELAY_PIN, OUTPUT);
    
    printf("GPIO initialized\n");
}

// TODO: Read motion sensor
int read_motion(void) {
    // return digitalRead(PIR_SENSOR_PIN);
    return 0;  // Placeholder
}

// TODO: Read door contact
int read_door(void) {
    // return digitalRead(DOOR_CONTACT_PIN);
    return 0;  // Placeholder
}

// TODO: Read temperature sensor
float read_temperature(void) {
    // int raw = analogRead(TEMP_SENSOR_PIN);
    // float voltage = raw * (5.0 / 1023.0);
    // float temp = voltage * 100.0;  // LM35: 10mV/°C
    // return temp;
    return 25.0;  // Placeholder
}

// TODO: LED control
void led_on(void) {
    // digitalWrite(LED_PIN, HIGH);
    printf("[LED] ON\n");
}

void led_off(void) {
    // digitalWrite(LED_PIN, LOW);
    printf("[LED] OFF\n");
}

// TODO: Relay control
void relay_activate(void) {
    // digitalWrite(RELAY_PIN, HIGH);
    printf("[RELAY] ACTIVATED\n");
}

void relay_deactivate(void) {
    // digitalWrite(RELAY_PIN, LOW);
    printf("[RELAY] DEACTIVATED\n");
}

// TODO: Serial output
void send_status(int motion, int door, float temp) {
    printf("\n=== SURVEILLANCE STATUS ===\n");
    printf("MOTION: %d\n", motion);
    printf("DOOR: %d\n", door);
    printf("TEMP: %.1f°C\n", temp);
    
    if (motion) {
        printf("STATUS: ⚠️  MOTION DETECTED!\n");
    } else if (door) {
        printf("STATUS: ⚠️  DOOR OPEN!\n");
    } else if (temp > TEMP_THRESHOLD) {
        printf("STATUS: 🔥 OVERHEATING!\n");
    } else {
        printf("STATUS: ✅ NORMAL\n");
    }
    printf("==========================\n\n");
}

// TODO: Alert system
void check_alerts(int motion, int door, float temp) {
    static bool alert_active = false;
    
    // Motion detected
    if (motion) {
        led_on();
        alert_active = true;
    } else if (!door && temp < TEMP_THRESHOLD) {
        led_off();
        alert_active = false;
    }
    
    // Temperature alert
    if (temp > TEMP_THRESHOLD) {
        // Blink LED or activate alarm
        printf("⚠️  TEMPERATURE ALERT!\n");
        relay_activate();  // Could trigger cooling or alarm
    }
    
    // Door alert
    if (door) {
        printf("⚠️  UNAUTHORIZED ACCESS!\n");
    }
}

// Main monitoring loop
int main(void) {
    printf("🔧 MOONLIGHT Surveillance System\n");
    printf("Episode 21: GPIO & Sensors\n\n");
    
    // Initialize
    gpio_init();
    
    printf("System initialized. Starting monitoring...\n\n");
    
    // Simulation loop (replace with actual Arduino loop)
    for (int i = 0; i < 10; i++) {
        // Read sensors
        int motion = read_motion();
        int door = read_door();
        float temp = read_temperature();
        
        // Send status
        send_status(motion, door, temp);
        
        // Check for alerts
        check_alerts(motion, door, temp);
        
        // Delay (in real Arduino: delay(1000))
        // usleep(1000000);
    }
    
    printf("\n✅ Monitoring complete\n");
    return 0;
}

/*
 * TASKS:
 * 1. Implement gpio_init() with actual pin configuration
 * 2. Implement sensor reading functions
 * 3. Implement actuator control
 * 4. Add Serial communication (if on Arduino)
 * 5. Test with real hardware or simulator
 * 
 * BONUS:
 * - Add PWM for LED brightness
 * - Implement interrupt-driven motion detection
 * - Add EEPROM logging
 */
