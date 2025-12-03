#ifndef MSG_HANDLER_H
#define MSG_HANDLER_H

#include <stddef.h>

// 初始化消息处理模块
void msg_handler_init(void);

// 处理来自 TCP Server 的 JSON 消息
void msg_handler_process(int sock, const char *json_str);

#endif // MSG_HANDLER_H
