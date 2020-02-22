#include "NeoPixelModule.h"
NeoPixelModuleClass::NeoPixelModuleClass() : ModuleClass("neopixel") {

  layer = new NeopixelLayer();

  Pixel[0] =
      {
          .rmtChannel = 0,
          .gpioNum = GPIO_NUM_26,
          .ledType = LED_WS2812B_V1,
          .brightLimit = 16,
          .numPixels = 48,
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
  pixel->r = layer->Color.R*layer->Color.Opac;
  pixel->g = layer->Color.G*layer->Color.Opac;
  pixel->b = layer->Color.B*layer->Color.Opac;
}

void NeoPixelModuleClass::RunLoop() {
  vTaskDelay(10 / portTICK_RATE_MS);
  for (int j = 0; j < 1; j++) {
    for (int i = 0; i < Pixel[j].numPixels; i++) {
      Pixel[j].pixels[i].r = 0;
      Pixel[j].pixels[i].g = 0;
      Pixel[j].pixels[i].b = 0;
    }
  }
  // digitalLeds_updatePixels(Pixel);

  if (pixelActivated) {
    for (int j = 0; j < 1; j++) {
      for (int i = 0; i < Pixel[j].numPixels; i++) {
        FlameEffect(layer);
        // vTaskDelay(10/portTICK_PERIOD_MS);
        ApplyEffect(&(Pixel[j].pixels[i]));
      }
    }
  }
  digitalLeds_updatePixels(Pixel);
};

void NeoPixelModuleClass::FlameEffect(NeopixelLayer* layer) {
  // ESP_LOGI("flame","flame on")
  uint32_t now = xTaskGetTickCount();
  if (flameTargetTimestamp < now) {
    uint32_t timeToTarget = (10 + (esp_random() / (UINT32_MAX / 45)));
    oldflameValueTarget = flameValue;
    flameValueTarget = 25 + (esp_random() / (UINT32_MAX / 75));
    oldflameTargetTimestamp = now;
    flameTargetTimestamp = now + timeToTarget;
    // ESP_LOGI("flame", "time to target %u", timeToTarget);
    // ESP_LOGI("flame", "target %u", flameValueTarget);
  }

  float aVal = ((float)flameValueTarget - (float)oldflameValueTarget) / ((float)flameTargetTimestamp - (float)oldflameTargetTimestamp);
  float bVal = (float)flameValueTarget - aVal * (float)flameTargetTimestamp;
  flameValue = aVal * now + bVal;
  // ESP_LOGI("flame", "\ntargetVal: %i \t oldTargetVal: %i \t now:%u\ntargetTime: %u \t oldTargetTime: %u", flameValueTarget, oldflameValueTarget, now, flameTargetTimestamp, oldflameTargetTimestamp);
  // ESP_LOGI("flame", "aVal: %f", aVal);
  // ESP_LOGI("flame", "value %u", flameValue);

  layer->Color.G = (flameValue * 0.55f + flameValue * (aVal / 30.0f)) / 2.0f;
  layer->Color.R = flameValue;
  layer->Color.B = 0;
  layer->Color.Opac = 1;
}
