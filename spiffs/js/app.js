let progressTimer =null;
let upgradeTimerOutFlag = null;
let currentTaskId =null;

let lastProgress = -1;
let stuckCounter =0;

const DEFAULT_DEVICE_STATUS = [
  { device_name: "Veh1_SCM1", version: "-", connection: "-" },
  { device_name: "Veh1_SCM2", version: "-", connection: "-" },
  { device_name: "Veh2_SCM3", version: "-", connection: "-" }
];


function showSection(sectionId) {
  document.querySelectorAll('.section').forEach(sec => sec.classList.add('hidden'));
  document.getElementById(sectionId).classList.remove('hidden');

  // 如果进入 update 区域，加载任务信息
  if (sectionId === 'update') {
    loadPendingTask();
    loadDeviceStatus();
  }
}


function updateotaprogressBar(percentage){
  const bar = document.getElementById("progressBar");
  bar.style.width = percentage + "%";
  bar.textContent = percentage + "%";
}


function fetchProgress(){
  if (!currentTaskId) return;
  //const params = new URLSearchParams({task_id: currentTaskId});
  fetch(`/progress_info?task_id=${encodeURIComponent(currentTaskId)}`)
    .then(response => response.json())
    .then(data =>{
      updateotaprogressBar(data.progress); // need matching with the ota_handler progress data
      const progress = Number(data.progress) || 0;

      if (data.progress >= 100 && data.state == "complete"){
        finishProgress("success");
        return;
      }

      if (progress === lastProgress){
        stuckCounter++;
        if (stuckCounter >=5){
          finishProgress("failed", "Upgrade Failed : Progress Stuck");
          return;
        }
      }else {
        stuckCounter =0;
        lastProgress= progress;
      }

      const elapsed = Date.now() - upgradeTimerOutFlag;  //calculate the time interval;
      if (elapsed > 60000){
        finishProgress("timeout", "Upgrade Failed: Timeout");
        return;
      }
      console.log("State:", data.state, "Progress: ", data.progress);
    })
    .catch(err => {
      console.error("Error fetching progress", err);
      finishProgress("failed", " Upgrade Failed: Cannot get Progress");
    });
}


function statusBadge(status){
  const label = (typeof status === "string" && status.trim()) ? status : "-";
  const normalized = label.toLowerCase();
  let badgeClass = "status-unknown";
  if (normalized === "online") {
    badgeClass = "status-online";
  } else if (normalized === "offline") {
    badgeClass = "status-offline";
  }
  return `<span class="status-badge ${badgeClass}">${label}</span>`;
}


function renderDeviceStatus(devices){
  const tbody = document.getElementById('device-status-tbody');
  if (!tbody) return;
  const rows = (Array.isArray(devices) && devices.length) ? devices : DEFAULT_DEVICE_STATUS;
  tbody.innerHTML = rows.map(dev => `
    <tr>
      <td>${dev.device_name || '-'}</td>
      <td>${dev.version || '-'}</td>
      <td>${statusBadge(dev.connection)}</td>
    </tr>
  `).join('');
}


function loadDeviceStatus(){
  fetch('/device_status')
    .then(resp => resp.json())
    .then(devices => {
      renderDeviceStatus(devices);
    })
    .catch(err => {
      console.error('Failed to load device status', err);
      renderDeviceStatus(DEFAULT_DEVICE_STATUS);
    });
}


// 加载待确认任务
function loadPendingTask() {
  fetch('/task_info')
    .then(resp => resp.json())
    .then(tasks => {
      let tbody = document.getElementById('update-tbody');
      tbody.innerHTML = '';

      if (tasks.length > 0) {
        tasks.forEach(task =>{
          tbody.innerHTML += `
            <tr>
              <td>${task.task_id || '-'}</td>
              <td>${task.client_id || '-'}</td>
              <td>${task.version}</td>
              <td>${task.features || '-'}</td>
              <td>
                <button onclick="sendUserResponse('accept', '${task.task_id}')">Accept</button>
                <button onclick="sendUserResponse('reject', '${task.task_id}')">Reject</button>
              </td>
            </tr>
          `;

        });
      } else {
        tbody.innerHTML = '<tr><td colspan="5">No pending tasks</td></tr>';
      }
    })
    .catch(err => {
      console.error('Failed to load tasks', err);
    });
}

