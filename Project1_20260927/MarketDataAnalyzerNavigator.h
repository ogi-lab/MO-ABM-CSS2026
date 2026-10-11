#pragma once
#include "MarketDataAnalyzer.h"
#include "DataAnalyzer.h"
#include "DataAnalyzerNavigator.h"

/**
 * 消費者購買市場ナビゲータ
 *
 * 暫定的に１地域市場のみを扱う消費者購買用の最終消費財市場ナビゲータ
 */
class MarketDataAnalyzerNavigator : public DataAnalyzerNavigator {
    /** 扱うdata */
    MarketDataAnalyzer& marketdataAnalyzer;
 public:
     MarketDataAnalyzerNavigator(MarketDataAnalyzer& marketdataAnalyzer) : marketdataAnalyzer(marketdataAnalyzer){
	  // printf("address of marketdataAnalyzer =%x in navigator\n",&marketdataAnalyzer);
	 }
    
    /**
     * 最終消費財市場取得
     * 引数にいれたarea_marketでmarketを初期化する
	 *　&marketはSimulatorAppクラスのretailMarketと同じアドレスとなる
     * 現在の実装は、常に自身が保持している地域市場を返します。
     *
     * @param agent_id エージェントID
     * @param item_type_id 商品種ID
     */
    DataAnalyzer* get_dataAnalyzer() {
        return &marketdataAnalyzer;
    }
};
