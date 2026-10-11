#pragma once

#include <vector>
#include "Item.h"
#include "Seller.h"
#include "Shipment.h"
#include "SimulatorConfig.h"
#include "SimulatorContext.h"

extern SimulatorContext app_context;

using namespace std;

/** 単一市場出荷 */
class SingleMarketShipment : public Shipment {
    /** 取り扱う市域市場*/
    AreaMarket& market;
	//MarketDataAnallyzer* marketdataAnalyzler;
public:
    SingleMarketShipment(AreaMarket& retail_market) : market(retail_market){}
	//initializer: marketをretail_marketで初期化する
	//ということはmarketのアドレスはretail_marketのアドレスと同じはず
    virtual ~SingleMarketShipment() {}
   /**
     * 出荷
     *
     * @param item 出荷対象商品
     * @param seller 販売者
     */
	
    void ship(Item* item, Seller* seller){

        // 販売者をセットして市場に追加
        item->set_seller(seller);		//販売者として自分（企業）の参照（この関数を呼び出したときの引数）をセットする   20120523記　高島
        market.add_item(item);			//市場への出荷、marketは宣言時に当てている（現在はAreaMarket　  20120523記　高島
		//&market=399c50
		//fprintf(app_context.config->out_temporary,"seller_id=%d がmarket=%xへ出荷しました\n",seller->Get_agent_id(),&market);
		//marketdataAnalyzerのsuppliedItemのvectorに出荷製品情報を格納する(2012_5_1荻林追加,2012_5_20荻林Item経由に修正）
		//market.get_marketdataAnalyzer()->set_suppliedItem(app_context.period_count,item->get_item_type_id(),item->get_price(),item->Get_seller()->Get_agent_id(),item->Get_seller()->get_qmax(item->get_item_type_id()));
		//item->get_dataAnalyzer()->set_dataAnalyzer_type(1);
		item->get_dataAnalyzer()->set_suppliedItem(app_context.period_count,item->get_item_type_id(),int (item->get_price()),item->Get_seller()->Get_agent_id(),item->Get_seller()->get_qmax(item->get_item_type_id()));
		//fprintf(app_context.config->out_temporary, "dataAnalyzer address=%x market=%x\n",item->get_dataAnalyzer(),&market);
		// 企業の在庫に追加
        seller->add_stock(item);		//自分の在庫集合に追加　所管Producer　  20120523記　高島

	   }
	

    /**
     * 出荷
     *
     * @param items 出荷対象商品
     * @param seller 販売者
     */
    void ship(vector<Item*> items, Seller* seller){
		//一度に複数個出荷するための関数、2012_3_30白木氏作成
		//(app_context.config->out_temporary,"seller %d の在庫にitem_type_no=%d price=%dの製品を %d 個追加します。出荷先market=%x\n",seller->Get_agent_id(),items[0]->get_item_type_id(),items[0]->get_price(),items.size(),&market);
		for (int i = 0; i < int(items.size());i++) {
			ship(items[i], seller);
			//if (i == 0) fprintf(app_context.config->out_temporary, "seller_id=%d destination address=%x\n", seller->Get_agent_id(), items[i]->Get_destination());
		}
		//fprintf(app_context.config->out,"seller %d の在庫に追加しました　item_type_no=%d price=%d\n",seller->Get_agent_id(),items[0]->get_item_type_id(),items[0]->get_price());
	}

	/**
     * 価格改定
     *
     * @param item 出荷対象商品
     * @param seller 販売者
     */

	void price_change(Item* item ,long long new_price)
	{
		//shipment経由で価格の改定を行う高島作成（その後Item経由で価格の改定を行うように変更（荻林）したので今は使っていいない関数）
		market.notify_price_changed(item,new_price);
		//fprintf(app_context.config->out2,"address of market %x in SingleMarketShipment price_change\n",&market);
 	
		//fprintf(app_context.config->out,"seller %d の在庫に追加しました　item_type_no=%d price=%d\n",item->Get_seller()->Get_agent_id(),item->get_item_type_id(),item->get_price());
	}
	
};

