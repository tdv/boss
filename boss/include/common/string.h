#ifndef __BOSS_COMMON_STRING_H__
#define __BOSS_COMMON_STRING_H__

#include "../core/base.h"
#include "../core/co_class.h"
#include "../core/error_codes.h"
#include "istring.h"
#include "buffer.h"

#include <string>
#include <memory>
#include <mutex>

namespace Boss
{
  
  class String
    : public CoClass<Crc32("Boss.String"), IString>
  {
  public:
    String()
    {
    }

    String(std::string const &str, bool utf8 = false)
    {
      if (!utf8)
        Ansi.reset(new std::string(str));
      else
        Utf8.reset(new std::string(str));
    }
    String(std::wstring const &str)
      : Unicode(new std::wstring(str))
    {
    }
    
    void SetString(std::string const &str, bool utf8 = false)
    {
      std::lock_guard<std::recursive_mutex> Lock(Mtx);
      if (!utf8)
      {
        Ansi.reset(new std::string(str));
        Utf8.reset();
      }
      else
      {
        Utf8.reset(new std::string(str));
        Ansi.reset();
      }
      Unicode.reset();
    }
    void SetString(std::wstring const &str)
    {
      std::lock_guard<std::recursive_mutex> Lock(Mtx);
      Unicode.reset(new std::wstring(str));
      Ansi.reset();
      Utf8.reset();
    }

    // IString
    virtual RetCode BOSS_CALL GetString(UInt type, IBuffer **str) const
    {
      if (!str || *str)
        return Status::InvalidArgument;
      try
      {
        std::lock_guard<std::recursive_mutex> Lock(Mtx);
        if (!Ansi.get() && !Utf8.get() && !Unicode.get())
          return Base<RawBuffer>::Create((void const *)0, 0).QueryInterface(str);
        switch (type)
        {
          case AnsiString :
            {
              if (Ansi.get())
                return Base<RawBuffer>::Create(Ansi->c_str(), Ansi->length()).QueryInterface(str);
              return Status::NotImplemented;
            }
            break;
          case Utf8String :
            {
              if (Utf8.get())
                return Base<RawBuffer>::Create(Utf8->c_str(), Utf8->length()).QueryInterface(str);
              return Status::NotImplemented;
            }
            break;
          case UnicodeString :
            {
              if (Unicode.get())
                return Base<RawBuffer>::Create(Unicode->c_str(), sizeof(std::wstring::value_type) * Unicode->length()).QueryInterface(str);
              return Status::NotImplemented;
            }
            break;
          default :
            return Status::Fail;            
        }
      }
      catch (...)
      {
        return Status::Fail;
      }
      return Status::Ok;
    }
    
  private:
    mutable std::recursive_mutex Mtx;
    std::unique_ptr<std::string> Ansi;
    std::unique_ptr<std::string> Utf8;
    std::unique_ptr<std::wstring> Unicode;
  };
  
}

#endif  // !__BOSS_COMMON_STRING_H__
