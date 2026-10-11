#pragma once

#include <iostream>
#include <list>
#include <map>
#include <set>
#include "Globals.h"
#include "MarketListener.h"
#include "RetailMarket.h"
#include "Destination.h"
#include "Item.h"
#include "PredForItemByPrice.h"
#include "SimulatorConfig.h"
#include "SimulatorContext.h"
#include "MarketDataAnalyzer.h"
#include "MarketDataAnalyzerNavigator.h"
#include "MarketData.h"
extern SimulatorContext app_context;
using namespace std;


/** 地域市場 */
class AreaMarket : public RetailMarket, public Destination {
        //市場がretailMarketかmaterialMarketかはオブジェクトとして区別される。クラスとしては同じ
    int market_type = 0;

	MarketDataAnalyzer* marketdataAnalyzer;/** <市場データ解析者

    /** 市場取引リスナ */
    list<MarketListener*> listeners;


    /** 価格によってソートしながら保持する製品集合(商品種ID別集合） */
    map<int, ITEM_SET*> item_map;//市場にある商品の集合

    /**
     * 指定した商品種IDに対応する商品集合を返す
     *
     * @param item_map->first はitem_type_id 商品種ID
     * @return item_map->secondは商品集合（ITEM_SET)
     */
    ITEM_SET* get_item_set(int item_type_id) {

        map<int, ITEM_SET*>::iterator it = item_map.find(item_type_id);

        // setがmap内になければ作成して登録して返す
        if (it == item_map.end()) {
             ITEM_SET* new_item_set = new ITEM_SET();
             item_map.insert(pair<int, ITEM_SET*>(item_type_id, new_item_set));
             return new_item_set;
        }
        return it->second;		//要素の値（Item setを返している）secondはmapにおける要素の値、fistはmapの要素のキー　  20120524記　高島
    }
	/*DataAnalyzer* get_dataAnalyzer() {
		DataAnalyzerNavigator* data_navi=app_context.dataAnalyzer_navi_for_retailMarket;
		DataAnalyzer* dataAnalyzer=data_navi->get_dataAnalyzer();
		return dataAnalyzer;
	}*/
    vector<long long>ave_price_list;//製品種毎の期首平均価格を格納する

public:
    AreaMarket() {  }
    ~AreaMarket() {}
    int get_market_type() { return market_type; }
    void set_market_type(int market_type) { this->market_type = market_type; }
     // void print_map_size() { printf("inside AreaMarket ITEM_SET(10)_size=%d\n", get_item_set(10)->size()); }
    /*
    long long get_ave_price(int item_type_id) {
        //ave_price_listはAreaMarketオブジェクトに固有のもの
        int size = ave_price_list.size();
        int size2 = 0;
        long long ave_price = 0;
        if (get_market_type() == 1){
                size2 = app_context.config->item_type_idk.size(); 
				//item_type_idk: retailer及びsoleproprietor生産品種 SimulatorConfig.h参照
                if (size != size2) fprintf(app_context.config->error_message, "vectorのサイズが等しくありません, ave_price_list.size()=,%d,item_type_idk.size()=,%d, \n", ave_price_list.size(), app_context.config->item_type_idk.size());
            for (int i = 0; i < size; i++) {
                 if (app_context.config->item_type_idk[i] == item_type_id) ave_price = ave_price_list[i];
            }
        }
        else if (get_market_type() == 2) {
            size2 = app_context.config->item_type_idkw.size();
            if (size != size2) fprintf(app_context.config->error_message, "vectorのサイズが等しくありません, ave_price_list.size()=,%d,item_type_idkw.size()=,%d, \n", ave_price_list.size(), app_context.config->item_type_idkw.size());
            for (int i = 0; i < size; i++) {
                if (app_context.config->item_type_idkw[i] == item_type_id) ave_price = ave_price_list[i];
            }
        }
        return ave_price;
    }
    */
    long long get_ave_price(int item_type_id) {
        //ave_price_listはAreaMarketオブジェクトに固有のもの
        int size = ave_price_list.size();
        int size2 = 0;
        long long ave_price = 0;  // 初期化を追加
        if (get_market_type() == 1) {
            size2 = app_context.config->item_type_idk.size();
            if (size != size2) fprintf(app_context.config->error_message, "vectorのサイズが等しくありません, ave_price_list.size()=,%d,item_type_idk.size()=,%d, \n", ave_price_list.size(), app_context.config->item_type_idk.size());
            for (int i = 0; i < size; i++) {
                if (app_context.config->item_type_idk[i] == item_type_id) {
                    ave_price = ave_price_list[i];
                    break;  // 見つかったら終了
                }
            }
        }
        else if (get_market_type() == 2) {
            size2 = app_context.config->item_type_idkw.size();
            if (size != size2) fprintf(app_context.config->error_message, "vectorのサイズが等しくありません, ave_price_list.size()=,%d,item_type_idkw.size()=,%d, \n", ave_price_list.size(), app_context.config->item_type_idkw.size());
            for (int i = 0; i < size; i++) {
                if (app_context.config->item_type_idkw[i] == item_type_id) {
                    ave_price = ave_price_list[i];
                    break;  // 見つかったら終了
                }
            }
        }
        return ave_price;
    }



