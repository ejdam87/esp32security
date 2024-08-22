# ESP32-based room security system

## Hardware used:
### Motion module
  - [ESP-WROOM32](https://www.amazon.com/HiLetgo-ESP-WROOM-32-Development-Microcontroller-Integrated/dp/B0718T232Z)
  - [PIR sensor](https://www.amazon.com/Stemedu-HC-SR505-Pyroelectric-Infrared-Detector/dp/B096NVJ439)
  - Solderless breadboard
  - Jumper wires
  - Batteries (4 x 1.2V)

### Camera module
  - [ESP32-CAM](https://www.amazon.com/Hosyond-ESP32-CAM-Bluetooth-Development-Compatible/dp/B09TB1GJ7P/ref=sr_1_13?dib=eyJ2IjoiMSJ9.oLYJOUxbrHkCb-EvkMiDuevv7kGyBI9tsA3qvP_GZX8EJQb-SX__GJshnTgkX18K_oV7kI7_OfCeNxHpAH0odX7r4-T7lrvLU4GDjwl9-ucmfAEMALFfDJg2F435IXRWVPjo0FkxO-iBAI2-x8dDkbXHzQa3rYBmx4EJZW_jvyPv0b_ng_xaiecz5FDl5TSJV8x4aGgYkZE9s-G4ZCxZQQu_b1MbXU5oDpOx-NUIqtQ.5ddJceZzRyI1lB4lBL-gfe_oHLfpwM8BvNYntU-dJuM&dib_tag=se&keywords=esp32+cam&qid=1724314662&sr=8-13)
  - [FTDI programmer](https://www.amazon.com/HiLetgo-FT232RL-Converter-Adapter-Breakout/dp/B00IJXZQ7C/ref=sr_1_4?dib=eyJ2IjoiMSJ9.NRVAVgW6COuNEt1acZzDB36b5h72yBUhHJwQ04Hzbg9cC4jh4DRr4R_kBa_S9fWFgrR3AG3M-iL0GCsrrVW8VYXD8pWfK4Qafnvvs2OBgLQDBxMVgdiajk_nJCUf1LlfBSgdejboD_qNW6qwaYm1bT7Sa31chjL_W9hbl6gHkyWFpGytb5CqabJ5PdwvoDvXXWrtz2LvFMrY73cz6jkzytvUoCJpHqaFexNGqDOvY-I.fVY3ZV02OR6j_PHROtpyVfGyyKRaua3hjEyym2q2HZ8&dib_tag=se&keywords=FTDI&qid=1724316485&sr=8-4)
  - Jumper wires
  - Power bank

## Code
The program executed by ESP32-CAM board is present in `cam` directory, while ESP-WROOM32 is running the code shown in `motion` directory.

## Abstract scheme of the system