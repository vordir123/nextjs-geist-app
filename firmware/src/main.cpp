/**
 * ESP32 eBike Tuning Chip - Main Application
 * 
 * This is the main entry point for the ESP32-based eBike tuning chip firmware.
 * It initializes all modules and manages the main application loop.
 * 
 * @author ESP32 eBike Development Team
 * @version 1.0.0
 * @date 2024
 */

#include <Arduino.h>
#include <WiFi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

// Module includes
#include "modules/CANBusHandler.h"
#include "modules/SensorEmulator.h"
#include "modules/ConfigManager.h"
#include "modules/OTAUpdater.h"
#include "modules/SafetyMonitor.h"

// Configuration constants
#define FIRMWARE_VERSION "1.0.0"
#define DEVICE_NAME "ESP32-eBike-Tuning-Chip"

// Task handles
TaskHandle_t canBusTaskHandle = NULL;
TaskHandle_t sensorTaskHandle = NULL;
TaskHandle_t safetyTaskHandle = NULL;
TaskHandle_t otaTaskHandle = NULL;

// Module instances
CANBusHandler canBusHandler;
SensorEmulator sensorEmulator;
ConfigManager configManager;
OTAUpdater otaUpdater;
SafetyMonitor safetyMonitor;

// System state
typedef enum {
    SYSTEM_INIT,
    SYSTEM_NORMAL,
    SYSTEM_TUNING_ACTIVE,
    SYSTEM_STEALTH_MODE,
    SYSTEM_ERROR,
    SYSTEM_SHUTDOWN
} SystemState;

SystemState currentState = SYSTEM_INIT;

/**
 * CAN Bus Task - Handles CAN communication with Bosch system
 */
void canBusTask(void *parameter) {
    Serial.println("CAN Bus Task started");
    
    while (true) {
        if (currentState == SYSTEM_NORMAL || currentState == SYSTEM_TUNING_ACTIVE) {
            canBusHandler.processMessages();
            canBusHandler.sendHeartbeat();
        }
        
        vTaskDelay(pdMS_TO_TICKS(10)); // 10ms cycle time
    }
}

/**
 * Sensor Emulation Task - Handles speed signal processing
 */
void sensorTask(void *parameter) {
    Serial.println("Sensor Task started");
    
    while (true) {
        if (currentState == SYSTEM_TUNING_ACTIVE) {
            sensorEmulator.processSpeedSignal();
        } else if (currentState == SYSTEM_STEALTH_MODE) {
            sensorEmulator.stealthMode();
        }
        
        vTaskDelay(pdMS_TO_TICKS(5)); // 5ms cycle time for real-time processing
    }
}

/**
 * Safety Monitor Task - Monitors system health and safety parameters
 */
