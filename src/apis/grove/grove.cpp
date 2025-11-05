#include "grove.hpp"

HardwareSerial GroveSerial(1);

void grove_init() {
    GroveSerial.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
}

void grove_serial_loop() {
    if (GroveSerial.available()) {
        GroveSerial.write(1);
    }
}