
function capturePhoto()
{
    let xhr = new XMLHttpRequest();
    xhr.open('GET', "/capture", true);

    xhr.responseType = 'blob'; // Set the response type to 'blob' for binary data
    loadImage(xhr);
    xhr.send();
}

function loadImage(xhr)
{
    xhr.onload = function() {
    if (xhr.status === 200) {
      var blob = xhr.response;
      var img = document.getElementById('photo');
      var url = URL.createObjectURL(blob); // Create a URL for the blob data
      img.src = url; // Set the image source to the blob URL

      // Optionally, revoke the object URL after the image is loaded
      img.onload = function() {
        URL.revokeObjectURL(url);
      };
    } else {
      console.error('Failed to load image. Status: ' + xhr.status);
    }
    };

    xhr.onerror = function() {
    console.error('Network error occurred while loading image.');
    };
}

function toggle()
{
    let xhr = new XMLHttpRequest();
    xhr.open('POST', "/toggle", true);
    xhr.send();   
}

function motionHandle()
{
    let xhr = new XMLHttpRequest();
    xhr.open('GET', "/motion", true);

    xhr.onload = function() {
        if (xhr.status === 200) {
            document.getElementById("motion-time").textContent = "Last trigger time: " + xhr.responseText;
        } else {
            console.error('Error:', xhr.statusText);
        }
    };

    xhr.send();
}
