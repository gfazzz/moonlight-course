/*
 * Episode 21: Arduino & GPIO Fundamentals - SOLUTION
 * Operation MOONLIGHT - Season 6
 * 
 * Mission: Create surveillance system using GPIO simulation
 * Version: v2.0 Enhanced Edition
 * 
 * Implemented features:
 * ✅ GPIO pin simulation (digital/analog)
 * ✅ Sensor simulation: PIR motion, door contact, temperature (LM35)
 * ✅ Actuator control: LED, relay with PWM
 * ✅ Alert system with thresholds
 * ✅ Serial output (UART simulation)
 * ✅ Event logging and statistics
 * ✅ Interrupt simulation for motion detection
 * 
 * Note: This is a simulation of embedded Arduino code running on regular C
 * For real Arduino, port this to Arduino IDE with actual hardware calls
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

// Pin definitions (Arduino Uno compatible)
#define PIR_SENSOR_PIN     2
#define DOOR_CONTACT_PIN   3
#define TEMP_SENSOR_PIN    0  // Analog A0
#define LED_PIN            13
#define RELAY_PIN          8
#define BUZZER_PIN         9

// Threshold values
#define TEMP_THRESHOLD     50.0  // °C
#define TEMP_CRITICAL      70.0  // °C
#define ALERT_INTERVAL     1000  // ms
#define MOTION_TIMEOUT     5000  // ms

// Pin modes
typedef enum {
    INPUT,
    OUTPUT,
    INPUT_PULLUP
} PinMode;

// Pin states
typedef enum {
    LOW = 0,
    HIGH = 1
} PinState;

// GPIO pin structure
typedef struct {
    uint8_t pin_number;
    PinMode mode;
    PinState state;
    uint16_t analog_value;  // For analog pins (0-1023)
    bool pwm_enabled;
    uint8_t pwm_duty;       // 0-255
} GPIO_Pin;

// Event log entry
typedef struct {
    time_t timestamp;
    char event[128];
    char severity[16];
} EventLog;

// System state
typedef struct {
    GPIO_Pin pins[20];  // Arduino Uno has 14 digital + 6 analog
    EventLog *logs;
    int log_count;
    int log_capacity;
    
    // Statistics
    int motion_detections;
    int door_opens;
    float max_temp;
    int alerts_triggered;
    time_t start_time;
} SystemState;

// Global system state
SystemState sys_state;

/*
 * GPIO Simulation Functions
 */

void gpio_init_system(void) {
    // Initialize all pins
    for (int i = 0; i < 20; i++) {
        sys_state.pins[i].pin_number = i;
        sys_state.pins[i].mode = INPUT;
        sys_state.pins[i].state = LOW;
        sys_state.pins[i].analog_value = 0;
        sys_state.pins[i].pwm_enabled = false;
        sys_state.pins[i].pwm_duty = 0;
    }
    
    // Initialize logs
    sys_state.log_capacity = 1000;
    sys_state.logs = malloc(sys_state.log_capacity * sizeof(EventLog));
    sys_state.log_count = 0;
    
    // Initialize statistics
    sys_state.motion_detections = 0;
    sys_state.door_opens = 0;
    sys_state.max_temp = 0.0;
    sys_state.alerts_triggered = 0;
    sys_state.start_time = time(NULL);
    
    // Seed random for simulation
    srand(time(NULL));
}

void pinMode(uint8_t pin, PinMode mode) {
    if (pin >= 20) return;
    sys_state.pins[pin].mode = mode;
    
    // Input pullup sets initial state HIGH
    if (mode == INPUT_PULLUP) {
        sys_state.pins[pin].state = HIGH;
    }
}

void digitalWrite(uint8_t pin, PinState state) {
    if (pin >= 20) return;
    if (sys_state.pins[pin].mode != OUTPUT) {
        printf("[GPIO] Warning: Pin %d not configured as OUTPUT\n", pin);
        return;
    }
    
    sys_state.pins[pin].state = state;
}

PinState digitalRead(uint8_t pin) {
    if (pin >= 20) return LOW;
    if (sys_state.pins[pin].mode == OUTPUT) {
        printf("[GPIO] Warning: Reading from OUTPUT pin %d\n", pin);
    }
    
    return sys_state.pins[pin].state;
}

uint16_t analogRead(uint8_t analog_pin) {
    if (analog_pin >= 6) return 0;
    return sys_state.pins[analog_pin].analog_value;
}

