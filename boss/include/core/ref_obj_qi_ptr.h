//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_CORE_REF_OBJ_QI_PTR_H__
#define __BOSS_CORE_REF_OBJ_QI_PTR_H__

#include "ref_obj_ptr.h"

namespace Boss
{

  template <typename T>
  class RefObjQIPtr final
  {
  public:
    explicit RefObjQIPtr(T *ptr = 0)
      : Ptr(ptr)
    {
      if (Ptr)
        Ptr->AddRef();
    }
    template <typename Other>
    explicit RefObjQIPtr(Other *ptr)
      : Ptr(0)
    {
      if (ptr)
        ptr->QueryInterface(InterfaceTraits<T>::Id, reinterpret_cast<Boss::Ptr *>(&Ptr));
    }
    RefObjQIPtr(RefObjQIPtr const &ptr)
      : Ptr(ptr.Ptr)
    {
      if (Ptr)
        Ptr->AddRef();
    }
    template <typename Other>
    RefObjQIPtr(RefObjQIPtr<Other> const &ptr)
      : Ptr(0)
    {
      if (ptr.Ptr)
        const_cast<RefObjQIPtr<Other> &>(ptr).QueryInterface(&Ptr);
    }
    RefObjQIPtr(RefObjQIPtr &&ptr)
      : Ptr(ptr.Ptr)
    {
      ptr.Ptr = 0;
    }
    template <typename Other>
    RefObjQIPtr(RefObjQIPtr<Other> &&ptr)
      : Ptr(0)
    {
      if (ptr.Ptr)
        ptr.QueryInterface(&Ptr);
      ptr.Release();
    }
    RefObjQIPtr(RefObjPtr<T> const &ptr)
      : Ptr(ptr.Ptr)
    {
      if (Ptr)
        Ptr->AddRef();
    }
    template <typename Other>
    RefObjQIPtr(RefObjPtr<Other> const &ptr)
      : Ptr(0)
    {
      if (ptr.Get())
        const_cast<RefObjPtr<Other> &>(ptr).QueryInterface(&Ptr);
    }
    RefObjQIPtr(RefObjPtr<T> &&ptr)
      : Ptr(ptr.Ptr)
    {
      ptr.Ptr = 0;
    }
    template <typename Other>
    RefObjQIPtr(RefObjPtr<Other> &&ptr)
      : Ptr(0)
    {
      if (ptr.Ptr)
        ptr.QueryInterface(&Ptr);
      ptr.Release();
    }
    RefObjQIPtr& operator = (T *ptr)
    {
      RefObjQIPtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    template <typename Other>
    RefObjQIPtr& operator = (Other *ptr)
    {
      RefObjQIPtr<T> Tmp;
      if (ptr)
        ptr->QueryInterface(InterfaceTraits<T>::Id, reinterpret_cast<Boss::Ptr *>(Tmp.GetPPtr()));
      Swap(Tmp);
      return *this;
    }
    RefObjQIPtr& operator = (RefObjQIPtr const &ptr)
    {
      RefObjQIPtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    template <typename Other>
    RefObjQIPtr& operator = (RefObjQIPtr<Other> const &ptr)
    {
      RefObjQIPtr<T> Tmp;
      if (ptr.Ptr)
        ptr.QueryInterface(Tmp.GetPPtr());
      Swap(Tmp);
      return *this;
    }
    RefObjQIPtr& operator = (RefObjQIPtr &&ptr)
    {
      Release();
      Ptr = ptr.Ptr;
      ptr.Ptr = 0;
      return *this;
    }
    template <typename Other>
    RefObjQIPtr& operator = (RefObjQIPtr<Other> &&ptr)
    {
      RefObjQIPtr<T> Tmp;
      if (ptr.Ptr)
        ptr.QueryInterface(Tmp.GetPPtr());
      Swap(Tmp);
      ptr.Release();
      return *this;
    }
    RefObjQIPtr& operator = (RefObjPtr<T> const &ptr)
    {
      RefObjQIPtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    template <typename Other>
    RefObjQIPtr& operator = (RefObjPtr<Other> &&ptr)
    {
      RefObjQIPtr<T> Tmp;
      if (ptr.Ptr)
        ptr.QueryInterface(Tmp.GetPPtr());
      Swap(Tmp);
      ptr.Release();
      return *this;
    }
    ~RefObjQIPtr()
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
    void Swap(RefObjQIPtr &ptr)
    {
      T *Tmp = ptr.Ptr;
      ptr.Ptr = Ptr;
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
    operator RefObjPtr<T> () const
    {
      return std::move(RefObjPtr<T>(Ptr));
    }

  private:
    template <typename Other>
    friend class RefObjQIPtr;
    T *Ptr;
  };

}

#endif  // !__BOSS_CORE_REF_OBJ_QI_PTR_H__
