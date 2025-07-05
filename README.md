# Smart Warehouse AVR (ATmega328P)

> **Ngôn ngữ:** 🇻🇳 Vietnamese / 🇬🇧 English

---

## 📌 Giới thiệu – Overview

Dự án **Nhà kho thông minh** (Smart Warehouse) là hệ thống giám sát và điều khiển môi trường kho, sử dụng vi điều khiển **ATmega328P @16 MHz** lập trình bằng **CodeVisionAVR**.

Chức năng chính gồm: cảm biến nhiệt độ, độ ẩm, phát hiện lửa, điều khiển servo mở cửa, còi buzzer cảnh báo, LED báo hiệu, hiển thị LCD I²C và gửi log dữ liệu ra UART.

---

## 🧰 Phần cứng – Hardware

| Thiết bị                  | Chân kết nối         | Ghi chú                         |
|--------------------------|----------------------|----------------------------------|
| ATmega328P @16 MHz       | –                    | Thạch anh ngoài 16 MHz          |
| LCD 16×2 I²C (PCF8574)   | PC4 (SDA), PC5 (SCL) | Địa chỉ `0x27`                  |
| Cảm biến DHT11           | PD5                  | Đo nhiệt độ & độ ẩm             |
| Cảm biến lửa (flame IR)  | PD2                  | Mức LOW khi phát hiện lửa       |
| Buzzer                   | PD4                  | Mức HIGH kích hoạt              |
| Servo SG90 180°          | PB2 (D10)            | PWM 50Hz                        |
| Cảm biến vật cản LM393   | PD7                  | Mức LOW khi có vật cản          |
| LED báo                  | PB4                  | Mức HIGH để bật                 |

---

## 🚀 Tính năng – Features

- Đọc **nhiệt độ** và **độ ẩm** mỗi giây, hiển thị LCD và gửi log UART.
- Phát hiện **lửa** sẽ:
  - Bật **buzzer**
  - Nhấp nháy **LED**
  - Mở **servo** cửa khẩn cấp
- Điều khiển **servo** mở/đóng qua UART, có kiểm tra vật cản.
- Điều khiển **LED** từ xa bằng UART.
- Giao tiếp UART tốc độ **9600 bps (8N1)** với các lệnh điều khiển:
  - `B` – Buzzer kêu
  - `F` – Mở cửa (servo)
  - `G` – Đóng cửa (nếu không có vật cản)
  - `D` – Bật LED
  - `E` – Tắt LED

---

## 📁 Cấu trúc thư mục – Directory Structure


---

## 🛠️ Yêu cầu – Requirements

- ✅ **CodeVisionAVR 3.x** hoặc mới hơn
- ✅ **USBasp** hoặc ArduinoISP để nạp chương trình
- ⭕ Tùy chọn: `avr-gcc`, `avrdude` nếu build dòng lệnh

---

## 🧪 Biên dịch & Nạp – Build & Flash

### ➤ CodeVisionAVR IDE

1. Tạo project `.prj` nếu chưa có.
2. Cấu hình:
   - MCU: **ATmega328P**
   - Clock: **External 16 MHz**
3. Nhấn **F9** để build.
4. Nạp qua **Tools → Chip Programmer** bằng USBasp.

---

## 🧑‍💻 Sử dụng – Usage

- Mở terminal UART tốc độ **9600 bps**
- Gửi các lệnh: `B`, `F`, `G`, `D`, `E`
- Ví dụ dữ liệu UART:
Nhiet do:29C,Do am:61%,Lua:KHONG
Nhiet do:31C,Do am:58%,Lua:CO

---

## 🔧 Đóng góp – Contributing

Mọi đóng góp cải tiến hoặc sửa lỗi đều được hoan nghênh!  

---

© 2025 Duc Trung