    void define_ave_price_in_market() {
		//SimulatorAppのprocess_periodで生産開始前期首の在庫状況に基づき計算される
        //vector  ave_price_listの各要素を計算して格納する
        int item_type_id = 0;
        if (get_market_type() == 1) {
            fprintf(app_context.config->retailMarketData->out_ave_price_list, "%d,", app_context.period_count);
            if (ave_price_list.size() == 0){
                for (int i = 0; i < app_context.config->item_type_id_count; i++)
					//item_type_id_count = item_type_id_count_r + item_type_id_count_s; SimulatorConfig.h参照 
                    ave_price_list.push_back(0);
            }
            for (int i=0; i <app_context.config-> item_type_id_count; i++) {
                item_type_id = app_context.config->item_type_idk[i];
                if (get_ITEM_SET_size(item_type_id) == 0) {
					ave_price_list[i] = 0;//在庫がなければ平均価格は0   とする。
                }
                else {
                    ave_price_list[i] = get_ave_price(get_market_type(), item_type_id);
                    //498行目  get_ave_price(get_market_type(), item_type_id)の定義;   
                }
                fprintf(app_context.config->retailMarketData->out_ave_price_list, "%d,%I64d,", item_type_id,ave_price_list[i]);
            }
            fprintf(app_context.config->retailMarketData->out_ave_price_list, "\n");
        }
        else if (get_market_type() == 2) {
            if (ave_price_list.size() == 0) {
                for (int i = 0; i < app_context.config->item_type_id_count_w; i++)
                    ave_price_list.push_back(0);
            }
            fprintf(app_context.config->materialMarketData->out_ave_price_list, "%d,", app_context.period_count);
            for (int i=0; i < app_context.config->item_type_id_count_w; i++) {
                item_type_id = app_context.config->item_type_idkw[i];
                if (get_ITEM_SET_size(item_type_id) == 0) {
                    ave_price_list[i] = 0;
                }
                else {
                    ave_price_list[i] = get_ave_price(get_market_type(), item_type_id);
                }
                fprintf(app_context.config->materialMarketData->out_ave_price_list, "%d,%I64d,", item_type_id, ave_price_list[i]);
            }
            fprintf(app_context.config->materialMarketData->out_ave_price_list, "\n");
        }
    }
    int get_ITEM_SET_size(int item_type_id) {
        return get_item_set(item_type_id)->size();
    }
    
