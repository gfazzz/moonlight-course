/*
 * Episode 24: IoT Integration & System Bypass - SOLUTION (FINALE)
 * Operation MOONLIGHT - Season 6
 * 
 * Mission: Create backdoor in integrated security system
 * Version: v2.0 Enhanced Edition
 * 
 * Implemented features:
 * ✅ MQTT message simulation (pub/sub)
 * ✅ Integrated security system (Access + Cameras + Fire alarm)
 * ✅ Event injection and manipulation
 * ✅ Ghost mode (invisible bypass)
 * ✅ Log tampering and evidence removal
 * ✅ Backdoor installation
 * ✅ Forensics evasion
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAX_DEVICES 100
#define MAX_MESSAGES 1000
#define MAX_LOGS 1000

// Device types
typedef enum {
    DEVICE_DOOR,
    DEVICE_CAMERA,
    DEVICE_FIRE_ALARM,
    DEVICE_MOTION_SENSOR,
    DEVICE_SERVER
} DeviceType;

// MQTT message
typedef struct {
    char topic[64];
    char payload[256];
    time_t timestamp;
    bool injected;
} MQTTMessage;

// IoT Device
typedef struct {
    int id;
    char name[32];
    DeviceType type;
    char mqtt_topic[64];
    bool online;
    bool compromised;
    int messages_sent;
    int messages_received;
} IoTDevice;

// System log
typedef struct {
    time_t timestamp;
    char event[128];
    char source[32];
    bool tampered;
} SystemLog;

// System state
typedef struct {
    IoTDevice devices[MAX_DEVICES];
    int device_count;
    
    MQTTMessage messages[MAX_MESSAGES];
    int message_count;
    
    SystemLog logs[MAX_LOGS];
    int log_count;
    
    // Backdoor state
    bool backdoor_active;
    bool ghost_mode;
    int injected_events;
    int logs_tampered;
    
    // Statistics
    int total_messages;
    int compromised_devices;
} SecuritySystem;

SecuritySystem sys;

/*
 * MQTT Simulation
 */

void mqtt_publish(const char *topic, const char *payload, bool injected) {
    if (sys.message_count >= MAX_MESSAGES) return;
    
    MQTTMessage *msg = &sys.messages[sys.message_count];
    strncpy(msg->topic, topic, sizeof(msg->topic) - 1);
    strncpy(msg->payload, payload, sizeof(msg->payload) - 1);
    msg->timestamp = time(NULL);
    msg->injected = injected;
    
    sys.message_count++;
    sys.total_messages++;
    
    if (injected) {
        sys.injected_events++;
    }
    
    if (!sys.ghost_mode || !injected) {
        printf("📡 MQTT [%s]: %s\n", topic, payload);
    }
}

void mqtt_subscribe(IoTDevice *device) {
    // Simulate subscription
    device->messages_received++;
}

/*
 * System Logging
 */

void log_event(const char *event, const char *source, bool tampered) {
    if (sys.log_count >= MAX_LOGS) return;
    
    SystemLog *log = &sys.logs[sys.log_count];
    log->timestamp = time(NULL);
    strncpy(log->event, event, sizeof(log->event) - 1);
    strncpy(log->source, source, sizeof(log->source) - 1);
    log->tampered = tampered;
    
    sys.log_count++;
    
    if (tampered) {
        sys.logs_tampered++;
    }
    
    if (!sys.ghost_mode || !tampered) {
        char time_str[64];
        struct tm *tm_info = localtime(&log->timestamp);
        strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);
        
        printf("📝 LOG [%s] [%s]: %s\n", time_str, source, event);
    }
}

void tamper_log(int log_index) {
    if (log_index < 0 || log_index >= sys.log_count) return;
    
    sys.logs[log_index].tampered = true;
    strcpy(sys.logs[log_index].event, "[DELETED]");
    sys.logs_tampered++;
    
    if (!sys.ghost_mode) {
        printf("🔥 Log entry %d tampered\n", log_index);
    }
}

/*
 * Device Management
 */

void add_device(int id, const char *name, DeviceType type, const char *topic) {
    if (sys.device_count >= MAX_DEVICES) return;
    
    IoTDevice *dev = &sys.devices[sys.device_count];
    dev->id = id;
    strncpy(dev->name, name, sizeof(dev->name) - 1);
    dev->type = type;
    strncpy(dev->mqtt_topic, topic, sizeof(dev->mqtt_topic) - 1);
    dev->online = true;
    dev->compromised = false;
    dev->messages_sent = 0;
    dev->messages_received = 0;
    
    sys.device_count++;
}

void compromise_device(int device_id) {
    for (int i = 0; i < sys.device_count; i++) {
        if (sys.devices[i].id == device_id) {
            sys.devices[i].compromised = true;
            sys.compromised_devices++;
            
            if (!sys.ghost_mode) {
                printf("💀 Device compromised: %s\n", sys.devices[i].name);
            }
            
            log_event("Device compromised", sys.devices[i].name, false);
            return;
        }
    }
}

