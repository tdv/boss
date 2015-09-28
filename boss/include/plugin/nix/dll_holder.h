//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_PLUGIN_NIX_DLL_HOLDER_H__
#define __BOSS_PLUGIN_NIX_DLL_HOLDER_H__

#include "core/exceptions.h"
#include "common/utility.h"

#include <string>
#include <utility>

#include <pthread.h>
#include <dlfcn.h>
#include <assert.h>

namespace Boss
{

  BOSS_DECLARE_RUNTIME_EXCEPTION(DllHolder)

  namespace Private
  {

    class DllHolder final
    {
    public:
      DllHolder(DllHolder const &) = delete;
      DllHolder& operator = (DllHolder const &) = delete;
      
      DllHolder(const std::string &path)
        : DllInst(dlopen(path.c_str(), RTLD_LAZY))
      {
        if (DllInst == nullptr)
          throw DllHolderException("failed to load \"" + path + "\"");
      }
      DllHolder(DllHolder &&dll)
        : DllInst(dll.DllInst)
      {
        dll.DllInst = 0;
      }
      DllHolder& operator = (DllHolder &&dll)
      {
        UnloadDll();
        DllInst = dll.DllInst;
        dll.DllInst = 0;
        return *this;
      }
      ~DllHolder()
      {
        UnloadDll();
      }

      template <typename T>
      T GetProc(const std::string &procName)
      {
        if (DllInst == nullptr)
          throw DllHolderException("Library was not loaded.");
        void *Proc = dlsym(DllInst, procName.c_str());
        if (!Proc)
          throw DllHolderException("Function \"" + procName + "\" not found.");
        return reinterpret_cast<T>(Proc);
      }

    private:
      void *DllInst;
      void UnloadDll()
      {
        if (DllInst)
        {
          int Res = dlclose(DllInst);
          assert(!Res && "Failed to unload dll.");
          Unused(Res);
        }
        DllInst = 0;
      }
    };

  }

  typedef Private::DllHolder DllHolder;

}

#endif  // !__BOSS_PLUGIN_NIX_DLL_HOLDER_H__
