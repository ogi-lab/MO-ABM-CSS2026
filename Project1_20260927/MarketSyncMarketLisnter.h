#pragma once

#include "GovernmentMarket.h"
#include "MarketListener.h"

/** 市場同期市場取引リスナ */
class MarketSyncMarketListener : public MarketListener {
    GovernmentMarket& governmentMarket;

public:
     MarketSyncMarketListener(GovernmentMarket& governmentMarket)
         : governmentMarket(governmentMarket) {}

    /**
     * 商品追加イベント
     *
     * @param item 追加された商品
     */
    void notify_item_added(Item* item) {
        governmentMarket.add_item(item);
    }

    /**
     * 商品回収イベント
     *
     * @param item 回収された商品
     */
    void notify_item_deleted(Item* item) {
        governmentMarket.delete_item(item);
    }

};
