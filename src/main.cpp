/**
 * @file main.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-04-22
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <app.h>
#include <SPIFFS.h>
#include "hal_unitcam_s3_5mp/hal_unitcam_s3_5mp.h"

void setup()
{
    Serial.begin(115200);
    delay(100); // Give serial time to initialize

    Serial.println("UnitCamS3-5MP starting...");

    spdlog::set_pattern("[%H:%M:%S] [%L] %v");

    APP::SetupCallback_t callback;

    callback.AssetPoolInjection = []()
    {
        if (!SPIFFS.begin(true))
        {
            spdlog::error("SPIFFS mount failed!");
            return;
        }

        File assetFile = SPIFFS.open("/assetpool.bin", "r");
        if (!assetFile)
        {
            spdlog::error("Asset pool file not found!");
            return;
        }

        size_t fileSize = assetFile.size();
        uint8_t *buffer = (uint8_t *)malloc(fileSize);
        if (!buffer)
        {
            spdlog::error("Memory allocation failed!");
            assetFile.close();
            return;
        }

        assetFile.read(buffer, fileSize);
        assetFile.close();

        // Inject the asset
        AssetPool::InjectStaticAsset((StaticAsset_t *)buffer);
    };

    callback.HalInjection = []()
    {
        HAL::Inject(new HAL_UnitCamS3_5MP);
        // Start AP server immediately since camera failed
        delay(2000); // Give time for HAL to initialize
        HAL::StartApServer();
    };

    APP::Setup(callback);
}

void loop()
{
    APP::Loop();
}
