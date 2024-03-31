
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

    for (let y = 0; y < height; y++) {
      for (let x = 0; x < width; x++) {
        const index = y * width + x;
        const rgb = pixels[index].split(" ").map(val => parseInt(val));
        const color = `rgb(${rgb[0]}, ${rgb[1]}, ${rgb[2]})`;
        ctx.fillStyle = color;
        ctx.fillRect(x, y, 1, 1);
      }
    }

    const container = document.getElementById("image-container");
    container.innerHTML = "";
    container.appendChild(canvas);
  }


image = 'C:\\Users\\ernes\\Desktop\\Travail\\S8\\isim\\Raytracer\\test\\around\\around1.ppm';
function loadSpecificImage(file_name) {
    fetch(file_name)
        .then(response => response.text())
        .then(data => {
        displayPPMImage(data);
        })
        .catch(error => {
        console.error("Error loading specific image:", error);
        });
}
loadSpecificImage(image)

document.addEventListener("DOMContentLoaded", function() {
  const fileInput = document.getElementById("file-input");

  fileInput.addEventListener("change", function(event) {
    const file = event.target.files[i];
    console.log(typeof(file))
    loadPPImage(file);
  });
});

i = 0;

window.addEventListener("keydown", function (event) {
    if (event.defaultPrevented) {
      return; // Do nothing if the event was already processed
    }
  
    switch (event.key) {
      case "ArrowDown":
        console.log("ArrowDown");
        break;
      case "ArrowUp":
        console.log("ArrowUp");
        break;
      case "ArrowLeft":
        console.log("ArrowLeft");
        i = (i + 1) % event.target.files.length;
        loadPPImage(event.target.files[i]);
        break;
      case "ArrowRight":
        i = (i - 1) % event.target.files.length;
        loadPPImage(event.target.files[i]);    
        console.log("ArrowRight");
        break;
      default:
        return; // Quit when this doesn't handle the key event.
    }
  
    // Cancel the default action to avoid it being handled twice
    event.preventDefault();
  }, true);
  // the last option dispatches the event to the listener first,
  // then dispatches event to window