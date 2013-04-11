#ifndef __BOSS_CORE_UTILS_H__
#define __BOSS_CORE_UTILS_H__

#include "core_types.h"

namespace Boss
{

  template <ClassId ClsId, typename ... T>
  class CoClass;
  
  template <typename T>
  struct InterfaceTraits
  {
    typedef T Interface;
    static InterfaceId const Id = Interface::GetInterfaceId();
  };
  
  template <typename T>
  class GetCoClassId
  {
  private:
    template <ClassId ClsId, typename ... Y>
    static constexpr ClassId const ExtractId(CoClass<ClsId, Y ... > const *)
    {
      return ClsId;
    }

  public:
    enum { Id = ExtractId(static_cast<T const *>(0)) };
  };

}

#endif  // !__BOSS_CORE_UTILS_H__
