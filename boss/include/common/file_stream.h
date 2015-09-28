//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_COMMON_FILE_STREAM_H__
#define __BOSS_COMMON_FILE_STREAM_H__

#ifdef _MSC_VER
  #pragma warning(disable : 4068)
  #pragma warning(disable : 4996)
#endif  // !_MSC_VER

#include "iistream.h"
#include "iostream.h"
#include "iiostream.h"
#include "buffer.h"
#include "../core/base.h"
#include "../core/co_class.h"
#include "../core/error_codes.h"
#include "../core/exceptions.h"

#include <string>
#include <cstdio>
#include <iterator>

#include <assert.h>

namespace Boss
{

  BOSS_DECLARE_RUNTIME_EXCEPTION(FileStream)
  
  namespace Private
  {
    
    template <typename T>
    class IOFileStream
      : public T
    {
    public:
      IOFileStream()
        : File(nullptr)
      {
      }
      virtual ~IOFileStream()
      {
        if (File)
          std::fclose(File);
      }
      
      // IIStream
      virtual RetCode BOSS_CALL Read(UInt bytes, IBuffer **buffer)
      {
        if (!buffer || *buffer)
          return Status::InvalidArgument;
        assert(!!File && "File not open.");
        if (!File)
          return Status::Fail;
        try
        {
          if (!bytes)
            return Base<RawBuffer>::Create(nullptr, 0).QueryInterface(buffer);
          std::vector<char> Buffer(bytes);
          std::size_t Res = std::fread(&Buffer[0], 1, Buffer.size(), File);
          if (!Res || Res > Buffer.size())
            return Status::Fail;
          return Base<RawBuffer>::Create(&Buffer[0], Res).QueryInterface(buffer);
        }
        catch (...)
        {
        }
        return Status::Fail;
      }
      virtual RetCode BOSS_CALL Read(IBuffer **buffer)
      {
        if (!buffer || *buffer)
          return Status::InvalidArgument;
        assert(!!File && "File not open.");
        if (!File)
          return Status::Fail;
        try
        {
          std::vector<char> Buffer;
          for (std::vector<char> Tmp(ReadBufSize) ; !std::feof(File) ; )
          {
            std::size_t Res = std::fread(&Tmp[0], 1, Tmp.size(), File);
            if (!Res || Res > Tmp.size())
              return Status::Fail;
            std::copy(std::begin(Tmp), std::end(Tmp), std::back_inserter(Buffer));
          }
          return Base<RawBuffer>::Create(Buffer.empty() ? nullptr : &Buffer[0], Buffer.size()).QueryInterface(buffer);
        }
        catch (...)
        {
        }
        return Status::Fail;
      }
      // IOStream
      virtual RetCode BOSS_CALL Write(void const *data, UInt bytes)
      {
        if (!data || !bytes)
          return Status::False;
        assert(!!File && "File not open.");
        if (!File)
          return Status::Fail;
        return std::fwrite(data, 1, bytes, File) == bytes ? Status::Ok : Status::Fail;;
      }
      
    protected:
      void Open(std::string const &fileName, std::string const &mode)
      {
        assert(!File && "File already open.");
        if (!(File = std::fopen(fileName.c_str(), mode.c_str())))
          throw FileStreamException("Failed to open file \"" + fileName + "\".");
      }
      
    private:
      enum { ReadBufSize = 8192 };
      std::FILE *File;
    };
    
  }
  
  class IFileStream
    : public SimpleCoClass<Private::IOFileStream<IIStream>>
  {
  public:
    IFileStream(std::string const &fileName)
    {
      Open(fileName, "rb");
    }
  };
  
  class OFileStream
    : public SimpleCoClass<Private::IOFileStream<IOStream>>
  {
  public:
    OFileStream(std::string const &fileName)
    {
      Open(fileName, "w+b");
    }
  };
  
  class IOFileStream
    : public SimpleCoClass<Private::IOFileStream<IIOStream>>
  {
  public:
    IOFileStream(std::string const &fileName)
    {
      // TODO:
      Open(fileName, "w+r+");
    }
  };
  
}

#endif  // !__BOSS_COMMON_FILE_STREAM_H__
