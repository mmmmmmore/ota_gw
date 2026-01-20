# ESP32-S3 GNSS Time Calibration System Implementation

## Overview

This document describes the implementation of a GNSS-based time calibration system for the ESP32-S3 OTA Gateway project using the Ublox NEO-6M module. The system receives NMEA GPGGA data via UART and provides time synchronization capabilities to the web interface.

## System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    ESP32-S3 OTA Gateway                      │
├──────────────────────────────┬──────────────────────────────┤
│      Web Interface (HTTP)     │    GNSS Subsystem            │
│  ├─ /query_time (GET)        │  ├─ UART1 (9600 baud)        │
│  ├─ /sync_time (POST)        │  ├─ NMEA GPGGA Parser        │
│  └─ query_time button        │  ├─ Data Queue               │
│                              │  └─ Time Sync Logic          │
└──────────────────┬───────────┴──────────────────┬───────────┘
                   │                              │
              ┌────▼──────┐                  ┌────▼──────────┐
              │   GPIO4-6  │◄─────UART───────┤ Ublox NEO-6M  │
              │   (UART)   │                  └───────────────┘
              └────────────┘
```

## GPIO Configuration

The ESP32-S3 uses the following GPIO pins for Ublox NEO-6M connection:

| GPIO Pin | Function | Description |
|----------|----------|-------------|
| GPIO4 | UART TX | UART transmit to Ublox module |
| GPIO5 | UART RX | UART receive from Ublox module (NMEA data) |
| GPIO6 | PPS | Pulse Per Second input (precision timing reference) |

### Modified Motor Driver GPIO Assignments

To accommodate the Ublox module, the motor driver GPIO assignments have been updated:

**Motor Driver #1:**
- GPIO11: AIN1
- GPIO12: AIN2
- GPIO13: BIN1
- GPIO14: BIN2
- GPIO15: PWMA
- GPIO16: PWMB

**Motor Driver #2:**
- GPIO21: AIN1
- GPIO22: AIN2
- GPIO23: BIN1
- GPIO24: BIN2
- GPIO25: PWMA
- GPIO26: PWMB

**Shared:**
- GPIO17: STBY (Standby pin for both drivers)

## Components

### 1. Modified common_gpio Component

**Files:**
- `components/common_gpio/common_gpio.h` - GPIO definitions
- `components/common_gpio/common_gpio.c` - GPIO initialization

**Changes:**
- Added GPIO definitions for Ublox connection (GPIO4, GPIO5, GPIO6)
- Added UART configuration parameters (UART_NUM_1, 9600 baud)
- Added `uart_ublox_init()` function for UART configuration
- Reorganized motor driver GPIO assignments

**New Function:**
```c
void uart_ublox_init(void);
```

### 2. New GNSS Handler Component

**Files:**
- `components/gnss_handler/gnss_handler.h` - API definitions
- `components/gnss_handler/gnss_handler.c` - Implementation
- `components/gnss_handler/CMakeLists.txt` - Build configuration
- `components/gnss_handler/README.md` - Component documentation

**Features:**
- NMEA GPGGA sentence parsing with checksum verification
- UART data reception with buffering
- JSON output for web interface
- System time synchronization
- Real-time GPGGA data storage

**Data Structure:**
```c
typedef struct {
    char time[10];          // UTC time (hhmmss.ss)
    char latitude[12];      // Latitude
    char ns_indicator;      // N/S indicator
    char longitude[13];     // Longitude
    char ew_indicator;      // E/W indicator
    char fix_quality[2];    // Fix quality (0-8)
    char num_satellites[3]; // Number of satellites
    char hdop[6];          // Horizontal dilution of precision
    char altitude[10];      // Altitude in meters
    char altitude_unit;     // Height unit (M)
    char timestamp_ms;      // Millisecond timestamp
    bool valid;            // Data validity flag
} gnss_gpgga_data_t;
```

**Public API:**
```c
void gnss_handler_init(void);                                  // Initialize GNSS handler
void gnss_handler_start_task(void);                           // Start receiving GNSS data
void gnss_handler_stop_task(void);                            // Stop receiving data
bool gnss_handler_get_latest_gpgga(gnss_gpgga_data_t *data);  // Get latest GPGGA data
char* gnss_handler_get_gpgga_json(void);                      // Get GPGGA as JSON
bool gnss_handler_sync_system_time(void);                     // Sync system time from GNSS
bool gnss_handler_get_time_synced(void);                      // Get sync status
uint32_t gnss_handler_get_last_sync_timestamp(void);          // Get last sync timestamp
```

### 3. Updated Web Server

**File:**
- `protocol/webserver/webserver.c` - HTTP server implementation

**New Endpoints:**

#### GET /query_time
Returns the latest GNSS GPGGA data as JSON.

**Response Example:**
```json
{
  "time": "120530.00",
  "latitude": "3723.2475",
  "ns_indicator": "N",
  "longitude": "12158.3416",
  "ew_indicator": "W",
  "fix_quality": "1",
  "num_satellites": "08",
  "hdop": "0.9",
  "altitude": "545.4",
  "altitude_unit": "M",
  "valid": true,
  "time_synced": false
}
```

#### POST /sync_time
Attempts to synchronize the ESP32-S3 system time from the latest GNSS data.

**Response Example:**
```json
{
  "success": true,
  "message": "System time synchronized from GNSS"
}
```

### 4. Updated Web Interface

**Files:**
- `spiffs/index.html` - HTML interface
- `spiffs/js/app.js` - JavaScript functionality
- `spiffs/css/style.css` - Styling

**New UI Elements:**

1. **System Information Section** - Enhanced with GNSS controls
   - "Query Time" button - Fetches latest GNSS data
   - "Sync System Time" button - Synchronizes ESP32 system clock
   - GNSS data display table showing:
     - UTC Time
     - Latitude / Longitude
     - Number of Satellites
     - Fix Quality
     - Altitude
     - Sync Status

**JavaScript Functions:**

```javascript
// Query GNSS time data
function queryGNSSTime()

