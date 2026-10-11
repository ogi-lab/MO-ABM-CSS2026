#pragma once
#include "RetailMarket.h"

/** 最終消費財市場ナビゲータ */
class RetailMarketNavigator {
public:
    virtual ~RetailMarketNavigator() {}
        //子クラスはCustomerMarketNavigator.h
    /**
     * 最終消費財市場取得
     *
     * @param agent_id エージェントID
     * @param item_type_id 商品種ID
     */
    //virtual RetailMarket* get_retail_market(int agent_id, int item_type_id) = 0;
    virtual RetailMarket* get_retail_market( int item_type_id) = 0;
};

