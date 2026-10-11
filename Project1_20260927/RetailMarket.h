#pragma once
#include "Item.h"

/** 最終消費財市場 */
class RetailMarket {
public:
    virtual ~RetailMarket() {}

    /**
     * 商品選択
     *
     * @param item_type_id 商品種ID
     */
    virtual Item* select_one(int item_type_id) = 0;
    virtual int get_ITEM_SET_size(int item_type_id) { return 13; }
    virtual void materialMarket_print_all_items(int index) {};
    virtual long long get_ave_price(int market_type, int item_type_id) = 0;
    virtual long long get_ave_price(int item_type_id) = 0;
    virtual int get_size_of_stock_in_market(int item_type_id, int agent_id) { return 0; }
};

