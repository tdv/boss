//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_CORE_BASE_H__
#define __BOSS_CORE_BASE_H__

#include "ibase.h"
#include "ref_obj_ptr.h"
#include "utils.h"
#include "error_codes.h"
#include "co_class.h"
#include "private/module_ref_counter.h"

#include <atomic>
#include <thread>
#include <utility>
#include <type_traits>

namespace Boss
{

  namespace Private
  {

    enum { EndNegIntIter = static_cast<std::size_t>(-1) };
    
    template <typename T>
    class HasBeforeRelease
    {
    private:
      typedef char (&No)[1];
      typedef char (&Yes)[10];
      template <typename U, void (U::*)() = &U::BeforeRelease>
      struct CheckMtd
      {
        typedef Yes Type;
      };
      template <typename U>
      static typename CheckMtd<U>::Type Check(U const *);
      static No Check(...);
    public:
      enum { Has = sizeof(Check(static_cast<T const *>(0))) == sizeof(Yes) };
    };

    template <bool HasBeforeRelease>
    struct CallBeforeRelease
    {
      template <typename ObjType>
      static void Call(ObjType *obj)
      {
        if (GetConstructedFlag(obj))
          obj->BeforeRelease();
        SetConstructedFlag(obj, false);
      }
    };

    template <>
    struct CallBeforeRelease<false>
    {
      template <typename ObjType>
      static void Call(ObjType *obj)
      {
        SetConstructedFlag(obj, false);
      }
    };

    template <typename T>
    class HasFinalizeConstruct
    {
    private:
      typedef char (&No)[1];
      typedef char (&Yes)[10];
      template <typename U, void (U::*)() = &U::FinalizeConstruct>
      struct CheckMtd
      {
        typedef Yes Type;
      };
      template <typename U>
      static typename CheckMtd<U>::Type Check(U const *);
      static No Check(...);
    public:
      enum { Has = sizeof(Check(static_cast<T const *>(0))) == sizeof(Yes) };
    };

    template <bool HasFinalizeConstruct>
    struct CallFinalizeConstruct
    {
      template <typename ObjType>
      static void Call(ObjType *obj)
      {
        obj->FinalizeConstruct();
        SetConstructedFlag(obj, true);
      }
    };

    template <>
    struct CallFinalizeConstruct<false>
    {
      template <typename ObjType>
      static void Call(ObjType *obj)
      {
        SetConstructedFlag(obj, true);
      }
    };

    template
    <
      typename T,
      bool IsCoClass = std::is_base_of<CoClassBase, T>::value
    >
    struct BeforeRelease
    {
      template <typename ObjType>
      static void Release(ObjType *obj)
      {
      }
    };

    template <typename T, std::size_t I>
    struct BeforeReleaseIter
    {
      template <typename ObjType>
      static void Release(ObjType *obj)
      {
        typedef typename std::tuple_element<I, T>::type CurType;
        BeforeRelease<CurType>::Release(static_cast<CurType *>(obj));
        BeforeReleaseIter<T, I - 1>::Release(obj);
      }
    };

    template <typename T>
    struct BeforeReleaseIter<T, EndNegIntIter>
    {
      template <typename ObjType>
      static void Release(ObjType *)
      {
      }
    };

    template <typename T>
    struct BeforeRelease<T, true>
    {
      template <typename ObjType>
      static void Release(ObjType *obj)
      {
        typedef typename T::BaseEntities BaseEntities;
        enum { BaseEntityCount = std::tuple_size<BaseEntities>::value - 1 };
        CallBeforeRelease<HasBeforeRelease<T>::Has>::Call(obj);
        BeforeReleaseIter<BaseEntities, BaseEntityCount>::Release(obj);
      }
    };

    template
    <
      typename T,
      bool IsCoClass = std::is_base_of<CoClassBase, T>::value
    >
    struct FinalizeConstruct
    {
      template <typename ObjType>
      static void Construct(ObjType *)
      {
      }
    };

    template <typename T, std::size_t I>
    struct FinalizeConstructIter
    {
      template <typename ObjType>
      static void Construct(ObjType *obj)
      {
        typedef typename std::tuple_element<I, T>::type CurType;
        FinalizeConstructIter<T, I - 1>::Construct(obj);
        FinalizeConstruct<CurType>::Construct(static_cast<CurType *>(obj));
      }
    };

    template <typename T>
    struct FinalizeConstructIter<T, EndNegIntIter>
    {
      template <typename ObjType>
      static void Construct(ObjType *)
      {
      }
    };

    template <typename T>
    struct FinalizeConstruct<T, true>
    {
      template <typename ObjType>
      static void Construct(ObjType *obj)
      {
        typedef typename T::BaseEntities BaseEntities;
        enum { BaseEntityCount = std::tuple_size<BaseEntities>::value - 1 };
        FinalizeConstructIter<BaseEntities, BaseEntityCount>::Construct(obj);
        CallFinalizeConstruct<HasFinalizeConstruct<T>::Has>::Call(obj);
      }
    };

    template <typename T, bool IsImpl>
    struct QueryInterface;

