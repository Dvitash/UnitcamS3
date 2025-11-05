#include <HardwareSerial.h>

extern HardwareSerial GroveSerial;

#define RX_PIN (19)
#define TX_PIN (20)

void grove_init();
void grove_serial_loop();