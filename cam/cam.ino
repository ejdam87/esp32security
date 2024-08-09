/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-cam-take-photo-display-web-server/
  
  IMPORTANT!!! 
   - Select Board "AI Thinker ESP32-CAM"
   - GPIO 0 must be connected to GND to upload a sketch
   - After connecting GPIO 0 to GND, press the ESP32-CAM on-board RESET button to put your board in flashing mode
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include "credentials.h" // secret stuff

#include "WiFi.h"
#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
#include <ESPAsyncWebServer.h>
#include <StringArray.h>
#include <SPIFFS.h>
#include <FS.h>
#include <esp_now.h>
#include <time.h>

const char* TIMEZONE = "CET-1CEST,M3.5.0/02:00:00,M10.5.0/03:00:00";
const char* NTP_SERVER = "pool.ntp.org";

bool flashlight_on = false;
struct tm sensor_trigger_time = {0};

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Photo File names to save in SPIFFS
#define USER_PHOTO "/user_photo.jpg"
#define SENSOR_PHOTO "/sensor_photo.jpg"

// OV2640 camera module pins (CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define FLASHLIGHT         4

// callback function that will be executed when data is received
void OnDataRecv(const esp_now_recv_info* mac, const uint8_t *incomingData, int len)
{
  Serial.println("Motion sensor signal received");
  Serial.println("Capturing the photo...");
  capturePhotoSaveSpiffs(SENSOR_PHOTO);
  Serial.println("Capturing receiving time..."); 
  getLocalTime(&sensor_trigger_time);
}

void setup() {

  // Initialize serial port for debugging purposes
  Serial.begin(115200);

  // --- ESP-NOW communication with motion sensor
  WiFi.mode(WIFI_AP_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else
  {
    Serial.println("ESP-NOW initialized successfully");
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
  // ---

  // Initialize flashlight pin
  pinMode(FLASHLIGHT, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Synchronize time
  configTzTime(TIMEZONE, NTP_SERVER);

  // Initialize file system (storing frontend files and captured photos)
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    ESP.restart();
  }
  else {
    delay(500);
    Serial.println("SPIFFS mounted successfully");
  }

  // Print ESP32 Local IP Address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Turn-off the 'brownout detector'
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  // OV2640 camera module
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    ESP.restart();
  }

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html");
  });

  // front-end files responses
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js");
  });

  server.on("/capture-icon.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/capture-icon.png");
  });

  server.on("/flashlight.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/flashlight.png");
  });

  server.on("/motion.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/motion.png");
  });

  // --- functional responses
  server.on("/capture", HTTP_GET, [](AsyncWebServerRequest * request) {
    capturePhotoSaveSpiffs(USER_PHOTO);
    request->send(SPIFFS, USER_PHOTO, "image/jpg", false);
  });

  server.on("/toggle", HTTP_POST, [](AsyncWebServerRequest * request) {
    flashlight_on = !flashlight_on;
    digitalWrite(FLASHLIGHT, flashlight_on);
  });

  server.on("/motion", HTTP_GET, [](AsyncWebServerRequest *request) {
    char timeString[64];
    strftime(timeString, sizeof(timeString), "%A, %B %d %Y %H:%M:%S", &sensor_trigger_time);
    Serial.println(timeString);
    request->send(200, "text/plain", timeString);
  });
  // ---

  // Start server
  server.begin();
}

void loop() {}

bool checkPhoto( fs::FS &fs, const char* path ) {
  File f_pic = fs.open(path);
  unsigned int pic_sz = f_pic.size();
  return ( pic_sz > 100 );
}

void capturePhotoSaveSpiffs( const char* path ) {
  camera_fb_t * fb = NULL; // pointer
  bool ok = 0; // Boolean indicating if the picture has been taken correctly

  do {
    Serial.println("Taking a photo...");

    // Take a photo with the camera
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      return;
    }

    // Photo file name
    Serial.printf("Picture file name: %s\n", path);
    File file = SPIFFS.open(path, FILE_WRITE);

    // Insert the data in the photo file
    if (!file) {
      Serial.println("Failed to open file in writing mode");
    }
    else {
      file.write(fb->buf, fb->len); // payload (image), payload length
      Serial.print("The picture has been saved in ");
      Serial.print(path);
      Serial.print(" - Size: ");
      Serial.print(file.size());
      Serial.println(" bytes");
    }
    // Close the file
    file.close();
    esp_camera_fb_return(fb);

    // check if file has been correctly saved in SPIFFS
    ok = checkPhoto(SPIFFS, path);
  } while ( !ok );
}
