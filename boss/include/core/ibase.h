//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_CORE_IBASE_H__
#define __BOSS_CORE_IBASE_H__

#include "core_types.h"
#include "boss_call.h"
#include "utils.h"

#include <tuple>

#define BOSS_DECLARE_IFACEID(ifaceid_) \
  static Boss::InterfaceId const BOSSInterfaceId__ = Boss::MakeId(ifaceid_);

#define BOSS_DECLARE_IFACEID_HEX(hex_id_) \
  static Boss::InterfaceId const BOSSInterfaceId__ = hex_id_;

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
    BOSS_DECLARE_IFACEID_HEX(0x2CC77D66)  // CRC32(Boss.IBase)
    
    virtual ~IBase() {}
    BOSS_DECLARE_IBASE_METHODS()
  };

}

#endif  // !__BOSS_CORE_IBASE_H__
