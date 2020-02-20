#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "math.h"
#include "Module.h"
#include <esp32_digital_led_lib.h>

enum layerMode {
  Add_e,
  Normalize_e,
  Overlay_e,
  Absolute_e,
  Brighter_e
};


struct NeopixelColor {
  uint8_t R = 0;
  uint8_t G = 0;
  uint8_t B = 0;
  double Opac = 0.0;
};

struct NeopixelLayer {
  NeopixelColor Color;
  void (*LayerFunc)(NeopixelLayer* layer) = nullptr;
  layerMode Mode = Add_e;
  bool enabled = false;
  uint32_t timestamp = 0;
};

class NeoPixelModuleClass : public ModuleClass {
public:
  void RunLoop();
  void SetNeoPixelColor(int color);
  static NeoPixelModuleClass* GetInstance();
  int ShutDown();
  int StartUp();
  int Status();
  void ActivatePixel();
  void DeactivatePixel();

private:
  uint8_t FindFreeLayer();
  NeoPixelModuleClass();
  static const uint8_t numberOfLayers = 4;
  strand_t Pixel[1];
  NeopixelLayer Layers[numberOfLayers];
  SemaphoreHandle_t layersSemaphore;
  bool pixelActivated = true;
  uint32_t pixelActivateTimestamp = 0;

  void ProcessLayer(pixelColor_t* pixel, uint8_t layerNum);
};
