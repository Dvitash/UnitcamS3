/**
 * @file hal_camera.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-08-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "../hal_unitcam_s3_5mp.h"
#include "../hal_config.h"
#include <mooncake.h>
#include <esp_camera.h>
#include <Arduino.h> // For ESP.getPsramSize()

void HAL_UnitCamS3_5MP::_camera_init()
{
    spdlog::info("camera init");

    const bool psram_available = psramFound();
    if (psram_available)
    {
        spdlog::info("PSRAM available: {} bytes - camera will use PSRAM for frame buffers", ESP.getPsramSize());
    }
    else
    {
        spdlog::warn("PSRAM not detected, falling back to DRAM (reduced frame size)");
        spdlog::warn("If you expect PSRAM, double-check board variant and efuses");
    }

    camera_config_t config = {};
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = CAMERA_PIN_D0;
    config.pin_d1 = CAMERA_PIN_D1;
    config.pin_d2 = CAMERA_PIN_D2;
    config.pin_d3 = CAMERA_PIN_D3;
    config.pin_d4 = CAMERA_PIN_D4;
    config.pin_d5 = CAMERA_PIN_D5;
    config.pin_d6 = CAMERA_PIN_D6;
    config.pin_d7 = CAMERA_PIN_D7;
    config.pin_xclk = CAMERA_PIN_XCLK;
    config.pin_pclk = CAMERA_PIN_PCLK;
    config.pin_vsync = CAMERA_PIN_VSYNC;
    config.pin_href = CAMERA_PIN_HREF;
    config.pin_sccb_sda = CAMERA_PIN_SIOD; // Fixed: use new API
    config.pin_sccb_scl = CAMERA_PIN_SIOC; // Fixed: use new API
    config.pin_pwdn = CAMERA_PIN_PWDN;
    config.pin_reset = CAMERA_PIN_RESET;
    config.xclk_freq_hz = XCLK_FREQ_HZ;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = psram_available ? FRAMESIZE_VGA : FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = psram_available ? 2 : 1;
    config.fb_location = psram_available ? CAMERA_FB_IN_PSRAM : CAMERA_FB_IN_DRAM;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;

    spdlog::info("camera config - SDA:{}, SCL:{}, RESET:{}, PWDN:{}, XCLK:{}",
                 config.pin_sccb_sda, config.pin_sccb_scl, config.pin_reset,
                 config.pin_pwdn, config.pin_xclk);

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        spdlog::error("camera init failed with error: {} (0x{:x})", esp_err_to_name(err), (int)err);

        // Provide more specific error diagnosis
        switch (err)
        {
        case ESP_ERR_NOT_FOUND:
            spdlog::error("Camera sensor not detected. Check:");
            spdlog::error("  - Camera module physically connected");
            spdlog::error("  - Camera power supply (check pins 3.3V/GND)");
            spdlog::error("  - I2C pull-up resistors on SDA/SCL lines");
            spdlog::error("  - Correct camera sensor type (OV2640/OV3660)");
            spdlog::error("  - UnitCamS3-5MP should have PSRAM - check board variant");
            break;
        case ESP_ERR_INVALID_ARG:
            spdlog::error("Invalid camera configuration parameters");
            break;
        case ESP_ERR_NO_MEM:
            spdlog::error("Not enough memory for camera initialization");
            break;
        default:
            spdlog::error("Unknown camera initialization error");
        }

        _camera_ready = false;
        return;
    }

    // Get camera sensor info
    sensor_t *s = esp_camera_sensor_get();
    if (s)
    {
        spdlog::info("camera sensor detected: PID=0x{:x}, VER=0x{:x}, MIDL=0x{:x}, MIDH=0x{:x}",
                     s->id.PID, s->id.VER, s->id.MIDL, s->id.MIDH);
    }
    else
    {
        spdlog::error("could not get camera sensor info");
    }

    _camera_ready = true;
    spdlog::info("camera init successful");

    // xQueueFrameO = frame_o;
    // xTaskCreatePinnedToCore(task_process_handler, TAG, 3 * 1024, NULL, 5, NULL, 1);
}
