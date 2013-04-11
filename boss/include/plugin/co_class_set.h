#ifndef __BOSS_PLUGIN_CO_CLASS_SET_H__
#define __BOSS_PLUGIN_CO_CLASS_SET_H__

#include "../core/exceptions.h"
#include "../core/ref_obj_ptr.h"
#include "../core/ibase.h"
#include "../core/base.h"

namespace Boss
{

  BOSS_DECLARE_RUNTIME_EXCEPTION(CoClassSet)
  
  namespace Private
  {

    template <typename T, unsigned I>
    struct CoClassSet
    {
      template <typename ... Args>
      static RefObjPtr<IBase> CreateObject(ClassId clsId, Args const & ... args)
      {
        typedef typename std::tuple_element<I, T>::type CurType;
        if (clsId == GetCoClassId<CurType>::Id)
        {
          RefObjPtr<IBase> Obj;
          Base<CurType>::Create(args ... ).QueryInterface(Obj.GetPPtr());
          return std::move(Obj);
        }
        return std::move(CoClassSet<T, I - 1>::CreateObject(clsId, args ... ));
      }
      static ClassId GetId(UInt pos)
      {
        typedef typename std::tuple_element<I, T>::type CurType;
        return pos == I ? GetCoClassId<CurType>::Id : CoClassSet<T, I - 1>::GetId(pos);
      }
    };
    
    template <typename T>
    struct CoClassSet<T, -1>
    {
      template <typename ... Args>
      static RefObjPtr<IBase> CreateObject(ClassId clsId, Args const & ...)
      {
        throw CoClassSetException("Failed to create object with CoClassId = " +
                                  std::to_string(clsId) + ".");
      }
      static ClassId GetId(UInt pos)
      {
        throw CoClassSetException("Position " + std::to_string(pos) +
                                  " is out of range.");
      }
    };
        
  }
  
  template <typename T>
  class CoClassSet;
  
  template <typename ... T>
  class CoClassSet<std::tuple<T ... >>
  {
  private:
    typedef std::tuple<T ... > CoClasses;
    enum { CoClassCount = std::tuple_size<CoClasses>::value };
    
  public:
    template <typename ... Args>
    static RefObjPtr<IBase> CreateObject(ClassId clsId, Args const & ... args)
    {
      return Private::CoClassSet<CoClasses, CoClassCount - 1>::CreateObject(clsId, args ... );
    }
    template <typename ... Args, unsigned N>
    static RefObjPtr<IBase> CreateObject(char const (&clsId)[N], Args const & ... args)
    {
      return CreateObject(Crc32(clsId), args ... );
    }
    static UInt GetClassCount()
    {
      return CoClassCount;
    }
    static ClassId GetId(UInt pos)
    {
      return Private::CoClassSet<CoClasses, CoClassCount - 1>::GetId(pos);
    }
  };
    
}

#endif  // !__BOSS_PLUGIN_CO_CLASS_SET_H__
