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
#include "core/co_class.h"
#include "core/base.h"

  #include <iostream>

namespace MyNs
{
  
  namespace IFaces
  {

    struct IInputObject
      : public Boss::Inherit<Boss::IBase>
    {
      BOSS_DECLARE_IFACEID_HEX(0xC3C67A7B)  // CRC32(MyNs.IFaces.IInputObject)

      virtual Boss::RetCode BOSS_CALL Method() = 0;
    };
    
    
    struct IOutputObject
      : public Boss::Inherit<Boss::IBase>
    {
      BOSS_DECLARE_IFACEID_HEX(0xC3C67A7B)  // CRC32(MyNs.IFaces.IInputObject)

      virtual Boss::RetCode BOSS_CALL Method() = 0;
    };
    
    struct ISimpleObject
      : public Boss::Inherit<Boss::IBase>
    {
      BOSS_DECLARE_IFACEID_HEX(0x03119278)  // CRC32(MyNs.IFaces.ISimpleObject)

      virtual Boss::RetCode BOSS_CALL SetObject(IInputObject *obj) = 0;
      virtual Boss::RetCode BOSS_CALL GetObject(IInputObject **obj) = 0;
      virtual Boss::RetCode BOSS_CALL CreateNewObject(IOutputObject **obj) = 0;
    };

  }
  
  namespace Impl
  {
    
    class InputObject
      : public Boss::SimpleCoClass<IFaces::IInputObject>
    {
    public:
      InputObject()
      {
        std::cout << "InputObject" << std::endl;
      }
      ~InputObject()
      {
        std::cout << "~InputObject" << std::endl;
      }
      
    private:
      // IInputObject
      virtual Boss::RetCode BOSS_CALL Method()
      {
        std::cout << "InputObject::Method" << std::endl;
        return Boss::Status::Ok;
      }
    };
    
    class OutputObject
      : public Boss::SimpleCoClass<IFaces::IOutputObject>
    {
    public:
      OutputObject()
      {
        std::cout << "OutputObject" << std::endl;
      }
      ~OutputObject()
      {
        std::cout << "~OutputObject" << std::endl;
      }
      
    private:
      // IOutputObject
      virtual Boss::RetCode BOSS_CALL Method()
      {
        std::cout << "OutputObject::Method" << std::endl;
        return Boss::Status::Ok;
      }
    };
    
    class SimpleObject
      : public Boss::SimpleCoClass<IFaces::ISimpleObject>
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
      Boss::RefObjPtr<IFaces::IInputObject> Obj;
          
      // ISimpleObject
      virtual Boss::RetCode BOSS_CALL SetObject(IFaces::IInputObject *obj)
      {
        if (!obj)
          return Boss::Status::InvalidArgument;
        Obj = obj;
        return Boss::Status::Ok;
      }
      virtual Boss::RetCode BOSS_CALL GetObject(IFaces::IInputObject **obj)
      {
        return Obj.Get() ? Obj.QueryInterface(obj) : Boss::Status::Fail;
      }
      virtual Boss::RetCode BOSS_CALL CreateNewObject(IFaces::IOutputObject **obj)
      {
        try
        {
          auto NewInst = Boss::Base<OutputObject>::Create();
          return NewInst.QueryInterface(obj);
        }
        catch (...)
        {
        }
        return Boss::Status::Fail;
      }
    };
    
  }
  
}

int main()
{
  try
  {
    Boss::RefObjPtr<MyNs::IFaces::ISimpleObject> Obj = Boss::Base<MyNs::Impl::SimpleObject>::Create();
    if (Obj->SetObject(Boss::Base<MyNs::Impl::InputObject>::Create().Get()) != Boss::Status::Ok)
    {
      std::cerr << "failed to set object." << std::endl;
      return -1;
    }
    Boss::RefObjPtr<MyNs::IFaces::IInputObject> In;
    if (Obj->GetObject(In.GetPPtr()) != Boss::Status::Ok)
    {
      std::cerr << "failed to get object." << std::endl;
      return -1;
    }
    In->Method();
    Boss::RefObjPtr<MyNs::IFaces::IOutputObject> Out;
    if (Obj->CreateNewObject(Out.GetPPtr()) != Boss::Status::Ok)
    {
      std::cerr << "failed to create new object." << std::endl;
      return -1;
    }
    Out->Method();
  }
  catch (std::exception const &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
