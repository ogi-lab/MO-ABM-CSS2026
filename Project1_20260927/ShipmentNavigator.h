#pragma once
#include "Shipment.h"

/** 出荷ナビゲータ */
class ShipmentNavigator {
public:
    virtual ~ShipmentNavigator() {}

    /**
     * 出荷方法取得
     *
     * @param agent_id エージェントID
     * @param item_type_id 商品種ID
     */
    virtual Shipment* get_shipment(int agent_id, int item_type_id) = 0;
};

