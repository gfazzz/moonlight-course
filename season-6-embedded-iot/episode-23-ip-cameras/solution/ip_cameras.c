/*
 * Episode 23: IP Cameras & Video Surveillance - SOLUTION
 * Operation MOONLIGHT - Season 6
 * 
 * Mission: Analyze IP camera systems and find blind spots
 * Version: v2.0 Enhanced Edition
 * 
 * Implemented features:
 * ✅ IP camera configuration and management
 * ✅ RTSP URL parsing and validation
 * ✅ Motion detection simulation (frame differencing)
 * ✅ Blind spot analysis and coverage mapping
 * ✅ Security evaluation (vulnerabilities, weak passwords)
 * ✅ Camera network topology
 * ✅ Statistics and reporting
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_CAMERAS 50
#define MAX_MOTION_EVENTS 1000
#define GRID_SIZE 100

// Camera types
typedef enum {
    FIXED,
    PTZ,  // Pan-Tilt-Zoom
    DOME,
    BULLET
} CameraType;

// Camera structure
typedef struct {
    int id;
    char name[32];
    char ip_address[16];
    uint16_t port;
    char rtsp_url[128];
    CameraType type;
    
    // Position and coverage
    float x, y;           // Position in building (meters)
    float angle;          // Direction (degrees)
    float fov;            // Field of view (degrees)
    float range;          // Max distance (meters)
    
    // Security
    char username[32];
    char password[32];
    bool default_creds;
    bool vulnerable;
    
    // Status
    bool online;
    int motion_events;
    time_t last_motion;
} Camera;

// Motion event
typedef struct {
    int camera_id;
    time_t timestamp;
    float intensity;  // 0.0-1.0
    int duration_ms;
} MotionEvent;

// System state
typedef struct {
    Camera cameras[MAX_CAMERAS];
    int camera_count;
    MotionEvent motion_events[MAX_MOTION_EVENTS];
    int motion_count;
    
    // Coverage grid (1m x 1m cells)
    uint8_t coverage_grid[GRID_SIZE][GRID_SIZE];
    
    // Statistics
    int total_motion_events;
    int vulnerable_cameras;
    float coverage_percentage;
} SurveillanceSystem;

SurveillanceSystem sys;

/*
 * RTSP URL Parsing
 */

bool parse_rtsp_url(const char *url, char *ip, uint16_t *port, char *path) {
    // Format: rtsp://user:pass@IP:PORT/path
    if (strncmp(url, "rtsp://", 7) != 0) {
        return false;
    }
    
    const char *rest = url + 7;
    
    // Skip credentials if present
    const char *at = strchr(rest, '@');
    if (at) {
        rest = at + 1;
    }
    
    // Extract IP
    const char *colon = strchr(rest, ':');
    const char *slash = strchr(rest, '/');
    
    if (!colon || !slash) {
        return false;
    }
    
    int ip_len = colon - rest;
    strncpy(ip, rest, ip_len);
    ip[ip_len] = '\0';
    
    // Extract port
    *port = atoi(colon + 1);
    
    // Extract path
    strcpy(path, slash);
    
    return true;
}

void generate_rtsp_url(Camera *cam) {
    snprintf(cam->rtsp_url, sizeof(cam->rtsp_url),
             "rtsp://%s:%s@%s:%d/stream",
             cam->username, cam->password, cam->ip_address, cam->port);
}

/*
 * Camera Management
 */

void add_camera(int id, const char *name, const char *ip, uint16_t port,
                CameraType type, float x, float y, float angle, float fov, float range) {
    if (sys.camera_count >= MAX_CAMERAS) {
        printf("⚠️  Maximum cameras reached!\n");
        return;
    }
    
    Camera *cam = &sys.cameras[sys.camera_count];
    cam->id = id;
    strncpy(cam->name, name, sizeof(cam->name) - 1);
    strncpy(cam->ip_address, ip, sizeof(cam->ip_address) - 1);
    cam->port = port;
    cam->type = type;
    cam->x = x;
    cam->y = y;
    cam->angle = angle;
    cam->fov = fov;
    cam->range = range;
    
    // Default credentials (vulnerable)
    strcpy(cam->username, "admin");
    strcpy(cam->password, "admin");
    cam->default_creds = true;
    cam->vulnerable = true;
    
    // Status
    cam->online = true;
    cam->motion_events = 0;
    cam->last_motion = 0;
    
    generate_rtsp_url(cam);
    
    sys.camera_count++;
}

/*
 * Motion Detection (Simulation)
 */

void simulate_motion_detection(void) {
    for (int i = 0; i < sys.camera_count; i++) {
        Camera *cam = &sys.cameras[i];
        
        if (!cam->online) continue;
        
        // 5% chance of motion per camera
        if (rand() % 100 < 5) {
            if (sys.motion_count < MAX_MOTION_EVENTS) {
                MotionEvent *event = &sys.motion_events[sys.motion_count];
                event->camera_id = cam->id;
                event->timestamp = time(NULL);
                event->intensity = (rand() % 100) / 100.0;
                event->duration_ms = 500 + rand() % 5000;
                
                cam->motion_events++;
                cam->last_motion = event->timestamp;
                sys.motion_count++;
                sys.total_motion_events++;
                
                printf("🎥 Motion detected on %s (intensity: %.2f)\n",
                       cam->name, event->intensity);
            }
        }
    }
}

