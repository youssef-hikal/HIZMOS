
![WhatsApp Image 2026-02-07 at 1 03 42 AM (1)](https://github.com/user-attachments/assets/6f943263-805b-42b5-a672-a311f1e12852)


## Complete Connection Diagram

```text
ESP32-S3
│
├── I2C
│   ├── SDA  → GPIO8
│   └── SCL  → GPIO9
│
├── IR
│   ├── TX   → GPIO35
│   └── RX   → GPIO36
│
├── nRF24 #1
│   ├── CE   → GPIO10
│   ├── CSN  → GPIO11
│   ├── SCK  → GPIO18
│   ├── MISO → GPIO16
│   └── MOSI → GPIO17
│
├── nRF24 #2
│   ├── CE   → GPIO12
│   ├── CSN  → GPIO13
│   ├── SCK  → GPIO18
│   ├── MISO → GPIO16
│   └── MOSI → GPIO17
│
├── CC1101 #1
│   ├── CS   → GPIO45
│   ├── GDO0 → GPIO21
│   ├── GDO2 → GPIO47
│   ├── SCK  → GPIO15
│   ├── MISO → GPIO3
│   └── MOSI → GPIO46
│
├── CC1101 #2
│   ├── CS   → GPIO40
│   ├── GDO0 → GPIO41
│   ├── GDO2 → GPIO42
│   ├── SCK  → GPIO15
│   ├── MISO → GPIO3
│   └── MOSI → GPIO46
│
├── SD Card
│   ├── CS   → GPIO37
│   ├── SCK  → GPIO14
│   ├── MISO → GPIO39
│   └── MOSI → GPIO38
│
└── Buttons
    ├── UP     → GPIO4
    ├── DOWN   → GPIO5
    ├── SELECT → GPIO6
    ├── BACK   → GPIO7
    ├── LEFT   → GPIO1
    └── RIGHT  → GPIO2



# Flashing ESP32-S3 Firmware with ESP Web Tool

Use the following files and flash addresses:

| Address | File |
|----------|----------|
| 0x0000 | HIZMOS_OLED_U8G2lib.ino.bootloader.bin |
| 0x8000 | HIZMOS_OLED_U8G2lib.ino.partitions.bin |
| 0x10000 | HIZMOS_OLED_U8G2lib.ino.bin |

## Steps

1. Open https://esptool.spacehuhn.com/
2. Click **Connect** and select your ESP32-S3 serial port.
3. Click **Add File** and add:
   - `HIZMOS_OLED_U8G2lib.ino.bootloader.bin` at address `0x0000`
   - `HIZMOS_OLED_U8G2lib.ino.partitions.bin` at address `0x8000`
   - `HIZMOS_OLED_U8G2lib.ino.bin` at address `0x10000`
4. Enable **Erase Before Flashing** (recommended).
5. Click **Program**.
6. Wait for flashing to complete and reboot the device.

## Notes

- Target chip: **ESP32-S3(N16R8)**


- Target chip: **ESP32-S3(N16R8)**

