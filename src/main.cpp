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

#define LOOP_HZ (400)

void setup()
{
    // Init
    HAL::hal::GetHal()->init();
}

void loop()
{
    delay(1000 / LOOP_HZ);
    grove_serial_loop();
}
