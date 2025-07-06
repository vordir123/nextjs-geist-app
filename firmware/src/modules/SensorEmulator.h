/**
 * Sensor Emulator Module
 * 
 * Handles speed sensor signal manipulation and emulation.
 * Provides real-time signal processing with adaptive algorithms.
 * 
 * @author ESP32 eBike Development Team
 * @version 1.0.0
 */

#ifndef SENSOREMULATOR_H
#define SENSOREMULATOR_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Signal processing constants
#define MAX_SPEED_KMH           50
#define MIN_SPEED_KMH           0
#define SIGNAL_BUFFER_SIZE      128
#define FILTER_WINDOW_SIZE      8

// Operating modes
typedef enum {
    MODE_DISABLED = 0,
    MODE_ECO = 1,
    MODE_SPORT = 2,
    MODE_UNLIMITED = 3,
    MODE_STEALTH = 4
} OperatingMode;

// Performance levels
typedef enum {
    PERFORMANCE_NORMAL = 0,
    PERFORMANCE_REDUCED = 1,
    PERFORMANCE_MAXIMUM = 2
} PerformanceMode;

// Sensor configuration
typedef struct {
    int inputPin;
    int outputPin;
    int pulsePerRevolution;
    float wheelCircumference;
    uint16_t maxSpeedLimit;
    OperatingMode defaultMode;
    bool enableSmoothing;
    bool enableAntiAlias;
} SensorConfig;

// Signal processing parameters
typedef struct {
    float frequencyDivider;
    uint16_t smoothingFactor;
    uint16_t antiAliasThreshold;
    uint32_t signalTimeout;
    bool adaptiveProcessing;
} ProcessingParams;

// Signal statistics
typedef struct {
    uint32_t totalPulses;
    uint32_t validPulses;
    uint32_t droppedPulses;
    float averageFrequency;
    float currentSpeed;
    float maxSpeed;
    uint32_t lastPulseTime;
    uint32_t signalQuality;
} SignalStats;

class SensorEmulator {
private:
    SensorConfig config;
    ProcessingParams params;
    SignalStats stats;
    OperatingMode currentMode;
    PerformanceMode performanceMode;
    
    // Signal processing buffers
    uint32_t pulseBuffer[SIGNAL_BUFFER_SIZE];
    uint16_t bufferIndex;
    uint32_t filterWindow[FILTER_WINDOW_SIZE];
    uint8_t filterIndex;
    
    // Timing variables
    uint32_t lastInputPulse;
    uint32_t lastOutputPulse;
    uint32_t pulseInterval;
    uint32_t outputInterval;
    
    // State variables
    bool tuningActive;
    bool stealthActive;
    bool signalValid;
    volatile bool newPulseReceived;
    
    // Interrupt handling
    static void IRAM_ATTR inputPulseISR();
    static SensorEmulator* instance;
    
    // Signal processing functions
    void processInputPulse();
    uint32_t calculateOutputInterval(uint32_t inputInterval);
    float applySpeedLimiting(float inputSpeed);
    float applyModeProcessing(float inputSpeed);
    float applySmoothingFilter(float inputSpeed);
    float applyAntiAliasFilter(float inputSpeed);
    
    // Mode-specific processing
    float processEcoMode(float inputSpeed);
    float processSportMode(float inputSpeed);
    float processUnlimitedMode(float inputSpeed);
    float processStealthMode(float inputSpeed);
    
    // Adaptive algorithms
    void updateAdaptiveParams();
    float calculateOptimalDivider(float targetSpeed, float currentSpeed);
    void adjustForBoschGeneration();
    
    // Signal generation
    void generateOutputPulse();
    void generateNaturalSignal();
    void generateStealthSignal();
    
    // Utility functions
    float calculateSpeed(uint32_t pulseInterval);
    uint32_t speedToPulseInterval(float speed);
    void updateSignalStats();
    bool validateSignal();
    void resetBuffers();

public:
    SensorEmulator();
    ~SensorEmulator();
    
    // Initialization and configuration
    bool begin(const SensorConfig& cfg);
    void end();
    bool isInitialized();
    
    // Main processing function
    void processSpeedSignal();
    
    // Mode control
    void setOperatingMode(OperatingMode mode);
    OperatingMode getOperatingMode();
    void setPerformanceMode(PerformanceMode mode);
    PerformanceMode getPerformanceMode();
    
    // Tuning control
    void enableTuning();
    void disableTuning();
    bool isTuningActive();
    
    // Stealth mode
    void enableStealthMode();
    void disableStealthMode();
    void stealthMode();
    bool isStealthActive();
    
    // Speed limiting
    void setSpeedLimit(uint16_t limitKmh);
    uint16_t getSpeedLimit();
    void setFrequencyDivider(float divider);
    float getFrequencyDivider();
    
    // Signal processing configuration
    void enableSmoothing(bool enable);
    void setSmoothingFactor(uint16_t factor);
    void enableAntiAlias(bool enable);
    void setAntiAliasThreshold(uint16_t threshold);
    void enableAdaptiveProcessing(bool enable);
    
    // Status and diagnostics
    SignalStats getSignalStats();
    float getCurrentSpeed();
    float getInputSpeed();
    float getOutputSpeed();
    uint32_t getSignalQuality();
    bool isSignalValid();
    
    // Calibration
    void calibrateWheelCircumference();
    void calibratePulseCount();
    void resetCalibration();
    
    // Testing and debugging
    void injectTestSignal(float speedKmh);
    void enableDebugOutput(bool enable);
    void dumpSignalBuffer();
    void runSelfTest();
};

#endif // SENSOREMULATOR_H
