//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __COMMON_XML_PROPERTY_BAG_H__
#define __COMMON_XML_PROPERTY_BAG_H__

#include "iproperty_bag.h"
#include "iserializable.h"
#include "ientity_id.h"
#include "istring.h"
#include "ienum.h"
#include "named_value.h"
#include "enum.h"
#include "entity_id.h"
#include "string.h"

#include "core/base.h"
#include "core/co_class.h"
#include "core/error_codes.h"
#include "core/ref_obj_qi_ptr.h"
#include "core/utils.h"
#include "core/private/empty_type.h"
#include "other/rapidxml-1.13/rapidxml_print.hpp"

#include <unordered_map>
#include <string>
#include <mutex>
#include <sstream>
#include <iterator>
#include <stdexcept>

namespace Boss
{

  namespace Private
  {
    
    template <typename T>
    struct XmlPropertyBagTags
    {
      static char const TypeTag[];
      static char const ItemTag[];
      
      static char const EntityIdTag[];
      static char const StringTag[];
      static char const EnumTag[];
      static char const PropertyBagTag[];
    };
    
    template <typename T>
    char const XmlPropertyBagTags<T>::TypeTag[] = "Type";
    
    template <typename T>
    char const XmlPropertyBagTags<T>::ItemTag[] = "Item";
    
    template <typename T>
    char const XmlPropertyBagTags<T>::EntityIdTag[] = "Id";
    
    template <typename T>
    char const XmlPropertyBagTags<T>::StringTag[] = "String";
    
    template <typename T>
    char const XmlPropertyBagTags<T>::EnumTag[] = "Enum";
    
    template <typename T>
    char const XmlPropertyBagTags<T>::PropertyBagTag[] = "Props";
    
  }
  
