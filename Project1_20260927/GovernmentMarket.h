#pragma once

#include <iostream>
#include <set>
#include "Destination.h"
#include "Globals.h"
#include "Item.h"
#include "SimulatorConfig.h"
#include "SimulatorContext.h"

extern SimulatorContext app_context;
using namespace std;


/** 政府購買市場 */
class GovernmentMarket {

    /** 価格によってソートしながら保持する製品集合(最終消費財集合) */
    ITEM_SET item_set;

public:
    GovernmentMarket() {}
    ~GovernmentMarket() {}

    /**
     * 商品選択
     *
     * @return 選択した商品。該当無しの場合はNULL。
     */
    Item* select_one() {
        if (item_set.begin() == item_set.end()) {
            return NULL;
        }

        return *item_set.begin();
    }

    /**
     * 商品追加
     *
     * @param item 追加対象商品
     */
    void add_item(Item* item) {

       // printf("[g]--- add item [%d,%d]\n", item->get_item_type_id(), item->get_price());
	 // fprintf(app_context.config->out,"[g]--- add item [%d,%d]\n", item->get_item_type_id(), item->get_price());

        item_set.insert(item);
    }

    /**
     * 商品回収
     *
     * @param item 回収対象商品
     */
    void delete_item(Item* item) {

        //printf("[g]--- delete item [%d,%d]\n", item->get_item_type_id(), item->get_price());
	//	fprintf(app_context.config->out,"[g]--- delete item [%d,%d]\n", item->get_item_type_id(), item->get_price());

        // 同一最安値のオブジェクトが複数とれるので、同一オブジェクトを判定して１つだけ削除する
        ITEM_SET::iterator start;
        for (start = item_set.find(item); start != item_set.end(); start++) {
            if (*start == item) { // 該当要素かどうかを確認
                ITEM_SET::iterator end = start;
				end++;
				item_set.erase(start, end); // 1要素だけ削除する
				break;
			}
		}
    }

    void print_all_items() {
    //    cout << "--- all items in GovernmentMarket ----\n" << endl;

        ITEM_SET::iterator it;
        for ( it=item_set.begin(); it != item_set.end(); it++ ){
        //    cout << (*it)->get_item_type_id() << "-" << (*it)->get_price() << endl;
        }
    }
};


