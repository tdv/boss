#ifndef __BOSS_PLUGIN_MODULE_HOLDER_H__
#define __BOSS_PLUGIN_MODULE_HOLDER_H__

#include <vector>

#include "core/ibase.h"
#include "core/exceptions.h"
#include "core/error_codes.h"
#include "core/ref_obj_ptr.h"
#include "core/ref_obj_qi_ptr.h"
#include "dll_holder.h"


namespace Boss
{

  BOSS_DECLARE_RUNTIME_EXCEPTION(ModuleHolder)

  class ModuleHolder
  {
  public:
    typedef RefObjPtr<IBase> IBasePtr;
    typedef std::vector<ClassId> ClassIdPool;

    ModuleHolder(ModuleHolder const &) = delete;
    ModuleHolder& operator = (ModuleHolder const &) = delete;
    
    ModuleHolder(DllHolder &&dll)
      : Dll(std::move(dll))
    {
    }
    ModuleHolder(ModuleHolder &&module)
      : Dll(std::move(module.Dll))
    {
    }
    ModuleHolder& operator = (ModuleHolder &&module)
    {
      Dll = std::move(module.Dll);
      return *this;
    }
    ServiceId const GetServiceId() const
    {
      typedef RetCode (BOSS_CALL *PFNBossGetServiceId)(ServiceId *);
      auto *BossGetServiceId = Dll.GetProc<PFNBossGetServiceId>("BossGetServiceId");
      ServiceId SrvId = 0;
      if (BossGetServiceId(&SrvId) != Status::Ok)
        throw ModuleHolderException("Failed to call BossGetServiceId.");
      return SrvId;
    }
    ClassIdPool const GetClassIds() const
    {
      typedef RetCode (BOSS_CALL *PFNBossGetClassCount)(UInt *);
      UInt Count = 0;
      auto *BossGetClassCount = Dll.GetProc<PFNBossGetClassCount>("BossGetClassCount");
      if (BossGetClassCount(&Count) != Status::Ok)
        throw ModuleHolderException("Failed to call BossGetClassCount.");
      typedef RetCode (BOSS_CALL *PFNBossGetClassId)(UInt, ClassId *);
      auto *BossGetClassId = Dll.GetProc<PFNBossGetClassId>("BossGetClassId");
      ClassIdPool Ids;
      for (UInt i = 0 ; i < Count ; ++i)
      {
        ClassId Id = 0;
        if (BossGetClassId(i, &Id) != Status::Ok)
          throw ModuleHolderException("Failed to call BossGetClassId.");
        Ids.push_back(Id);
      }
      return std::move(Ids);
    }
    IBasePtr CreateObject(ClassId classId)
    {
      typedef RetCode (BOSS_CALL *PFNBossCreateObject)(ClassId, IBase **);
      IBasePtr Ret;
      auto *BossCreateObject = Dll.GetProc<PFNBossCreateObject>("BossCreateObject");
      if (BossCreateObject(classId, Ret.GetPPtr()) != Status::Ok)
        throw ModuleHolderException("Failed to create object.");
      return std::move(Ret);
    }
    template <typename T>
    RefObjQIPtr<T> CreateObject(ClassId classId)
    {
      RefObjQIPtr<T> Ret(CreateObject(classId));
      if (!Ret.Get())
        throw ModuleHolderException("Interface not found.");
      return std::move(Ret);
    }
    UInt const GetRefCount() const
    {
      typedef RetCode (BOSS_CALL *PFNBossGetRefCount)(UInt *);
      auto *BossGetRefCount = Dll.GetProc<PFNBossGetRefCount>("BossGetRefCount");
      UInt RefCount = 0;
      if (BossGetRefCount(&RefCount) != Status::Ok)
        throw ModuleHolderException("Failed to call BossGetRefCount.");
      return RefCount;
    }

  private:
    mutable DllHolder Dll;
  };

}

#endif  // !__BOSS_PLUGIN_MODULE_HOLDER_H__
