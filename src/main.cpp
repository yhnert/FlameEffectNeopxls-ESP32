#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "NeoPixelModule.h"
#include "ButtonModule.h"

ButtonModuleClass* Button;
NeoPixelModuleClass* Pixel;

void ModuleTask(void* pvParams) {
  ModuleClass* module = (ModuleClass*)pvParams;

  for (;;) {
    module->RunLoop();
  }
}


extern "C" void app_main() {
  Pixel = NeoPixelModuleClass::GetInstance();
  Button = new ButtonModuleClass();

  xTaskCreate(ModuleTask,Pixel->GetNameC(),15000,Pixel,15,NULL);
  xTaskCreate(ModuleTask,Button->GetNameC(),5000,Button,10,NULL);

  printf("main task entering endless loop");
  while (true) {
    vTaskDelay(500);
  }
}
