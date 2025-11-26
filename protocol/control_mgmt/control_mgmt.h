#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#define MAX_CLIENTS 8
#define UUID_LEN 64

void control_manager_init(void);
bool control_request(const char *uuid);
bool control_release(const char *uuid);
const char *control_get_current(void);
bool control_is_owner(const char *uuid);

#ifdef __cplusplus
}
#endif
