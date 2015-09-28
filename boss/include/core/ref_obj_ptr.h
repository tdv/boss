//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_CORE_REF_OBJ_PTR_H__
#define __BOSS_CORE_REF_OBJ_PTR_H__

#include "core_types.h"
#include "error_codes.h"
#include "utils.h"

namespace Boss
{

  template <typename T>
  class RefObjQIPtr;

  template <typename T>
  class RefObjPtr final
  {
  public:
    explicit RefObjPtr(T *ptr = nullptr)
      : Ptr(ptr)
    {
      if (Ptr)
        Ptr->AddRef();
    }
    template <typename Other>
    explicit RefObjPtr(T *ptr)
      : Ptr(ptr)
    {
      if (Ptr)
        Ptr->AddRef();
    }
    RefObjPtr(RefObjPtr const &ptr)
      : Ptr(ptr.Ptr)
    {
      if (Ptr)
        Ptr->AddRef();
    }
    template <typename Other>
    RefObjPtr(RefObjPtr<Other> const &ptr)
      : Ptr(ptr.Ptr)
    {
      if (Ptr)
        Ptr->AddRef();
    }
    RefObjPtr(RefObjPtr &&ptr)
      : Ptr(ptr.Ptr)
    {
      ptr.Ptr = 0;
    }
    template <typename Other>
    RefObjPtr(RefObjPtr<Other> &&ptr)
      : Ptr(ptr.Ptr)
    {
      ptr.Ptr = 0;
    }
    RefObjPtr& operator = (T *ptr)
    {
      RefObjPtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    template <typename Other>
    RefObjPtr& operator = (Other *ptr)
    {
      RefObjPtr<Other> Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    RefObjPtr& operator = (RefObjPtr const &ptr)
    {
      RefObjPtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    template <typename Other>
    RefObjPtr& operator = (RefObjPtr<Other> const &ptr)
    {
      RefObjPtr<Other> Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    RefObjPtr& operator = (RefObjPtr &&ptr)
    {
      Release();
      Ptr = ptr.Ptr;
      ptr.Ptr = 0;
      return *this;
    }
    template <typename Other>
    RefObjPtr& operator = (RefObjPtr<Other> &&ptr)
    {
      Release();
      Ptr = ptr.Ptr;
      ptr.Ptr = 0;
      return *this;
    }
    ~RefObjPtr()
    {
      Release();
    }
    UInt AddRef()
    {
      return Ptr ? Ptr->AddRef() : 0;
    }
    UInt Release()
    {
      UInt Ret = 0;
      if (Ptr)
        Ret = Ptr->Release();
      Ptr = 0;
      return Ret;
    }
    template <typename IFace>
    RetCode QueryInterface(IFace **iface)
    {
      return !Ptr ? Status::Fail :
        Ptr->QueryInterface(InterfaceTraits<IFace>::Id,
          reinterpret_cast<Boss::Ptr *>(iface));
    }
    T* Get()
    {
      return Ptr;
    }
    T const* Get() const
    {
      return Ptr;
    }
    T** GetPPtr()
    {
      return &Ptr;
    }
    void Swap(RefObjPtr &ptr)
    {
      T *Tmp = ptr.Ptr;
      ptr.Ptr = Ptr;
      Ptr = Tmp;
    }
    template <typename Other>
    void Swap(RefObjPtr<Other> &ptr)
    {
      auto *Tmp = ptr.Ptr;
      ptr.Ptr = static_cast<decltype(ptr.Ptr)>(Ptr);
      Ptr = Tmp;
    }
    T* operator -> ()
    {
      return Ptr;
    }
    T const* operator -> () const
    {
      return Ptr;
    }

  private:
    template <typename Other>
    friend class RefObjPtr;
    template <typename Other>
    friend class RefObjQIPtr;
    
    T *Ptr;
  };

}

#endif  // !__BOSS_CORE_REF_OBJ_PTR_H__
