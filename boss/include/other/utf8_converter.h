//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_OTHER_UTF8_CONVERTER_H__
#define __BOSS_OTHER_UTF8_CONVERTER_H__

#include <string>

#include <string.h>

namespace Boss
{

  namespace Private
  {

    class Utf8Converter final
    {
      enum
      {
        MASKBITS = 0x3F,
        MASKBYTE = 0x80,
        MASK2BYTES = 0xC0,
        MASK3BYTES = 0xE0,
        MASK4BYTES = 0xF0,
        MASK5BYTES = 0xF8,
        MASK6BYTES = 0xFC
      };
      
    public:
      static std::wstring FromUtf8(char const *src)
      {
        std::wstring Ret;
        int len = strlen(src);
        for(int i = 0 ; i < len ; )
        {
          unsigned short ch = 0;
          if((src[i] & MASK3BYTES) == MASK3BYTES)
          {
            ch = ((src[i] & 0x0F) << 12) | ((src[i+1] & MASKBITS) << 6) | (src[i+2] & MASKBITS);
            i += 3;
          }
          else
          if((src[i] & MASK2BYTES) == MASK2BYTES)
          {
            ch = ((src[i] & 0x1F) << 6) | (src[i+1] & MASKBITS);
            i += 2;
          }
          else
          if(src[i] < static_cast<char>(MASKBYTE))
          {
            ch = src[i];
            i += 1;
          }
          Ret += ch;
        }
        return Ret;
      }

      static std::string ToUtf8(wchar_t const *src)
      {
        std::string Ret;
        int len = wcslen(src);
        for(int i = 0 ; i < len ; ++i)
        {
          if(src[i] < 0x80)
            Ret += static_cast<char>(src[i]);
          else
          if(src[i] < 0x800)
          {
            Ret += static_cast<char>(MASK2BYTES | src[i] >> 6);
            Ret += static_cast<char>(MASKBYTE | (src[i] & MASKBITS));
          }
          else
          if(src[i] < 0x10000)
          {
            Ret += static_cast<char>(MASK3BYTES | src[i] >> 12);
            Ret+= static_cast<char>(MASKBYTE | ((src[i] >> 6) & MASKBITS));
            Ret += static_cast<char>(MASKBYTE | (src[i] & MASKBITS));
          }
        }
        return Ret;
      }
    };

  }

  inline std::wstring FromUtf8(char const *src)
  {
    return Private::Utf8Converter::FromUtf8(src);
  }
  
  inline std::string ToUtf8(wchar_t const *src)
  {
    return Private::Utf8Converter::ToUtf8(src);
  }

}

#endif	// !__BOSS_OTHER_UTF8_CONVERTER_H__
