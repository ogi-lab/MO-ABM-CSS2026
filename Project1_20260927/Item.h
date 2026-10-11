#pragma once
#include "Buyer.h"
#include "Seller.h"
#include "Destination.h"
#include "DataAnalyzer.h"
#include "DataAnalyzerNavigator.h"
/** 商品 */
class Item {
    //int item_id;    /**< 商品ID */
    int item_type_id = 0; /**< 商品種ID */
    long long price = 0;      /**< 価格   */
    Buyer* buyer;   /**< 購入者 */
    Seller* seller; /**< 販売者 */
    Destination *destination; /**< 出荷先 */
	DataAnalyzer* dataAnalyzer;//データアナライザ
    DataAnalyzerNavigator* data_navi;
    int production_period = 0;
    // ITEMの生成順(item_seq)を管理する静的カウンタ。
    // Producerの在庫集合(stock_map内のset<Item*>)の要素比較にポインタの値を直接使うと、
    // ヒープアドレスは実行するごとに異なるため、価格をランダムに淀らす処理などで
    // 在庫を走査する順番が実行のたびに変わってしまい、同一シード・同一コードでも
    // シミュレーション結果が再現しない原因となる。
    // そのため、ポインタ値ではなく生成順そのものを比較キーに使うことで、
    // 在庫集合の走査順を常に決定的（ヒープアドレスに依存しない）にする。
    inline static long long item_seq_counter = 0;
    long long item_seq = 0;
public:
    Item(int item_type_id, long long price):item_type_id(item_type_id),price(price) {
        //生成時点でitem_type_id, priceは引数で設定する
        item_seq = item_seq_counter++;
    }
    long long get_item_seq() const { return item_seq; }
    ~Item() {}
    void set_production_period(int period) { production_period = period; }
    int get_production_period() { return production_period; }
    long long genka_per_item = 0;
    void set_genka_per_item(long long genka_per_item) { this->genka_per_item = genka_per_item; }
    long long imported_inventory_per_item = 0;
    void set_imported_inventory_per_item(long long imported_inventory_per_item) { this->imported_inventory_per_item = imported_inventory_per_item; }
    long long get_genka_per_item() { return genka_per_item; }
    long long get_imported_imventory_per_item() { return imported_inventory_per_item; }
    long long get_price() {return price;}
    void set_price(long long new_price) {this->price = new_price;}
    int get_item_type_id() {return item_type_id;}
    void set_seller(Seller* seller){this->seller = seller;}
	Seller* Get_seller(){return this->seller;};
	Buyer* get_buyer(){return this->buyer;};
    void set_destination(Destination* destination){this->destination = destination;}
	Destination* Get_destination(){return destination;};
		//ITEM経由で価格改訂、荻林追加　2012_4_16
	//void set_dataAnalyzer(DataAnalyzer* dataAnalyzer){this->dataAnalyzer=dataAnalyzer;}
	//int get_item_producer_id(){this->seller->get_agent_id();}
	DataAnalyzer* get_dataAnalyzer() {
        //DataAnalyzerNavigator* data_navi;
       /* if (buyer->Get_AgentType() == 31) {
            data_navi = app_context.dataAnalyzer_navi_for_retailMarket;
        }
        else if (buyer->Get_AgentType() == 2131) {
            data_navi = app_context.dataAnalyzer_navi_for_materialMarket;
        }
        else  data_navi = app_context.dataAnalyzer_navi_for_retailMarket;
       */
        if(this->destination->get_market_type()==1)    data_navi = app_context.dataAnalyzer_navi_for_retailMarket;
        else data_navi = app_context.dataAnalyzer_navi_for_materialMarket;
       // fprintf(app_context.config->out_temporary, "inside item->get_dataAnalyzer() market_type=%d\n", this->destination->get_market_type());
        return data_navi->get_dataAnalyzer();
       // if(buyer->Get_AgentType()==32)        data_navi=app_context.dataAnalyzer_navi_for_retailMarket;
        //else data_navi = app_context.dataAnalyzer_navi_for_retailMarket;
		//DataAnalyzer* dataAnalyzer=data_navi->get_dataAnalyzer();
		//return dataAnalyzer;
	}

