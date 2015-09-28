//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_COMMON_SERVICE_LOCATOR_H__
#define __BOSS_COMMON_SERVICE_LOCATOR_H__

#include "iservice_locator.h"
#include "core/base.h"
#include "core/error_codes.h"

#include <string>
#include <unordered_map>
#include <mutex>

namespace Boss
{

  class ServiceLocator
    : public SimpleCoClass<IServiceLocator
        >
  {
  public:
    // IServiceLocator
    virtual RetCode BOSS_CALL GetService(UInt serviceId, IBase **service) const
    {
      if (!service || *service)
        return Status::InvalidArgument;
      std::lock_guard<std::recursive_mutex> Lock(Mtx);
      ServicePool::iterator Iter = Services.find(serviceId);
      if (Iter == Services.end())
        return Status::Fail;
      return Iter->second.QueryInterface(service);
    }
    virtual RetCode BOSS_CALL AddService(UInt serviceId, IBase *service)
    {
      if (!service)
        return Status::InvalidArgument;
      std::lock_guard<std::recursive_mutex> Lock(Mtx);
      if (Services.find(serviceId) != Services.end())
        return Status::Fail;
      Services[serviceId] = service;
      return Status::Ok;
    }
    virtual RetCode BOSS_CALL DelService(UInt serviceId)
    {
      std::lock_guard<std::recursive_mutex> Lock(Mtx);
      ServicePool::iterator Iter = Services.find(serviceId);
      if (Iter == Services.end())
        return Status::Fail;
      Services.erase(Iter);
      return Status::Ok;
    }

  private:
    mutable std::recursive_mutex Mtx;
    
    typedef RefObjPtr<IBase> IBasePtr;
    typedef std::unordered_map<UInt, IBasePtr> ServicePool;
    mutable ServicePool Services;
  };

}

#endif  // !__BOSS_COMMON_SERVICE_LOCATOR_H__