/*
 * Attack Functions
 */

void inject_door_unlock(const char *door_name) {
    printf("\n🚪 Injecting door unlock event...\n");
    
    char topic[64];
    snprintf(topic, sizeof(topic), "security/doors/%s/command", door_name);
    mqtt_publish(topic, "{\"action\":\"unlock\"}", true);
    
    log_event("Door unlocked (injected)", door_name, true);
    
    if (!sys.ghost_mode) {
        printf("✅ Door '%s' unlocked via MQTT injection\n", door_name);
    }
}

void inject_camera_blind(const char *camera_name) {
    printf("\n📹 Blinding camera...\n");
    
    char topic[64];
    snprintf(topic, sizeof(topic), "security/cameras/%s/status", camera_name);
    mqtt_publish(topic, "{\"status\":\"offline\",\"reason\":\"maintenance\"}", true);
    
    log_event("Camera offline (maintenance)", camera_name, false);
    
    // Tamper the real reason
    tamper_log(sys.log_count - 1);
    
    if (!sys.ghost_mode) {
        printf("✅ Camera '%s' blinded with fake maintenance message\n", camera_name);
    }
}

void inject_fire_alarm_false(void) {
    printf("\n🔥 Injecting false fire alarm...\n");
    
    mqtt_publish("security/fire/zone_A", "{\"status\":\"alarm\",\"zone\":\"A\"}", true);
    mqtt_publish("security/fire/zone_B", "{\"status\":\"alarm\",\"zone\":\"B\"}", true);
    
    log_event("FIRE ALARM - Zone A", "FireSystem", true);
    log_event("FIRE ALARM - Zone B", "FireSystem", true);
    
    if (!sys.ghost_mode) {
        printf("✅ False fire alarm triggered - creating distraction\n");
    }
}

void enable_ghost_mode(void) {
    sys.ghost_mode = true;
    printf("\n👻 Ghost mode ENABLED\n");
    printf("   All injected events will be invisible to monitoring.\n\n");
}

void disable_ghost_mode(void) {
    sys.ghost_mode = false;
    printf("\n👻 Ghost mode DISABLED\n\n");
}

void install_backdoor(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                     BACKDOOR INSTALLATION                         ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("🔓 Installing persistent backdoor...\n\n");
    
    // Step 1: Compromise MQTT broker
    printf("[1/5] Compromising MQTT broker...\n");
    compromise_device(10);  // Server
    printf("      ✅ Broker access obtained\n\n");
    
    // Step 2: Create rogue topic
    printf("[2/5] Creating rogue MQTT topics...\n");
    mqtt_publish("security/admin/backdoor", "{\"status\":\"active\",\"port\":31337}", false);
    printf("      ✅ Backdoor topic created\n\n");
    
    // Step 3: Inject persistent rules
    printf("[3/5] Injecting persistent firewall rules...\n");
    log_event("Firewall rule added: ALLOW 31337", "System", false);
    printf("      ✅ Firewall bypass installed\n\n");
    
    // Step 4: Install log cleanup
    printf("[4/5] Installing log cleanup routine...\n");
    for (int i = sys.log_count - 3; i < sys.log_count; i++) {
        if (i >= 0) tamper_log(i);
    }
    printf("      ✅ Evidence removed\n\n");
    
    // Step 5: Activate
    printf("[5/5] Activating backdoor...\n");
    sys.backdoor_active = true;
    printf("      ✅ Backdoor ACTIVE on port 31337\n\n");
    
    printf("🎉 SUCCESS! Persistent backdoor installed.\n");
    printf("   Access: nc <target_ip> 31337\n\n");
}

/*
 * Reporting
 */

void print_device_status(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                       DEVICE STATUS                               ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    for (int i = 0; i < sys.device_count; i++) {
        IoTDevice *dev = &sys.devices[i];
        
        const char *type_str = "Unknown";
        switch (dev->type) {
            case DEVICE_DOOR: type_str = "Door"; break;
            case DEVICE_CAMERA: type_str = "Camera"; break;
            case DEVICE_FIRE_ALARM: type_str = "Fire Alarm"; break;
            case DEVICE_MOTION_SENSOR: type_str = "Motion Sensor"; break;
            case DEVICE_SERVER: type_str = "Server"; break;
        }
        
        const char *status = dev->online ? "🟢" : "🔴";
        const char *security = dev->compromised ? "💀 COMPROMISED" : "🔒 Secure";
        
        printf("%s Device %d: %s (%s)\n", status, dev->id, dev->name, type_str);
        printf("   Topic: %s\n", dev->mqtt_topic);
        printf("   Security: %s\n", security);
        printf("   Messages: %d sent, %d received\n", dev->messages_sent, dev->messages_received);
        printf("\n");
    }
}

