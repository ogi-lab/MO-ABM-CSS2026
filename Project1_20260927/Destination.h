#pragma once
#include "Item.h"
//#include "MarketDataAnalyzer.h"
//class MarketDataAnalyer;

/** 出荷先 */
class Destination {
    //MarketDataAnalyzer* marketdataAnalyzer;/** <市場データ解析者
  
public:
    virtual ~Destination() {}

    /**
     *  商品回収
     *
     *  @param item 商品
     */
    virtual void delete_item(Item* item) = 0;

    /**
     *  価格改定通知
     *
     *  @param item 商品
     */
    virtual void notify_price_changed(Item* item, long long new_price) = 0;
  
    /*
    * MarketDataAnalyzer の取得
    * 
    */
  //  virtual MarketDataAnalyzer* get_marketdataAnalyzer() = 0;
    virtual int get_market_type() = 0;


};