    void print_ITEM_SET_size(int index) {
        //各item_type_id毎にget_ITEM_SET_size(item_type_id)をプリントアウトする。
        //SimulatorAppで呼ばれる（retailMarketかmaterialMarketかを指定して）
        //index=1 before production  index=2 after production before buying  index=3 after buying
        int item_type_id = 0;
        if (get_market_type() == 1) {
            if (index == 1) {
                fprintf(app_context.config->retailMarketData->ITEM_SET_size_1, " %d,", app_context.period_count);
                for (int i = 0; i < app_context.config->item_type_id_count; i++) {
                    item_type_id = app_context.config->item_type_idk[i];
                    fprintf(app_context.config->retailMarketData->ITEM_SET_size_1, " %d, %d, ", item_type_id, get_ITEM_SET_size(item_type_id));
                }
                fprintf(app_context.config->retailMarketData->ITEM_SET_size_1, "\n");
            }
            else if (index == 2) {
                fprintf(app_context.config->retailMarketData->ITEM_SET_size_2, " %d,", app_context.period_count);
                for (int i = 0; i < app_context.config->item_type_id_count; i++) {
                    item_type_id = app_context.config->item_type_idk[i];
                    fprintf(app_context.config->retailMarketData->ITEM_SET_size_2, " %d, %d, ", item_type_id, get_ITEM_SET_size(item_type_id));
                }
                fprintf(app_context.config->retailMarketData->ITEM_SET_size_2, "\n");
            }
            else if (index == 3) {
                fprintf(app_context.config->retailMarketData->ITEM_SET_size_3, " %d,", app_context.period_count);
                for (int i = 0; i < app_context.config->item_type_id_count; i++) {
                    item_type_id = app_context.config->item_type_idk[i];
                    fprintf(app_context.config->retailMarketData->ITEM_SET_size_3, " %d, %d, ", item_type_id, get_ITEM_SET_size(item_type_id));
                }
                fprintf(app_context.config->retailMarketData->ITEM_SET_size_3, "\n");
            }
            else {}
        }
        else if (get_market_type() == 2) {
           // printf("inside printITEM_SET\n");
            if (index == 1) {
                fprintf(app_context.config->materialMarketData->ITEM_SET_size_1, " %d,", app_context.period_count);
                for (int i = 0; i < app_context.config->item_type_id_count_w; i++) {
                    item_type_id = app_context.config->item_type_idkw[i];
                    fprintf(app_context.config->materialMarketData->ITEM_SET_size_1, " %d, %d, ", item_type_id, get_ITEM_SET_size(item_type_id));
                }
                fprintf(app_context.config->materialMarketData->ITEM_SET_size_1, "\n");
            }
            else if (index == 2) {
                fprintf(app_context.config->materialMarketData->ITEM_SET_size_2, " %d,", app_context.period_count);
                for (int i = 0; i < app_context.config->item_type_id_count_w; i++) {
                    item_type_id = app_context.config->item_type_idkw[i];
                    fprintf(app_context.config->materialMarketData->ITEM_SET_size_2, " %d, %d, ", item_type_id, get_ITEM_SET_size(item_type_id));
                }
                fprintf(app_context.config->materialMarketData->ITEM_SET_size_2, "\n");
            }
            else if (index == 3) {
                fprintf(app_context.config->materialMarketData->ITEM_SET_size_3, " %d,", app_context.period_count);
                for (int i = 0; i < app_context.config->item_type_id_count_w; i++) {
                    item_type_id = app_context.config->item_type_idkw[i];
                    fprintf(app_context.config->materialMarketData->ITEM_SET_size_3, " %d, %d, ", item_type_id, get_ITEM_SET_size(item_type_id));
                }
                fprintf(app_context.config->materialMarketData->ITEM_SET_size_3, "\n");
            }
            else {}
        }
        else {}
    }
    void set_up() {
        marketdataAnalyzer = new MarketDataAnalyzer();
        app_context.dataAnalyzer_navi_for_retailMarket = new MarketDataAnalyzerNavigator(*marketdataAnalyzer); //app_context内にアドレス参照を渡して生成
        marketdataAnalyzer->set_up();	//boughtRecord.set_up();suppliedRecord.set_up();stockRecord.set_up();areamarketRecord.set_up();
        fprintf(app_context.config->retailMarketData->areamarket1, "生産開始前期首在庫リスト,\n"); fprintf(app_context.config->retailMarketData->areamarket1, "period, item_type_id,price,seller_id,\n");
        fprintf(app_context.config->retailMarketData->areamarket2, "生産後購買前在庫リスト,\n");
        fprintf(app_context.config->retailMarketData->areamarket3, "購買後期末在庫リスト,\n");
        fprintf(app_context.config->materialMarketData->areamarket1, "生産開始前期首在庫リスト,\n"); fprintf(app_context.config->materialMarketData->areamarket1, "period, item_type_id,price,seller_id,\n");
        fprintf(app_context.config->materialMarketData->areamarket2, "生産後購買前在庫リスト,\n");
        fprintf(app_context.config->materialMarketData->areamarket3, "購買後期末在庫リスト,\n");
    }
    void retailMarket_set_up() {
        //market_type = 1;
        set_market_type(1);
        marketdataAnalyzer = new MarketDataAnalyzer();
        printf("address of marketdataAnalyzer in retailMarket=%x\n", unsigned (marketdataAnalyzer));
        //retailMarket->set_dataAnalyzer(marketdataAnalyzer);
        app_context.dataAnalyzer_navi_for_retailMarket = new MarketDataAnalyzerNavigator(*marketdataAnalyzer); //app_context内にアドレス参照を渡して生成
        //printf("address of marketdataAnalyzer =%x in SimulatorApp\n",marketdataAnalyzer);
        marketdataAnalyzer->set_market_type(1);
        marketdataAnalyzer->set_dataAnalyzer_type(1);
        marketdataAnalyzer->set_up();	//boughtRecord.set_up();suppliedRecord.set_up();stockRecord.set_up();areamarketRecord.set_up();
        fprintf(app_context.config->out_temporary, "inside retailMarket_set_up() dataAnalyzer_type=%d", marketdataAnalyzer->get_dataAnalyzer_type());
        fprintf(app_context.config->retailMarketData->areamarket1, "生産開始前期首在庫リスト,\n"); fprintf(app_context.config->retailMarketData->areamarket1, "period, item_type_id,price,seller_id,\n");
        fprintf(app_context.config->retailMarketData->areamarket2, "生産後購買前在庫リスト,\n"); fprintf(app_context.config->retailMarketData->areamarket2, "period, item_type_id,price,seller_id,\n");
        fprintf(app_context.config->retailMarketData->areamarket3, "購買後期末在庫リスト,\n"); fprintf(app_context.config->retailMarketData->areamarket3, "period, item_type_id,price,seller_id,\n");
       // fprintf(app_context.config->out_temporary, "inside retailMarket_set_up market_type=%d address of marketdataAnalyzer =%x\n",market_type,marketdataAnalyzer);
        fprintf(app_context.config->retailMarketData->ITEM_SET_size_1, "ITEM_SET size before production,\n  period, item_type_id, ITEM_SET_size,\n");
        fprintf(app_context.config->retailMarketData->ITEM_SET_size_2, " ITEM_SET size after production before buying ,\n period, item_type_id, ITEM_SET_size,\n");
        fprintf(app_context.config->retailMarketData->ITEM_SET_size_3, " ITEM_SET size after buying ,\n period, item_type_id, ITEM_SET_size,\n");
    }
    void materialMarket_set_up() {
        //printf("inside areamarket  materialmarket_wet_up\n");
        //market_type = 2;
        set_market_type(2);
        marketdataAnalyzer = new MarketDataAnalyzer();
        printf("address of marketdataAnalyzer in materiallMarket=%x\n", unsigned (marketdataAnalyzer));
        //retailMarket->set_dataAnalyzer(marketdataAnalyzer);
        app_context.dataAnalyzer_navi_for_materialMarket = new MarketDataAnalyzerNavigator(*marketdataAnalyzer); //app_context内にアドレス参照を渡して生成
        //printf("address of marketdataAnalyzer =%x in SimulatorApp\n",marketdataAnalyzer);
        marketdataAnalyzer->set_market_type(2);
        marketdataAnalyzer->set_dataAnalyzer_type(2);
        //printf("before set_up\n");
        marketdataAnalyzer->set_up();	//boughtRecord.set_up();suppliedRecord.set_up();stockRecord.set_up();areamarketRecord.set_up(); 
        //printf("after marektdataAnalyzer_set_up\n");
        fprintf(app_context.config->out_temporary, "inside materialMarket_set_up() dataAnalyzer_type=%d", marketdataAnalyzer->get_dataAnalyzer_type());
        fprintf(app_context.config->materialMarketData->areamarket1, "生産開始前期首在庫リスト,\n"); fprintf(app_context.config->materialMarketData->areamarket1, "period, item_type_id,price,seller_id,\n");
        fprintf(app_context.config->materialMarketData->areamarket2, "生産後購買前在庫リスト,\n"); fprintf(app_context.config->materialMarketData->areamarket2, "period, item_type_id,price,seller_id,\n");
        fprintf(app_context.config->materialMarketData->areamarket3, "購買後期末在庫リスト,\n"); fprintf(app_context.config->materialMarketData->areamarket3, "period, item_type_id,price,seller_id,\n");
         //  fprintf(app_context.config->out_temporary, "inside materialMarket_set_up market_type=%d address of marketdataAnalyzer =%x\n",market_type,marketdataAnalyzer );
        fprintf(app_context.config->materialMarketData->ITEM_SET_size_1, "ITEM_SET size before production,\n  period, item_type_id, ITEM_SET_size,\n");
        fprintf(app_context.config->materialMarketData->ITEM_SET_size_2, " ITEM_SET size after production before buying ,\n period, item_type_id, ITEM_SET_size,\n");
        fprintf(app_context.config->materialMarketData->ITEM_SET_size_3, " ITEM_SET size after buying ,\n period, item_type_id, ITEM_SET_size,\n");
    }
 
