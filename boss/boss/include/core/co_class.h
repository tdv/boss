#ifndef __BOSS_CORE_CO_CLASS_H__
#define __BOSS_CORE_CO_CLASS_H__

#include "ibase.h"

#include <tuple>

namespace Boss
{

  namespace Private
  {

    struct CoClassAdditive
    {
      virtual ~CoClassAdditive() {}
    };

    template <typename T>
    void SetConstructedFlag(T *obj, bool constructed)
    {
      obj->Constructed = constructed;
    }

    template <typename T>
    bool GetConstructedFlag(T *obj)
    {
      return  obj->Constructed;
    }
  }

  template <ClassId ClsId, typename ... T>
  class CoClass
    : public virtual Private::CoClassAdditive
    , public T ...
  {
  public:
    typedef std::tuple<T ... > BaseEntities;
    CoClass()
      : Constructed(false)
    {
    }

    // IBase
    BOSS_DECLARE_IBASE_METHODS()

  private:
    template <typename Y>
    friend void Private::SetConstructedFlag(Y *, bool);
    template <typename Y>
    friend bool Private::GetConstructedFlag(Y *);

    bool Constructed;
  };

}

#endif  // !__BOSS_CORE_CO_CLASS_H__
