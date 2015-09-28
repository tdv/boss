//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     03.05.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#include "ifaces.h"
#include "class_ids.h"

#include "plugin/loader.h"

  #include <iostream>

int main()
{
  try
  {
    Boss::Loader Ldr("Registry.xml", "./" MAKE_MODULE_NAME("service_registry"),
                     "./" MAKE_MODULE_NAME("class_factory"));
    auto Obj1 = Boss::CreateObject<MyNs::IFace1>(MyNs::Service::Id::Face1);
    if (Obj1->Method() != Boss::Status::Ok)
      std::cerr << "Failed to call IFace1::Method" << std::endl;
    auto Obj2 = Boss::CreateObject<MyNs::IFace2>(MyNs::Service::Id::Face2);
    if (Obj2->Method() != Boss::Status::Ok)
      std::cerr << "Failed to call IFace2::Method" << std::endl;
  }
  catch (std::exception const &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