    MarketDataAnalyzer* get_marketdataAnalyzer() {
        return marketdataAnalyzer;

    }

	/**　データアナライザのセット*/
	/* void set_dataAnalyzer(MarketDataAnalyzer* marketdataAnalyzer){
		this->marketdataAnalyzer=marketdataAnalyzer;
	}
	 MarketDataAnalyzer* get_marketdataAnalyzer(){
		 return marketdataAnalyzer;
	 }
	 */

    /**
     * 商品選択
     *
     * @param item_type_id 商品種ID
     * @return 選択した商品。該当無しの場合はNULL。
     */
    Item* select_one(int item_type_id) {
        ITEM_SET* item_set = get_item_set(item_type_id);			//Item mapの中から該当するItem setを取り出す　自関　  20120524記　高島
        if (item_set->begin() == item_set->end()) {					//意味：item_setの最初のアドレスと最後のアドレスが一致　→　そのsetには何も値が入っていない　  20120524記　高島
            return NULL;
        }
	    int ncase=1; 
		//ncase=0;//ランダマイズなし
		//ncase=1;//ランダマイズあり
        MarketData*  marketData=NULL;
        if (get_market_type() == 1) marketData = app_context.config->retailMarketData;
        else if (get_market_type() == 2) marketData = app_context.config->materialMarketData;
        else {}
        if (ncase == 0) return *item_set->begin();
        else if (ncase == 1) {
            // 同一item_type_idで同一価格の製品が複数ある場合は、複数個の中からランダムに一つを選択する
            vector <Item*> mem_item;
            vector <int> v;
            RandomGenerator rg;
            ITEM_SET::iterator it, itr;
            itr = item_set->begin();
            long long min_price = 0, max_price = 0, ave_price = 0, price_range = 0;
            min_price = (*itr)->get_price();
            max_price = min_price;
                for (it = item_set->begin(); it != item_set->end(); it++) {
                    if ((*it)->get_price() > max_price) max_price = (*it)->get_price();
                }
            ave_price =long long  (double (min_price + max_price) / 2);
            price_range = long long(double(max_price - min_price) * app_context.config->areamarketParameter->price_range_ratio);
            //price_range = long long(double(max_price - min_price) * 0.1);
            int ik = 0;
            int n = 0;
            if (app_context.period_count < 10)
            fprintf(marketData->out_confirm_random_shaffle, "period=,%d, before shaffle,\n", app_context.period_count);
            for (it = item_set->begin(); it != item_set->end(); it++) {
               // if ((*it)->get_price()<=ave_price) {
               // if((*it)->get_price()==min_price){
                if((*it)->get_price()>=min_price && (*it)->get_price()<=min_price+price_range){
                    mem_item.push_back(*it);
                    v.push_back(ik);
                    ik++;
                    if (app_context.period_count < 10)
                    fprintf(marketData->out_confirm_random_shaffle, "seller_id[%d]=,%d,", ik,(*it)->Get_seller()->Get_agent_id());
                }
            }
            n = ik;
            if (app_context.period_count < 10)
            fprintf(marketData->out_confirm_random_shaffle, "\n,period=,%d, afrter shaffle,\n", app_context.period_count);
            RandomGenerator::shuffle(v.begin(), v.end());
            if (app_context.period_count < 10 ) {
                for (int i = 0; i < n; i++) {
                    fprintf(marketData->out_confirm_random_shaffle, "seller_id[%d]=,%d,", v[i], mem_item[v[i]]->Get_seller()->Get_agent_id());
                }
                    fprintf(marketData->out_confirm_random_shaffle, "period=,%d,seller_id=,%d,\n", app_context.period_count, mem_item[v[*v.begin()]]->Get_seller()->Get_agent_id());
            }/*
            if (mem_item.empty()) {
                return *item_set->begin();  // 最低価格の商品を返す2026/1/7
            }
            */

            return mem_item[v[*v.begin()]];
            //ランダマイズ化の追加プログラム終了
            //confirm_random_shaffle

        }
        else return NULL;

    }

