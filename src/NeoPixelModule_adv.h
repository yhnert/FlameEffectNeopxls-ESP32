#pragma once
#include "Module.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "math.h"
#include <esp32_digital_led_lib.h>

struct flamepixel{
  flamepixel* previousPixel = nullptr;
  uint32_t delayFromPrevious = 0;
  uint8_t flamevalue = 0;
  uint32_t flameTargetTimestamp = 0;
  uint32_t oldflameTargetTimestamp = 0;
  int32_t flameValueTarget = 0;
  int32_t oldflameValueTarget = 0;
};

class NeoPixelModuleClassADV : public ModuleClass {
public:
  void RunLoop();
  void SetNeoPixelColor(int color);
  static NeoPixelModuleClassADV* GetInstance();
  void ActivatePixel();
  void DeactivatePixel();
  bool pixelActivated = true;

private:
  NeoPixelModuleClassADV();
  flamepixel FlamePixels[11];
  strand_t Pixel[2];
  uint32_t pixelActivateTimestamp = 0;
  void ApplyEffect();
  void RunEffect();
  void CheckTargets();
};