    /**
     * 売買確定
     *
     * @param buyer 購入者
     */
    void deal(Buyer* buyer){

        this->buyer = buyer;

        // 代金支払
        this->buyer->pay(this);					//現時点のbuyerはconsumerまたは retailer
                                                //payの中でbuyerの（手元資金Eb更新）
       // fprintf(app_context.config->out, "seller_id= %d item_type_id=%d buyer= %d\n", this->Get_seller()->Get_agent_id(), this->get_item_type_id(), this->get_buyer()->Get_agent_id());
        //代金受領
        this->Get_seller()->receive_payments(this);	       
        //this->seller->add_soldItem(this);
        /*
		//売れた製品情報の格納
        fprintf(app_context.config->out_trace_supply_sell, "period=,%d,seller_id=,%d,item_type_id=,%d,price=,%lld,buyer_id=,%d,\n",
           app_context.period_count,this->Get_seller()->Get_agent_id(),item_type_id,price,this->buyer->Get_agent_id());
        */	
        this->Get_seller()->set_soldItem(app_context.period_count,this->item_type_id,this->get_price());
		//購入商品情報をmarketDataAnalyzerに渡す（2012get_dataAnalyzer()経由に修正荻林）
		//printf("address of dataAnalzer=%x\n",this->dataAnalyzer);
		//this->dataAnalyzer->add_item(this);
		//get_dataAnalyzer()->add_item(this);		//データ編集のためにアイテムの参照を取得、その後、アクセス利用している模様　MArketDataAnalyzerが所管  20120524記　高島
        //destination->get_dataAnalyzer()->set_boughtItem(app_context.period_count, this->item_type_id, this->get_price(), this->Get_seller()->Get_agent_id(), this->get_buyer()->Get_agent_id());
       // fprintf(app_context.config->out_temporary, "buyer_type=%d seller_id=%d address of destination=%x market_type=%d\n", this->get_buyer()->Get_AgentType(),this->Get_seller()->Get_agent_id(),this->Get_destination(),this->destination->get_market_type());
        if(this->buyer->Get_AgentType() == 2131 ) data_navi = app_context.dataAnalyzer_navi_for_materialMarket;
        else  data_navi = app_context.dataAnalyzer_navi_for_retailMarket;
        data_navi->get_dataAnalyzer()->set_boughtItem(app_context.period_count,this->item_type_id,int(this->get_price()),this->Get_seller()->Get_agent_id(),this->get_buyer()->Get_agent_id());
        //fprintf(app_context.config->out_temporary, "period=%d buyer_id=%d buyer_style=%d \n", app_context.period_count, this->buyer->Get_agent_id(), this->buyer->Get_AgentType());
        // fprintf(app_context.config->out_temporary, "inside deal peirod=%d agent_id=%d agent_type=%d\n", app_context.period_count,buyer->Get_agent_id(), buyer->Get_AgentType());
        //購入された商品情報をdataAnalyzlerに移管、但しadd_item(this)でmap経由ではなく直接boughtItem構造体vectorに格納2012_5_29記　荻林
        // 市場から商品を回収
        this->remove_from_market();			//市場製品集合から購買された当該製品を除去　～～注意！集合から削除されているだけでItem ob自体はメモリ内に残存～～  20120524記　高島

    }

    /** 市場回収 */
    void remove_from_market(){
		
        // 市場から商品を回収
        this->destination->delete_item(this);	//現在、Destination継承のAreaMarketが所管　同時にMarketSyncMarketListenerを通してGovernmentMarketの製品集合からも同期して削除  20120524記　高島

        // 在庫から削除
        this->seller->remove_stock(this);		//販売者（Item自身が生成されたときに持ってる参照）の在庫集合から自分を削除

    }

	void delete_self()		//自己ob削除用関数　20120528　創　高島
	{
		delete this;
	}
};

