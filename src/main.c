#include <io.h>
#include <mega328p.h>
#include <delay.h>
#include <stdio.h>
#include "soft_i2c.h"

#define F_CPU 16000000UL
#define LCD_ADDR 0x27

// DHT11: PD5
#define DHT_DDR  DDRD
#define DHT_PORT PORTD
#define DHT_PIN  PIND
#define DHT_BIT  5

// Cảm biến lửa: PD2
#define FIRE_DDR  DDRD
#define FIRE_PORT PORTD
#define FIRE_PIN  PIND
#define FIRE_BIT  2

// Buzzer: PD4
#define BUZZER_DDR  DDRD
#define BUZZER_PORT PORTD
#define BUZZER_BIT  4

// Servo 180 độ: PB2 (D10)
#define SERVO_180_DDR  DDRB
#define SERVO_180_PORT PORTB
#define SERVO_180_PIN  1  // PB2 = D10

// Cảm biến vật cản: PD7
#define OBSTACLE_SENSOR_DDR  DDRD
#define OBSTACLE_SENSOR_PORT PORTD
#define OBSTACLE_SENSOR_PIN  PIND
#define OBSTACLE_SENSOR_BIT  7

#define LED_DDR  DDRB
#define LED_PORT PORTB
#define LED_PIN  4 

unsigned char dht_data[5];

// Delay micro giây
void delay_us_variable(int us) {
    int i;
    for (i = 0; i < us; i++) {
        #asm("nop")
        #asm("nop")
        #asm("nop")
        #asm("nop")
    }
}

// Gửi xung PWM cho servo 180 độ
void pulse_servo(char angle, char mode) {
    int t_high;
    if (mode == 0)
        t_high = 1000 + ((int)angle * 1000) / 180;
    else
        t_high = 2450 + ((int)angle * 1000) / 180;

    SERVO_180_PORT |= (1 << SERVO_180_PIN); // PB2 (D10)
    delay_us_variable(t_high);
    SERVO_180_PORT &= ~(1 << SERVO_180_PIN);
    delay_ms(20); // Chu kỳ 20ms (50Hz)
}

// UART init
void uart_init(void) {
    UBRR0H = 0;
    UBRR0L = 103; // Baud rate 9600 với F_CPU 16MHz
    UCSR0B = (1<<RXEN0) | (1<<TXEN0); // Bật TX và RX
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // 8-bit data
}

// Gửi ký tự UART
void uart_send_char(char c) {
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = c;
}

// Gửi chuỗi UART
void uart_send_string(char *s) {
    while (*s) uart_send_char(*s++);
}

// Nhận ký tự từ UART
int uart_getchar(void) {
    if (UCSR0A & (1<<RXC0)) {
        return UDR0;
    }
    return -1;
}

// LCD functions
void lcd_send(unsigned char data, unsigned char rs) {
    unsigned char high = data & 0xF0;
    unsigned char low = (data << 4) & 0xF0;
    unsigned char control = 0x08 | (rs ? 0x01 : 0x00);

    i2c_Start();
    i2c_Write(LCD_ADDR << 1);
    i2c_Write(high | control); delay_us(1);
    i2c_Write(high | control | 0x04); delay_us(1);
    i2c_Write(high | control); delay_us(50);
    i2c_Write(low | control); delay_us(1);
    i2c_Write(low | control | 0x04); delay_us(1);
    i2c_Write(low | control); delay_us(50);
    i2c_Stop();
}

void lcd_command(unsigned char cmd) {
    lcd_send(cmd, 0);
    if (cmd == 0x01 || cmd == 0x02) delay_ms(2);
}

void lcd_data(unsigned char data) {
    lcd_send(data, 1);
}

void lcd_clear(void) {
    lcd_command(0x01);
    delay_ms(2);
}

void lcd_init(void) {
    delay_ms(50);
    lcd_send(0x03, 0); delay_ms(5);
    lcd_send(0x03, 0); delay_ms(1);
    lcd_send(0x03, 0); delay_ms(1);
    lcd_send(0x02, 0); delay_ms(1);
    lcd_command(0x28);
    lcd_command(0x08);
    lcd_command(0x01);
    lcd_command(0x06);
    lcd_command(0x0C);
}

void lcd_gotoxy(unsigned char x, unsigned char y) {
    lcd_command(0x80 + (y ? 0x40 : 0x00) + x);
}

void lcd_puts(char *s) {
    while (*s) lcd_data(*s++);
}

// DHT11 functions
void dht_start() {
    DHT_DDR |= (1 << DHT_BIT);
    DHT_PORT &= ~(1 << DHT_BIT);
    delay_ms(20);
    DHT_PORT |= (1 << DHT_BIT);
    delay_us(30);
    DHT_DDR &= ~(1 << DHT_BIT);
}

unsigned char dht_response() {
    delay_us(40);
    if (DHT_PIN & (1 << DHT_BIT)) return 0;
    delay_us(80);
    if (!(DHT_PIN & (1 << DHT_BIT))) return 0;
    delay_us(80);
    return 1;
}

unsigned char dht_read_byte() {
    unsigned char i, result = 0;
    for (i = 0; i < 8; i++) {
        while (!(DHT_PIN & (1 << DHT_BIT)));
        delay_us(40);
        if (DHT_PIN & (1 << DHT_BIT))
            result |= (1 << (7 - i));
        while (DHT_PIN & (1 << DHT_BIT));
    }
    return result;
}

