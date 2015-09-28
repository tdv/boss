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
#include <stdexcept>

namespace MyNs
{
  
  namespace IFaces
  {

    struct IFace1
      : public Boss::Inherit<Boss::IBase>
    {
      BOSS_DECLARE_IFACEID_HEX(0xAEC344BF)  // CRC32(MyNs.IFaces.IFace1)
    };
    
    
    struct IFace2
      : public Boss::Inherit<Boss::IBase>
    {
      BOSS_DECLARE_IFACEID_HEX(0x37CA1505)  // CRC32(MyNs.IFaces.IFace2)
    };
    
    struct IFace3
      : public Boss::Inherit<Boss::IBase>
    {
      BOSS_DECLARE_IFACEID_HEX(0x40CD2593)  // CRC32(MyNs.IFaces.IFace3)
    };

  }
  
  namespace Impl
  {
    
    class SomeClass
    {
    public:
      SomeClass()
      {
        std::cout << "SomeClass" << std::endl;
      }
      virtual ~SomeClass()
      {
        std::cout << "~SomeClass" << std::endl;
      }
      
      virtual void SomeClassMethod()
      {
        std::cout << "SomeClass::SomeClassMethod" << std::endl;
      }
    };
    
    class Face1
      : public Boss::SimpleCoClass<IFaces::IFace1>
    {
    public:
      Face1()
      {
        std::cout << "Face1" << std::endl;
      }
      ~Face1()
      {
        std::cout << "~Face1" << std::endl;
      }
      void FinalizeConstruct()
      {
        std::cout << "Face1::FinalizeConstruct" << std::endl;
      }
      void BeforeRelease()
      {
        std::cout << "Face1::BeforeRelease" << std::endl;
      }
    };
    
    class Face2
      : public Boss::SimpleCoClass<IFaces::IFace2>
      , public SomeClass
    {
    public:
      Face2()
      {
        std::cout << "Face2" << std::endl;
      }
      ~Face2()
      {
        std::cout << "~Face2" << std::endl;
      }
      void FinalizeConstruct()
      {
        std::cout << "Face2::FinalizeConstruct" << std::endl;
        SomeClassMethod();
      }
      void BeforeRelease()
      {
        std::cout << "Face2::BeforeRelease" << std::endl;
      }
    };
    
    class Face_1_2_3
      : public Boss::SimpleCoClass<IFaces::IFace3, Face1, Face2>
    {
    public:
      Face_1_2_3(bool throwInSomeClassMethod)
        : ThrowInSomeClassMethod(throwInSomeClassMethod)
      {
        std::cout << "Face_1_2_3" << std::endl;
      }
      ~Face_1_2_3()
      {
        std::cout << "~Face_1_2_3" << std::endl;
      }
      void FinalizeConstruct()
      {
        std::cout << "Face_1_2_3::FinalizeConstruct" << std::endl;
      }
      void BeforeRelease()
      {
        std::cout << "Face_1_2_3::BeforeRelease" << std::endl;
      }
      virtual void SomeClassMethod()
      {
        std::cout << "Face_1_2_3::SomeClassMethod" << std::endl;
        if (ThrowInSomeClassMethod)
          throw std::runtime_error("Exception from Face_1_2_3::SomeClassMethod.");
      }
      
    private:
      bool ThrowInSomeClassMethod;
    };
    
  }
  
}

int main()
{
  try
  {
    Boss::Base<MyNs::Impl::Face_1_2_3>::Create(false);
    std::cout << std::endl;
    Boss::Base<MyNs::Impl::Face_1_2_3>::Create(true);
  }
  catch (std::exception const &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