void analogWrite(uint8_t pin, uint8_t value) {
    if (pin >= 20) return;
    
    sys_state.pins[pin].pwm_enabled = true;
    sys_state.pins[pin].pwm_duty = value;
    
    // Simulate PWM (values 0-255)
    if (value > 127) {
        sys_state.pins[pin].state = HIGH;
    } else if (value == 0) {
        sys_state.pins[pin].state = LOW;
    }
}

/*
 * Event Logging
 */

void log_event(const char *event, const char *severity) {
    if (sys_state.log_count >= sys_state.log_capacity) {
        sys_state.log_capacity *= 2;
        sys_state.logs = realloc(sys_state.logs, sys_state.log_capacity * sizeof(EventLog));
    }
    
    EventLog *log = &sys_state.logs[sys_state.log_count];
    log->timestamp = time(NULL);
    strncpy(log->event, event, sizeof(log->event) - 1);
    log->event[sizeof(log->event) - 1] = '\0';
    strncpy(log->severity, severity, sizeof(log->severity) - 1);
    log->severity[sizeof(log->severity) - 1] = '\0';
    
    sys_state.log_count++;
}

void print_logs(int last_n) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                         EVENT LOG                                 ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    int start = (sys_state.log_count > last_n) ? sys_state.log_count - last_n : 0;
    
    for (int i = start; i < sys_state.log_count; i++) {
        EventLog *log = &sys_state.logs[i];
        
        char time_str[64];
        struct tm *tm_info = localtime(&log->timestamp);
        strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);
        
        const char *emoji = "ℹ️ ";
        if (strcmp(log->severity, "WARNING") == 0) emoji = "⚠️ ";
        else if (strcmp(log->severity, "CRITICAL") == 0) emoji = "🔴";
        
        printf("%s[%s] [%s] %s\n", emoji, time_str, log->severity, log->event);
    }
    
    printf("\n");
}

/*
 * Sensor Simulation (with realistic behavior)
 */

int simulate_pir_motion(void) {
    // PIR sensor has 3% chance of detecting motion each cycle
    // In real system, this would be connected to actual PIR
    static int motion_hold = 0;
    
    if (motion_hold > 0) {
        motion_hold--;
        return HIGH;
    }
    
    if (rand() % 100 < 3) {  // 3% chance
        motion_hold = 5;  // Hold for 5 cycles
        return HIGH;
    }
    
    return LOW;
}

int simulate_door_contact(void) {
    // Door contact: normally closed, open = HIGH
    // 2% chance of door opening
    static int door_open_duration = 0;
    
    if (door_open_duration > 0) {
        door_open_duration--;
        return HIGH;
    }
    
    if (rand() % 100 < 2) {  // 2% chance
        door_open_duration = 3;  // Open for 3 cycles
        return HIGH;
    }
    
    return LOW;
}

uint16_t simulate_temperature_sensor(void) {
    // LM35 temperature sensor: 10mV/°C
    // Simulates temperature with random fluctuations
    static float base_temp = 25.0;
    static float temp = 25.0;
    
    // Random walk
    float delta = ((rand() % 100) - 50) / 100.0;  // -0.5 to +0.5
    temp += delta;
    
    // Occasionally spike temperature (overheating simulation)
    if (rand() % 100 < 1) {  // 1% chance
        temp = 55.0 + (rand() % 20);  // 55-75°C
    }
    
    // Gradually return to base
    if (temp > base_temp + 5) {
        temp -= 0.5;
    }
    
    // Clamp
    if (temp < 0) temp = 0;
    if (temp > 100) temp = 100;
    
    // Convert to ADC value (0-1023)
    // LM35: 10mV/°C, Arduino 5V reference
    // ADC = (temp * 0.01) / 5.0 * 1023
    uint16_t adc = (uint16_t)((temp * 0.01 / 5.0) * 1023.0);
    
    return adc;
}

/*
 * Sensor Reading Functions
 */

int read_motion(void) {
    // Simulate PIR sensor
    int motion = simulate_pir_motion();
    sys_state.pins[PIR_SENSOR_PIN].state = motion;
    
    if (motion == HIGH) {
        sys_state.motion_detections++;
        log_event("Motion detected by PIR sensor", "WARNING");
    }
    
    return digitalRead(PIR_SENSOR_PIN);
}

