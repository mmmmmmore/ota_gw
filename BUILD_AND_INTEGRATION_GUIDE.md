# GNSS Implementation - Build & Integration Checklist

## Pre-Build Steps

- [x] Added Ublox GPIO definitions to `common_gpio.h`
- [x] Created new `gnss_handler` component
- [x] Updated `common_gpio.c` with UART initialization
- [x] Added GNSS includes to webserver and main
- [x] Created new web endpoints (/query_time, /sync_time)
- [x] Updated web UI with GNSS controls
- [x] Added CSS styling for GNSS section

## Build Verification Steps

Run the following commands from project root:

```bash
# 1. Clean previous build
idf.py fullclean

# 2. Reconfigure project (optional, but recommended)
idf.py reconfigure

# 3. Build project
idf.py build

# 4. Check for compilation errors
# Output should end with: "Building complete. Your app.bin file is ready"
```

## Expected Build Output

```
Compiling gnss_handler.c
Compiling common_gpio.c
Compiling webserver.c
Compiling main.c
Linking app.elf
```

## Post-Build: Flash & Verify

```bash
# 1. Flash to ESP32-S3
idf.py flash

# 2. Monitor serial output (look for GNSS logs)
idf.py monitor

# Expected logs:
# - "GNSS handler initialized"
# - "UART Ublox initialized: TXD=4, RXD=5, PPS=6..."
# - "GNSS receive task started"
# - "GPGGA parsed: time=..."
```

## Hardware Verification Checklist

Before running, verify:

- [ ] Ublox NEO-6M module powered on
- [ ] GPIO4 (TX) connected to Ublox RXD
- [ ] GPIO5 (RX) connected to Ublox TXD
- [ ] GPIO6 (optional) connected to Ublox PPS
- [ ] GND connected between ESP32 and Ublox
- [ ] Ublox antenna has clear sky view
- [ ] Module has acquired satellite fix (LED blinking pattern)

## Runtime Testing

### Test 1: Check GNSS Reception
```
Expected: Monitor shows "GPGGA parsed: time=..." messages
If not: Check UART connections and baud rate
```

### Test 2: Query GNSS Data via Web
```bash
curl http://[ESP32_IP]/query_time
```
Expected: JSON response with valid GNSS data

### Test 3: Time Synchronization
```bash
# Check ESP32 time before:
date

# Send sync request:
curl -X POST http://[ESP32_IP]/sync_time

# Check response:
# {"success":true,"message":"System time synchronized..."}
```

### Test 4: Web Interface
1. Open browser to `http://[ESP32_IP]/`
2. Navigate to "System" tab
3. Click "Query Time" button
4. Verify GNSS data appears in table
5. Click "Sync System Time" button
6. Verify success message

## Component Dependencies

The `gnss_handler` component requires:
- `common_gpio` - For UART configuration
- `esp_driver_uart` - UART driver
- `cjson` - JSON parsing/generation
- `freertos` - Task scheduling
- `esp_system` - System time functions

These are automatically resolved by the build system.

## File Size Reference

After clean build, expected sizes:
- app.bin: ~800-1000 KB (varies with other components)
- GNSS handler adds: ~50-80 KB

## Troubleshooting Build Issues

### Issue: "gnss_handler.h not found"
**Solution:** Verify `components/gnss_handler/` directory exists with correct files

### Issue: "uart driver not found"
**Solution:** Ensure esp_driver_uart is in `managed_components` or IDF

### Issue: "undefined reference to gnss_handler_init"
**Solution:** Check CMakeLists.txt includes gnss_handler in REQUIRES

### Issue: UART compilation errors
**Solution:** Verify driver/uart.h include is present in gnss_handler.c

## Verification Commands

```bash
# Check compilation commands
grep -r "gnss_handler" build/compile_commands.json

# List component files built
ls -la build/esp-idf/gnss_handler/

# Check UART driver compilation
ls -la build/esp-idf/esp_driver_uart/
```

## Performance Baseline

After successful build and flash, monitor should show:
- Boot time: ~3-5 seconds
- GNSS task CPU: <2%
- UART reception rate: ~1 sentence/second
- JSON response time: <50ms

## Rollback Steps

If issues occur, rollback to previous version:
```bash
# Restore from version control
git checkout HEAD -- components/common_gpio/
git checkout HEAD -- protocol/webserver/
git checkout HEAD -- main/main.c
git checkout HEAD -- spiffs/

# Remove new component
rm -rf components/gnss_handler/

# Rebuild
idf.py fullclean
idf.py build
```

## Final Validation

After successful build and deployment:

```
✓ GNSS handler initialized
✓ UART communication established
✓ NMEA data received and parsed
✓ Web endpoints responding
✓ GNSS UI displaying correctly
✓ Time synchronization working
✓ System time updated from GNSS
```

If all items are checked, the implementation is complete and working!

## Support Documentation

Refer to:
- `GNSS_IMPLEMENTATION.md` - Detailed technical documentation
- `GNSS_QUICK_REFERENCE.md` - Quick reference guide
- `components/gnss_handler/README.md` - Component-specific documentation

## Additional Resources

- Ublox NEO-6M Datasheet (included in module documentation)
- ESP-IDF UART Driver: https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/uart.html
- NMEA 0183 Standard: Standard for interfacing marine electronics devices
- HTTP Server: https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/protocols/esp_http_server.html

