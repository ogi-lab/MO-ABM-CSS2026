#pragma once

#include <vector>
#include "Item.h"
#include "Seller.h"

using namespace std;

/** 出荷方法 */
class Shipment {
public:
    virtual ~Shipment() {}
    //派生型は、ShipmentNavigatorImpl, SingleMarketshipment

    //TODO 複数商品を１度に扱うように変更
	
    /**
     * 出荷
     *
     * @param item 出荷対象商品
     * @param seller 販売者
     */
    virtual void ship(Item* item, Seller* seller) = 0;

    /**
     * 出荷
     *
     * @param items 出荷対象商品
     * @param seller 販売者
     */
    virtual void ship(vector<Item*> items, Seller* seller) = 0;
		//一度に複数個出荷するための関数、2012_3_30白木氏作成
	/**
	 * 価格改定
     *
     * @param items 価格改定対象商品
     * @param seller 販売者
     */
	virtual void price_change(Item* item ,long long  new_price) = 0 ; 
	  //shipment経由で価格改定するための関数、高島作成、その後Item経由で価格改定するように変更（荻林）したので今は使っていない

};