void print_attack_summary(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                      ATTACK SUMMARY                               ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("💀 Compromise Status:\n");
    printf("   Backdoor Active: %s\n", sys.backdoor_active ? "✅ YES" : "❌ NO");
    printf("   Ghost Mode: %s\n", sys.ghost_mode ? "👻 ACTIVE" : "⚪ Inactive");
    printf("   Compromised Devices: %d / %d\n", sys.compromised_devices, sys.device_count);
    printf("\n");
    
    printf("📊 Attack Statistics:\n");
    printf("   Total MQTT Messages: %d\n", sys.total_messages);
    printf("   Injected Events: %d\n", sys.injected_events);
    printf("   Logs Tampered: %d / %d\n", sys.logs_tampered, sys.log_count);
    printf("\n");
    
    printf("🎯 Attack Success Rate: %.1f%%\n",
           (float)sys.injected_events / sys.total_messages * 100.0);
    printf("\n");
}

/*
 * Main Program
 */

int main(void) {
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║  Episode 24: IoT Integration & System Bypass - SOLUTION (FINALE) ║\n");
    printf("║                 Operation MOONLIGHT - Season 6                    ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("🔧 Location: Skolkovo Innovation Center, Moscow, Russia\n");
    printf("🎯 Mission: Create backdoor in integrated security system\n\n");
    
    srand(time(NULL));
    memset(&sys, 0, sizeof(sys));
    
    // Initialize devices
    printf("📡 Initializing IoT devices...\n\n");
    
    add_device(1, "Main_Door", DEVICE_DOOR, "security/doors/main");
    add_device(2, "Lab_Door", DEVICE_DOOR, "security/doors/lab");
    add_device(3, "Entrance_Cam", DEVICE_CAMERA, "security/cameras/entrance");
    add_device(4, "Lab_Cam", DEVICE_CAMERA, "security/cameras/lab");
    add_device(5, "Motion_A", DEVICE_MOTION_SENSOR, "security/motion/zone_a");
    add_device(6, "Motion_B", DEVICE_MOTION_SENSOR, "security/motion/zone_b");
    add_device(7, "Fire_A", DEVICE_FIRE_ALARM, "security/fire/zone_a");
    add_device(8, "Fire_B", DEVICE_FIRE_ALARM, "security/fire/zone_b");
    add_device(10, "MQTT_Server", DEVICE_SERVER, "security/admin/*");
    
    printf("✅ %d devices initialized\n", sys.device_count);
    
    print_device_status();
    
    // Normal operation simulation
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                    NORMAL OPERATION (Baseline)                    ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    mqtt_publish("security/doors/main", "{\"status\":\"locked\"}", false);
    mqtt_publish("security/cameras/entrance", "{\"status\":\"recording\"}", false);
    log_event("System started", "Server", false);
    log_event("All devices online", "Server", false);
    
    printf("\n");
    
    // Start attack
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                        ATTACK SEQUENCE                            ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n");
    
    // Phase 1: Reconnaissance
    printf("\n=== PHASE 1: Reconnaissance ===\n");
    printf("🔍 Scanning MQTT topics...\n");
    printf("✅ Found %d devices\n", sys.device_count);
    printf("✅ Identified vulnerabilities\n");
    
    // Phase 2: Enable ghost mode
    printf("\n=== PHASE 2: Stealth Mode ===\n");
    enable_ghost_mode();
    
    // Phase 3: Compromise devices
    printf("=== PHASE 3: Device Compromise ===\n");
    compromise_device(3);  // Entrance camera
    compromise_device(4);  // Lab camera
    
    // Phase 4: Event injection
    printf("\n=== PHASE 4: Event Injection ===\n");
    inject_door_unlock("main");
    inject_camera_blind("entrance");
    inject_camera_blind("lab");
    inject_fire_alarm_false();
    
    // Phase 5: Install backdoor
    printf("\n=== PHASE 5: Backdoor Installation ===\n");
    install_backdoor();
    
    // Disable ghost mode for final report
    disable_ghost_mode();
    
    // Final reports
    print_device_status();
    print_attack_summary();
    
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                      MISSION COMPLETE!                            ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("🏆 Achievement Unlocked: \"IoT Master Hacker\"\n");
    printf("   You've mastered IoT system exploitation.\n\n");
    
    printf("📊 Final Stats:\n");
    printf("   Devices Compromised: %d\n", sys.compromised_devices);
    printf("   Events Injected: %d\n", sys.injected_events);
    printf("   Logs Tampered: %d\n", sys.logs_tampered);
    printf("   Backdoor: %s\n", sys.backdoor_active ? "✅ ACTIVE" : "❌ Inactive");
    printf("\n");
    
    printf("💡 Season 6 Complete!\n");
    printf("   You've mastered Embedded & IoT security.\n\n");
    
    printf("➡️  Next: Season 7 - System Programming\n");
    printf("    Master processes, threads, and IPC.\n\n");
    
    return 0;
}

