#include "control_mgmt.h"
#include <string.h>
#include <stdio.h>

static char current_owner[UUID_LEN] = {0};
static char pending_list[MAX_CLIENTS][UUID_LEN];
static int pending_count = 0;

void control_manager_init(void) {
    memset(current_owner, 0, sizeof(current_owner));
    pending_count = 0;
}

bool control_is_owner(const char *uuid) {
    return strcmp(current_owner, uuid) == 0;
}

const char *control_get_current(void) {
    return current_owner;
}

bool control_request(const char *uuid) {
    // 如果没有当前控制者，直接赋予控制权
    if (strlen(current_owner) == 0) {
        strncpy(current_owner, uuid, UUID_LEN);
        return true;
    }

    // 如果已经是控制者，返回 true
    if (control_is_owner(uuid)) {
        return true;
    }

    // 加入等待队列
    if (pending_count < MAX_CLIENTS) {
        strncpy(pending_list[pending_count], uuid, UUID_LEN);
        pending_count++;
    }

    return false;
}

bool control_release(const char *uuid) {
    if (!control_is_owner(uuid)) {
        return false;
    }

    // 清除当前控制者
    memset(current_owner, 0, sizeof(current_owner));

    // 如果有等待队列，赋予第一个请求者控制权
    if (pending_count > 0) {
        strncpy(current_owner, pending_list[0], UUID_LEN);

        // 移除已赋权的 UUID
        for (int i = 1; i < pending_count; ++i) {
            strncpy(pending_list[i - 1], pending_list[i], UUID_LEN);
        }
        pending_count--;
    }

    return true;
}
