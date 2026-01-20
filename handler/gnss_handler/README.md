# GNSS Handler Component

This component handles GNSS data reception from the Ublox NEO-6M module via UART.

## Features

- Receives NMEA GPGGA data from Ublox NEO-6M module
- Parses GPGGA sentences with checksum verification
- Provides JSON formatted GPGGA data for web interface
- Supports system time synchronization from GNSS data
- Tracks time synchronization status

## GPIO Configuration

- GPIO4: UART TX (Ublox TXD)
- GPIO5: UART RX (Ublox RXD)  
- GPIO6: PPS (Pulse Per Second) - for precision timing

## UART Settings

- Baud Rate: 9600 bps
- Data Bits: 8
- Stop Bits: 1
- Parity: None
- Flow Control: None

## API Usage

```c
// Initialize GNSS handler
gnss_handler_init();

// Start receiving GNSS data
gnss_handler_start_task();

// Get latest GPGGA data
gnss_gpgga_data_t data;
if (gnss_handler_get_latest_gpgga(&data)) {
    printf("Time: %s, Latitude: %s %c\n", data.time, data.latitude, data.ns_indicator);
}

// Get GPGGA as JSON
char *json = gnss_handler_get_gpgga_json();
// Use json...
free(json);

// Sync system time from GNSS
if (gnss_handler_sync_system_time()) {
    printf("System time synchronized\n");
}

// Check sync status
if (gnss_handler_get_time_synced()) {
    printf("System time is synchronized\n");
}
```

## NMEA GPGGA Sentence Format

```
$GPGGA,hhmmss.ss,llll.lll,a,yyyyy.yyy,a,x,xx,x.x,x.x,M,x.x,M,,*hh
```

Where:
- hhmmss.ss = UTC time
- llll.lll = Latitude
- a = N/S indicator
- yyyyy.yyy = Longitude
- a = E/W indicator
- x = Fix quality (0=No fix, 1=GPS, 2=DGPS, etc.)
- xx = Number of satellites
- x.x = HDOP (horizontal dilution of precision)
- x.x = Altitude in meters
- M = Meters
- hh = Checksum