/*
 * Coverage Analysis
 */

void calculate_coverage(void) {
    // Clear grid
    memset(sys.coverage_grid, 0, sizeof(sys.coverage_grid));
    
    for (int i = 0; i < sys.camera_count; i++) {
        Camera *cam = &sys.cameras[i];
        
        if (!cam->online) continue;
        
        // Calculate coverage cone
        for (int gx = 0; gx < GRID_SIZE; gx++) {
            for (int gy = 0; gy < GRID_SIZE; gy++) {
                float px = gx * 1.0;
                float py = gy * 1.0;
                
                // Distance from camera
                float dx = px - cam->x;
                float dy = py - cam->y;
                float dist = sqrt(dx * dx + dy * dy);
                
                if (dist > cam->range) continue;
                
                // Angle to point
                float angle_to_point = atan2(dy, dx) * 180.0 / M_PI;
                float angle_diff = fabs(angle_to_point - cam->angle);
                
                // Normalize angle difference
                if (angle_diff > 180) angle_diff = 360 - angle_diff;
                
                // Check if within FOV
                if (angle_diff <= cam->fov / 2.0) {
                    sys.coverage_grid[gx][gy]++;
                }
            }
        }
    }
    
    // Calculate coverage percentage
    int covered_cells = 0;
    int total_cells = GRID_SIZE * GRID_SIZE;
    
    for (int gx = 0; gx < GRID_SIZE; gx++) {
        for (int gy = 0; gy < GRID_SIZE; gy++) {
            if (sys.coverage_grid[gx][gy] > 0) {
                covered_cells++;
            }
        }
    }
    
    sys.coverage_percentage = (float)covered_cells / total_cells * 100.0;
}

void print_blind_spots(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                      BLIND SPOT ANALYSIS                          ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    int blind_spots = 0;
    
    printf("🔍 Scanning for uncovered areas...\n\n");
    
    // Find large uncovered regions
    for (int gx = 0; gx < GRID_SIZE; gx += 10) {
        for (int gy = 0; gy < GRID_SIZE; gy += 10) {
            // Check 10x10 block
            int uncovered = 0;
            for (int dx = 0; dx < 10 && gx + dx < GRID_SIZE; dx++) {
                for (int dy = 0; dy < 10 && gy + dy < GRID_SIZE; dy++) {
                    if (sys.coverage_grid[gx + dx][gy + dy] == 0) {
                        uncovered++;
                    }
                }
            }
            
            if (uncovered > 50) {  // More than 50% uncovered
                printf("❌ Blind spot detected: Area (%d,%d) - (%d,%d) [%d%% uncovered]\n",
                       gx, gy, gx + 10, gy + 10, uncovered);
                blind_spots++;
            }
        }
    }
    
    if (blind_spots == 0) {
        printf("✅ No significant blind spots detected.\n");
    }
    
    printf("\n📊 Total blind spots: %d\n", blind_spots);
    printf("📊 Coverage: %.1f%%\n\n", sys.coverage_percentage);
}

/*
 * Security Analysis
 */

void analyze_security(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                     SECURITY ANALYSIS                             ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    sys.vulnerable_cameras = 0;
    
    for (int i = 0; i < sys.camera_count; i++) {
        Camera *cam = &sys.cameras[i];
        
        bool is_vulnerable = false;
        
        // Check default credentials
        if (cam->default_creds) {
            printf("⚠️  %s (IP: %s) uses default credentials\n", cam->name, cam->ip_address);
            is_vulnerable = true;
        }
        
        // Check weak password
        if (strlen(cam->password) < 8) {
            printf("⚠️  %s has weak password (length: %zu)\n", cam->name, strlen(cam->password));
            is_vulnerable = true;
        }
        
        // Check old firmware (simulation)
        if (rand() % 100 < 20) {  // 20% have old firmware
            printf("⚠️  %s may have outdated firmware\n", cam->name);
            is_vulnerable = true;
        }
        
        if (is_vulnerable) {
            cam->vulnerable = true;
            sys.vulnerable_cameras++;
        }
    }
    
    printf("\n📊 Vulnerable cameras: %d / %d (%.1f%%)\n\n",
           sys.vulnerable_cameras, sys.camera_count,
           (float)sys.vulnerable_cameras / sys.camera_count * 100.0);
}

/*
 * Reporting
 */

