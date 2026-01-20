# Quick Reference - GNSS Implementation

## What's Been Changed

### 1. GPIO Configuration (`components/common_gpio/`)
- **New Ublox GPIO pins:**
  - GPIO4: UART TX (to Ublox TXD)
  - GPIO5: UART RX (from Ublox RXD)
  - GPIO6: PPS (Pulse Per Second)
  
- **Motor drivers reassigned:** GPIO pins shifted to 11-26 range to avoid conflicts

### 2. New Component (`components/gnss_handler/`)
- Complete GNSS data handling system
- NMEA GPGGA sentence parsing
- System time synchronization
- JSON output for web interface

### 3. Web Server Endpoints
- **GET /query_time** - Returns latest GNSS GPGGA data
- **POST /sync_time** - Syncs ESP32 system time from GNSS

### 4. Web Interface
- New "Query Time" button in System section
- New "Sync System Time" button
- Real-time display of:
  - UTC time
  - Latitude/Longitude
  - Number of satellites
  - Fix quality
  - Altitude
  - Sync status

### 5. Updated Files
- `main/main.c` - Added GNSS initialization
- `protocol/webserver/webserver.c` - Added time endpoints
- `spiffs/index.html` - Added UI controls
- `spiffs/js/app.js` - Added query/sync functions
- `spiffs/css/style.css` - Added styling for GNSS section

## How It Works

```
1. User clicks "Query Time" button
   ↓
2. Browser sends GET /query_time
   ↓
3. Server fetches latest GNSS GPGGA data
   ↓
4. Returns JSON with position, time, satellite count
   ↓
5. UI displays the data in a formatted table

6. User clicks "Sync System Time" button
   ↓
7. Browser sends POST /sync_time
   ↓
8. Server extracts UTC time from GNSS data
   ↓
9. Updates ESP32 system clock
   ↓
10. Confirms success/failure to browser
```

## Key Features

✓ NMEA GPGGA parsing with checksum verification
✓ Real-time GNSS data reception via UART
✓ System time synchronization from satellite data
✓ Web-based time calibration interface
✓ JSON output for integration
✓ Error handling and validation
✓ Modular component architecture

## UART Configuration

```c
Port:          UART_NUM_1
Baud Rate:     9600 bps
Data Bits:     8
Stop Bits:     1
Parity:        None
Flow Control:  Disabled
TX Pin:        GPIO4
RX Pin:        GPIO5
```

## Testing the System

### 1. Check GNSS Data Reception
```bash
idf.py monitor
# Look for: "GNSS handler started"
# Look for: "GPGGA parsed: time=..."
```

### 2. Query Time from Command Line
```bash
curl http://[ESP32_IP]/query_time
```

Expected response:
```json
{
  "time": "120530.00",
  "latitude": "3723.2475",
  "num_satellites": "08",
  "valid": true,
  "time_synced": false
}
```

### 3. Sync Time from Web Browser
```
1. Open: http://[ESP32_IP]/
2. Go to: System tab
3. Click: "Query Time" button
   → Shows current GNSS data
4. Click: "Sync System Time" button
   → Updates ESP32 clock if data is valid
```

## Initialization Sequence

When ESP32-S3 boots:
1. Common GPIO init (including UART setup)
2. GNSS handler initialization
3. UART Ublox configuration
4. GNSS receive task starts
5. System begins receiving NMEA data
6. Web interface ready for queries

## Common Issues & Solutions

| Issue | Solution |
|-------|----------|
| No data from /query_time | Check UART connections, verify Ublox power |
| Fix quality = 0 | Let module acquire satellites (1-5 min), check antenna |
| Time sync fails | Ensure fix quality > 0 before syncing |
| UART errors | Verify baud rate is 9600 bps |

## File Structure

```
components/
├── gnss_handler/
│   ├── gnss_handler.h         (API definitions)
│   ├── gnss_handler.c         (Implementation)
│   ├── CMakeLists.txt
│   └── README.md
└── common_gpio/
    ├── common_gpio.h          (Updated with Ublox GPIO)
    └── common_gpio.c          (Updated with uart_ublox_init)

protocol/
└── webserver/
    └── webserver.c            (Added /query_time, /sync_time)

spiffs/
├── index.html                 (Added GNSS UI section)
├── js/app.js                  (Added queryGNSSTime, syncSystemTime)
└── css/style.css              (Added GNSS styling)

main/
└── main.c                      (Added GNSS initialization)

GNSS_IMPLEMENTATION.md          (Detailed documentation)
```

## Performance Impact

- **CPU Usage:** ~1-2% (GNSS task at priority 5)
- **Memory Used:** ~4KB stack + ~1KB data structures
- **UART Overhead:** ~1-2% (9600 baud, ~100 bytes/sec)
- **Update Rate:** ~1 Hz (typical GNSS output rate)

## Next Steps / Future Work

1. Integrate PPS signal (GPIO6) for higher precision
2. Add historical GNSS data logging
3. Implement location-based features
4. Add multiple NMEA sentence support
5. Enhance UI with maps/visualization

