#pragma once
#include "DataAnalyzer.h"

/** 最終消費財市場ナビゲータ */
class DataAnalyzerNavigator {
public:
    virtual ~DataAnalyzerNavigator() {}

    /**
     * 最終消費財市場取得
     *
     * @param agent_id エージェントID
     * @param item_type_id 商品種ID
     */
    virtual DataAnalyzer* get_dataAnalyzer() = 0;
};