// Synchronize system time from GNSS
function syncSystemTime()

// Display GNSS data in UI
function displayGNSSData(data)
```

### 5. Updated Main Application

**File:**
- `main/main.c` - Application entry point

**Changes:**
- Added GNSS handler initialization
- Added GNSS task startup
- Added required includes

## Data Flow

```
1. UART Reception:
   Ublox Module → GPIO5 → UART1 → Circular Buffer

2. NMEA Processing:
   Circular Buffer → Sentence Detection → Checksum Verification → Parsing

3. Data Storage:
   Parsed GPGGA → Global Structure (g_latest_gpgga)

4. Web Access:
   Browser → /query_time → GNSS Handler → JSON Response → Display

5. Time Synchronization:
   Browser → /sync_time → GNSS Handler → Extract Time → settimeofday() → System Clock
```

## NMEA GPGGA Sentence Format

```
$GPGGA,hhmmss.ss,llll.llll,a,yyyyy.yyyy,a,x,xx,x.x,x.x,M,x.x,M,,*hh

Example:
$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*42

Fields:
1. UTC Time: 123519 (12:35:19 UTC)
2. Latitude: 4807.038 (48° 07.038' N)
3. N/S Indicator: N (North)
4. Longitude: 01131.000 (11° 31.000' E)
5. E/W Indicator: E (East)
6. Fix Quality: 1 (GPS fix)
7. Number of Satellites: 08
8. HDOP: 0.9 (Horizontal Dilution of Precision)
9. Altitude: 545.4 meters
10. Unit: M (Meters)
11. Checksum: *42
```

## UART Configuration Details

- **Port:** UART1
- **Baud Rate:** 9600 bps
- **Data Bits:** 8
- **Stop Bits:** 1
- **Parity:** None
- **Flow Control:** Disabled
- **TX GPIO:** GPIO4
- **RX GPIO:** GPIO5
- **Buffer Size:** 512 bytes (256x2)

## Time Synchronization Process

1. **Data Reception:** GNSS handler continuously receives NMEA sentences via UART
2. **Parsing:** GPGGA sentences are extracted and parsed
3. **Validation:** Checksum verification ensures data integrity
4. **Storage:** Valid GPGGA data is stored in global structure
5. **Query:** Web interface can query latest GPGGA data via HTTP GET
6. **Synchronization:** On user request, system time is extracted from UTC time field and applied via `settimeofday()`

## Build Instructions

1. Ensure the project includes the new gnss_handler component:
   ```bash
   # The component is auto-discovered in components/ directory
   ```

2. Configure the project:
   ```bash
   idf.py menuconfig
   ```

3. Build the project:
   ```bash
   idf.py build
   ```

4. Flash to ESP32-S3:
   ```bash
   idf.py flash
   ```

5. Monitor output:
   ```bash
   idf.py monitor
   ```

## Troubleshooting

### No GNSS Data Received

**Symptoms:** /query_time returns `{"error":"No GNSS data available"}`

**Solutions:**
1. Check UART connections (GPIO4, GPIO5)
2. Verify Ublox module power and serial output
3. Check UART baud rate: 9600 bps
4. Verify UART RX pin receives data

### Invalid GNSS Data

**Symptoms:** Fix quality shows 0, no satellites detected

**Solutions:**
1. Allow module to acquire satellite fix (typically 1-5 minutes)
2. Ensure clear sky view for antenna
3. Check antenna connection
4. Verify module is powered correctly

### Time Sync Fails

**Symptoms:** /sync_time returns `{"success":false}`

**Solutions:**
1. Ensure GNSS data is valid (fix quality > 0)
2. Wait for satellite fix before attempting sync
3. Check system time library permissions
4. Verify no other processes are modifying system time

## Performance Considerations

- **GNSS Handler Task Priority:** 5 (moderate-high priority)
- **Task Stack Size:** 4096 bytes
- **Queue Capacity:** 10 GPGGA messages
- **Update Frequency:** ~1 update per second (typical GNSS rate)
- **JSON Response Size:** ~200-300 bytes

## Future Enhancements

1. **PPS Integration:** Implement precise time synchronization using GPIO6 PPS signal
2. **Logging:** Store GNSS data history for analysis
3. **Geofencing:** Add location-based alerts and notifications
4. **Multi-NMEA:** Support additional NMEA sentence types (RMC, GGA, etc.)
5. **Error Recovery:** Implement automatic UART reconnection on errors
6. **Kalman Filtering:** Improve time sync accuracy with filtering algorithms

## Testing Procedures

### Unit Tests

1. **NMEA Parsing Test:**
   ```c
   gnss_gpgga_data_t test_data;
   const char *sentence = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*42";
   assert(parse_gpgga_sentence(sentence, &test_data) == true);
   assert(test_data.valid == true);
   ```

2. **Web Interface Test:**
   ```bash
   curl http://esp32-ip/query_time
   curl -X POST http://esp32-ip/sync_time
   ```

### Integration Tests

1. Connect Ublox module and power on
2. Monitor UART output for NMEA sentences
3. Query /query_time endpoint repeatedly
4. Verify time synchronization works
5. Check system clock before and after sync

## References

- Ublox NEO-6M Datasheet
- NMEA 0183 Protocol Specification
- ESP-IDF UART Driver Documentation
- ESP-IDF HTTP Server Documentation

