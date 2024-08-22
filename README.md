# ESP32-based room security system

## Hardware used:
### Motion module
  - [ESP-WROOM32](https://www.amazon.com/HiLetgo-ESP-WROOM-32-Development-Microcontroller-Integrated/dp/B0718T232Z)
  - [PIR sensor](https://www.amazon.com/Stemedu-HC-SR505-Pyroelectric-Infrared-Detector/dp/B096NVJ439)
  - Solderless breadboard
  - Jumper wires
  - Batteries (4 x 1.2V)
![Motion module](https://github.com/ejdam87/esp32security/blob/main/doc_photo/motion_sensor_module.jpg)

### Camera module
  - [ESP32-CAM](https://www.amazon.com/Hosyond-ESP32-CAM-Bluetooth-Development-Compatible/dp/B09TB1GJ7P/ref=sr_1_13?dib=eyJ2IjoiMSJ9.oLYJOUxbrHkCb-EvkMiDuevv7kGyBI9tsA3qvP_GZX8EJQb-SX__GJshnTgkX18K_oV7kI7_OfCeNxHpAH0odX7r4-T7lrvLU4GDjwl9-ucmfAEMALFfDJg2F435IXRWVPjo0FkxO-iBAI2-x8dDkbXHzQa3rYBmx4EJZW_jvyPv0b_ng_xaiecz5FDl5TSJV8x4aGgYkZE9s-G4ZCxZQQu_b1MbXU5oDpOx-NUIqtQ.5ddJceZzRyI1lB4lBL-gfe_oHLfpwM8BvNYntU-dJuM&dib_tag=se&keywords=esp32+cam&qid=1724314662&sr=8-13)
  - [FTDI programmer](https://www.amazon.com/HiLetgo-FT232RL-Converter-Adapter-Breakout/dp/B00IJXZQ7C/ref=sr_1_4?dib=eyJ2IjoiMSJ9.NRVAVgW6COuNEt1acZzDB36b5h72yBUhHJwQ04Hzbg9cC4jh4DRr4R_kBa_S9fWFgrR3AG3M-iL0GCsrrVW8VYXD8pWfK4Qafnvvs2OBgLQDBxMVgdiajk_nJCUf1LlfBSgdejboD_qNW6qwaYm1bT7Sa31chjL_W9hbl6gHkyWFpGytb5CqabJ5PdwvoDvXXWrtz2LvFMrY73cz6jkzytvUoCJpHqaFexNGqDOvY-I.fVY3ZV02OR6j_PHROtpyVfGyyKRaua3hjEyym2q2HZ8&dib_tag=se&keywords=FTDI&qid=1724316485&sr=8-4)
  - Jumper wires
  - Power bank
![Camera module](https://github.com/ejdam87/esp32security/blob/main/doc_photo/camera_module.jpg)

## Abstract scheme of the system
![Scheme](https://github.com/ejdam87/esp32security/blob/main/doc_photo/scheme.png)

## Code
Both modules need hidden file `credentials.h` which stores secrets only known to the administrator of the system. This includes:
- SSID and password for the WiFi connection (necessary for both modules)
- telegram bot token and user ID (only for motion module)
- webserver authentication name, password, and token (only for camera module)

The codebase presented in this repository does not include such files due to privacy preservation. You should include your own `credentials.h` files for both modules.

### Motion module
ESP-WROOM32 is running the code shown in `motion` directory. The board is listening for sensor trigger at GPIO25 pin. After the trigger, a telegram notification is sent to the client. Simultaneously, the board sends a signal that the motion sensor was triggered to ESP32-CAM board using ESP-NOW wireless comunication protocol.

### Camera module
The program executed by ESP32-CAM board is present in `cam` directory. The board runs a webserver with authentication to access and control the camera. The currently supported functionalities are:
- Camera flashlight toggle
- Photo capture
- Get the time of the most recent motion sensor trigger

When the ESP32-CAM board receives a signal from the motion module, it immediately captures the photo to potentially capture the object triggering it.
![webserver](https://github.com/ejdam87/esp32security/blob/main/doc_photo/web.png)