// Submit user response
function sendUserResponse(response_value, task_id_value) {
  fetch('/ota_user_response', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ 
      task_id: task_id_value,
      response: response_value   //UI accept / reject 
    })
  })
  .then(resp => resp.json())
  .then(result =>{
    if(response_value === 'accept'){
      //display the progress bar
      document.getElementById("progressModal").classList.remove("hidden");
      //cycle check and update the value
      upgradeTimerOutFlag = Date.now();   // timer out trigger start.
      currentTaskId = task_id_value;
      progressTimer = setInterval(fetchProgress, 2000);
      console.log("ota upgrade under progress... ")
    }else if(response_value ==='reject'){
      console.log(" User Reject this task", task_id_value);
    }
  })
  .catch(err => {
    console.error('提交响应失败', err);
  });
}

function finishProgress(status, message) {
  clearInterval(progressTimer);
  progressTimer = null;

  const bar = document.getElementById("progressBar");
  if (status === "failed"){
    bar.classList.add("failed");
  }else if (status === "timeout"){
    bar.classList.add("timeout");
  }

  document.getElementById("confirmBtn").style.display = "inline-block";

  if (message) {
    alert(message);
  }

  console.log("OTA finished with status: ", status);

}

//update the upgrade progress percentage by cycle query the status.
document.getElementById("confirmBtn").addEventListener("click", ()=>{  
  document.getElementById("confirmBtn").style.display = "none";
  clearInterval(progressTimer);
  progressTimer = null;

  lastProgress = -1;
  stuckCounter =0;
  currentTaskId = null;
  const bar = document.getElementById("progressBar");
  bar.classList.remove("failed", "timeout");
  updateotaprogressBar(0);
  
  document.getElementById("progressModal").classList.add("hidden");
});

// ========== GNSS/时间处理函数 ==========

// 查询 GNSS 时间
function queryGNSSTime() {
  fetch('/query_time')
    .then(response => response.json())
    .then(data => {
      displayGNSSData(data);
    })
    .catch(error => {
      console.error('Error querying GNSS time:', error);
      alert('Failed to query GNSS time');
    });
}

// 同步系统时间
function syncSystemTime() {
  fetch('/sync_time', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    }
  })
    .then(response => response.json())
    .then(data => {
      if (data.success) {
        alert('System time synchronized successfully!');
        console.log(data.message);
      } else {
        alert('Failed to sync time: ' + data.message);
        console.log(data.message);
      }
    })
    .catch(error => {
      console.error('Error syncing time:', error);
      alert('Error syncing system time');
    });
}

// 显示 GNSS 数据
function displayGNSSData(data) {
  const display = document.getElementById('gnss-data-display');
  
  if (!display) return;
  
  // Always show the data, even if invalid
  document.getElementById('gnss-time').textContent = data.time || '-';
  document.getElementById('gnss-latitude').textContent = (data.latitude + ' ' + data.ns_indicator) || '-';
  document.getElementById('gnss-longitude').textContent = (data.longitude + ' ' + data.ew_indicator) || '-';
  document.getElementById('gnss-satellites').textContent = data.num_satellites || '-';
  document.getElementById('gnss-fix-quality').textContent = data.fix_quality || '-';
  document.getElementById('gnss-altitude').textContent = (data.altitude + ' ' + data.altitude_unit) || '-';
  
  // Show status with visual indicator
  let statusText = 'Invalid - No Signal';
  let statusClass = 'status-error';
  
  if (data.valid) {
    statusText = 'Valid - Fix OK';
    statusClass = 'status-ok';
  }
  
  const syncStatus = data.time_synced ? 'Synced' : 'Not Synced';
  const statusElement = document.getElementById('gnss-status');
  statusElement.textContent = statusText + ' (' + syncStatus + ')';
  statusElement.className = 'gnss-status ' + statusClass;
  
  // Show message about GNSS status
  const messageElement = document.getElementById('gnss-message');
  if (messageElement) {
    if (!data.valid) {
      messageElement.textContent = data.message || 'Waiting for GNSS signal. Using default values (2020-01-01 00:00:00)';
      messageElement.className = 'gnss-message warning';
    } else {
      messageElement.textContent = 'GNSS signal acquired and ready for time synchronization.';
      messageElement.className = 'gnss-message success';
    }
  }
  
  display.classList.remove('hidden');
}

// 给刷新按钮绑定事件
document.addEventListener('DOMContentLoaded', () => {
  const refreshBtn = document.getElementById('refresh-btn');
  if (refreshBtn) {
    refreshBtn.addEventListener('click', () => {
      loadPendingTask();
      loadDeviceStatus();
    });
  }
});





