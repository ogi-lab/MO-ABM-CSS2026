#pragma once
#include "Shipment.h"
#include "SingleMarketShipment.h"
#include "SimulatorConfig.h"
#include "SimulatorContext.h"

extern SimulatorContext app_context;
/**
 * 出荷ナビゲータ実装
 *
 * 単一市場出荷を返す仮実装
 */
class ShipmentNavigatorImpl : public ShipmentNavigator {
    SingleMarketShipment* shipment;
public:
    ShipmentNavigatorImpl(AreaMarket& market) {
        shipment = new SingleMarketShipment(market);
		//fprintf(app_context.config->out2,"address of market=%x in SingleMarketShipment defined at ShipmentNavigaterImpl\n",market);
    }

    virtual ~ShipmentNavigatorImpl() {}

    /**
     * 出荷方法取得
     *
     * @param agent_id エージェントID
     * @param item_type_id 商品種ID
     */
   // virtual Shipment* get_shipment(int agent_id, int item_type_id) {
    Shipment* get_shipment(int agent_id, int item_type_id) {
        // 単一市場出荷を返す仮実装
        return shipment;
    }
};

