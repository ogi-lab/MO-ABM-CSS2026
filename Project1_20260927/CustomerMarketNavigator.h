#pragma once
#include "AreaMarket.h"
#include "RetailMarket.h"
#include "RetailMarketNavigator.h"

/**
 * 消費者購買市場ナビゲータ
 *
 * 暫定的に１地域市場のみを扱う消費者購買用の最終消費財市場ナビゲータ
 */
class CustomerMarketNavigator : public RetailMarketNavigator {
    /** 扱う地域市場 */
    AreaMarket& market;
 public:
     CustomerMarketNavigator(AreaMarket& area_market) : market(area_market){}
    
    /**
     * 最終消費財市場取得
     * 引数にいれたarea_marketでmarketを初期化する
	 *　&marketはSimulatorAppクラスのretailMarketと同じアドレスとなる
     * 現在の実装は、常に自身が保持している地域市場を返します。
     *
     * @param agent_id エージェントID
     * @param item_type_id 商品種ID
     */
    //RetailMarket* get_retail_market(int agent_id, int item_type_id) {
     RetailMarket* get_retail_market(int item_type_id) {
        return &market;//retail_marketのアドレス
    }
};

