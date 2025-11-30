function showSection(sectionId) {
  document.querySelectorAll('.section').forEach(sec => sec.classList.add('hidden'));
  document.getElementById(sectionId).classList.remove('hidden');
}

// 后续逻辑：填充 update-tbody 与 /client_register 对接
function loadUpdateTable() {
  // 示例：后续通过 fetch('/client_register') 获取设备列表
  let tbody = document.getElementById('update-tbody');
  tbody.innerHTML = `
    <tr>
      <td>Device_1</td>
      <td>v1.0.0</td>
      <td><button onclick="updateDevice('Device_1')">更新</button></td>
    </tr>
  `;
}

function updateDevice(name) {
  alert("准备更新设备: " + name);
}
