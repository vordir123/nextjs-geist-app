/**
 * CAN Bus Handler Module
 * 
 * Handles communication with Bosch eBike drive systems via CAN bus.
 * Supports multiple Bosch generations and protocols.
 * 
 * @author ESP32 eBike Development Team
 * @version 1.0.0
 */

#ifndef CANBUSHANDLER_H
#define CANBUSHANDLER_H

#include <Arduino.h>
#include <CAN.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

// Bosch CAN message IDs
#define BOSCH_SPEED_MSG_ID      0x181
#define BOSCH_MOTOR_MSG_ID      0x182
#define BOSCH_BATTERY_MSG_ID    0x183
#define BOSCH_DISPLAY_MSG_ID    0x184
#define BOSCH_DIAGNOSTIC_MSG_ID 0x185

// CAN bus configuration
typedef struct {
    int canTxPin;
    int canRxPin;
    long baudRate;
    int boschGeneration;
    bool enableDiagnostics;
} CANBusConfig;

// CAN message structure
typedef struct {
    uint32_t id;
    uint8_t length;
    uint8_t data[8];
    uint32_t timestamp;
} CANMessage;

// Bosch system generations
typedef enum {
    BOSCH_GEN1 = 1,
    BOSCH_GEN2 = 2,
    BOSCH_GEN3 = 3,
    BOSCH_GEN4 = 4,
    BOSCH_GEN5_SMART = 5
} BoschGeneration;

// System status
typedef struct {
    bool isConnected;
    uint16_t currentSpeed;
    uint8_t motorPower;
    uint8_t batteryLevel;
    uint8_t assistLevel;
    uint32_t lastMessageTime;
    uint16_t errorCode;
} BoschSystemStatus;

class CANBusHandler {
private:
    CANBusConfig config;
    QueueHandle_t messageQueue;
    BoschSystemStatus systemStatus;
    uint32_t heartbeatInterval;
    uint32_t lastHeartbeat;
    
    // Protocol handlers for different generations
    bool handleGen1Message(const CANMessage& msg);
    bool handleGen2Message(const CANMessage& msg);
    bool handleGen3Message(const CANMessage& msg);
    bool handleGen4Message(const CANMessage& msg);
    bool handleGen5Message(const CANMessage& msg);
    
    // Message parsing functions
    void parseSpeedMessage(const CANMessage& msg);
    void parseMotorMessage(const CANMessage& msg);
    void parseBatteryMessage(const CANMessage& msg);
    void parseDisplayMessage(const CANMessage& msg);
    void parseDiagnosticMessage(const CANMessage& msg);
    
    // Message generation functions
    CANMessage createSpeedMessage(uint16_t speed);
    CANMessage createMotorMessage(uint8_t power);
    CANMessage createHeartbeatMessage();
    
    // Utility functions
    uint16_t calculateChecksum(const uint8_t* data, uint8_t length);
    bool validateMessage(const CANMessage& msg);
    void logMessage(const CANMessage& msg, bool incoming);

public:
    CANBusHandler();
    ~CANBusHandler();
    
    // Initialization and configuration
    bool begin(const CANBusConfig& cfg);
    void end();
    bool isInitialized();
    
    // Message handling
    bool sendMessage(const CANMessage& msg);
    bool receiveMessage(CANMessage& msg);
    void processMessages();
    
    // System interaction
    bool sendSpeedData(uint16_t speed);
    bool sendMotorData(uint8_t power);
    bool sendHeartbeat();
    bool sendShutdownMessage();
    
    // Status and diagnostics
    BoschSystemStatus getSystemStatus();
    bool isSystemConnected();
    uint16_t getCurrentSpeed();
    uint8_t getMotorPower();
    uint8_t getBatteryLevel();
    uint16_t getLastError();
    
    // Configuration
    void setBoschGeneration(BoschGeneration gen);
    void enableDiagnostics(bool enable);
    void setHeartbeatInterval(uint32_t interval);
    
    // Error handling
    void clearErrors();
    bool hasErrors();
    String getErrorDescription(uint16_t errorCode);
};

#endif // CANBUSHANDLER_H
