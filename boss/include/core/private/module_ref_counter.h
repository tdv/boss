#ifndef __BOSS_CORE_PRIVATE_MODULE_REF_COUNTER_H__
#define __BOSS_CORE_PRIVATE_MODULE_REF_COUNTER_H__

#include "../core_types.h"

#include <atomic>

namespace Boss
{

  namespace Private
  {
 
    struct ModuleRefCounterTypeStub
    {
    };

    template <typename T>
    class ModuleRefCounter
    {
    public:
      static void AddRef()
      {
        Counter.fetch_add(1, std::memory_order_relaxed);
      }
      static void Release()
      {
        Counter.fetch_sub(1, std::memory_order_relaxed);
      }
      static UInt GetCounter()
      {
        return Counter;
      }

    private:
      static std::atomic<UInt> Counter;

    public:
      class ScopedLock
      {
      public:
        ScopedLock(ScopedLock const &) = delete;
        ScopedLock(ScopedLock &&) = delete;
        ScopedLock operator = (ScopedLock const &) = delete;
        ScopedLock operator = (ScopedLock &&) = delete;

        ScopedLock()
        {
          ModuleRefCounter<T>::AddRef();
        }
        ~ScopedLock()
        {
          ModuleRefCounter<T>::Release();
        }
      };
    };

    template <typename T>
    std::atomic<UInt> ModuleRefCounter<T>::Counter(0);

    typedef ModuleRefCounter<ModuleRefCounterTypeStub> ModuleCounter;

  }

}

#endif  // !__BOSS_CORE_PRIVATE_MODULE_REF_COUNTER_H__
