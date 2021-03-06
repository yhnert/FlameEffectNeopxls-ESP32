#pragma once
#include "Module.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "math.h"
#include <esp32_digital_led_lib.h>

struct NeopixelColor {
  uint8_t R = 0;
  uint8_t G = 0;
  uint8_t B = 0;
  double Opac = 0.0;
};

struct NeopixelLayer {
  NeopixelColor Color;
  void (*LayerFunc)(NeopixelLayer* layer) = nullptr;
  bool enabled = false;
  uint32_t timestamp = 0;
};

class NeoPixelModuleClass : public ModuleClass {
public:
  void RunLoop();
  void SetNeoPixelColor(int color);
  static NeoPixelModuleClass* GetInstance();
  void ActivatePixel();
  void DeactivatePixel();
  bool pixelActivated = true;

private:
  NeoPixelModuleClass();
  strand_t Pixel[2];
  uint32_t pixelActivateTimestamp = 0;
  uint32_t flameTargetTimestamp = 0;
  uint32_t oldflameTargetTimestamp = 0;
  int32_t flameValueTarget = 0;
  int32_t oldflameValueTarget = 0;
  int32_t flameValue = 0;
  NeopixelLayer* layer;
  void ApplyEffect(pixelColor_t* pixel);
  void FlameEffect(NeopixelLayer* layer);
  //right eye
  uint8_t bottomLeftPxlR = 1;
  uint8_t bottomRightPxlR = 24;
  //left eye
  uint8_t bottomLeftPxlL = 1;
  uint8_t bottomRightPxlL = 24;
};
