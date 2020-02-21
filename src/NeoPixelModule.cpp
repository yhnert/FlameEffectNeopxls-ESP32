#include "NeoPixelModule.h"
NeoPixelModuleClass::NeoPixelModuleClass() : ModuleClass("neopixel") {

  layer = new NeopixelLayer();

  Pixel[0] =
      {
          .rmtChannel = 1,
          .gpioNum = GPIO_NUM_26,
          .ledType = LED_WS2813_V3,
          .brightLimit = 16,
          .numPixels = 1,
          .pixels = nullptr,
          ._stateVars = nullptr};

  digitalLeds_initStrands(Pixel, 1);
}

NeoPixelModuleClass* NeoPixelModuleClass::GetInstance() {
  static NeoPixelModuleClass* instance = new NeoPixelModuleClass();

  return instance;
}

void NeoPixelModuleClass::ActivatePixel() {
  pixelActivateTimestamp = xTaskGetTickCount();
  pixelActivated = true;
}

void NeoPixelModuleClass::DeactivatePixel() {
  pixelActivated = false;
}

void NeoPixelModuleClass::ApplyEffect(pixelColor_t* pixel) {
  pixel->r = layer->Color.R;
  pixel->g = layer->Color.G;
  pixel->b = layer->Color.B;
}

void NeoPixelModuleClass::RunLoop() {
  vTaskDelay(20 / portTICK_RATE_MS);
  Pixel->pixels[0].r = 0;
  Pixel->pixels[0].g = 0;
  Pixel->pixels[0].b = 0;

  if (pixelActivated) {
    FlameEffect(layer);
    ApplyEffect(&Pixel->pixels[0]);
  }
  digitalLeds_updatePixels(Pixel);
};

void NeoPixelModuleClass::FlameEffect(NeopixelLayer* layer) {
  // ESP_LOGI("flame","flame on")
  uint32_t now = xTaskGetTickCount();
  if (flameTargetTimestamp < now) {
    uint32_t timeToTarget = (100 + (esp_random() / (UINT32_MAX / 600)));
    oldflameValueTarget = flameValue;
    flameValueTarget = 10 + (esp_random() / (UINT32_MAX / 25));
    oldflameTargetTimestamp = now;
    flameTargetTimestamp = now + timeToTarget;
    ESP_LOGI("flame", "time to target %u", timeToTarget);
    ESP_LOGI("flame", "target %u", flameValueTarget);
  } else {

    flameValue = (((now - oldflameTargetTimestamp) * (flameValueTarget - oldflameValueTarget)) / (flameTargetTimestamp - oldflameValueTarget)) + oldflameValueTarget;
    // flameValue = abs(flameTargetTimestamp - now) * oldflameValueTarget + now * flameValueTarget;
    // flameValue = oldflameValueTarget
    //NewValue = (((OldValue - OldMin) * (NewMax - NewMin)) / (OldMax - OldMin)) + NewMin
    ESP_LOGI("flame", "value %u", flameValue);
  }

  // layer->Color.G = flameValue * 0.45; // + (esp_random()/(UINT32_MAX/5));
  // layer->Color.R = flameValue;
  // layer->Color.B = 0;
  layer->Color.Opac = 1;
}
