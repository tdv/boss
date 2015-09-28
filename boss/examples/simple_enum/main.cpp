//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     04.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#include "core/module.h"
#include "core/ibase.h"
#include "core/co_class.h"
#include "core/base.h"
#include "common/ienum.h"
#include "core/exceptions.h"
#include "common/enum.h"
#include "common/enum_helper.h"

#include <vector>
  #include <iostream>
  #include <iterator>

namespace MyNs
{
  
  namespace IFaces
  {

    struct IFixPrice
      : public Boss::Inherit<Boss::IBase>
    {
      BOSS_DECLARE_IFACEID_HEX(0x29AB589C)  // CRC32(MyNs.IFaces.IFixPrice)

      virtual Boss::RetCode BOSS_CALL GetPrice(double *price) const = 0;
    };
    
    struct IBestOffer
      : public Boss::Inherit<Boss::IBase>
    {
      BOSS_DECLARE_IFACEID_HEX(0x06F972B9)  // CRC32(MyNs.IFaces.IBestOffer)

      virtual Boss::RetCode BOSS_CALL MyOffer(double price) = 0;
      virtual Boss::RetCode BOSS_CALL GetAllOffers(Boss::IEnum **offers) const = 0;
    };
    
    struct IAuction
      : public Boss::Inherit<Boss::IBase>
    {
      BOSS_DECLARE_IFACEID_HEX(0xCCB08104)  // CRC32(MyNs.IFaces.IAuction)

      virtual Boss::RetCode BOSS_CALL MyBet(double bet) = 0;
    };
    
  }
  
  namespace Impl
  {
    
    BOSS_DECLARE_RUNTIME_EXCEPTION(FixPrice)

    class FixPrice
      : public Boss::SimpleCoClass<IFaces::IFixPrice>
    {
    public:
      FixPrice(double price = 0)
        : Price(price)
      {
        if (Price < 0)
          throw FixPriceException("Invalid price.");
      }
      
    private:
      double Price;
      
      // IFixPrice
      virtual Boss::RetCode BOSS_CALL GetPrice(double *price) const
      {
        if (!price)
          return Boss::Status::InvalidArgument;
        *price = Price;
        return Boss::Status::Ok;
      }
      
    protected:
      void SetPrice(double price)
      {
        if (price < 0)
          throw FixPriceException("Invalid price.");
        Price = price;
      }
      double GetPriceValue() const
      {
        return Price;
      }
    };
    
    class BestOffer
      : public Boss::SimpleCoClass<FixPrice, IFaces::IBestOffer>
    {
    public:
      BestOffer(double price)
      {
        SetPrice(price);
      }
      
    private:
      std::vector<double> Prices;
      // IBestOffer
      virtual Boss::RetCode BOSS_CALL MyOffer(double price)
      {
        if (price < 0)
          return Boss::Status::InvalidArgument;
        try
        {
          Prices.push_back(price);
        }
        catch (std::exception const &)
        {
          return Boss::Status::Fail;
        }
        return Boss::Status::Ok;
      }
      virtual Boss::RetCode BOSS_CALL GetAllOffers(Boss::IEnum **offers) const
      {
        try
        {
          auto Offers = Boss::Base<Boss::Enum>::Create();
          for (auto i : Prices)
          {
            Offers->AddItem(Boss::Base<FixPrice>::Create(i));
          }
          return Offers.QueryInterface(offers);
        }
        catch (std::exception const &)
        {
        }
        return Boss::Status::Fail;
      }
    };
    
    class Auction
      : public Boss::SimpleCoClass<FixPrice, IFaces::IAuction>
    {
    public:
      Auction(double price)
      {
        SetPrice(price);
      }
      
    private:
      virtual Boss::RetCode BOSS_CALL MyBet(double bet)
      {
        if (GetPriceValue() < bet)
        {
          SetPrice(bet);
          return Boss::Status::Ok;
        }
        return Boss::Status::False;
      }
    };
    
  }
  
}

Boss::RefObjPtr<Boss::IEnum> CreateTestCollection()
{
  auto Collection = Boss::Base<Boss::Enum>::Create();
  // FixPrice
  Collection->AddItem(Boss::Base<MyNs::Impl::FixPrice>::Create(10));
  Collection->AddItem(Boss::Base<MyNs::Impl::FixPrice>::Create(15));
  // BestOffer
  Collection->AddItem(Boss::Base<MyNs::Impl::BestOffer>::Create(7));
  // Auction
  Collection->AddItem(Boss::Base<MyNs::Impl::Auction>::Create(42));
  Collection->AddItem(Boss::Base<MyNs::Impl::Auction>::Create(17));
  Collection->AddItem(Boss::Base<MyNs::Impl::Auction>::Create(55));
  return Collection;
}