    template <typename T, std::size_t I>
    struct QueryInterfacesListIter
    {
      template <typename ObjType>
      static RetCode Query(ObjType *obj, InterfaceId ifaceId, Ptr *iface)
      {
        typedef typename std::tuple_element<I, T>::type CurType;
        if (ifaceId == InterfaceTraits<CurType>::Id)
        {
          *iface = static_cast<CurType *>(obj);
          return Status::Ok;
        }
        return QueryInterfacesListIter<T, I - 1>::Query(obj, ifaceId, iface) == Status::Ok ?
          Status::Ok : QueryInterface<CurType, false>::Query(obj, ifaceId, iface);
      }
    };

    template <typename T>
    struct QueryInterfacesListIter<T, EndNegIntIter>
    {
      template <typename ObjType>
      static RetCode Query(ObjType *, InterfaceId, Ptr *)
      {
        return Status::NotFound;
      }
    };

    template <typename T>
    struct QueryFromInterfacesList
    {
      template <typename ObjType>
      static RetCode Query(ObjType *obj, InterfaceId ifaceId, Ptr *iface)
      {
        typedef typename T::BaseInterfaces BaseInterfaces;
        enum { BaseInterfaceCount = std::tuple_size<BaseInterfaces>::value - 1 };
        return QueryInterfacesListIter<BaseInterfaces, BaseInterfaceCount>::Query(obj, ifaceId, iface);
      }
    };

    template <>
    struct QueryFromInterfacesList<IBase>
    {
      template <typename ObjType>
      static RetCode Query(ObjType *obj, InterfaceId ifaceId, Ptr *iface)
      {
        if (ifaceId == InterfaceTraits<IBase>::Id)
        {
          *iface = static_cast<IBase *>(obj);
          return Status::Ok;
        }
        return Status::NotFound;
      }
    };

    template
    <
      typename T,
      bool IsCoClass = std::is_base_of<CoClassBase, T>::value
    >
    struct QueryInterface
    {
      template <typename ObjType>
      static RetCode Query(ObjType *obj, InterfaceId ifaceId, Ptr *iface)
      {
        if (ifaceId == InterfaceTraits<T>::Id)
        {
          *iface = static_cast<T *>(obj);
          return Status::Ok;
        }
        return QueryFromInterfacesList<T>::Query(static_cast<T *>(obj), ifaceId, iface);
      }
    };

    template <typename T, std::size_t I>
    struct QueryInterfaceIter
    {
      template <typename ObjType>
      static RetCode Query(ObjType *obj, InterfaceId ifaceId, Ptr *iface)
      {
        typedef typename std::tuple_element<I, T>::type CurType;
        return QueryInterface<CurType>::Query(static_cast<ObjType *>(obj), ifaceId, iface) == Status::Ok ?
          Status::Ok : QueryInterfaceIter<T, I - 1>::Query(obj, ifaceId, iface);
      }
    };

    template <typename T>
    struct QueryInterfaceIter<T, EndNegIntIter>
    {
      template <typename ObjType>
      static RetCode Query(ObjType *obj, InterfaceId ifaceId, Ptr *iface)
      {
        return Status::NotFound;
      }
    };

    template <typename T>
    struct QueryInterface<T, true>
    {
      template <typename ObjType>
      static RetCode Query(ObjType *obj, InterfaceId ifaceId, Ptr *iface)
      {
        typedef typename T::BaseEntities BaseEntities;
        enum { BaseEntityCount = std::tuple_size<BaseEntities>::value - 1 };
        return QueryInterfaceIter<BaseEntities, BaseEntityCount>::Query(static_cast<T *>(obj), ifaceId, iface);
      }
    };

    struct IBaseCastHelper
      : public IBase
    {
    };
    
  }

  template <typename T>
  class Base final
    : public T
    , private Private::IBaseCastHelper
  {
  public:
    Base(Base const &) = delete;
    Base const & operator = (Base const &) = delete;
    Base(Base &&) = delete;
    Base const & operator = (Base &&) = delete;

    template <typename ... Args>
    static RefObjPtr<T> Create(Args const & ... args)
    {
      Private::ModuleCounter::ScopedLock Lock;
      RefObjPtr<T> NewInst(new Base<T>(args ...));
      Private::FinalizeConstruct<T>::Construct(NewInst.Get());
      return std::move(NewInst);
    }

  private:
    std::atomic<UInt> Counter;

    template <typename ... Args>
    Base(Args const & ... args)
      : T(args ...)
      , Counter(0)
    {
      Private::ModuleCounter::AddRef();
    }
    virtual ~Base()
    {
      Private::ModuleCounter::Release();
    }
    // IBaseProvider
    virtual IBase* GetThisIBase()
    {
      return static_cast<IBaseCastHelper *>(this);
    }
    // IBase
    virtual UInt BOSS_CALL AddRef()
    {
      return Counter.fetch_add(1, std::memory_order_relaxed) + 1;
    }
    virtual UInt BOSS_CALL Release()
    {
      UInt CurValue = Counter.fetch_sub(1, std::memory_order_relaxed);
      if (CurValue == 1)
      {
        Private::BeforeRelease<T>::Release(static_cast<T *>(this));
        std::atomic_thread_fence(std::memory_order_acquire);
        delete this;
      }
      return CurValue - 1;
    }
    virtual RetCode BOSS_CALL QueryInterface(InterfaceId ifaceId, Ptr *iface)
    {
      RetCode Ret = Private::QueryInterface<T>::Query(static_cast<T *>(this), ifaceId, iface);
      if (Ret == Status::Ok)
        AddRef();
      return Ret;
    }
  };

}

#endif  // !__BOSS_CORE_BASE_H__
