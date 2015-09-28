//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_COMMON_STRING_H__
#define __BOSS_COMMON_STRING_H__

#include "../core/base.h"
#include "../core/co_class.h"
#include "../core/error_codes.h"
#include "../other/utf8_converter.h"
#include "istring.h"
#include "buffer.h"

#include <string>
#include <cstring>
#include <memory>
#include <mutex>

namespace Boss
{
  
  class String
    : public SimpleCoClass<IString>
  {
  public:
    String()
    {
    }

    String(std::string const &str, bool utf8 = false)
      : Type(utf8 ? Utf8String : AnsiString)
      , Length(str.length())
      , Data(std::move(Base<RawBuffer>::Create(str.c_str(), str.length())))
    {
    }
    String(std::wstring const &str)
      : Type(UnicodeString)
      , Length(str.length())
      , Data(std::move(Base<RawBuffer>::Create(str.c_str(), str.length() * sizeof(std::wstring::value_type))))
    {
    }
    
    void SetString(std::string const &str, bool utf8 = false)
    {
      std::lock_guard<std::recursive_mutex> Lock(Mtx);
      SetStringInternal(str, utf8);
    }
    void SetString(std::wstring const &str)
    {
      std::lock_guard<std::recursive_mutex> Lock(Mtx);
      SetStringInternal(str);
    }

    // IString
    virtual RetCode BOSS_CALL GetLength(UInt *length) const
    {
      if (!length)
        return Status::InvalidArgument;
      try
      {
        std::lock_guard<std::recursive_mutex> Lock(Mtx);
        *length = Length;
        return Status::Ok;
      }
      catch (std::exception const &)
      {
      }
      return Status::Fail;
    }
    virtual RetCode BOSS_CALL GetType(UInt *type) const
    {
      if (!type)
        return Status::InvalidArgument;
      try
      {
        std::lock_guard<std::recursive_mutex> Lock(Mtx);
        *type = Type;
        return Status::Ok;
      }
      catch (std::exception const &)
      {
      }
      return Status::Fail;
    }
    virtual RetCode BOSS_CALL GetString(UInt type, IBuffer **str) const
    {
      if (!str || *str)
        return Status::InvalidArgument;
      try
      {
        std::lock_guard<std::recursive_mutex> Lock(Mtx);
        if (Type == UnknownStringType)
          return Status::False;
        RetCode Code = const_cast<String *>(this)->ConvertInternal(type);
        return Code == Status::Ok ? Data.QueryInterface(str) : Code;
      }
      catch (std::exception const &)
      {
      }
      return Status::Fail;
    }
    virtual RetCode BOSS_CALL IsEqual(IString *str) const
    {
      if (!str)
        return Status::InvalidArgument;
      if (str == this)
        return Status::Ok;
      try
      {
        std::lock_guard<std::recursive_mutex> Lock(Mtx);
        UInt StrType = UnknownStringType;
        if (str->GetType(&StrType) != Status::Ok)
          return Status::Fail;
        if (StrType != Type)
          return Status::False;
        if (Type == UnknownStringType)
          return Status::Ok;
        UInt StrLength = 0;
        if (str->GetLength(&StrLength) != Status::Ok)
          return Status::Fail;
        if (StrLength != Length)
          return Status::False;
        RefObjPtr<IBuffer> Buf;
        if (str->GetString(StrType, Buf.GetPPtr()) != Status::Ok)
          return Status::Fail;
        void const *StrBufData = 0;
        UInt StrBufSize = 0;
        if (Buf->GetSize(&StrBufSize) != Status::Ok || Buf->GetData(&StrBufData) != Status::Ok)
          return Status::Fail;
        void const *BufData = 0;
        UInt BufSize = 0;
        if (Data->GetSize(&BufSize) != Status::Ok || Data->GetData(&BufData) != Status::Ok)
          return Status::Fail;
        return StrBufSize == BufSize && !std::memcmp(StrBufData, BufData, StrBufSize);
      }
      catch (std::exception const &)
      {
      }
      return Status::Fail;
    }
    virtual RetCode BOSS_CALL SetString(UInt type, IBuffer *str)
    {
      if (!str || (type != AnsiString && type != Utf8String && type != UnicodeString))
        return Status::InvalidArgument;
      try
      {
        std::lock_guard<std::recursive_mutex> Lock(Mtx);
        void const *Buf = 0;
        UInt Len = 0;
        if (str->GetSize(&Len) != Status::Ok || str->GetData(&Buf) != Status::Ok)
          return Status::Fail;
        if (Type == UnicodeString && Len % sizeof(std::wstring::value_type))
          return Status::InvalidArgument;
        Data = std::move(Base<RawBuffer>::Create(Buf, Len));
        Type = type;
        Length = Len;
        if (Type == UnicodeString)
          Length /= sizeof(std::wstring::value_type);
      }
      catch (std::exception const &)
      {
      }
      return Status::Fail;
    }
    virtual RetCode BOSS_CALL Convert(UInt type)
    {
      try
      {
        std::lock_guard<std::recursive_mutex> Lock(Mtx);
        return ConvertInternal(type);
      }
      catch (std::exception const &)
      {
      }
      return Status::Fail;
    }
    virtual RetCode BOSS_CALL Clear()
    {
      try
      {
        std::lock_guard<std::recursive_mutex> Lock(Mtx);
        Data.Release();
        Type = UnknownStringType;
        Length = 0;
        return Status::Ok;
      }
      catch (std::exception const &)
      {
      }
      return Status::Fail;
    }
    
  private:
    mutable std::recursive_mutex Mtx;
    UInt Type = UnknownStringType;
    UInt Length = 0;
    mutable RefObjPtr<IBuffer> Data;
    
    virtual RetCode ConvertInternal(UInt type)
    {
      if (Type == UnknownStringType && type != UnknownStringType)
        return Status::InvalidArgument;
      if (Type == type)
        return Status::Ok;
      if (Type == AnsiString || type == AnsiString)
        return Status::NotImplemented;
      if (Type == Utf8String && type == UnicodeString)
      {
        void const *Buf = 0;
        if (Data->GetData(&Buf) != Status::Ok)
          return Status::Fail;
        SetStringInternal(FromUtf8(std::string(reinterpret_cast<std::string::value_type const *>(Buf), Length).c_str()));
        return Status::Ok;
      }
      if (Type == UnicodeString && type == Utf8String)
      {
        void const *Buf = 0;
        if (Data->GetData(&Buf) != Status::Ok)
          return Status::Fail;
        SetStringInternal(ToUtf8(std::wstring(reinterpret_cast<std::wstring::value_type const *>(Buf), Length).c_str()), true);
        return Status::Ok;
      }
      return Status::Fail;
    }
    void SetStringInternal(std::string const &str, bool utf8 = false)
    {
      Data = std::move(Base<RawBuffer>::Create(str.c_str(), str.length()));
      Type = utf8 ? Utf8String : AnsiString;
      Length = str.length();
    }
    void SetStringInternal(std::wstring const &str)
    {
      Data = std::move(Base<RawBuffer>::Create(str.c_str(), str.length() * sizeof(std::wstring::value_type)));
      Type = UnicodeString;
      Length = str.length();
    }
  };
  
}

#endif  // !__BOSS_COMMON_STRING_H__
