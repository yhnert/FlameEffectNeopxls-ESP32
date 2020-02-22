#pragma once

#include "Module.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "NeoPixelModule.h"
#include "NeoPixelModule_adv.h"

class ButtonModuleClass : public ModuleClass
{
private:

  void ButtonAction();
  NeoPixelModuleClassADV* pxl;

public:
  ButtonModuleClass();
  void RunLoop();
};