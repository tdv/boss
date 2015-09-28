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
#include "core/ref_obj_qi_ptr.h"

  #include <iostream>

namespace MyNs
{
  
  namespace IFaces
  {

    struct IFace1
      : public Boss::Inherit<Boss::IBase>
    {
      BOSS_DECLARE_IFACEID_HEX(0xAEC344BF)  // CRC32(MyNs.IFaces.IFace1)

      virtual Boss::RetCode BOSS_CALL Method1() = 0;
    };
    
    
    struct IFace2
      : public Boss::Inherit<Boss::IBase>
    {
      BOSS_DECLARE_IFACEID_HEX(0x37CA1505)  // CRC32(MyNs.IFaces.IFace2)

      virtual Boss::RetCode BOSS_CALL Method2() = 0;
    };

  }
  
  namespace Impl
  {
    
    class Face_1_2
      : public Boss::SimpleCoClass<IFaces::IFace1, IFaces::IFace2>
    {
    public:
      Face_1_2()
      {
        std::cout << "Face_1_2" << std::endl;
      }
      ~Face_1_2()
      {
        std::cout << "~Face_1_2" << std::endl;
      }
      
    private:
      // IFace1
      virtual Boss::RetCode BOSS_CALL Method1()
      {
        std::cout << "Face_1_2::Method1" << std::endl;
        return Boss::Status::Ok;
      }
      // IFace2
      virtual Boss::RetCode BOSS_CALL Method2()
      {
        std::cout << "Face_1_2::Method2" << std::endl;
        return Boss::Status::Ok;
      }
    };

  }
  
}

void FuncForIFace1(MyNs::IFaces::IFace1 *obj)
{
  if (!obj)
  {
    std::cerr << "No object." << std::endl;
    return;
  }
  if (obj->Method1() != Boss::Status::Ok)
    std::cout << "Failed to call IFace1::Method1" << std::endl;
}

void FuncForIFace2(MyNs::IFaces::IFace2 *obj)
{
  if (!obj)
  {
    std::cerr << "No object." << std::endl;
    return;
  }
  if (obj->Method2() != Boss::Status::Ok)
    std::cout << "Failed to call IFace2::Method2" << std::endl;
}

int main()
{
  try
  {
    auto Obj = Boss::Base<MyNs::Impl::Face_1_2>::Create();
    FuncForIFace1(Boss::RefObjQIPtr<MyNs::IFaces::IFace1>(Obj).Get());
    FuncForIFace2(Boss::RefObjQIPtr<MyNs::IFaces::IFace2>(Obj).Get());
  }
  catch (std::exception const &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
