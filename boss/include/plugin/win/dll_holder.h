#ifndef __BOSS_PLUGIN_DLL_HOLDER_H__
#define __BOSS_PLUGIN_DLL_HOLDER_H__

#include "core/exceptions.h"
#include "common/utility.h"

#include <string>
#include <utility>

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#include <windows.h>
#include <assert.h>

namespace Boss
{

  BOSS_DECLARE_RUNTIME_EXCEPTION(DllHolder)

  namespace Private
  {

    class DllHolder
    {
    public:
      DllHolder(DllHolder const &) = delete;
      DllHolder& operator = (DllHolder const &) = delete;
      
      DllHolder(const std::string &path)
        : DllInst(0)
      {
        if ((DllInst = LoadLibraryA(path.c_str())) == 0)
          throw DllHolderException("Failed to load dll.");
      }
      DllHolder(DllHolder &&dll)
        : DllInst(std::move(dll.DllInst))
      {
        dll.DllInst = 0;
      }
      DllHolder& operator = (DllHolder &&dll)
      {
        UnloadDll();
        DllInst = std::move(dll.DllInst);
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
        FARPROC Proc = GetProcAddress(DllInst, procName.c_str());
        if (!Proc)
          throw DllHolderException("Function not found");
        return reinterpret_cast<T>(Proc);
      }

    private:
      HINSTANCE DllInst;
      
      void UnloadDll()
      {
        BOOL Res = FreeLibrary(DllInst);
        assert(!!Res && "Failed to unload dll.");
        Unused(Res);
      }
    };

  }

  typedef Private::DllHolder DllHolder;

}

#endif  // !__BOSS_PLUGIN_DLL_HOLDER_H__
