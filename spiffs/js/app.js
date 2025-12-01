function showSection(sectionId) {
  document.querySelectorAll('.section').forEach(sec => sec.classList.add('hidden'));
  document.getElementById(sectionId).classList.remove('hidden');

  // 如果进入 update 区域，加载任务信息
  if (sectionId === 'update') {
    loadPendingTask();
  }
}

// 加载待确认任务
function loadPendingTask() {
  fetch('/task_info')
    .then(resp => resp.json())
    .then(task => {
      let tbody = document.getElementById('update-tbody');
      tbody.innerHTML = '';

      if (task.version) {
        tbody.innerHTML = `
          <tr>
            <td>${task.task_id || '-'}</td>
            <td>${task.device_name || '-'}</td>
            <td>${task.client_id || '-'}</td>
            <td>${task.version}</td>
            <td>${task.features || '-'}</td>
            <td>
              <button onclick="sendUserResponse('accept', '${task.client_id}')">接受</button>
              <button onclick="sendUserResponse('reject', '${task.client_id}')">拒绝</button>
            </td>
          </tr>
        `;

        
      } else {
        tbody.innerHTML = '<tr><td colspan="5">当前没有待确认任务</td></tr>';
      }
    })
    .catch(err => {
      console.error('加载任务失败', err);
    });
}

// 提交用户选择
function sendUserResponse(decision, mac) {
  fetch('/ota_user_response', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ decision: decision, mac: mac })
  })
  .then(resp => resp.json())
  .then(data => {
    alert('响应已提交: ' + decision);
    // 清空表格或刷新
    loadPendingTask();
  })
  .catch(err => {
    console.error('提交响应失败', err);
  });
}

// 给刷新按钮绑定事件
document.addEventListener('DOMContentLoaded', () => {
  const refreshBtn = document.getElementById('refresh-btn');
  if (refreshBtn) {
    refreshBtn.addEventListener('click', () => {
      loadPendingTask();
    });
  }
});

