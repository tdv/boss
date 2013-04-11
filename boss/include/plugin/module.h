#ifndef __BOSS_PLUGIN_MODULE_H__
#define __BOSS_PLUGIN_MODULE_H__

#include "co_class_set.h"
#include "../core/private/module_ref_counter.h"

#ifdef __GNUC__
  #define BOSS_PLUGIN_EXPORT extern "C"
#else
  #error BOSS_PLUGIN_EXPORT not implemented
#endif

#define BOSS_DECLARE_MODULE_ENTRY_POINT(service_id_, exported_co_classes_) \
  namespace Boss \
  { \
    namespace Private \
    { \
      typedef exported_co_classes_ ModuleExportedCoClasses; \
    } \
  } \
  BOSS_PLUGIN_EXPORT \
  Boss::RetCode BOSS_CALL BossGetServiceId(Boss::ServiceId *serviceId) \
  { \
    if (!serviceId) \
      return Boss::Status::InvalidArgument; \
    *serviceId = Boss::Crc32(#service_id_); \
    return Boss::Status::Ok; \
  } \
  BOSS_PLUGIN_EXPORT \
  Boss::RetCode BOSS_CALL BossCreateObject(Boss::ClassId clsId, Boss::IBase **inst) \
  { \
    if (!inst) \
      return Boss::Status::InvalidArgument; \
    try \
    { \
      return Boss::CoClassSet<Boss::Private::ModuleExportedCoClasses>::CreateObject(clsId).QueryInterface(inst); \
    } \
    catch (std::exception const &) \
    { \
      return Boss::Status::Fail; \
    } \
    return Boss::Status::Fail; \
  } \
  BOSS_PLUGIN_EXPORT \
  Boss::RetCode BOSS_CALL BossGetClassCount(Boss::UInt *count) \
  { \
    if (!count) \
      return Boss::Status::InvalidArgument; \
    try \
    { \
      *count = Boss::CoClassSet<Boss::Private::ModuleExportedCoClasses>::GetClassCount(); \
    } \
    catch (std::exception const &) \
    { \
      return Boss::Status::Fail; \
    } \
    return Boss::Status::Ok; \
  } \
  BOSS_PLUGIN_EXPORT \
  Boss::RetCode BOSS_CALL BossGetClassId(Boss::UInt index, Boss::ClassId *clsId) \
  { \
    if (!clsId) \
      return Boss::Status::InvalidArgument; \
    try \
    { \
      *clsId = Boss::CoClassSet<Boss::Private::ModuleExportedCoClasses>::GetId(index); \
    } \
    catch (std::exception const &) \
    { \
      return Boss::Status::Fail; \
    } \
    return Boss::Status::Ok; \
  } \
  BOSS_PLUGIN_EXPORT \
  Boss::RetCode BOSS_CALL BossGetRefCount(Boss::UInt *refs) \
  { \
    if (!refs) \
      return Boss::Status::InvalidArgument; \
    try \
    { \
      *refs = Boss::Private::ModuleCounter::GetCounter(); \
    } \
    catch (std::exception const &) \
    { \
      return Boss::Status::Fail; \
    } \
    return Boss::Status::Ok; \
  }

#endif  // !__BOSS_PLUGIN_MODULE_H__
