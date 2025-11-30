function showContent(sectionId) {
  document.querySelectorAll('.section').forEach(sec => sec.classList.add('hidden'));
  document.getElementById(sectionId).classList.remove('hidden');
}

// 定时检查是否有任务
async function pollTask() {
  let res = await fetch('/task_info');
  let data = await res.json();
  if (Object.keys(data).length > 0) {
    let btn = document.getElementById('checkUpdateBtn');
    btn.classList.add('active');
    btn.disabled = false;
    btn.onclick = showModal;
  }
}

function showModal() {
  document.getElementById('modal').style.display = 'block';
  loadTaskInfo();
}

async function loadTaskInfo() {
  let res = await fetch('/task_info');
  let data = await res.json();
  document.getElementById('taskInfo').innerText = JSON.stringify(data, null, 2);
}

async function sendResponse(choice) {
  await fetch('/ota_user_response', {
    method: 'POST',
    body: choice
  });
  if (choice === 'Accept') {
    startProgressPolling();
  } else {
    alert("用户拒绝升级");
    document.getElementById('modal').style.display = 'none';
  }
}

async function startProgressPolling() {
  let progressBar = document.getElementById('progressBar').firstElementChild;
  let progressText = document.getElementById('progressText');

  let interval = setInterval(async () => {
    let res = await fetch('/progress_info');
    let data = await res.json();

    // 假设只展示第一个 Client 的进度
    let client = data[0];
    progressBar.style.width = client.progress + "%";
    progressText.innerText = "进度: " + client.progress + "%";

    if (client.progress >= 100) {
      clearInterval(interval);
      alert("升级成功！");
      document.getElementById('modal').style.display = 'none';
    }
  }, 2000);
}

// 每5秒检查是否有新任务
setInterval(pollTask, 5000);