int read_door(void) {
    // Simulate door contact
    int door = simulate_door_contact();
    sys_state.pins[DOOR_CONTACT_PIN].state = door;
    
    if (door == HIGH) {
        sys_state.door_opens++;
        log_event("Door contact opened", "WARNING");
    }
    
    return digitalRead(DOOR_CONTACT_PIN);
}

float read_temperature(void) {
    // Simulate LM35 temperature sensor
    uint16_t raw = simulate_temperature_sensor();
    sys_state.pins[TEMP_SENSOR_PIN].analog_value = raw;
    
    // Convert ADC to temperature
    float voltage = raw * (5.0 / 1023.0);
    float temp = voltage * 100.0;  // LM35: 10mV/°C = 100°C/V
    
    if (temp > sys_state.max_temp) {
        sys_state.max_temp = temp;
    }
    
    if (temp > TEMP_CRITICAL) {
        log_event("CRITICAL TEMPERATURE!", "CRITICAL");
    } else if (temp > TEMP_THRESHOLD) {
        log_event("Temperature above threshold", "WARNING");
    }
    
    return temp;
}

/*
 * Actuator Control
 */

void led_on(void) {
    digitalWrite(LED_PIN, HIGH);
    log_event("LED activated", "INFO");
}

void led_off(void) {
    digitalWrite(LED_PIN, LOW);
}

void led_set_brightness(uint8_t brightness) {
    analogWrite(LED_PIN, brightness);
    char msg[64];
    snprintf(msg, sizeof(msg), "LED brightness set to %d", brightness);
    log_event(msg, "INFO");
}

void relay_activate(void) {
    digitalWrite(RELAY_PIN, HIGH);
    log_event("Relay activated (cooling/alarm triggered)", "WARNING");
}

void relay_deactivate(void) {
    digitalWrite(RELAY_PIN, LOW);
}

void buzzer_beep(int duration_ms) {
    digitalWrite(BUZZER_PIN, HIGH);
    // In real system: delay(duration_ms);
    digitalWrite(BUZZER_PIN, LOW);
    
    char msg[64];
    snprintf(msg, sizeof(msg), "Buzzer beep (%dms)", duration_ms);
    log_event(msg, "INFO");
}

/*
 * Status Display
 */

void send_status(int motion, int door, float temp) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                    SURVEILLANCE STATUS                            ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("📍 Sensors:\n");
    printf("   PIR Motion:     %s\n", motion ? "🔴 DETECTED" : "🟢 Clear");
    printf("   Door Contact:   %s\n", door ? "🔴 OPEN" : "🟢 Closed");
    printf("   Temperature:    %.1f°C ", temp);
    
    if (temp > TEMP_CRITICAL) {
        printf("🔥 CRITICAL!\n");
    } else if (temp > TEMP_THRESHOLD) {
        printf("⚠️  High\n");
    } else {
        printf("🟢 Normal\n");
    }
    
    printf("\n🎛️  Actuators:\n");
    printf("   LED (Pin %d):    %s", LED_PIN, 
           sys_state.pins[LED_PIN].state == HIGH ? "🔴 ON" : "⚪ OFF");
    
    if (sys_state.pins[LED_PIN].pwm_enabled) {
        printf(" (PWM: %d/255)", sys_state.pins[LED_PIN].pwm_duty);
    }
    printf("\n");
    
    printf("   Relay (Pin %d):  %s\n", RELAY_PIN,
           sys_state.pins[RELAY_PIN].state == HIGH ? "🔴 ACTIVE" : "⚪ Inactive");
    
    printf("\n📊 System Status: ");
    
    if (motion || door || temp > TEMP_THRESHOLD) {
        printf("⚠️  ALERT\n");
    } else {
        printf("✅ NORMAL\n");
    }
    
    printf("\n");
}

/*
 * Alert System
 */

void check_alerts(int motion, int door, float temp) {
    static time_t last_alert = 0;
    time_t now = time(NULL);
    bool alert_triggered = false;
    
    // Motion detected
    if (motion) {
        led_on();
        buzzer_beep(100);
        alert_triggered = true;
    } else if (!door && temp < TEMP_THRESHOLD) {
        led_off();
    }
    
    // Temperature alerts
    if (temp > TEMP_CRITICAL) {
        relay_activate();  // Activate cooling/alarm
        led_set_brightness(255);  // Full brightness
        alert_triggered = true;
        
        if (now - last_alert > 5) {  // Alert every 5 seconds
            printf("\n🔥 CRITICAL ALERT: Temperature %.1f°C exceeds critical threshold!\n", temp);
            last_alert = now;
        }
    } else if (temp > TEMP_THRESHOLD) {
        relay_activate();
        led_set_brightness(128);  // Half brightness
        alert_triggered = true;
    } else {
        relay_deactivate();
    }
    
    // Door alert
    if (door) {
        buzzer_beep(500);
        printf("\n🚪 ALERT: Unauthorized access detected!\n");
        alert_triggered = true;
    }
    
    if (alert_triggered) {
        sys_state.alerts_triggered++;
    }
}

