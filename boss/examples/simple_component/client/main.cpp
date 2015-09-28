//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#include "isum.h"
#include "class_ids.h"

#include "plugin/loader.h"

  #include <iostream>

int main()
{
  try
  {
    Boss::Loader Ldr("Registry.xml", "./" MAKE_MODULE_NAME("service_registry"),
                     "./" MAKE_MODULE_NAME("class_factory"));
    auto Obj = Boss::CreateObject<MyNs::ISum>(MyNs::Service::Id::Sum);
    int Res = 0;
    if (Obj->CalcSum(10, 20, &Res))
      std::cout << "Failed to calc sum." << std::endl;
    std::cout << "Sum: " << Res << std::endl;
  }
  catch (std::exception const &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