void ShowAllObjects(Boss::RefObjPtr<Boss::IEnum> collection)
{
  Boss::EnumHelper<Boss::IBase> Collection(collection);
  for (auto i = Collection.First() ; i.Get() ; i = Collection.Next())
  {
    double Price = 0;
    {
      Boss::RefObjQIPtr<MyNs::IFaces::IFixPrice> FixPrice(i);
      if (!FixPrice.Get())
        throw std::runtime_error("Unknown object.");
      if (FixPrice->GetPrice(&Price) != Boss::Status::Ok)
        throw std::runtime_error("Failed to get price.");
    }
    std::string ObjectType = "FixPrice";
    Boss::RefObjQIPtr<MyNs::IFaces::IBestOffer> BestOffer(i);
    std::vector<double> Offers;
    if (BestOffer.Get())
    {
      ObjectType = "BestOffer";
      Boss::RefObjPtr<Boss::IEnum> AllOffers;
      if (BestOffer->GetAllOffers(AllOffers.GetPPtr()) != Boss::Status::Ok)
        throw std::runtime_error("Failed to get all offers for \"BestOffer\" object.");
      Boss::EnumHelper<MyNs::IFaces::IFixPrice> OffersEnum(AllOffers);
      for (auto j = OffersEnum.First() ; j.Get() ; j = OffersEnum.Next())
      {
        double OfferPrice = 0;
        if (j->GetPrice(&OfferPrice) != Boss::Status::Ok)
          throw std::runtime_error("Failed to get offer price.");
        Offers.push_back(OfferPrice);
      }
    }
    else
    {
      Boss::RefObjQIPtr<MyNs::IFaces::IAuction> Auction(i);
      if (Auction.Get())
        ObjectType = "Auction";
    }
    std::cout << "ObjectType: " << ObjectType << std::endl;
    std::cout << "\tPrice: " << Price << std::endl;
    if (!Offers.empty())
    {
      std::cout << "\tOffers:\n\t\t";
      std::copy(std::begin(Offers), std::end(Offers),
                std::ostream_iterator<double>(std::cout, "\n\t\t"));
    }
    std::cout << std::endl;
  }
}

void SetMyOffers(Boss::RefObjPtr<Boss::IEnum> collection,
                 unsigned minPercent, unsigned maxPercent)
{
  Boss::EnumHelper<MyNs::IFaces::IBestOffer> Collection(collection);
  for (auto i = Collection.First(); i.Get() ; i = Collection.Next())
  {
    Boss::RefObjQIPtr<MyNs::IFaces::IFixPrice> FixPrice(i);
    if (!FixPrice.Get())
      throw std::runtime_error("Failed to get price object.");
    double Price = 0;
    if (FixPrice->GetPrice(&Price) != Boss::Status::Ok)
      throw std::runtime_error("Failed to get price for \"BestOffer\".");
    if (i->MyOffer(Price + (Price * minPercent) / 100) != Boss::Status::Ok)
      throw std::runtime_error("Failed to set my min offer for \"BestOffer\".");
    if (i->MyOffer(Price + (Price * maxPercent) / 100) != Boss::Status::Ok)
      throw std::runtime_error("Failed to set my max offer for \"BestOffer\".");
  }
}

void SetMyBets(Boss::RefObjPtr<Boss::IEnum> collection, unsigned percent)
{
  Boss::EnumHelper<MyNs::IFaces::IAuction> Collection(collection);
  for (auto i = Collection.First() ; i.Get() ; i = Collection.Next())
  {
    Boss::RefObjQIPtr<MyNs::IFaces::IFixPrice> FixPrice(i);
    if (!FixPrice.Get())
      throw std::runtime_error("Failed to get price object.");
    double Price = 0;
    if (FixPrice->GetPrice(&Price) != Boss::Status::Ok)
      throw std::runtime_error("Failed to get price for \"BestOffer\".");
    if (i->MyBet(Price + (Price * percent) / 100) != Boss::Status::Ok)
      throw std::runtime_error("Failed to set my bet for \"Auction\".");
  }
}

int main()
{
  try
  {
    auto Collection = CreateTestCollection();
    std::cout << "--------------- Before --------------- " << std::endl;
    ShowAllObjects(Collection);
    SetMyOffers(Collection, 5, 7);
    SetMyBets(Collection, 7);
    std::cout << "--------------- After --------------- " << std::endl;
    ShowAllObjects(Collection);
  }
  catch (std::exception const &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
