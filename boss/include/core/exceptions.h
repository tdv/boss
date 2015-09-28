//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_CORE_EXCEPTIONS_H__
#define __BOSS_CORE_EXCEPTIONS_H__

#include <stdexcept>

#define BOSS_DECLARE_EXCEPTION_EX(class_, ext_, base_) \
  class class_##ext_ \
    : public base_ \
  { \
  public: \
    template <typename ... Args> \
    class_##ext_(Args const & ... args) \
      : base_(args ... ) \
    { \
    } \
  };

#define BOSS_DECLARE_RUNTIME_EXCEPTION(class_) \
  BOSS_DECLARE_EXCEPTION_EX(class_, Exception, std::runtime_error)

#endif  // !__BOSS_CORE_EXCEPTIONS_H__