    /**
     * 商品追加
     *
     * @param item 追加対象商品
     */
    void add_item(Item* item) {
		//item->set_dataAnalyzer(marketdataAnalyzer);
       // printf("[a]--- add item [%d,%d]\n", item->get_item_type_id(), item->get_price());
		//fprintf(app_context.config->out,"[a]--- add item [%d,%d] in AreaMarket\n", item->get_item_type_id(), item->get_price());
        ITEM_SET* item_set = get_item_set(item->get_item_type_id());
        item_set->insert(item);
        item->set_destination(this);
       // fprintf(app_context.config->out_temporary, "address of this=%x address of *this=%x, address of &this=%x\n", this, *this);
		list<MarketListener*>::iterator itr;
        for (itr = listeners.begin(); itr != listeners.end(); itr++) {
            (*itr)->notify_item_added(item);
        }
    }

    /**
     * 商品回収
     *
     * @param item 回収対象商品
     */
    void delete_item(Item* item) {

        //printf("[a]--- delete item [%d,%d]\n", item->get_item_type_id(), item->get_price());
		//fprintf(app_context.config->out,"[a]--- delete item [%d,%d]\n", item->get_item_type_id(), item->get_price());
        ITEM_SET* item_set = get_item_set(item->get_item_type_id());
		//printf("size:%d\n", item_set->size());
		//fprintf(app_context.config->out,"size:%d\n", item_set->size());

		//printf("address of marketdataAnalyzer=%x\n",marketdataAnalyzer);
        // 同一最安値のオブジェクトが複数とれるので、同一オブジェクトを判定して１つだけ削除する
        ITEM_SET::iterator start;
        int n = 0;
        if (app_context.period_count < 30 && item->Get_seller()->Get_agent_id() == 452) {
           // fprintf(app_context.config->out_test5, "item to be deleted, period=, %d, sellar_id=, %d, price=, %I64d, item_type_id=,%d, buyer_id=,%d,start_seller_id=,%d,\n"
           //     , app_context.period_count, item->Get_seller()->Get_agent_id(), item->get_price(), item->get_item_type_id(), item->get_buyer()->Get_agent_id(),(*item_set->find(item))->Get_seller()->Get_agent_id());
            fprintf(app_context.config->out_test5, "item to be deleted, period=, %d, sellar_id=, %d, price=, %I64d, item_type_id=,%d, buyer_id=,%d,\n"
                     , app_context.period_count, item->Get_seller()->Get_agent_id(), item->get_price(), item->get_item_type_id(), item->get_buyer()->Get_agent_id());

        }
       // for (start = item_set->find(item);start != item_set->end();start++) 
        for(start=item_set->begin();start !=item_set->end();start++){
            if (app_context.period_count < 30 && item->Get_seller()->Get_agent_id() == 452) {
                n++;
                fprintf(app_context.config->out_test5, "before delete, period=,%d,sellar_id=,%d,price=,%I64d,item_type_id=,%d,n=,%d,\n"
                    , app_context.period_count, (*start)->Get_seller()->Get_agent_id(), (*start)->get_price(), (*start)->get_item_type_id(), n);
            }
            
            if (*start == item) { // 該当要素(buyerが定義されていてitemのbuyerと一致するもの）かどうかを確認,Select the item where the buyer is difined 
                ITEM_SET::iterator end = start;
				end++;
                if (app_context.period_count < 30 && item->Get_seller()->Get_agent_id() == 452) fprintf(app_context.config->out_test5, "     delete item,n=,%d\n",n );
			    item_set->erase(start, end); // 1要素だけ削除する
               // item_set->erase(start);
          //  if(app_context.period_count<30)    fprintf(app_context.config->out_test5, "after delet, period=,%d,sellar_id=,%I64d,price=,%d,item_type_id=,%d,\n", app_context.period_count, (*start)->Get_seller()->Get_agent_id(), (*start)->get_price(), (*start)->get_item_type_id());
				break;
			}
		}

        list<MarketListener*>::iterator listeners_itr;
        for (listeners_itr = listeners.begin(); listeners_itr != listeners.end(); listeners_itr++) {
            (*listeners_itr)->notify_item_deleted(item);
        }

    }
   int count_items(int agent_id,ITEM_SET* item_set) {
        //item_set内の特定Agentのitemsの数を数える
       int n=0;
    
       ITEM_SET::iterator start; 
       for (start = item_set->begin(); start!=item_set->end();start++) {
          
       }
    }
    /**
     * 価格改定通知
     *
     * @param item 対象商品
     * @param new_price 新価格
     */
    void notify_price_changed(Item* item, long long new_price) {

        // 一旦、回収して値段を付け替えて再度、追加する
        //
        // 【修正】以前はこのコメントの説明とは異なり、item->set_price(new_price)を
        // 直接呼ぶだけで、在庫の多重集合(ITEM_SET = multiset<Item*, PredForItemByPrice>)
        // から一旦取り除く処理が行われていなかった（上のコメントが説明する正しい処理は
        // コメントアウトされ、実行されていなかった）。
        // multisetは要素が比較子(価格)に関して常にソート済みであることを前提に内部の
        // 木構造を管理しているため、要素を取り除かずに比較キーである価格を直接書き換える
        // と、ソート順の不変条件が破られ、multisetの内部状態が事実上壊れてしまう
        // （挙動が未定義になる）。これによりselect_one()等がmin_price/max_priceを誤って
        // 計算したり、価格帯で商品を絞り込む際に本来とは異なる商品を選んでしまうことがあり、
        // しかもその結果は木の内部構造（挿入・削除の履歴）に依存するため、同一シード・
        // 同一コードでも実行のたびに異なる（再現しない）結果を招く一因となる。
        // 正しくは、価格を変更する前に一旦multisetから取り除き、新しい価格で改めて
        // 挿入し直す必要がある（delete_item()はリスナーへの削除通知を伴うため、
        // ここでは通知を発生させずに集合からの取り除きだけを行う）。
        ITEM_SET* item_set = get_item_set(item->get_item_type_id());
        ITEM_SET::iterator it;
        for (it = item_set->begin(); it != item_set->end(); ++it) {
            if (*it == item) break;
        }
        if (it != item_set->end()) {
            item_set->erase(it);
        }
        item->set_price(new_price);
        item_set->insert(item);
    }