unsigned char read_dht11() {
    unsigned char i;
    dht_start();
    if (!dht_response()) return 0;
    for (i = 0; i < 5; i++)
        dht_data[i] = dht_read_byte();
    if ((unsigned char)(dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3]) != dht_data[4])
        return 0;
    return 1;
}

// Cảm biến lửa
unsigned char check_fire() {
    return !(FIRE_PIN & (1 << FIRE_BIT)); // LOW = có lửa
}

// Cảm biến vật cản
unsigned char check_obstacle() {
    return !(OBSTACLE_SENSOR_PIN & (1 << OBSTACLE_SENSOR_BIT)); // LOW = có vật cản
}

// Buzzer
void buzzer_on() {
    BUZZER_PORT |= (1 << BUZZER_BIT);
}

void buzzer_off() {
    BUZZER_PORT &= ~(1 << BUZZER_BIT);
}

void main(void) {
    char lcd_buf[16];
    char uart_buf[64];
    int rx_byte;
    unsigned char fire;
    char mode = 0;

    // Cấu hình ban đầu
    BUZZER_DDR |= (1 << BUZZER_BIT);
    BUZZER_PORT &= ~(1 << BUZZER_BIT);

    FIRE_DDR &= ~(1 << FIRE_BIT);
    FIRE_PORT |= (1 << FIRE_BIT); // Bật pull-up
    DDRD |= (1 << 6); // PD6 là output cho LED

    OBSTACLE_SENSOR_DDR &= ~(1 << OBSTACLE_SENSOR_BIT); // PD7 input
    OBSTACLE_SENSOR_PORT |= (1 << OBSTACLE_SENSOR_BIT); // Kéo lên nội

    SERVO_180_DDR |= (1 << SERVO_180_PIN); // PB2 (D10) là output cho servo 180 độ
    LED_DDR |= (1 << LED_PIN);

    i2c_Init();
    lcd_init();
    lcd_clear();
    uart_init();

    uart_send_string("Da khoi dong... Gui B, F, G, D, E\n");
    lcd_puts("Khoi dong...");
    delay_ms(2000);
    lcd_clear();

    while (1) {
        // Kiểm tra ký tự từ UART
        rx_byte = uart_getchar();
        switch (rx_byte) {
            case 'B':
                buzzer_on();
                delay_ms(2000);
                buzzer_off();
                uart_send_string("Buzzer kich hoat\r\n");
                break;
            case 'F':
                {
                    unsigned char i;
                    for (i = 0; i < 50; i++) // Gửi 50 xung cho servo 180 độ
                        pulse_servo(180, 1);
                    uart_send_string("Servo 180 mo cua\r\n");
                    lcd_clear();
                    lcd_puts("Servo 180 mo");
                    delay_ms(500);
                }
                break;
            case 'G':
                {
                    unsigned char i;
                    while (check_obstacle()) { // PD7 = LOW, có vật cản
                        delay_ms(1000);
                    }
                    for (i = 0; i < 50; i++) // Gửi 50 xung cho servo 180 độ
                        pulse_servo(180, 0);
                    uart_send_string("Servo 180 dong cua\r\n");
                    lcd_clear();
                    lcd_puts("Servo 180 dong");
                    delay_ms(500);
                }
                break;
                case 'D':
                      LED_PORT |= (1 << LED_PIN); // Bật LED (PB4)
                      uart_send_string("LED bat\r\n");
                      lcd_clear();
                      lcd_puts("LED bat");
                      delay_ms(500);
                break;
                case 'E':
                      LED_PORT &= ~(1 << LED_PIN); // Tắt LED (PB4)
                      uart_send_string("LED tat\r\n");
                      lcd_clear();
                      lcd_puts("LED tat");
                      delay_ms(500);
                 break;
            default:
                // Không làm gì nếu ký tự không hợp lệ
                break;
        }

        fire = check_fire();
        if (read_dht11()) {  
            unsigned char i, j;
            lcd_clear();
            lcd_gotoxy(0, 0);
            sprintf(lcd_buf, "Temp: %uC", dht_data[2]);
            lcd_puts(lcd_buf);

            lcd_gotoxy(0, 1);
            sprintf(lcd_buf, "Hum: %u%%", dht_data[0]);
            lcd_puts(lcd_buf);

            if (fire) {
                buzzer_on(); 
                for (i = 0; i < 50; i++) // Gửi 50 xung cho servo 180 độ
                        pulse_servo(180, 1); 
                for (j = 0; j < 6; j++) { // Nhấp nháy 3 lần
                        LED_PORT |= (1 << LED_PIN);   // Bật LED
                         delay_ms(250);
                        LED_PORT &= ~(1 << LED_PIN);  // Tắt LED
                        delay_ms(250);
                }
                
                sprintf(uart_buf, "Nhiet do:%uC,Do am:%u%%,Lua:CO\r\n", dht_data[2], dht_data[0]);
            } else {
                buzzer_off();
                sprintf(uart_buf, "Nhiet do:%uC,Do am:%u%%,Lua:KHONG\r\n", dht_data[2], dht_data[0]);
            }
            uart_send_string(uart_buf);
        } else {
            lcd_clear();
            lcd_gotoxy(0, 0);
            lcd_puts("Loi DHT11");
            uart_send_string("Loi cam bien DHT11\r\n");
        }

        delay_ms(1000);
    }
}