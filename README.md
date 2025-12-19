# ota_gw
This code repository for GW function based on ESP32

Feature2 branch code freeze
        ↓
Function description and summary

In this stage focus on two topics
1 about tcp stable communication between OTA-server and GW
the issue we facing is TCP socket was not sync the ota_task json message in time, after socket disconnect or wifi disconnect the ota_task can be identified in GW side. 
consider using ring buffer method for recv json message handle, at first invetigation mainly about if ring buffer method error. and after setup seq in each cycle loop, confirmed the ring buffer function match design.

after that, the direction turn to TCP protocol, since after we both setup TCP_NODELAY mode in OTA Tx side and GW Rx side, this issue fixed?
**remark, in MacOS system, even TCP nodelay setup, the issue still keep exist, need further time to deep understanding that. 



2 about multi task management handle
use task_list manage all task lifetime and upgrade time to manage the task resource inside the GW. 
query and manage each task via task_id. 
js query function will use task_id follow the progress
by 251219 final stage2 finished.





