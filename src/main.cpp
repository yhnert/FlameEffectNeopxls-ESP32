#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "NeoPixelModule.h"
#include "ButtonModule.h"

// ButtonModuleClass* Button;
// NeoPixelModuleClass* Pixel;

void ModuleTask(void* pvParams) {
  // ModuleClass* module = (ModuleClass*)pvParams;

  for (;;) {
    // module->RunLoop();
  }
}


extern "C" void app_main() {
  

  printf("main task entering endless loop");
  while (true) {
    vTaskDelay(500);
  }
}
