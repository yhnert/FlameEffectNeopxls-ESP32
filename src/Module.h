#pragma once

#include "esp_log.h"
#include "sys/time.h"

#include <string>

class ModuleClass
{
public:
  ModuleClass(std::string _name);
  virtual void RunLoop() = 0;
  std::string GetName();
  const char *GetNameC();

protected:
  std::string name = "";
};