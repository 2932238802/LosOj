document.addEventListener("DOMContentLoaded", function () {
  ace.require("ace/ext/language_tools");
  var editor = ace.edit("editor");

  var themeSelector = document.getElementById("theme-selector");
  var fontSizeSelector = document.getElementById("fontsize-selector");
  var fontFamilySelector = document.getElementById("fontfamily-selector");
  var editorElement = document.getElementById("editor");

  function applySettings() {
    var savedTheme = localStorage.getItem("editorTheme") || "ace/theme/xcode";
    var savedFontSize = localStorage.getItem("editorFontSize") || "16";
    var savedFontFamily =
      localStorage.getItem("editorFontFamily") || "Fira Code";

    editor.setTheme(savedTheme);
    editor.setFontSize(parseInt(savedFontSize, 10));
    editorElement.style.fontFamily = savedFontFamily;
    editor.resize(true);

    themeSelector.value = savedTheme;
    fontSizeSelector.value = savedFontSize;
    fontFamilySelector.value = savedFontFamily;
  }

  applySettings();

  themeSelector.addEventListener("change", function () {
    var newTheme = this.value;
    editor.setTheme(newTheme);
    localStorage.setItem("editorTheme", newTheme);
  });

  fontSizeSelector.addEventListener("change", function () {
    var newSize = this.value;
    editor.setFontSize(parseInt(newSize, 10));
    localStorage.setItem("editorFontSize", newSize);
  });

  fontFamilySelector.addEventListener("change", function () {
    var newFont = this.value;
    editorElement.style.fontFamily = newFont;
    editor.resize(true);
    localStorage.setItem("editorFontFamily", newFont);
  });

  editor.session.setMode("ace/mode/c_cpp");
  editor.setShowPrintMargin(false);

  editor.setOptions({
    enableBasicAutocompletion: true,
    enableLiveAutocompletion: true,
  });

  const pathParts = window.location.pathname.split("/").filter((part) => part);
  const questionId = pathParts[pathParts.length - 1];
  const storageKey = `editorCode_${questionId}`;

  const savedCode = localStorage.getItem(storageKey);
  var initialCodeElement = document.getElementById("initial-code");

  if (savedCode) {
    editor.setValue(savedCode, -1);
    console.log("从 localStorage 加载了已保存的代码。");
  } else if (initialCodeElement) {
    editor.setValue(initialCodeElement.innerHTML, -1);
  }

  editor.session.on("change", function () {
    localStorage.setItem(storageKey, editor.getValue());
  });

  Split(["#problem-panel", "#editor-panel"], {
    sizes: [50, 50],
    minSize: 300,
    gutterSize: 15,
    cursor: "col-resize",
    onDrag: function () {
      editor.resize();
    },
  });

  var submitBtn = document.getElementById("submit-btn");
  var resultPanel = document.getElementById("result-panel");
  var resultContent = document.getElementById("result-content");

  submitBtn.addEventListener("click", submitCode);

  async function submitCode() {
    const pathParts = window.location.pathname
      .split("/")
      .filter((part) => part);
    const questionId = pathParts[pathParts.length - 1];

    var code = editor.getValue();

    if (!questionId) {
      alert("无法从URL中获取题目ID！请检查URL是否为 /question/[id] 格式。");
      return;
    }

    submitBtn.disabled = true;
    submitBtn.textContent = "判定中...";
    resultPanel.style.display = "block";
    resultContent.innerHTML =
      '<p class="message">正在向服务器提交代码并等待判题结果...</p>';

    try {
      const response = await fetch(`/judge/${questionId}`, {
        method: "POST",
        headers: {
          "Content-Type": "text/plain",
        },
        body: JSON.stringify({ code: code }),
      });

      if (!response.ok) {
        throw new Error(`HTTP 错误! 状态: ${response.status}`);
      }

      const resultData = await response.json();

      displayResults(resultData);
    } catch (error) {
      displayResults({
        status: "Client Error",
        message: "请求失败，请检查网络连接或联系管理员。",
        stderr: error.toString(),
      });
    } finally {
      submitBtn.disabled = false;
      submitBtn.textContent = "提交代码";
    }
  }

  function displayResults(data) {
    resultContent.innerHTML = "";

    let statusClass = "";
    let state = data.state;
    let statusText = "";

    if (state === 0) {
      if (data.right_number == data.all_number) {
        statusClass = "status-accepted";
        statusText = "Accepted";
      } else {
        statusClass = "status-wrong-answer";
        var rightNumber = data.right_number;
        var allNumber = data.all_number;
        statusText = `Wrong Answer ${rightNumber}/${allNumber}`;
      }
    } else {
      statusClass = "status-error";
      statusText = "Runtime Error";
    }

    let html = `<div class="status ${statusClass}">${statusText}</div>`;

    if (data.reason) {
      html += `<pre class="message">${data.reason}</pre>`;
    }

    if (data.stderr) {
      html += `<h5>错误输出:</h5><pre><code>${data.stderr}</code></pre>`;
    }

    // if (data.stdout) {
    //   html += `<h5>标准输出:</h5><pre><code>${data.stdout}</code></pre>`;
    // }

    resultContent.innerHTML = html;
  }
});
