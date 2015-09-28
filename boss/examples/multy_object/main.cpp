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
    
    struct IFace3
      : public Boss::Inherit<IFace1, IFace2>
    {
      BOSS_DECLARE_IFACEID_HEX(0x40CD2593)  // CRC32(MyNs.IFaces.IFace3)

      virtual Boss::RetCode BOSS_CALL Method3() = 0;
    };
    
    struct IFace4
      : public Boss::Inherit<Boss::IBase>
    {
      BOSS_DECLARE_IFACEID_HEX(0xDEA9B030)  // CRC32(MyNs.IFaces.IFace4)

      virtual Boss::RetCode BOSS_CALL Method4() = 0;
    };

  }
  
  namespace Impl
  {
    
    class Face4
      : public Boss::SimpleCoClass<IFaces::IFace4>
    {
    public:
      Face4()
      {
        std::cout << "Face4" << std::endl;
      }
      ~Face4()
      {
        std::cout << "~Face4" << std::endl;
      }
      
    private:
      // IFace4
      virtual Boss::RetCode BOSS_CALL Method4()
      {
        std::cout << "Face4::Method4" << std::endl;
        return Boss::Status::Ok;
      }
    };
    
    class Face_1_2_3_4
      : public Boss::SimpleCoClass<IFaces::IFace3,Face4>
    {
    public:
      Face_1_2_3_4()
      {
        std::cout << "Face_1_2_3_4" << std::endl;
      }
      ~Face_1_2_3_4()
      {
        std::cout << "~Face_1_2_3_4" << std::endl;
      }
      
    private:
      // IFace1
      virtual Boss::RetCode BOSS_CALL Method1()
      {
        std::cout << "Face_1_2_3_4::Method1" << std::endl;
        return Boss::Status::Ok;
      }
      // IFace2
      virtual Boss::RetCode BOSS_CALL Method2()
      {
        std::cout << "Face_1_2_3_4::Method2" << std::endl;
        return Boss::Status::Ok;
      }
      // IFace3
      virtual Boss::RetCode BOSS_CALL Method3()
      {
        std::cout << "Face_1_2_3_4::Method3" << std::endl;
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

void FuncForIFace3(MyNs::IFaces::IFace3 *obj)
{
  if (!obj)
  {
    std::cerr << "No object." << std::endl;
    return;
  }
  if (obj->Method3() != Boss::Status::Ok)
    std::cout << "Failed to call IFace3::Method3" << std::endl;
}

void FuncForIFace4(MyNs::IFaces::IFace4 *obj)
{
  if (!obj)
  {
    std::cerr << "No object." << std::endl;
    return;
  }
  if (obj->Method4() != Boss::Status::Ok)
    std::cout << "Failed to call IFace4::Method4" << std::endl;
}

int main()
{
  try
  {
    auto Obj = Boss::Base<MyNs::Impl::Face_1_2_3_4>::Create();
    FuncForIFace1(Boss::RefObjQIPtr<MyNs::IFaces::IFace1>(Obj).Get());
    FuncForIFace2(Boss::RefObjQIPtr<MyNs::IFaces::IFace2>(Obj).Get());
    FuncForIFace3(Boss::RefObjQIPtr<MyNs::IFaces::IFace3>(Obj).Get());
    FuncForIFace4(Boss::RefObjQIPtr<MyNs::IFaces::IFace4>(Obj).Get());
  }
  catch (std::exception const &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
