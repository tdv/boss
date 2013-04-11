#ifndef __BOSS_COMMON_NAMED_VALUE_H__
#define __BOSS_COMMON_NAMED_VALUE_H__

#include "../core/co_class.h"
#include "../core/base.h"
#include "string.h"
#include "inamed_value.h"

#include <string>

namespace Boss
{
  
  class NamedValue
    : public CoClass<Crc32("Boss.INamedValue"), INamedValue>
  {
  public:
    NamedValue(std::string const &name, RefObjPtr<IBase> value)
      : Name(Base<String>::Create(name))
      , Value(value)
    {
    }
    
    virtual RetCode BOSS_CALL GetName(IString **name) const
    {
      return !name || *name ? Status::InvalidArgument : Name.QueryInterface(name);
    }
    
    virtual RetCode BOSS_CALL GetValue(IBase **value) const
    {
      return !value || *value ? Status::InvalidArgument : Value.QueryInterface(value);
    }
    
  private:
    mutable RefObjPtr<IString> Name;
    mutable RefObjPtr<IBase> Value;
  };
  
}

#endif  // !__BOSS_COMMON_NAMED_VALUE_H__
