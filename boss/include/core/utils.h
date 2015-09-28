//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_CORE_UTILS_H__
#define __BOSS_CORE_UTILS_H__

#include "core_types.h"
#include "crc32.h"

#include <sstream>
#include <string>

namespace Boss
{

#if !(defined(_MSC_VER) || defined(__clang__))

  template <UInt N>
  inline constexpr UInt MakeId(char const (&str)[N])
  {
    return Crc32(str);
  }

#endif

  template <typename T>
  struct InterfaceTraits
  {
    typedef T Interface;
    static InterfaceId const Id = Interface::BOSSInterfaceId__;
  };
  
  template <typename T>
  struct GetCoClassId
  {
    enum { Id = T::CoClassId };
  };
  
  template <typename T>
  inline std::string ToString(T t)
  {
    std::stringstream Io;
    Io << t;
    return Io.str();
  }
  
  template <typename T>
  inline T FromString(std::string const &str)
  {
    std::stringstream Io;
    Io << str;
    T Ret;
    Io >> Ret;
    return Ret;
  }

}

#endif  // !__BOSS_CORE_UTILS_H__
