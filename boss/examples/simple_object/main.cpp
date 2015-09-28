//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     03.05.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#include "core/ibase.h"
#include "core/co_class.h"
#include "core/base.h"
#include "core/ref_obj_ptr.h"
#include "core/module.h"

  #include <iostream>

namespace MyNs
{
  
  struct ISimpleObject
    : public Boss::Inherit<Boss::IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0xFACA896C)  // CRC32(MyNs.ISimpleObject)

    virtual Boss::RetCode BOSS_CALL HelloWorld() = 0;
  };
    
    
  
  class SimpleObject
    : public Boss::SimpleCoClass<ISimpleObject>
  {
  public:
    SimpleObject()
    {
      std::cout << "SimpleObject" << std::endl;
    }
    ~SimpleObject()
    {
      std::cout << "~SimpleObject" << std::endl;
    }
    
  private:
    // ISimpleObject
    virtual Boss::RetCode BOSS_CALL HelloWorld()
    {
      std::cout << "BOSS. Hello World!" << std::endl;
      return Boss::Status::Ok;
    }
  };

}

int main()
{
  try
  {
    Boss::RefObjPtr<MyNs::ISimpleObject> Inst = Boss::Base<MyNs::SimpleObject>::Create();
    Inst->HelloWorld();
  }
  catch (std::exception const &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
