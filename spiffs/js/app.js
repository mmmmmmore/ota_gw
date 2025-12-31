let progressTimer =null;
let upgradeTimerOutFlag = null;
let currentTaskId =null;

let lastProgress = -1;
let stuckCounter =0;


function showSection(sectionId) {
  document.querySelectorAll('.section').forEach(sec => sec.classList.add('hidden'));
  document.getElementById(sectionId).classList.remove('hidden');

  // 如果进入 update 区域，加载任务信息
  if (sectionId === 'update') {
    loadPendingTask();
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
                <button onclick="sendUserResponse('accept', '${task.task_id}')">接受</button>
                <button onclick="sendUserResponse('reject', '${task.task_id}')">拒绝</button>
              </td>
            </tr>
          `;

        });
      } else {
        tbody.innerHTML = '<tr><td colspan="5">当前没有待确认任务</td></tr>';
      }
    })
    .catch(err => {
      console.error('加载任务失败', err);
    });
}

// 提交用户选择
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

// 给刷新按钮绑定事件
document.addEventListener('DOMContentLoaded', () => {
  const refreshBtn = document.getElementById('refresh-btn');
  if (refreshBtn) {
    refreshBtn.addEventListener('click', () => {
      loadPendingTask();
    });
  }
});





