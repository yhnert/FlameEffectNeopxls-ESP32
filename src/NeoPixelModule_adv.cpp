#include "NeoPixelModule_adv.h"
NeoPixelModuleClassADV::NeoPixelModuleClassADV() : ModuleClass("neopixel") {

  Pixel[0] =
      {
          .rmtChannel = 0,
          .gpioNum = GPIO_NUM_27,
          .ledType = LED_WS2812B_V1,
          .brightLimit = 16,
          .numPixels = 11,
          .pixels = nullptr,
          ._stateVars = nullptr};
  Pixel[1] =
      {
          .rmtChannel = 1,
          .gpioNum = GPIO_NUM_26,
          .ledType = LED_WS2812B_V1,
          .brightLimit = 16,
          .numPixels = 11,
          .pixels = nullptr,
          ._stateVars = nullptr};

  FlamePixels[10].previousPixel = &FlamePixels[9];
  FlamePixels[9].previousPixel = &FlamePixels[8];
  FlamePixels[8].previousPixel = &FlamePixels[7];
  FlamePixels[7].previousPixel = &FlamePixels[6];
  FlamePixels[6].previousPixel = &FlamePixels[5];
  FlamePixels[5].previousPixel = &FlamePixels[4];
  FlamePixels[2].previousPixel = &FlamePixels[3];
  FlamePixels[1].previousPixel = &FlamePixels[2];
  FlamePixels[0].previousPixel = &FlamePixels[1];

  FlamePixels[10].delayFromPrevious = 15;
  FlamePixels[9].delayFromPrevious = 15;
  FlamePixels[8].delayFromPrevious = 15;
  FlamePixels[7].delayFromPrevious = 15;
  FlamePixels[6].delayFromPrevious = 15;
  FlamePixels[5].delayFromPrevious = 15;
  FlamePixels[2].delayFromPrevious = 15;
  FlamePixels[1].delayFromPrevious = 15;
  FlamePixels[0].delayFromPrevious = 15;
  digitalLeds_initStrands(Pixel, 2);
}

NeoPixelModuleClassADV* NeoPixelModuleClassADV::GetInstance() {
  static NeoPixelModuleClassADV* instance = new NeoPixelModuleClassADV();

  return instance;
}

void NeoPixelModuleClassADV::ActivatePixel() {
  pixelActivateTimestamp = xTaskGetTickCount();
  pixelActivated = true;
}

void NeoPixelModuleClassADV::DeactivatePixel() {
  pixelActivated = false;
}

void NeoPixelModuleClassADV::RunLoop() {
  vTaskDelay(10 / portTICK_RATE_MS);
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < Pixel[j].numPixels; i++) {
      Pixel[j].pixels[i].r = 0;
      Pixel[j].pixels[i].g = 0;
      Pixel[j].pixels[i].b = 0;
    }
  }
  if (pixelActivated) {
    CheckTargets();
    RunEffect();
    ApplyEffect();
  }
  digitalLeds_updatePixels(&Pixel[0]);
  digitalLeds_updatePixels(&Pixel[1]);
};

void NeoPixelModuleClassADV::CheckTargets() {
  uint32_t now = xTaskGetTickCount();
  for (int i = 0; i < 11; i++) {
    if (now>FlamePixels[i].delayFromPrevious&&FlamePixels[i].flameTargetTimestamp < now - FlamePixels[i].delayFromPrevious) {
      if (FlamePixels[i].previousPixel == nullptr) {
        uint32_t rand = esp_random();
        uint32_t timeToTarget = (20 + (rand / (UINT32_MAX / 45)));
        FlamePixels[i].oldflameValueTarget = FlamePixels[i].flamevalue;
        FlamePixels[i].flameValueTarget = 25 + (rand / (UINT32_MAX / 75));
        FlamePixels[i].oldflameTargetTimestamp = now;
        FlamePixels[i].flameTargetTimestamp = now + timeToTarget;
      } else {
        FlamePixels[i].oldflameValueTarget = FlamePixels[i].flamevalue;
        FlamePixels[i].flameValueTarget = (FlamePixels[i].previousPixel->flameValueTarget / 1.7f)+2;
        FlamePixels[i].oldflameTargetTimestamp = FlamePixels[i].previousPixel->oldflameTargetTimestamp;
        FlamePixels[i].flameTargetTimestamp = FlamePixels[i].previousPixel->flameTargetTimestamp;
      }
    }
  }
}

void NeoPixelModuleClassADV::RunEffect() {
  uint32_t now = xTaskGetTickCount();
  for (int i = 10; i > -1; i--) {
    float aVal = ((float)FlamePixels[i].flameValueTarget - (float)FlamePixels[i].oldflameValueTarget) / ((float)FlamePixels[i].flameTargetTimestamp - (float)FlamePixels[i].oldflameTargetTimestamp);
    float bVal = (float)FlamePixels[i].flameValueTarget - aVal * (float)FlamePixels[i].flameTargetTimestamp;
    FlamePixels[i].flamevalue = aVal * (now - FlamePixels[i].delayFromPrevious) + bVal;
  }
}

void NeoPixelModuleClassADV::ApplyEffect() {
  for (int i = 0; i < 11; i++) {
    Pixel[0].pixels[i].r = FlamePixels[i].flamevalue;
    Pixel[0].pixels[i].g = FlamePixels[i].flamevalue * 0.48f;
    Pixel[0].pixels[i].b = 0;
    Pixel[1].pixels[10 - i].r = FlamePixels[i].flamevalue;
    Pixel[1].pixels[10 - i].g = FlamePixels[i].flamevalue * 0.48f;
    Pixel[1].pixels[10 - i].b = 0;
  }
}
