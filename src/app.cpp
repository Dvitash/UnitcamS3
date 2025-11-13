/**
 * @file app.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-04-21
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "app.h"
#include <mooncake.h>

namespace APP
{
    // Global Mooncake instance
    static MOONCAKE::Mooncake *_mooncake = nullptr;

    void Setup(SetupCallback_t callback)
    {
        // Create Mooncake instance
        _mooncake = new MOONCAKE::Mooncake();

        // Initialize Mooncake
        _mooncake->init();

        // Call injection callbacks
        if (callback.sharedDataInjection)
            callback.sharedDataInjection();

        if (callback.AssetPoolInjection)
            callback.AssetPoolInjection();

        if (callback.HalInjection)
            callback.HalInjection();
    }

    void Loop()
    {
        if (_mooncake)
        {
            _mooncake->update();
        }
    }

    void Destroy()
    {
        if (_mooncake)
        {
            delete _mooncake;
            _mooncake = nullptr;
        }
    }
} // namespace APP
