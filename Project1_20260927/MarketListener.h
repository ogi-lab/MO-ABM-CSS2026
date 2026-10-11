#pragma once

#include "Item.h"

/** 市場取引リスナ */
class MarketListener {
public:
    virtual ~MarketListener() {}

    /**
     * 商品追加イベント
     *
     * @param item 追加された商品
     */
    virtual void notify_item_added(Item* item) = 0;

    /**
     * 商品回収イベント
     *
     * @param item 回収された商品
     */
    virtual void notify_item_deleted(Item* item) = 0;
};

