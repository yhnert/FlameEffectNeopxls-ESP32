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

void NeoPixelModuleClass::ApplyEffect(pixelColor_t* pixel)
{
  pixel->r = layer->Color.R;
  pixel->g = layer->Color.G;
  pixel->b = layer->Color.B;
}

void NeoPixelModuleClass::RunLoop() {
  Pixel->pixels[0].r = 0;
  Pixel->pixels[0].g = 0;
  Pixel->pixels[0].b = 0;

  if (pixelActivated) {
    FlameEffect(layer);
    ApplyEffect(&Pixel->pixels[0]);
  }
  digitalLeds_updatePixels(Pixel);

  vTaskDelay(20 / portTICK_RATE_MS);
};

void NeoPixelModuleClass::FlameEffect(NeopixelLayer* layer) {
  uint32_t value = 0;
  value = 30 + (25 * sin(((xTaskGetTickCount() * portTICK_RATE_MS) % 5300) / (5300.0 / (2 * M_PI))));

  layer->Color.G = value * 0.75;
  layer->Color.R = value;
  layer->Color.B = 0;
  layer->Color.Opac = 1;
}
