# ota_gw
This code repository for GW function based on ESP32



Webserver 的职责（信息发布中心）
接收任务：监听 OTA Server 的推送（HTTP POST → /ota_task）。
通知 UI：将任务信息存储，并提供 /task_info 接口给前端页面，展示任务详情。
用户交互：前端页面提供 Accept/Deny 按钮，用户选择后通过 /ota_user_response 回传。
结果反馈：
  --如果用户选择 Accept → 调用 ota_handler 下发任务给 Client。
  --如果用户选择 Deny → 立即向 OTA Server 上报拒绝结果。


ota_handler 的职责（调度中心）
接收通知：从 webserver 接收到用户确认的 OTA 任务。
任务分发：根据任务中的设备编号，通知对应的 Client（通过 Wi‑Fi 中继，而非 SPI）。
升级执行：向 Client 下发升级包路径（URL），Client 自行下载并执行升级。
进度反馈：
    ota_handler 模拟升级进度（例如 0% → 100%），周期性更新到 webserver。
    webserver 提供 /progress_info 接口，前端 UI 可以轮询或 WebSocket 获取进度。

状态表维护：
  默认开机时所有 Client 显示为 A 分区。
  如果某个 Client 升级成功 → 状态表更新为 B 分区。
  如果失败 → 保持 A 分区。

结果上报：
    ota_handler 收集 Client 的升级结果。
    将结果传递给 webserver，由 webserver 转发给 OTA Server。


OTA Server (POST /ota_task)
        ↓
GW Webserver (/ota_task handler)
        ↓
UI 页面 (/task_info → 展示任务)
        ↓
用户选择 Accept/Deny (/ota_user_response)
        ↓
GW Webserver → ota_handler
        ↓
ota_handler → Client (Wi‑Fi 中继，发送升级包路径)
        ↓
Client 执行升级 → 返回结果
        ↓
ota_handler 更新状态表 & 模拟进度
        ↓
Webserver 提供进度接口 (/progress_info)
        ↓
Webserver 转发结果给 OTA Server (/ota_result)





Web Server：触发 OTA 升级请求，发送任务 JSON 给 GW。

GW ota_handler：接收 Web 的任务，生成 OTA 任务 JSON。

GW ota_dispatch：决定哪些 Client 需要执行任务。

GW tcp_server：通过 TCP 下发任务 JSON 给 Client。

Client ota_handler：执行 OTA 升级，完成后上报结果 JSON。

GW tcp_server → ota_dispatch → ota_handler：接收结果，更新 Client 状态，并最终汇总反馈给 Web。



<img width="1536" height="1024" alt="image" src="https://github.com/user-attachments/assets/71ede548-7f17-445f-801d-e976e371f1dd" />