  class XmlPropertyBag
    : public SimpleCoClass<IPropertyBag, ISerializable>
  {
  public:
    XmlPropertyBag(std::string const &rootName)
      : RootName(rootName)
    {
    }
    
    // IPropertyBag
    virtual RetCode BOSS_CALL GetProperty(char const *name, IBase **value) const
    {
      if (!name || !*name || !value || *value)
        return Status::InvalidArgument;
      try
      {
        std::lock_guard<std::recursive_mutex> Lock(Mtx);
        auto Iter = Props.find(name);
        if (Iter == std::end(Props))
          return Status::NotFound;
        return Iter->second.QueryInterface(value);
      }
      catch (...)
      {
        return Status::Fail;
      }
    }
    virtual RetCode BOSS_CALL SetProperty(char const *name, IBase *value)
    {
      if (!name || !*name || !value)
        return Status::InvalidArgument;
      try
      {
        std::lock_guard<std::recursive_mutex> Lock(Mtx);
        Props[name] = value;
        return Status::Ok;
      }
      catch (...)
      {
        return Status::Fail;
      }
    }
    virtual RetCode BOSS_CALL EnumProperties(IEnum **props)
    {
      if (!props || *props)
        return Status::InvalidArgument;
      try
      {
        std::lock_guard<std::recursive_mutex> Lock(Mtx);
        auto PropEnum = Base<Enum>::Create();
        for (auto &prop : Props)
          PropEnum->AddItem(Base<NamedValue>::Create(prop.first, prop.second));
        return PropEnum.QueryInterface(props);
      }
      catch (...)
      {
        return Status::Fail;
      }
    }
    
    // ISerializable
    virtual RetCode BOSS_CALL Load(IIStream *stream)
    {
      if (!stream)
        return Status::InvalidArgument;
      try
      {
        std::lock_guard<std::recursive_mutex> Lock(Mtx);
        RefObjPtr<IBuffer> Buffer;
        if (stream->Read(Buffer.GetPPtr()) != Status::Ok)
          return Status::Fail;
        void const *Data = 0;
        UInt DataSize = 0;
        if (Buffer->GetSize(&DataSize) != Status::Ok || !DataSize ||
            Buffer->GetData(&Data) != Status::Ok)
        {
          return Status::Fail;
        }
        std::vector<char> Xml(reinterpret_cast<char const *>(Data),
                              reinterpret_cast<char const *>(Data) + DataSize);
        Xml.push_back(0);
        std::string NewRootName;
        PropPool NewProps;
        rapidxml::xml_document<> Doc;
        Doc.parse<rapidxml::parse_full>(&Xml[0]);
        auto const *RootNode = Doc.first_node();
        if (!RootNode)
          return Status::Fail;
        NewRootName.assign(RootNode->name(), RootNode->name_size());
        if (NewRootName.empty())
          return Status::Fail;
        for (auto const *i = RootNode->first_node() ; i ; i = i->next_sibling())
        {
          std::string Name(i->name(), i->name_size());
          if (Name.empty())
            return Status::Fail;
          NewProps[Name] = DeserializeItem(i);
        }
        std::swap(NewProps, Props);
        std::swap(NewRootName, RootName);
        return Status::Ok;
      }
      catch (...)
      {
        return Status::Fail;
      }
    }
    virtual RetCode BOSS_CALL Save(IOStream *stream)
    {
      if (!stream)
        return Status::InvalidArgument;
      try
      {
        std::lock_guard<std::recursive_mutex> Lock(Mtx);
        rapidxml::xml_document<> Doc;
        auto *RootNode = Doc.allocate_node(rapidxml::node_element, RootName.c_str());
        Doc.append_node(RootNode);
        for (auto &prop : Props)
          SerializeItem(prop.first, prop.second.Get(), &Doc, RootNode);
        std::stringstream Io;
        rapidxml::print(std::ostream_iterator<char>(Io), Doc, 0);
        std::string Xml(Io.str());
        return stream->Write(Xml.c_str(), Xml.length());
      }
      catch (...)
      {
        return Status::Fail;
      }
    }
    
  private:
    typedef RefObjPtr<IBase> IBasePtr;
    typedef std::unordered_map<std::string, IBasePtr> PropPool;
    
    mutable std::recursive_mutex Mtx;
    mutable PropPool Props;
    
    std::string RootName = "PropertyBag";

    typedef Private::XmlPropertyBagTags<Private::EmptyType> Tags;
    
    void SerializeItem(std::string const &name, IBase *prop,
                       rapidxml::xml_document<> *doc,
                       rapidxml::xml_node<> *node) const
    {
      {
        RefObjQIPtr<IEntityId> Id(prop);
        if (Id.Get())
        {
          UInt EntityId = 0;
          if (Id->GetId(&EntityId) != Status::Ok)
            throw std::runtime_error("Failed to get EntityId.");
          auto *StrVal = doc->allocate_string(ToString(EntityId).c_str());
          auto *StrName = doc->allocate_string(name.c_str());
          auto *NewNode = doc->allocate_node(rapidxml::node_element, StrName, StrVal);
          node->append_node(NewNode);
          auto *TypeAttr = doc->allocate_attribute(Tags::TypeTag, Tags::EntityIdTag);
          NewNode->append_attribute(TypeAttr);
          return;
        }
      }
      {
        RefObjQIPtr<IString> String(prop);
        if (String.Get())
        {
          auto *StrVal = doc->allocate_string(GetString(String.Get()).c_str());
          auto *StrName = doc->allocate_string(name.c_str());
          auto *NewNode = doc->allocate_node(rapidxml::node_element, StrName, StrVal);
          node->append_node(NewNode);
          auto *TypeAttr = doc->allocate_attribute(Tags::TypeTag, Tags::StringTag);
          NewNode->append_attribute(TypeAttr);
          return;
        }
      }
      {
        RefObjQIPtr<IEnum> Enum(prop);
        if (Enum.Get())
        {
          auto *StrName = doc->allocate_string(name.c_str());
          auto *NewNode = doc->allocate_node(rapidxml::node_element, StrName);
          node->append_node(NewNode);
          auto *TypeAttr = doc->allocate_attribute(Tags::TypeTag, Tags::EnumTag);
          NewNode->append_attribute(TypeAttr);
          if (Enum->Reset() != Status::Ok)
            throw std::runtime_error("Failed to reset enum position.");
          for (RefObjPtr<IBase> i ; Enum->Next(i.GetPPtr()) == Status::Ok ; i.Release())
            SerializeItem(Tags::ItemTag, i.Get(), doc, NewNode);
          return;
        }
      }
      {
        RefObjQIPtr<IPropertyBag> PropBag(prop);
        if (PropBag.Get())
        {
          auto *StrName = doc->allocate_string(name.c_str());
          auto *NewNode = doc->allocate_node(rapidxml::node_element, StrName);
          node->append_node(NewNode);
          auto *TypeAttr = doc->allocate_attribute(Tags::TypeTag, Tags::PropertyBagTag);
          NewNode->append_attribute(TypeAttr);
          RefObjPtr<IEnum> Enum;
          if (PropBag->EnumProperties(Enum.GetPPtr()) != Status::Ok)
            throw std::runtime_error("Failed to get PropertyBag.");
          if (Enum->Reset() != Status::Ok)
            throw std::runtime_error("Failed to reset enum position.");
          for (RefObjPtr<IBase> i ; Enum->Next(i.GetPPtr()) == Status::Ok ; i.Release())
          {
            RefObjQIPtr<INamedValue> Prop(i);
            if (!Prop.Get())
              throw std::runtime_error("Failed to get PropertyItem.");
            RefObjPtr<IString> Name;
            if (Prop->GetName(Name.GetPPtr()) != Status::Ok)
              throw std::runtime_error("Failed to get property name.");
            RefObjPtr<IBase> Value;
            if (Prop->GetValue(Value.GetPPtr()) != Status::Ok)
              throw std::runtime_error("Failed to get property value.");
            SerializeItem(GetString(Name.Get()), Value.Get(), doc, NewNode);
          }
          return;
        }
      }
    }
    
    std::string GetString(IString *str) const
    {
      RefObjPtr<IBuffer> StrBuffer;
      if (str->GetString(IString::AnsiString, StrBuffer.GetPPtr()) != Status::Ok)
        throw std::runtime_error("Failed to get string.");
      UInt StrLen = 0;
      if (StrBuffer->GetSize(&StrLen) != Status::Ok)
        throw std::runtime_error("Failed to get string length.");
      void const *StrData = 0;
      if (StrLen && StrBuffer->GetData(&StrData) != Status::Ok)
        throw std::runtime_error("Failed to get string data.");
      return std::move(std::string(reinterpret_cast<char const *>(StrData), StrLen));
    }
    
    IBasePtr DeserializeItem(rapidxml::xml_node<> const *node) const
    {
      auto const *TypeAttr = node->first_attribute(Tags::TypeTag);
      if (!TypeAttr)
        throw std::runtime_error("Failed to get item type attribute.");
      std::string Type(TypeAttr->value(), TypeAttr->value_size());
      if (Type.empty())
        throw std::runtime_error("Failed to get item type.");
      if (Type == Tags::EntityIdTag)
      {
        std::string Id(node->value(), node->value_size());
        if (Id.empty())
          throw std::runtime_error("Empty EntityId value.");
        return Base<EntityId>::Create(FromString<UInt>(Id));
      }
      if (Type == Tags::StringTag)
      {
        std::string Str(node->value(), node->value_size());
        if (Str.empty())
          throw std::runtime_error("Empty string value.");
        return Base<String>::Create(Str);
      }
      if (Type == Tags::EnumTag)
      {
        auto NewEnum = Base<Enum>::Create();
        for (auto const *i = node->first_node(Tags::ItemTag) ; i ; i = i->next_sibling(Tags::ItemTag))
          NewEnum->AddItem(DeserializeItem(i));
        return NewEnum;
      }
      if (Type == Tags::PropertyBagTag)
      {
        auto NewPropBag = Base<XmlPropertyBag>::Create(std::string(node->name(), node->name_size()));
        for (auto const *i = node->first_node() ; i ; i = i->next_sibling())
        {
          std::string Name(i->name(), i->name_size());
          if (Name.empty())
            throw std::runtime_error("Failed to get node name.");
          NewPropBag->Props[Name] = DeserializeItem(i);
        }
        return IBasePtr(RefObjQIPtr<IBase>(NewPropBag).Get());
      }
      throw std::runtime_error("Unknown type.");
    }
  };
  
}

#endif  // !__COMMON_XML_PROPERTY_BAG_H__
