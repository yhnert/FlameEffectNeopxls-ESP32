#pragma once

#include "Module.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "NeoPixelModule.h"

class ButtonModuleClass : public ModuleClass
{
private:

  void ButtonAction();
  NeoPixelModuleClass* pxl;

public:
  ButtonModuleClass();
  void RunLoop();
};