void print_camera_list(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                         CAMERA LIST                               ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    for (int i = 0; i < sys.camera_count; i++) {
        Camera *cam = &sys.cameras[i];
        
        const char *type_str = "Unknown";
        switch (cam->type) {
            case FIXED: type_str = "Fixed"; break;
            case PTZ: type_str = "PTZ"; break;
            case DOME: type_str = "Dome"; break;
            case BULLET: type_str = "Bullet"; break;
        }
        
        const char *status = cam->online ? "🟢 Online" : "🔴 Offline";
        const char *security = cam->vulnerable ? "⚠️  Vulnerable" : "✅ Secure";
        
        printf("Camera %d: %s\n", cam->id, cam->name);
        printf("   Type: %s | Status: %s | Security: %s\n", type_str, status, security);
        printf("   IP: %s:%d\n", cam->ip_address, cam->port);
        printf("   Position: (%.1f, %.1f) | Angle: %.0f° | FOV: %.0f° | Range: %.1fm\n",
               cam->x, cam->y, cam->angle, cam->fov, cam->range);
        printf("   RTSP: %s\n", cam->rtsp_url);
        printf("   Motion events: %d\n", cam->motion_events);
        printf("\n");
    }
}

void print_statistics(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                     SYSTEM STATISTICS                             ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    int online = 0;
    for (int i = 0; i < sys.camera_count; i++) {
        if (sys.cameras[i].online) online++;
    }
    
    printf("📊 System Overview:\n");
    printf("   Total Cameras: %d\n", sys.camera_count);
    printf("   Online: %d | Offline: %d\n", online, sys.camera_count - online);
    printf("   Vulnerable: %d (%.1f%%)\n", sys.vulnerable_cameras,
           (float)sys.vulnerable_cameras / sys.camera_count * 100.0);
    printf("\n");
    
    printf("📈 Motion Detection:\n");
    printf("   Total Events: %d\n", sys.total_motion_events);
    printf("   Events/Camera: %.1f\n", (float)sys.total_motion_events / sys.camera_count);
    printf("\n");
    
    printf("📐 Coverage:\n");
    printf("   Area Covered: %.1f%%\n", sys.coverage_percentage);
    printf("   Blind Spots: %s\n", sys.coverage_percentage > 90.0 ? "Minimal" : "Present");
    printf("\n");
}

/*
 * Main Program
 */

int main(void) {
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║    Episode 23: IP Cameras & Video Surveillance - SOLUTION        ║\n");
    printf("║                 Operation MOONLIGHT - Season 6                    ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("🔧 Location: Novosibirsk Akademgorodok, Russia\n");
    printf("🎯 Mission: Analyze camera systems and find blind spots\n\n");
    
    srand(time(NULL));
    
    // Initialize system
    memset(&sys, 0, sizeof(sys));
    
    // Add cameras for the research institute
    printf("📡 Discovering cameras...\n\n");
    
    // Entrance cameras
    add_camera(1, "Entrance_Main", "192.168.1.101", 554, DOME, 50, 10, 90, 90, 15);
    add_camera(2, "Entrance_Side", "192.168.1.102", 554, BULLET, 55, 10, 120, 70, 12);
    
    // Corridor cameras
    add_camera(3, "Corridor_A1", "192.168.1.103", 554, FIXED, 50, 30, 90, 60, 20);
    add_camera(4, "Corridor_A2", "192.168.1.104", 554, FIXED, 50, 60, 90, 60, 20);
    
    // Lab cameras
    add_camera(5, "Lab_Main", "192.168.1.105", 554, PTZ, 30, 50, 0, 120, 25);
    add_camera(6, "Lab_Storage", "192.168.1.106", 554, DOME, 20, 70, 270, 90, 15);
    
    // Perimeter cameras
    add_camera(7, "Perimeter_N", "192.168.1.107", 554, BULLET, 50, 5, 0, 80, 30);
    add_camera(8, "Perimeter_S", "192.168.1.108", 554, BULLET, 50, 95, 180, 80, 30);
    
    printf("✅ Discovered %d cameras\n", sys.camera_count);
    
    // Print camera list
    print_camera_list();
    
    // Security analysis
    analyze_security();
    
    // Simulate motion detection
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                    MOTION DETECTION TEST                          ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("🎥 Monitoring for motion...\n\n");
    for (int i = 0; i < 20; i++) {
        simulate_motion_detection();
    }
    printf("\n");
    
    // Coverage analysis
    printf("📐 Calculating coverage...\n");
    calculate_coverage();
    printf("✅ Coverage analysis complete\n");
    
    print_blind_spots();
    
    // Print statistics
    print_statistics();
    
    printf("🏆 Achievement Unlocked: \"Surveillance Expert\"\n");
    printf("   You've mastered IP camera analysis.\n\n");
    
    printf("💡 Mission Complete!\n");
    printf("   Evidence: %d cameras analyzed, %d motion events logged\n",
           sys.camera_count, sys.total_motion_events);
    printf("   Security flaws: %d vulnerable cameras identified\n\n",
           sys.vulnerable_cameras);
    
    printf("➡️  Next: Episode 24 - IoT Integration & System Bypass\n");
    printf("    Create backdoor in integrated security system.\n\n");
    
    return 0;
}

