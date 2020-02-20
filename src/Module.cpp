
#include "Module.h"

ModuleClass::ModuleClass(std::string _name)
{
  name = _name;
}

std::string ModuleClass::GetName()
{
  return name;
}

const char *ModuleClass::GetNameC()
{
  return name.c_str();
}