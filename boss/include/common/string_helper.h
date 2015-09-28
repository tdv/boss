//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     05.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_COMMON_STRING_HELPER_H__
#define __BOSS_COMMON_STRING_HELPER_H__

#include "../core/exceptions.h"
#include "../core/ref_obj_ptr.h"
#include "../core/error_codes.h"
#include "../core/base.h"
#include "istring.h"
#include "buffer.h"

#include <string>
#include <cstring>

namespace Boss
{

  namespace Private
  {

    template <UInt TypeId>
    struct StringTypeFromTypeId;
    
    template <>
    struct StringTypeFromTypeId<IString::AnsiString>
    {
      typedef std::string Type;
    };
    
    template <>
    struct StringTypeFromTypeId<IString::Utf8String>
    {
      typedef std::string Type;
    };
    
    template <>
    struct StringTypeFromTypeId<IString::UnicodeString>
    {
      typedef std::wstring Type;
    };
    
  }
  
  BOSS_DECLARE_RUNTIME_EXCEPTION(StringHelper)
  
  class StringHelper final
  {
  public:
    typedef RefObjPtr<IString> IStringPtr;

    StringHelper(StringHelper const &) = delete;
    StringHelper& operator = (StringHelper const &) = delete;
    
    explicit StringHelper(IString *str)
      : Str(str)
    {
      if (!Str.Get())
        throw StringHelperException("Empty IString pointer.");
    }
    explicit StringHelper(IStringPtr str)
      : Str(str)
    {
      if (!Str.Get())
        throw StringHelperException("Empty IString pointer.");
    }
    StringHelper& operator = (IStringPtr str)
    {
      if (!str.Get())
        throw StringHelperException("Empty IString pointer.");
      Str = str;
      return *this;
    }
    void SetUtf8String(std::string const &str)
    {
      SetStringInternal(str.c_str(), str.length(), IString::Utf8String);
    }
    void SetUtf8String(char const *str)
    {
      if (!str)
        Clear();
      else
        SetStringInternal(str, std::strlen(str), IString::Utf8String);
    }
    StringHelper& operator = (std::string const &str)
    {
      SetStringInternal(str.c_str(), str.length(), IString::AnsiString);
      return *this;
    }
    StringHelper& operator = (std::wstring const &str)
    {
      SetStringInternal(str.c_str(), str.length() * sizeof(std::wstring::value_type),
                        IString::UnicodeString);
      return *this;
    }
    StringHelper& operator = (char const *str)
    {
      if (!str)
        Clear();
      else
        SetStringInternal(str, std::strlen(str), IString::AnsiString);
      return *this;
    }
    StringHelper& operator = (wchar_t const *str)
    {
      if (!str)
        Clear();
      else
        SetStringInternal(str, std::wcslen(str) * sizeof(wchar_t), IString::UnicodeString);
      return *this;
    }
    std::size_t GetLength() const
    {
      UInt Length = 0;
      if (Str->GetLength(&Length) != Status::Ok)
        throw StringHelperException("Failed to get string length.");
      return Length;
    }
    bool operator == (StringHelper const &str) const
    {
      if (this == &str)
        return true;
      RetCode Code = Str->IsEqual(str.Str.Get());
      if (Code == Status::Ok)
        return true;
      if (Code == Status::False)
        return false;
      throw StringHelperException("Failed to compare strings.");
    }
    bool operator != (StringHelper const &str) const
    {
      return !(*this == str);
    }
    void Clear()
    {
      if (Str->Clear() != Status::Ok)
        throw StringHelperException("Failed to clear string.");
    }
    template <UInt Type>
    typename Private::StringTypeFromTypeId<Type>::Type GetString() const
    {
      void const *Data = 0;
      UInt Size = 0;
      RefObjPtr<IBuffer> Buf;
      if (Str->GetString(Type, Buf.GetPPtr()) != Status::Ok ||
          Buf->GetSize(&Size)  != Status::Ok ||
          Buf->GetData(&Data)  != Status::Ok)
      {
        throw StringHelperException("Failed to get string.");
      }
      typedef typename Private::StringTypeFromTypeId<Type>::Type StrType;
      typedef typename StrType::value_type CharType;
      return std::move(StrType(reinterpret_cast<CharType const *>(Data),
                               Size / sizeof(CharType)));
    }
    
  private:
    mutable IStringPtr Str;
    
    void SetStringInternal(void const *data, UInt bytes, UInt type)
    {
      if (Str->SetString(type, Base<RawBuffer>::Create(data, bytes).Get()) != Status::Ok)
        throw StringHelperException("Failed to set string.");
    }
  };
  
}

#endif  // !__BOSS_COMMON_STRING_HELPER_H__
