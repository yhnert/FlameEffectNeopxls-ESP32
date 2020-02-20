#include "NeoPixelModule.h"
NeoPixelModuleClass::NeoPixelModuleClass() : ModuleClass("neopixel") {

  layersSemaphore = xSemaphoreCreateMutex();

  Pixel[0] =
      {
          .rmtChannel = 1,
          .gpioNum = 12,
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

void NeoPixelModuleClass::RunLoop() {
  if (xSemaphoreTake(layersSemaphore, 1000 / portTICK_PERIOD_MS) == pdTRUE) {
    Pixel->pixels[0].r = 0;
    Pixel->pixels[0].g = 0;
    Pixel->pixels[0].b = 0;

    if (pixelActivated) {
      for (uint8_t i = 0; i < numberOfLayers; i++) {
        if (Layers[i].enabled && Layers[i].LayerFunc != nullptr) {
          Layers[i].LayerFunc(&Layers[i]);
          ProcessLayer(&Pixel->pixels[0], i);
        }
      }
    }
    digitalLeds_updatePixels(Pixel);
    xSemaphoreGive(layersSemaphore);
  }
  vTaskDelay(20 / portTICK_RATE_MS);
};

void NeoPixelModuleClass::ProcessLayer(pixelColor_t* pixel, uint8_t layerNum) {
  uint16_t temp[3] = {0, 0, 0};
  switch (Layers[layerNum].Mode) {
  case Add_e: {
    pixel->r = pixel->r + Layers[layerNum].Color.R * Layers[layerNum].Color.Opac > 254 ? 255 : pixel->r + Layers[layerNum].Color.R * Layers[layerNum].Color.Opac;
    pixel->g = pixel->g + Layers[layerNum].Color.G * Layers[layerNum].Color.Opac > 254 ? 255 : pixel->g + Layers[layerNum].Color.G * Layers[layerNum].Color.Opac;
    pixel->b = pixel->b + Layers[layerNum].Color.B * Layers[layerNum].Color.Opac > 254 ? 255 : pixel->b + Layers[layerNum].Color.B * Layers[layerNum].Color.Opac;
  } break;
  case Normalize_e: {
    temp[0] = pixel->r;
    temp[1] = pixel->g;
    temp[2] = pixel->b;

    temp[0] += Layers[layerNum].Color.R * Layers[layerNum].Color.Opac;
    temp[1] += Layers[layerNum].Color.G * Layers[layerNum].Color.Opac;
    temp[2] += Layers[layerNum].Color.B * Layers[layerNum].Color.Opac;

    pixel->r = temp[0] / (layerNum + 1);
    pixel->g = temp[1] / (layerNum + 1);
    pixel->b = temp[2] / (layerNum + 1);
  } break;
  case Overlay_e: {
    pixel->r = pixel->r > Layers[layerNum].Color.R * Layers[layerNum].Color.Opac ? pixel->r : Layers[layerNum].Color.R * Layers[layerNum].Color.Opac;
    pixel->g = pixel->g > Layers[layerNum].Color.G * Layers[layerNum].Color.Opac ? pixel->g : Layers[layerNum].Color.G * Layers[layerNum].Color.Opac;
    pixel->b = pixel->b > Layers[layerNum].Color.B * Layers[layerNum].Color.Opac ? pixel->b : Layers[layerNum].Color.B * Layers[layerNum].Color.Opac;
  } break;
  case Absolute_e: {
    temp[0] = pixel->r * (1 - Layers[layerNum].Color.Opac);
    temp[1] = pixel->g * (1 - Layers[layerNum].Color.Opac);
    temp[2] = pixel->b * (1 - Layers[layerNum].Color.Opac);

    temp[0] += Layers[layerNum].Color.R * Layers[layerNum].Color.Opac;
    temp[1] += Layers[layerNum].Color.G * Layers[layerNum].Color.Opac;
    temp[2] += Layers[layerNum].Color.B * Layers[layerNum].Color.Opac;

    pixel->r = temp[0] / 2;
    pixel->g = temp[1] / 2;
    pixel->b = temp[2] / 2;
  } break;
  case Brighter_e: {
    pixel->r = pixel->r + pixel->g + pixel->b > (Layers[layerNum].Color.R + Layers[layerNum].Color.G + Layers[layerNum].Color.B) * Layers[layerNum].Color.Opac ? pixel->r : Layers[layerNum].Color.R * Layers[layerNum].Color.Opac;
    pixel->g = pixel->r + pixel->g + pixel->b > (Layers[layerNum].Color.R + Layers[layerNum].Color.G + Layers[layerNum].Color.B) * Layers[layerNum].Color.Opac ? pixel->g : Layers[layerNum].Color.G * Layers[layerNum].Color.Opac;
    pixel->b = pixel->r + pixel->g + pixel->b > (Layers[layerNum].Color.R + Layers[layerNum].Color.G + Layers[layerNum].Color.B) * Layers[layerNum].Color.Opac ? pixel->b : Layers[layerNum].Color.B * Layers[layerNum].Color.Opac;
  } break;
  default:
    break;
  }
}

/*----------------------------------Patterns--------------------------------------------*/
void NeoPixelModuleClass::Empty(NeopixelLayer* layer) {
  layer->Color.R = 0;
  layer->Color.G = 0;
  layer->Color.B = 0;
  layer->Color.Opac = 1;
}

void NeoPixelModuleClass::DeviceRegistration(NeopixelLayer* layer) {
  uint32_t value = 0;

  value = 30 + (25 * sin(((xTaskGetTickCount() * portTICK_RATE_MS) % 5300) / (5300.0 / (2 * M_PI))));

  layer->Color.G = value * 0.75;
  layer->Color.R = value;
  layer->Color.B = 0;
  layer->Color.Opac = 1;
}
void NeoPixelModuleClass::DeviceInitialSetup(NeopixelLayer* layer) {
  uint32_t value = 0;

  value = 30 + (25 * sin(((xTaskGetTickCount() * portTICK_RATE_MS) % 5300) / (5300.0 / (2 * M_PI))));

  layer->Color.R = value;
  layer->Color.G = value;
  layer->Color.B = value;
  layer->Color.Opac = 1;
}
void NeoPixelModuleClass::DeviceOn(NeopixelLayer* layer) {
  uint32_t value = 0;

  value = 30 + (25 * sin(((xTaskGetTickCount() * portTICK_RATE_MS) % 5300) / (5300.0 / (2 * M_PI))));

  layer->Color.R = value * 0.22;
  layer->Color.G = value;
  layer->Color.B = value * 0.40;
  layer->Color.Opac = 1;
}

void NeoPixelModuleClass::BatteryLow(NeopixelLayer* layer) {
  uint32_t frame = (xTaskGetTickCount() * portTICK_RATE_MS);
  uint32_t value = 0;

  value = abs(abs(((xTaskGetTickCount() * portTICK_RATE_MS) % 2300) - 1150) - 200);

  layer->Color.R = 255;
  layer->Color.G = 0;
  layer->Color.B = 0;
  layer->Color.Opac = value < 75 ? 1 : 0;
  layer->enabled = frame - layer->timestamp < 6900 ? true : false;
}
void NeoPixelModuleClass::BatteryCharging(NeopixelLayer* layer) {
  uint32_t frame = (xTaskGetTickCount() * portTICK_RATE_MS);
  uint32_t value = 0;

  value = abs(abs(((xTaskGetTickCount() * portTICK_RATE_MS) % 2300) - 1150) - 200);

  layer->Color.R = 0;
  layer->Color.G = 0;
  layer->Color.B = 255;
  layer->Color.Opac = value < 75 ? 1 : 0;
  layer->enabled = frame - layer->timestamp < 13800 ? true : false;
}
void NeoPixelModuleClass::BatteryFull(NeopixelLayer* layer) {
  uint32_t frame = (xTaskGetTickCount() * portTICK_RATE_MS);
  uint32_t value = 0;

  value = 30 + (25 * sin(((xTaskGetTickCount() * portTICK_RATE_MS) % 2300) / (2300.0 / (2 * M_PI))));

  layer->Color.R = 0;
  layer->Color.G = value;
  layer->Color.B = 0;
  layer->Color.Opac = 1;
  layer->enabled = frame - layer->timestamp < 6900 ? true : false;
}
void NeoPixelModuleClass::BatteryOK(NeopixelLayer* layer) {
  uint32_t frame = (xTaskGetTickCount() * portTICK_RATE_MS);
  layer->Color.R = 0;
  layer->Color.G = 255;
  layer->Color.B = 0;
  layer->Color.Opac = 1;
  layer->enabled = frame - layer->timestamp < 6900 ? true : false;
}

void NeoPixelModuleClass::AWSConnectionLost(NeopixelLayer* layer) {
  uint32_t frame = (xTaskGetTickCount() * portTICK_RATE_MS);
  uint32_t value = 0;

  value = abs(((xTaskGetTickCount() * portTICK_RATE_MS) % 1100) - 550);

  layer->Color.R = 255;
  layer->Color.G = 120;
  layer->Color.B = 0;
  layer->Color.Opac = value < 75 ? 1 : 0;
  layer->enabled = frame - layer->timestamp < 9900 ? true : false;
}

void NeoPixelModuleClass::GPSLocationSent(NeopixelLayer* layer) {
  uint32_t frame = (xTaskGetTickCount() * portTICK_RATE_MS);

  layer->Color.R = 255;
  layer->Color.G = 255;
  layer->Color.B = 255;
  layer->Color.Opac = frame % 23 < 13 ? 1 : 0;
  layer->enabled = frame - layer->timestamp < 3000 ? true : false;
}

void NeoPixelModuleClass::FallAlarmSent(NeopixelLayer* layer) {
  uint32_t frame = (xTaskGetTickCount() * portTICK_RATE_MS);

  uint32_t value = 60 + (45 * sin(frame / (1000 / (2 * M_PI))));

  layer->Color.R = frame % 2000 < 1000 ? value : 0;
  layer->Color.G = 0;
  layer->Color.B = frame % 2000 >= 1000 ? value : 0;
  layer->Color.Opac = 1;
  layer->enabled = frame - layer->timestamp < 10000 ? true : false;
}

int NeoPixelModuleClass::ShutDown() {

  return 0;
}
int NeoPixelModuleClass::StartUp() {
  return 0;
}
int NeoPixelModuleClass::Status() {
  return 0;
}