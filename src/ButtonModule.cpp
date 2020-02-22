#include "ButtonModule.h"

ButtonModuleClass::ButtonModuleClass() : ModuleClass("Button") {
  gpio_set_direction(GPIO_NUM_25, GPIO_MODE_INPUT);
  pxl = NeoPixelModuleClass::GetInstance();
}

void ButtonModuleClass::RunLoop() {
  if (gpio_get_level(GPIO_NUM_25) == 0) {
    // ButtonAction();
  }
  vTaskDelay(50 / portTICK_PERIOD_MS);
}

void ButtonModuleClass::ButtonAction() {
  if (pxl->pixelActivated) {
    pxl->DeactivatePixel();
    printf("deactivating pxl");
  } else {
    pxl->ActivatePixel();
    printf("activating pxl");
  }
  vTaskDelay(1000/portTICK_PERIOD_MS);
}