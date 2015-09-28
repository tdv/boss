//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     03.05.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#include "core/module.h"
#include "core/ibase.h"
#include "core/base.h"
#include "core/co_class.h"
#include "core/ref_obj_ptr.h"
#include "common/string.h"
#include "common/enum.h"
#include "common/string_helper.h"
#include "common/enum_helper.h"

  #include <iostream>

namespace MyNs
{
  
  struct ISimpleObject
    : public Boss::Inherit<Boss::IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0xFACA896C)  // CRC32(MyNs.ISimpleObject)

    virtual Boss::RetCode BOSS_CALL GetStrings(Boss::IEnum **strings) const = 0;
  };

  class SimpleObject
    : public Boss::SimpleCoClass<ISimpleObject>
  {
  public:
    SimpleObject()
    {
      auto StringEnum = Boss::Base<Boss::Enum>::Create();
      
      StringEnum->AddItem(Boss::Base<Boss::String>::Create("String 1"));
      StringEnum->AddItem(Boss::Base<Boss::String>::Create("String 2"));
      StringEnum->AddItem(Boss::Base<Boss::String>::Create("String 3"));
      
      Strings = std::move(StringEnum);
      
      std::cout << "SimpleObject" << std::endl;
    }
    ~SimpleObject()
    {
      std::cout << "~SimpleObject" << std::endl;
    }
    
  private:
    mutable Boss::RefObjPtr<Boss::IEnum> Strings;
        
    // ISimpleObject
    virtual Boss::RetCode BOSS_CALL GetStrings(Boss::IEnum **strings) const
    {
      if (!strings)
        return Boss::Status::InvalidArgument;
      return Strings.QueryInterface(strings);
    }
  };
    
}

int main()
{
  try
  {
    Boss::RefObjPtr<MyNs::ISimpleObject> Obj = Boss::Base<MyNs::SimpleObject>::Create();
    Boss::RefObjPtr<Boss::IEnum> Strings;
    if (Obj->GetStrings(Strings.GetPPtr()) != Boss::Status::Ok)
    {
      std::cerr << "failed to get strings." << std::endl;
      return -1;
    }
    Boss::EnumHelper<Boss::IString> Enum(Strings);
    for (Boss::RefObjPtr<Boss::IString> i = Enum.First() ; i.Get() ; i = Enum.Next())
      std::cout << Boss::StringHelper(i).GetString<Boss::IString::AnsiString>() << std::endl;
  }
  catch (std::exception const &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
