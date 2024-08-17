
let capture_url = document.getElementById('photo').src;
let motion_url = capture_url;

const MOTION_SWITCH_DESC = "Picture taken at the motion sensor trigger";
const CAPTURE_SWITCH_DESC = "Picture taken at the button click";

function capturePhoto()
{
    let xhr = new XMLHttpRequest();
    xhr.open('GET', "/capture", true);

    xhr.responseType = 'blob';
    loadImage(xhr, true);
    xhr.send();
}

function loadImage(xhr, capture)
{
    xhr.onload = function()
    {
        if (xhr.status === 200)
        {
            var blob = xhr.response;
            var img = document.getElementById('photo');
            var url = URL.createObjectURL(blob); // Create a URL for the blob data

            URL.revokeObjectURL(img.src);
            img.src = url; // Set the image source to the blob URL
            if (capture)
            {
                capture_url = url;
            }
            else
            {
                motion_url = url;
            }
        }
        else
        {
            console.error('Failed to load image. Status: ' + xhr.status);
        }
    };

    xhr.onerror = function()
    {
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
    // --- Time of the motion trigger event
    let xhr = new XMLHttpRequest();
    xhr.open('GET', "/motion-time", true);

    xhr.onload = function()
    {
        if (xhr.status === 200)
        {
            document.getElementById("motion-time").textContent = "Last trigger time: " + xhr.responseText;
        }
        else
        {
            console.error('Error:', xhr.statusText);
        }
    };
    xhr.send();
    // ---

    // --- Picture taken at that moment
    let xhr2 = new XMLHttpRequest();
    xhr2.open('GET', "/motion-picture", true);
    xhr2.responseType = 'blob';

    xhr2.onload = function()
    {
        if (xhr2.status === 200)
        {
            loadImage(xhr2, false);
        }
        else
        {
            console.error('Error:', xhr2.statusText);
        }
    };
    xhr2.send();
    // ---
}

document.getElementById("switch-status").textContent = CAPTURE_SWITCH_DESC;
document.getElementById("photo-select").addEventListener("change", function()
    {
        // motion photo
        if (this.checked)
        {
            document.getElementById("switch-status").textContent = MOTION_SWITCH_DESC;
            document.getElementById("photo").src = motion_url;
        }
        // classic photo
        else
        {
            document.getElementById("switch-status").textContent = CAPTURE_SWITCH_DESC;
            document.getElementById("photo").src = capture_url;
        }
    });
