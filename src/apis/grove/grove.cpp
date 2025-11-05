#include "grove.hpp"
#include <esp_camera.h>
#include "../../hal/hal.h"

HardwareSerial GroveSerial(1);

void grove_init()
{
    GroveSerial.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
}

void capture_and_send_image()
{
    // Check if camera is available
    if (!HAL::hal::GetHal()->isCameraValid())
    {
        // send camera not available error
        GroveSerial.write(0xFF);
        GroveSerial.write(0x01);
        return;
    }

    camera_fb_t *fb = esp_camera_fb_get();
    if (fb == NULL)
    {
        // send error indicator
        GroveSerial.write(0xFF);
        GroveSerial.write(0x00);
        return;
    }

    // send start of frame marker
    GroveSerial.write(0xFF);
    GroveSerial.write(0xAA);

    // send image size (4 bytes, little endian)
    uint32_t img_size = fb->len;
    GroveSerial.write((uint8_t *)&img_size, 4);

    // send image data
    GroveSerial.write(fb->buf, fb->len);

    // send end of frame marker
    GroveSerial.write(0xFF);
    GroveSerial.write(0xBB);

    esp_camera_fb_return(fb);
}

void grove_serial_loop(){
    if (GroveSerial.available())
    {
        uint8_t cmd = GroveSerial.read();
        if (cmd == 0x01)
        { // capture command
            capture_and_send_image();
        }
        else
        {
            // echo back any other data for debugging
            GroveSerial.write(cmd);
        }
    }
}