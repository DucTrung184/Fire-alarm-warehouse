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

### ➤ Dùng CodeVisionAVR IDE

#### 1. Tạo Project mới

1. Mở **CodeVisionAVR** → chọn **File → New → Project**
2. Chọn chip **ATmega328P**, crystal **16 MHz**, nhấn **Next** đến khi hoàn tất

#### 2. Cấu hình project thêm file `.c`

1. Nhấn nút **Project** trên thanh công cụ → chọn **Configuration**
2. Trong cửa sổ mới:
   - Vào tab **File**
   - **Xoá hết** các file có sẵn
   - **Add** 2 file từ thư mục `src/`:
     - `main.c`
     - `soft_i2c.c`
   - Nhấn **OK**
3. Không thêm bất kỳ file nào ngoài 2 file trên

#### 3. Biên dịch & nạp

Nhấn **F9** để biên dịch.

---

## 📱 Lưu ý khi sử dụng với ứng dụng Bluetooth

Dự án được nhóm phát triển song song với **ứng dụng riêng trên App Inventor** để điều khiển hệ thống qua **Bluetooth**.

### 🔹 Trường hợp 1: Dùng đúng app do nhóm cung cấp

- Người dùng cần tạo app bằng **MIT App Inventor**
- App này sẽ gửi chính xác các ký tự: `B`, `F`, `G`, `D`, `E`
- App sẽ tương thích 100% với mã nguồn dự án

### 🔹 Trường hợp 2: Dùng app có sẵn (trên CH Play)

- Các app như **Serial Bluetooth Terminal**, **Bluetooth Controller** có thể không gửi đúng ký tự
- Bạn cần:
  - **Sửa mã nguồn C** để phù hợp với lệnh từ app đang dùng
  - Hoặc chỉnh app để gửi đúng ký tự như trên

> ✅ Khuyên dùng app có thể tùy chỉnh nút gửi như:  
> [Serial Bluetooth Terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal)

---

## 🧑‍💻 Sử dụng – Usage

- Mở terminal UART (9600 bps)
- Gửi các lệnh: `B`, `F`, `G`, `D`, `E`
- Ví dụ log UART:

---

## 📝 Tác giả

- **Mai Đình Đức Trung**
- **Nguyễn Viết Minh**
- **Lê Văn Nam**
- **Lê Việt Cường**
- **Trần Văn Trọng**
- **Khiếu Hữu Tiến Dũng**
- **Vũ Thị Mai Phương**
- **Nguyễn Ngọc Gia Bảo**
- [DucTrung184](hhttps://github.com/DucTrung184)

---

## 🔧 Đóng góp – Contributing

Mọi đóng góp cải tiến hoặc sửa lỗi đều được hoan nghênh!  

---

© 2025 Duc Trung
