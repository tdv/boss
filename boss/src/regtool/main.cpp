//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#include "core/base.h"
#include "core/ref_obj_qi_ptr.h"
#include "core/error_codes.h"
#include "common/file_stream.h"
#include "plugin/module.h"
#include "plugin/service_info.h"
#include "plugin/module_holder.h"
#include "service_registry.h"

#include <iostream>
#include <cstring>

void ShowHelp()
{
  std::cout << "regtool -reg reg_file_name service_path" << std::endl;
  std::cout << "regtool -unreg reg_file_name service_name" << std::endl;
}

void Load(Boss::RefObjPtr<Boss::ISerializable> serializable, std::string const &filePath)
{
  if (serializable->Load(Boss::Base<Boss::IFileStream>::Create(filePath).Get()) != Boss::Status::Ok)
    throw std::runtime_error("Failed to load Registry.");
}

void Save(Boss::RefObjPtr<Boss::ISerializable> serializable, std::string const &filePath)
{
  if (serializable->Save(Boss::Base<Boss::OFileStream>::Create(filePath).Get()) != Boss::Status::Ok)
    throw std::runtime_error("Failed to save Registry.");
}

int main(int argc, char const **argv)
{
  try
  {
    if (argc < 2)
    {
      ShowHelp();
      return -1;
    }
    
    if (argc == 2 && !std::strcmp(argv[1], "-help"))
    {
      ShowHelp();
      return 0;
    }
    
    if (argc != 4)
    {
      std::cout << "Bad params." << std::endl;
      return -1;
    }
    
    std::string RegistryFilePath = argv[2];

    auto Reg = Boss::Base<Boss::ServiceRegistry>::Create();
    Boss::RefObjQIPtr<Boss::IServiceRegistryCtrl> Ctrl(Reg);    
    if (!Ctrl.Get())
      throw std::runtime_error("Failed to get RegCtrl.");
    Boss::RefObjQIPtr<Boss::ISerializable> Serializable(Ctrl);
    if (!Serializable.Get())
      throw std::runtime_error("Failed to get ISerializable interface from Registry object.");
    
    if (!std::strcmp(argv[1], "-reg"))
    {
      std::string ModulePath = argv[3];
      
      try
      {
        Load(Serializable, RegistryFilePath);
      }
      catch (std::exception const &)
      {
      }

      auto Info = Boss::Base<Boss::LocalServiceInfo>::Create();
      {
        auto Module = Boss::ModuleHolder(std::move(Boss::DllHolder(ModulePath)));
        
        Info->SetModulePath(ModulePath);
        Info->SetServiceId(Module.GetServiceId());
        
        for (auto const &i : Module.GetClassIds())
          Info->AddCoClassId(i);
        
        if (Ctrl->AddService(Info.Get()) != Boss::Status::Ok)
        {
          std::cout << "Failed to reg service." << std::endl;
          return -1;
        }
        
        Save(Serializable, RegistryFilePath);
      }
      return 0;
    }
    if (!std::strcmp(argv[1], "-unreg"))
    {
      std::string ServiceName = argv[3];
      
      Load(Serializable, RegistryFilePath);
      
      if (Ctrl->DelService(Boss::Crc32(ServiceName.c_str(), ServiceName.length())) != Boss::Status::Ok)
      {
        std::cout << "Failed to reg service." << std::endl;
        return -1;
      }
      
      Save(Serializable, RegistryFilePath);

      return 0;
    }
    
    std::cout << "Bad params." << std::endl;
    return -1;
    
  }
  catch (std::exception const &e)
  {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
