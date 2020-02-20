#include "ButtonModule.h"

ButtonModuleClass::ButtonModuleClass() : ModuleClass("Button") {
  gpio_set_direction(GPIO_NUM_27, GPIO_MODE_INPUT);
}

void ButtonModuleClass::RunLoop() {
  if (gpio_get_level(GPIO_NUM_27) == 0) {
    ButtonAction();
  }
  vTaskDelay(50 / portTICK_PERIOD_MS);
}

void ButtonModuleClass::ButtonAction() {
}