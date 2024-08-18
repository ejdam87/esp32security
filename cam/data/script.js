
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
            let selector = document.getElementById("photo-select");

            if (capture)
            {
                capture_url = url;
            }
            else
            {
                motion_url = url;
            }

            if (selector.checked == capture)
            {
                selector.checked = !selector.checked;
                select_handler(selector);
            }
            else
            {
                img.src = url;
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
    loadImage(xhr2, false);
    xhr2.send();
    // ---
}

function select_handler(select_obj)
{
    // motion photo
    if (select_obj.checked)
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
}

document.getElementById("switch-status").textContent = CAPTURE_SWITCH_DESC;
document.getElementById("photo-select").addEventListener("change", function() {select_handler(this)});