    /**
     * 市場取引リスナの追加
     *
     * @param listener 市場取引リスナ
     */
    void addMarketListener(MarketListener* listener) {
        listeners.push_back(listener);
    }

    void retailMarket_print_all_items(int index) {
        //index=1  生産開始前の在庫リスト　　　=2　生産後購買開始前の在庫リスト　　　=3　期末購入後の在庫リスト
     //   cout << "[a]--- all items ----" << endl;
		fprintf(app_context.config->out,"[a]--- all items ----in AreaMarket\n");
     //   fprintf(app_context.config->out3, "[a]--- all items ----in AreaMarket\n");
		//printf("before map in print all items period=%d\n",app_context.period_count);
        map<int, ITEM_SET*>::iterator it_map;
		//printf("after map in print all items period=%d\n",app_context.period_count);
        for (it_map = item_map.begin(); it_map != item_map.end(); it_map++) {
            ITEM_SET* item_set = it_map->second;
            ITEM_SET::iterator it;
            if (item_set->begin() == item_set->end()) {
            // if(item_set == NULL){
           //  if(item_set->empty()){
            //    if(app_context.period_count==0) {
                //	fprintf(app_context.config->out," %d   - no item in AreaMarket\n",it_map->first);
                //fprintf(app_context.config->out3,"no item [%d] \n",(*it)->get_item_type_id());
             //   fprintf(app_context.config->out3, "no item [] \n");
                if (index == 1) fprintf(app_context.config->retailMarketData->areamarket1, "period=,%d,no_item[],item_type_id=,%d, \n", app_context.period_count,it_map->first);
                else if (index == 2) fprintf(app_context.config->retailMarketData->areamarket2, "period=,%d,no_item[],item_type_id=,%d, \n", app_context.period_count,it_map->first);
                else fprintf(app_context.config->retailMarketData->areamarket3, "period=,%d,no_item[],item_type_id=,%d, \n", app_context.period_count,it_map->first);
            }
            else {
            for (it = item_set->begin(); it != item_set->end(); it++){
              //  printf("inside for index= %d\n",index); 
                    //  cout << (*it)->get_item_type_id() << "-" << (*it)->get_price() << endl;
                 //   fprintf(app_context.config->out3, " period=%d item_type_id= %d  price= %I64d seller id=%d \n", app_context.period_count, (*it)->get_item_type_id(), (*it)->get_price(), (*it)->Get_seller()->Get_agent_id());
                    if (index == 1) fprintf(app_context.config->retailMarketData->areamarket1, " %d , %d , %I64d, %d ,\n", app_context.period_count, (*it)->get_item_type_id(), (*it)->get_price(), (*it)->Get_seller()->Get_agent_id());
                    else if (index == 2) fprintf(app_context.config->retailMarketData->areamarket2, " %d , %d, %I64d, %d, \n", app_context.period_count, (*it)->get_item_type_id(), (*it)->get_price(), (*it)->Get_seller()->Get_agent_id());
                    else fprintf(app_context.config->retailMarketData->areamarket3, " %d , %d , %I64d, %d, \n", app_context.period_count, (*it)->get_item_type_id(), (*it)->get_price(), (*it)->Get_seller()->Get_agent_id());
                }
            }
          //  cout << endl;
        }
		//printf("after for in print all items \n");
    }
    int get_size_of_stock_in_market( int item_type_id, int agent_id) {
        //この関数はproducer.hが毎期首に呼出、自身が生産する生産品種の期首在庫の値を格納しておく
         //---市場にある（引数で指定した製品品種の）商品の個数を返す
        int size = 0;
            map<int, ITEM_SET*> ::iterator it_map;
            for (it_map = item_map.begin(); it_map != item_map.end(); it_map++) {
                ITEM_SET* item_set = it_map->second;
                ITEM_SET::iterator it;
                if (item_set->begin() != item_set->end()) {
                    for (it = item_set->begin(); it != item_set->end(); it++) {
                        if ((*it)->get_item_type_id() == item_type_id && (*it)->Get_seller()->Get_agent_id() == agent_id) {
                            size++;
                        }
                    }
                }

            }//for(it_map.....の終わり
            return size;
    }
    long long get_ave_price(int market_type, int item_type_id) {
        //この関数はproduce.hが毎期首に呼び出し、値を格納しておく。
        //---市場にある（引数で指定した製品品種の）商品の平均価格を返す
        long long ave_price = 0;
        long long sum_price = 0;
        int period = app_context.period_count;
        int n = 0;
        int k = 0;
        map<int, ITEM_SET*>::iterator it_map;
        for (it_map = item_map.begin(); it_map != item_map.end(); it_map++) {
            //item_type_idが若い番号から順に処理する。正常にave_priceが計算されたのちに次のitem_type_idで商品がないばあいにはave_priceがゼロになる。
            //そのため、ave_priceが正常に計算されたら、mapのループからbreakで抜け出すことが必要
            if (it_map->first == item_type_id) {
                ITEM_SET* item_set = it_map->second;
                ITEM_SET::iterator it;
                k++; if (app_context.period_count == 21) fprintf(app_context.config->out_test12, "inside get_ave_price, %d回目,\n", k);
                if (item_set->begin() == item_set->end()) {
                    ave_price = 0;//商品が市場に存在しなければ、ave_price=0を返す
                    if (app_context.period_count == 21) fprintf(app_context.config->out_test12, "it_map->first=,%d,\n", it_map->first);
                }
                else {
                    for (it = item_set->begin(); it != item_set->end(); it++) {
                        if ((*it)->get_item_type_id() == item_type_id) {
                            sum_price += (*it)->get_price();
                            n++;
                            ave_price = long long(double(sum_price) / n);
                            if (app_context.period_count == 21) fprintf(app_context.config->out_test12, " period=, %d, it_map_first=, %d,item_type_id=, %d,price=,%I64d,sum_price=,%I64d,n=,%d,seller_id=,%d,maket_type=,%d,\n ", app_context.period_count, it_map->first, item_type_id, (*it)->get_price(), sum_price, n, (*it)->Get_seller()->Get_agent_id(), market_type);
                            else {}
                        }
                        else {}
                    }
                    // if (sum_price> 0) ave_price = long long(double(sum_price) / n);
                    // else ave_price = 777;
                    if (app_context.period_count == 21)fprintf(app_context.config->out_test12, ",,,,ave_price=,%I64d,n=,%d,item_type_id=,%d,\n", ave_price, n, item_type_id);
                    // break;
                }
            }
            //--------------------
            if (app_context.period_count == 21)fprintf(app_context.config->out_test12, "before return ave_price in get_ave_price_in_market(),,,,ave_price=,%I64d,n=,%d,item_type_id=,%d,\n", ave_price, n, item_type_id);
        }
        return ave_price;
    }
    /*
    long long get_ave_price(int market_type, int item_type_id) {
        //この関数はproduce.hが毎期首に呼び出し、値を格納しておく。
        //---市場にある（引数で指定した製品品種の）商品の平均価格を返す
        long long ave_price = 0;
        long long sum_price = 0;
        int period = app_context.period_count;
        int n = 0;
        int k = 0;
        map<int, ITEM_SET*>::iterator it_map;
        for (it_map = item_map.begin(); it_map != item_map.end(); it_map++) {
            //item_type_idが若い番号から順に処理する。正常にave_priceが計算されたのちに次のitem_type_idで商品がないばあいにはave_priceがゼロになる。
            //そのため、ave_priceが正常に計算されたら、mapのループからbreakで抜け出すことが必要

            ITEM_SET* item_set = it_map->second;
            ITEM_SET::iterator it;
            k++; if (app_context.period_count == 21) fprintf(app_context.config->out_test12, "inside get_ave_price, %d回目,\n", k);
            if (item_set->begin() == item_set->end()) {
                ave_price = 888;//商品が市場に存在しなければ、ave_price=0を返す
                if (app_context.period_count == 21) fprintf(app_context.config->out_test12, "it_map->first=,%d,\n", it_map->first);
            }
            else {
                for (it = item_set->begin(); it != item_set->end(); it++) {
                    if ((*it)->get_item_type_id() == item_type_id) {
                        sum_price += (*it)->get_price();
                        n++;
                        ave_price = long long(double(sum_price) / n);
                        if (app_context.period_count == 21) fprintf(app_context.config->out_test12, " period=, %d, it_map_first=, %d,item_type_id=, %d,price=,%I64d,sum_price=,%I64d,n=,%d,seller_id=,%d,maket_type=,%d,\n ", app_context.period_count, it_map->first, item_type_id, (*it)->get_price(), sum_price, n, (*it)->Get_seller()->Get_agent_id(), market_type);
                        else {}
                    }
                    else {}
                }
                // if (sum_price> 0) ave_price = long long(double(sum_price) / n);
                // else ave_price = 777;
                if (app_context.period_count == 21)fprintf(app_context.config->out_test12, ",,,,ave_price=,%I64d,n=,%d,item_type_id=,%d,\n", ave_price, n, item_type_id);
                // break;
            }
        }
        //--------------------
        if (app_context.period_count == 21)fprintf(app_context.config->out_test12, "before return ave_price in get_ave_price_in_market(),,,,ave_price=,%I64d,n=,%d,item_type_id=,%d,\n", ave_price, n, item_type_id);

        return ave_price;
    }
    */
    void materialMarket_print_all_items(int index) {
        //index=1  生産開始前の在庫リスト　　　=2　生産後購買開始前の在庫リスト　　　=3　期末購入後の在庫リスト
     //   cout << "[a]--- all items ----" << endl;
        fprintf(app_context.config->out, "[a]--- all items ----in AreaMarket\n");
      //  fprintf(app_context.config->out3, "[a]--- all items ----in AreaMarket\n");
        //printf("before map in print all items period=%d\n",app_context.period_count);
        map<int, ITEM_SET*>::iterator it_map;
        //printf("after map in print all items period=%d\n",app_context.period_count);
        for (it_map = item_map.begin(); it_map != item_map.end(); it_map++) {
            ITEM_SET* item_set = it_map->second;
            ITEM_SET::iterator it;
            if (item_set->begin() == item_set->end()) {
                // if(item_set == NULL){
               //  if(item_set->empty()){
                //    if(app_context.period_count==0) {
                    //	fprintf(app_context.config->out," %d   - no item in AreaMarket\n",it_map->first);
                    //fprintf(app_context.config->out3,"no item [%d] \n",(*it)->get_item_type_id());
              //  fprintf(app_context.config->out3, "no item [] \n");
                if (index == 1) fprintf(app_context.config->materialMarketData->areamarket1, "period=,%d,no_item[],item_type_id=,%d, \n", app_context.period_count,it_map->first);
                else if (index == 2) fprintf(app_context.config->materialMarketData->areamarket2, "period=,%d,no_item[],item_type_id=,%d, \n", app_context.period_count,it_map->first);
                else fprintf(app_context.config->materialMarketData->areamarket3, "period=,%d,no_item[],item_type_id=,%d, \n", app_context.period_count,it_map->first);
            }
            else {
                for (it = item_set->begin(); it != item_set->end(); it++) {
                    //  printf("inside for index= %d\n",index); 
                          //  cout << (*it)->get_item_type_id() << "-" << (*it)->get_price() << endl;
                  //  fprintf(app_context.config->out3, " period=%d item_type_id= %d  price= %I64d seller id=%d \n", app_context.period_count, (*it)->get_item_type_id(), (*it)->get_price(), (*it)->Get_seller()->Get_agent_id());
                    if (index == 1) fprintf(app_context.config->materialMarketData->areamarket1, " %d , %d , %I64d, %d ,\n", app_context.period_count, (*it)->get_item_type_id(), (*it)->get_price(), (*it)->Get_seller()->Get_agent_id());
                    else if (index == 2) fprintf(app_context.config->materialMarketData->areamarket2, " %d , %d, %I64d, %d, \n", app_context.period_count, (*it)->get_item_type_id(), (*it)->get_price(), (*it)->Get_seller()->Get_agent_id());
                    else fprintf(app_context.config->materialMarketData->areamarket3, " %d , %d , %I64d, %d, \n", app_context.period_count, (*it)->get_item_type_id(), (*it)->get_price(), (*it)->Get_seller()->Get_agent_id());
                }
            }
            //  cout << endl;
        }
        //printf("after for in print all items \n");
    }
    void transfer_to_areamarketItems() {
        map<int, ITEM_SET*>::iterator it_map;
        for (it_map=item_map.begin(); it_map!= item_map.end(); it_map++) {
            ITEM_SET* item_set = it_map->second;
            ITEM_SET::iterator it;
			//if (item_set->begin() == item_set->end())
            for ( it=item_set->begin() ; it != item_set->end(); it++ ){
		    get_marketdataAnalyzer()->set_areamarketItem(app_context.period_count,(*it)->get_item_type_id(),int ((*it)->get_price()),(*it)->Get_seller()->Get_agent_id());
			//get_dataAna//lyzer()->set_areamarketItem(app_context.period_count,(*it)->get_item_type_id(),(*it)->get_price(),(*it)->Get_seller()->Get_agent_id());		
			}
        }
    }
};

 