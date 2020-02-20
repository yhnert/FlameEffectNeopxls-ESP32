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
  strand_t Pixel[1];
  uint32_t pixelActivateTimestamp = 0;
  NeopixelLayer* layer;
  void ApplyEffect(pixelColor_t* pixel);
  void FlameEffect(NeopixelLayer* layer);
};
