#include <Arduino.h>
#include <HardwareSerial.h>

class LoraSerialClass {
    public:
        void begin(int txPin, int rxPin) {
            _serial.begin(9600, SERIAL_8N1, rxPin, txPin);
        }

        void begin(int txPin, int rxPin, long baud) {
            _serial.begin(baud, SERIAL_8N1, rxPin, txPin);
        }

        void setMode(int m0pin, int m1pin, bool m0, bool m1) {
            pinMode(m0pin, OUTPUT);
            pinMode(m1pin, OUTPUT);
            digitalWrite(m0pin, m0 ? HIGH : LOW);
            digitalWrite(m1pin, m1 ? HIGH : LOW);
        }
        void println(const char* msg) {
            _serial.println(msg);
        }

        void println(const String& msg) {
            _serial.println(msg);
        }

        void println(int msg) {
            _serial.println(String(msg));
        }

        void println(unsigned int msg) {
            _serial.println(String(msg));
        }

        void println(long msg) {
            _serial.println(String(msg));
        }

        void println(unsigned long msg) {
            _serial.println(String(msg));
        }

        void println(float msg) {
            _serial.println(String(msg, 2));
        }

        void println(double msg) {
            _serial.println(String(msg, 2));
        }

        void print(const char* msg) {
            _serial.print(msg);
        }

        void print(const String& msg) {
            _serial.print(msg);
        }

        void print(int msg) {
            _serial.print(String(msg));
        }

        void print(unsigned int msg) {
            _serial.print(String(msg));
        }

        void print(long msg) {
            _serial.print(String(msg));
        }

        void print(unsigned long msg) {
            _serial.print(String(msg));
        }

        void print(float msg) {
            _serial.print(String(msg, 2));
        }

        void print(double msg) {
            _serial.print(String(msg, 2));
        }

        int available() {
            return _serial.available();
        }

        int read() {
            return _serial.read();
        }

        String readStringUntil(char terminator) {
            return _serial.readStringUntil(terminator);
        }

    private:
        HardwareSerial _serial = HardwareSerial(1);
};