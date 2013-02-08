#ifndef __BOSS_CORE_IBASE_H__
#define __BOSS_CORE_IBASE_H__

#include "core_types.h"
#include "boss_call.h"
#include "crc32.h"

#include <tuple>

#define BOSS_DECLARE_IFACEID(ifaceid_) \
  static constexpr Boss::InterfaceId GetInterfaceId() \
  { \
    return Boss::Crc32(ifaceid_); \
  }

#define BOSS_DECLARE_IBASE_METHODS() \
  virtual Boss::UInt BOSS_CALL AddRef() = 0; \
  virtual Boss::UInt BOSS_CALL Release() = 0; \
  virtual Boss::RetCode BOSS_CALL QueryInterface(Boss::InterfaceId ifaceId, Boss::Ptr *iface) = 0;

namespace Boss
{

  template <typename ... T>
  struct Inherit
    : public T ...
  {
    virtual ~Inherit() {}
    typedef std::tuple<T ... > BaseInterfaces;
    BOSS_DECLARE_IBASE_METHODS()
  };
  
  struct IBase
  {
    BOSS_DECLARE_IFACEID("Boss.IBase")
    virtual ~IBase() {}
    BOSS_DECLARE_IBASE_METHODS()
  };

}

#endif  // !__BOSS_CORE_IBASE_H__
