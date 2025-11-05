/**
 * @file main.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-10-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <mooncake.h>
#include <Arduino.h>
#include "hal/hal.h"
#include "apis/grove/grove.hpp"

#define LOOP_DELAY_MS (50)

void setup()
{
    // Init
    HAL::hal::GetHal()->init();
    grove_init();
}

void loop()
{
    delay(LOOP_DELAY_MS);
    grove_serial_loop();
}