void safetyTask(void *parameter) {
    Serial.println("Safety Monitor Task started");
    
    while (true) {
        SafetyStatus status = safetyMonitor.checkSystemHealth();
        
        if (status.criticalError) {
            Serial.println("CRITICAL ERROR: Switching to safe mode");
            currentState = SYSTEM_ERROR;
            sensorEmulator.disableTuning();
        }
        
        if (status.temperatureWarning) {
            Serial.println("Temperature warning - reducing performance");
            sensorEmulator.setPerformanceMode(PERFORMANCE_REDUCED);
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 second cycle time
    }
}

/**
 * OTA Update Task - Handles over-the-air firmware updates
 */
void otaTask(void *parameter) {
    Serial.println("OTA Task started");
    
    while (true) {
        if (WiFi.status() == WL_CONNECTED) {
            otaUpdater.checkForUpdates();
        }
        
        vTaskDelay(pdMS_TO_TICKS(30000)); // Check every 30 seconds
    }
}

/**
 * Initialize all system modules
 */
void initializeModules() {
    Serial.println("Initializing modules...");
    
    // Initialize configuration manager first
    if (!configManager.begin()) {
        Serial.println("ERROR: Failed to initialize ConfigManager");
        return;
    }
    
    // Load configuration
    DeviceConfig config = configManager.loadConfig();
    
    // Initialize CAN bus handler
    if (!canBusHandler.begin(config.canBusConfig)) {
        Serial.println("ERROR: Failed to initialize CAN Bus");
        return;
    }
    
    // Initialize sensor emulator
    if (!sensorEmulator.begin(config.sensorConfig)) {
        Serial.println("ERROR: Failed to initialize Sensor Emulator");
        return;
    }
    
    // Initialize safety monitor
    if (!safetyMonitor.begin(config.safetyConfig)) {
        Serial.println("ERROR: Failed to initialize Safety Monitor");
        return;
    }
    
    // Initialize OTA updater
    if (!otaUpdater.begin(config.otaConfig)) {
        Serial.println("WARNING: Failed to initialize OTA Updater");
        // Non-critical, continue without OTA
    }
    
    Serial.println("All modules initialized successfully");
}

/**
 * Create and start FreeRTOS tasks
 */
void createTasks() {
    Serial.println("Creating tasks...");
    
    // Create CAN Bus task
    xTaskCreatePinnedToCore(
        canBusTask,
        "CANBusTask",
        4096,
        NULL,
        2,
        &canBusTaskHandle,
        0  // Pin to core 0
    );
    
    // Create Sensor task
    xTaskCreatePinnedToCore(
        sensorTask,
        "SensorTask",
        4096,
        NULL,
        3,  // Higher priority for real-time processing
        &sensorTaskHandle,
        1   // Pin to core 1
    );
    
    // Create Safety task
    xTaskCreatePinnedToCore(
        safetyTask,
        "SafetyTask",
        2048,
        NULL,
        1,
        &safetyTaskHandle,
        0   // Pin to core 0
    );
    
    // Create OTA task
    xTaskCreatePinnedToCore(
        otaTask,
        "OTATask",
        8192,
        NULL,
        0,  // Lowest priority
        &otaTaskHandle,
        0   // Pin to core 0
    );
    
    Serial.println("All tasks created successfully");
}

/**
 * Setup function - called once at startup
 */
void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    Serial.println();
    Serial.println("=================================");
    Serial.println("ESP32 eBike Tuning Chip");
    Serial.println("Firmware Version: " FIRMWARE_VERSION);
    Serial.println("=================================");
    
    // Initialize modules
    initializeModules();
    
    // Create FreeRTOS tasks
    createTasks();
    
    // Set initial system state
    currentState = SYSTEM_NORMAL;
    
    Serial.println("System initialization complete");
    Serial.println("Entering main loop...");
}

/**
 * Main loop - handles system state management
 */
void loop() {
    // Handle system state transitions
    switch (currentState) {
        case SYSTEM_INIT:
            // Initialization complete, transition to normal
            currentState = SYSTEM_NORMAL;
            break;
            
        case SYSTEM_NORMAL:
            // Check if tuning should be activated
            if (configManager.isTuningEnabled()) {
                currentState = SYSTEM_TUNING_ACTIVE;
                Serial.println("Tuning activated");
            }
            break;
            
        case SYSTEM_TUNING_ACTIVE:
            // Check if stealth mode should be activated
            if (configManager.isStealthModeEnabled()) {
                currentState = SYSTEM_STEALTH_MODE;
                Serial.println("Stealth mode activated");
            } else if (!configManager.isTuningEnabled()) {
                currentState = SYSTEM_NORMAL;
                Serial.println("Tuning deactivated");
            }
            break;
            
        case SYSTEM_STEALTH_MODE:
            // Check if stealth mode should be deactivated
            if (!configManager.isStealthModeEnabled()) {
                currentState = SYSTEM_TUNING_ACTIVE;
                Serial.println("Stealth mode deactivated");
            }
            break;
            
        case SYSTEM_ERROR:
            // Try to recover from error state
            if (safetyMonitor.canRecover()) {
                currentState = SYSTEM_NORMAL;
                Serial.println("System recovered from error state");
            }
            break;
            
        case SYSTEM_SHUTDOWN:
            // Graceful shutdown
            Serial.println("System shutdown initiated");
            sensorEmulator.disableTuning();
            canBusHandler.sendShutdownMessage();
            ESP.restart();
            break;
    }
    
    // Small delay to prevent watchdog timeout
    delay(100);
}
