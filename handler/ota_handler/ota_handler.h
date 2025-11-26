#ifndef OTA_HANDLER_H
#define OTA_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Start OTA update from the given URL.
 *
 * @param ota_url The HTTP or HTTPS URL pointing to the firmware binary.
 */
void ota_start(const char *ota_url);

#ifdef __cplusplus
}
#endif

#endif // OTA_HANDLER_H
