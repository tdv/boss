//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_COMMON_BUFFER_H__
#define __BOSS_COMMON_BUFFER_H__

#include "ibuffer.h"
#include "../core/co_class.h"
#include "../core/error_codes.h"

#include <vector>

namespace Boss
{
  
  class RawBuffer
    : public SimpleCoClass<IBuffer>
  {
  public:
    RawBuffer(void const *data, UInt bytes)
      : Buffer([&] ()
               {
                 return !bytes ? std::move(std::vector<char>()) :
                   std::move(std::vector<char>(reinterpret_cast<char const *>(data),
                                               reinterpret_cast<char const *>(data) + bytes)); 
               } ()
              )
    {
    }

    // IBuffer
    virtual RetCode BOSS_CALL GetData(void const **data) const
    {
      if (!data || *data)
        return  Status::InvalidArgument;
      *data = Buffer.empty() ? 0 : &Buffer[0];
      return Status::Ok;
    }
    virtual RetCode BOSS_CALL GetSize(UInt *size) const
    {
      if (!size)
        return  Status::InvalidArgument;
      *size = Buffer.size();
      return Status::Ok;
    }
    
  private:
    std::vector<char> Buffer;
  };
  
}

#endif  // !__BOSS_COMMON_BUFFER_H__
