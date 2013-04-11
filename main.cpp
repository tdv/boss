  #include <iostream>

#include "plugin/loader.h"

int main()
{
  try
  {
    Boss::Loader Ldr("Registry.xml", "./libservice_registry.so", "./libclass_factory.so");
    Boss::RefObjQIPtr<Boss::IBase> SR;
    SR = Ldr.CreateObject<Boss::IBase>(111);
  }
  catch (std::exception const &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
