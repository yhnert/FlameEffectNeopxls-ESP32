#pragma once

#include "Module.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class ButtonModuleClass : public ModuleClass
{
private:

  void ButtonAction();

public:
  ButtonModuleClass();
  void RunLoop();
  int ShutDown();
  int StartUp();
  int Status();
};