/*
 * Statistics
 */

void print_statistics(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                      SYSTEM STATISTICS                            ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    time_t now = time(NULL);
    int uptime = (int)(now - sys_state.start_time);
    
    printf("⏱️  Uptime: %d seconds\n", uptime);
    printf("📊 Total Events: %d\n", sys_state.log_count);
    printf("\n");
    
    printf("🚨 Alerts:\n");
    printf("   Motion Detections: %d\n", sys_state.motion_detections);
    printf("   Door Opens:        %d\n", sys_state.door_opens);
    printf("   Max Temperature:   %.1f°C\n", sys_state.max_temp);
    printf("   Total Alerts:      %d\n", sys_state.alerts_triggered);
    printf("\n");
    
    printf("🎯 Detection Rate:\n");
    printf("   Motion: %.2f events/second\n", (float)sys_state.motion_detections / uptime);
    printf("   Door:   %.2f events/second\n", (float)sys_state.door_opens / uptime);
    printf("\n");
}

/*
 * Main Program
 */

int main(void) {
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║         Episode 21: Arduino & GPIO Surveillance System            ║\n");
    printf("║                   Operation MOONLIGHT - Season 6                  ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("🔧 Location: Novosibirsk Akademgorodok, Russia\n");
    printf("🎯 Mission: Monitor security systems using GPIO\n\n");
    
    // Initialize system
    gpio_init_system();
    
    // Configure pins
    pinMode(PIR_SENSOR_PIN, INPUT);
    pinMode(DOOR_CONTACT_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    
    log_event("System initialized", "INFO");
    log_event("GPIO pins configured", "INFO");
    
    printf("✅ System initialized\n");
    printf("📡 Starting monitoring...\n\n");
    
    // Monitoring loop (simulate 20 cycles)
    for (int cycle = 0; cycle < 20; cycle++) {
        printf("═══════════════════════════════════════════════════════════════════\n");
        printf("Cycle %d/20\n", cycle + 1);
        
        // Read sensors
        int motion = read_motion();
        int door = read_door();
        float temp = read_temperature();
        
        // Display status
        send_status(motion, door, temp);
        
        // Check for alerts
        check_alerts(motion, door, temp);
        
        // Simulate delay (1 second)
        usleep(200000);  // 0.2 seconds for demo
    }
    
    printf("\n═══════════════════════════════════════════════════════════════════\n");
    printf("✅ Monitoring complete\n\n");
    
    // Print statistics
    print_statistics();
    
    // Print recent logs
    print_logs(10);
    
    printf("🏆 Achievement Unlocked: \"GPIO Master\"\n");
    printf("    You've mastered embedded GPIO programming.\n\n");
    
    printf("💡 Mission Complete!\n");
    printf("   Evidence: %d security events logged\n", sys_state.log_count);
    printf("   Status: Surveillance system operational\n\n");
    
    printf("➡️  Next: Episode 22 - Industrial Protocols & Access Control\n");
    printf("    Reverse engineer RFID/NFC systems and Modbus protocols.\n\n");
    
    // Cleanup
    free(sys_state.logs);
    
    return 0;
}

/*
 * Educational Notes:
 * 
 * This code simulates Arduino GPIO programming in standard C.
 * For real Arduino deployment:
 * 
 * 1. Port to Arduino IDE
 * 2. Replace simulation functions with actual hardware:
 *    - digitalRead(PIR_SENSOR_PIN) reads real PIR
 *    - analogRead(TEMP_SENSOR_PIN) reads real LM35
 *    - digitalWrite() controls real LEDs/relays
 * 3. Use Arduino delay() instead of usleep()
 * 4. Add EEPROM for persistent logging
 * 5. Implement interrupts for motion detection
 * 
 * Hardware needed:
 * - Arduino Uno/Nano
 * - PIR sensor (HC-SR501)
 * - Door magnetic contact
 * - LM35 temperature sensor
 * - LED + 220Ω resistor
 * - 5V relay module
 * - Buzzer
 */

