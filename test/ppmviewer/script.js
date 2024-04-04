
function loadPPImage(file) {
    if (!file) return;

    const reader = new FileReader();
    reader.onload = function(e) {
      const data = e.target.result;
      displayPPMImage(data);
    };
    reader.readAsText(file);
}

function displayPPMImage(data) {
  const lines = data.trim().split("\n");
  const dimensions = lines[1].split(" ");
  const width = parseInt(dimensions[0]);
  const height = parseInt(dimensions[1]);
  const pixels = lines.slice(3); // Starting from line 3 are the pixel values

  const canvas = document.createElement("canvas");
  canvas.width = width;
  canvas.height = height;
  const ctx = canvas.getContext("2d");
  const imageData = ctx.createImageData(width, height);
  const buffer = new Uint32Array(imageData.data.buffer);

  for (let y = 0; y < height; y++) {
    for (let x = 0; x < width; x++) {
      const index = y * width + x;
      const rgb = pixels[index].split(" ").map(val => parseInt(val));
      const color = (255 << 24) | (rgb[2] << 16) | (rgb[1] << 8) | rgb[0];
      buffer[index] = color;
    }
  }

  ctx.putImageData(imageData, 0, 0);

  const container = document.getElementById("image-container");
  container.innerHTML = "";
  container.appendChild(canvas);
}


files = null;
document.addEventListener("DOMContentLoaded", function() {
  const fileInput = document.getElementById("file-input");

  fileInput.addEventListener("change", function(event) {
    files = event.target.files;
    loadPPImage(files[i]);
  });
});

let actionExecuted = false;
i = 0;
window.addEventListener("keydown", function (event) {
    if (event.defaultPrevented || actionExecuted) {
      return;
    }
  
    switch (event.key) {
      case "ArrowDown":
        console.log("ArrowDown");
        actionExecuted = true;
        break;
      case "ArrowUp":
        console.log("ArrowUp");
        actionExecuted = true;
        break;
      case "ArrowLeft":
        console.log("ArrowLeft");
        i = (i + 1) % files.length;
        loadPPImage(files[i]);
        actionExecuted = true;
        break;
      case "ArrowRight":
        console.log("ArrowRight");
        i = i - 1;
        if (i < 0)
          i = files.length - 1;
        loadPPImage(files[i]);
        actionExecuted = true;
        break;
      default:
        return;
    }

    event.preventDefault();
  }, true);

  document.addEventListener("keyup", function(event) {
    if (event.key === "ArrowDown" || event.key === "ArrowUp" || event.key === "ArrowLeft" || event.key === "ArrowRight") {
      actionExecuted = false;
    }
  });