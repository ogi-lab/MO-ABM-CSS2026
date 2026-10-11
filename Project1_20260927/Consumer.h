
#pragma once
#include <stdio.h>
#include "Agent.h"
#include "Buyer.h"
#include "Seller.h"
#include "Item.h"
#include "RetailMarket.h"
#include "RetailMarketNavigator.h"
#include "MarketBuyDealer.h"
#include "RollAggregator.h"
#include "BookKeeper.h"
#include "GovernmentAggregatorNavigator.h"
#include <set>
#include <unordered_map>
#include <unordered_set>
extern SimulatorContext app_context;

/** 消費者 */
class Consumer : public Agent, public Buyer {
	//long long Ebの変数宣言はAgent.h
	int item_type_id = 0;
	int income_count = 0;
	long long const_income = 0;
	long long var_income = 0;//サラリー
	long long consumer_income_received = 0;
//	long long paid_tax_amount;

	//int worker_type; //= 0: retailer_worker, =1:wholesaler_worker;

	BuyDealer* buyDealer;
protected:
	void initialize_consumer_income_received() {
		consumer_income_received = 0;
	}
	//set_income(), get_income()はAgent.hで定義してある。}	
	void set_var_income(long long var_income) {
		this->var_income = var_income;
	}	
	long long get_var_income() {
		return var_income;
	}	
	/* 下記変数はagent.hにて宣言してある。protected variable
	
    int agent_id = 0; //< エージェントID 
	long long cash = 0;  //  < 現金 
	long long deposit = 0;//  <預金
	long long initial_cash = 0;
	long long initial_deposit = 0;
	long long Eb = 0; // Budget for consumption during the buying actions
	long long Eb0 = 0; //Budget for consumption
	long long asset_begining = 0;  //期首資産残高
	long long asset_end = 0;       //期末資産残高
	long long income = 0;   //期の収入
	long long expenses = 0;  //期の支出
	double rwd = 0.0;   //預金引き出し率
	double rpc = 0.0;   //消費性向　propensity to consume
	long long fixed_wage = 0;
	long long wage = 0;

	long long  get_fixed_wage() { return fixed_wage; };
	long long get_wage() { return wage; }
	long long get_depoist() { return deposit; }
	virtual void pay_tax(long long income) {}
	virtual void pay_tax() {}
	void set_income(long long income) { this->income = income; }
	long long get_income() { return income; }
	long long get_expenses() { return expenses; }
	long long get_Eb() { return Eb; }
	long long get_Eb0() { return Eb0; }
	*/









    void pay_tax() {}
	/*void createBookKeepr(int agent_id){
		bookKeeper=new BookKeeper(this->agent_id);
	}*/
    /** 購買活動 */
	/*
	int select_item_type_id_for_maximize_utility(const std::set<int>& excluded_ids = {}) {
		//vector <int> bought_items_numberはAgent.hで宣言してある。
		double max = -1.0;
		int item_type_idkk = -1;
		int item_type_id = 0;
		RetailMarketNavigator* market_navi = app_context.retail_market_navi_for_customer;

		for (unsigned int i = 0; i < app_context.config->item_type_idk.size(); i++) {
			int current_id = app_context.config->item_type_idk[i];
			if (excluded_ids.find(current_id) != excluded_ids.end()) continue;

			double current_utility = utility_weight[i] * pow((double(bought_items_number[i]) + 1), -2);
			if (max < current_utility) {
				max = current_utility;
				item_type_idkk = current_id;
			}
		}
		return item_type_idkk;
	}
	void buy_retails() {
		int item_type_id = 0;
		//消費者は毎期１種類の製品種を購入する。その製品種は毎期異なる。
		// 購入製品種は消費者毎に固有の値とするため、setupに移行、ここではコメント化20191123荻林
		RetailMarketNavigator* market_navi = app_context.retail_market_navi_for_customer;
		//-------------------
		reset_bought_items_number();//購入済み製品数を0にリセットする、関数はAgent/hで定義済み20210627
		int i = 0;//item_type_id vectorの添え字

		std::set<int> failed_items;

		//fprintf(app_context.config->out_temporary, "before while period=%d agent_id=%d\n", app_context.period_count, agent_id);
	//	printf("start of buy_retails() agent_id=%d \n", get_agent_id());
		//Eb: Agentで変数宣言している。
		while (Eb > 0)
		{
			//効用最大となる製品種を選択する。FAILEDに含まれるものは除外する。
			item_type_id = select_item_type_id_for_maximize_utility(failed_items);
			if (item_type_id == -1) break; // 購入可能な候補がないため終了

			for (unsigned int j = 0; j < app_context.config->item_type_idk.size(); j++) {
				if (app_context.config->item_type_idk[j] == item_type_id) i = j;//item_type_idkのitem_type_idkの添え字をiとする
			}
			fprintf(app_context.config->out3, "agent_id=%d がmax_utility_id=%d　を選択しました i=%d\n", agent_id, item_type_id, i);
			//RetailMarket* market = market_navi->get_retail_market(agent_id, item_type_id);		//CustomerMarketNavigator所管　現時点ではAreamarketの参照　  20120524記　高島
			RetailMarket* market = market_navi->get_retail_market(item_type_id);
			//	printf("inside buy_retails of consumer before select_one period=%d agent_id=%d agent_type=%d \n", app_context.period_count, get_agent_id(), get_agent_type());
			Item* item = market->select_one(item_type_id);		//現在RetailMarket継承のAreaMarketの所管、ItemMapから当該ItemIDのItemSetを取り出す　  20120524記　高島
			//printf("after select_one agent_id=%d agent_type=%d address of Item=%x Eb=%I64d price=%I64d \n", get_agent_id(),
			//	get_agent_type(),item,Eb,item->get_price());
			//	printf("after select_one agent_id=%d agent_type=%d \n", get_agent_id(), get_agent_type());													//fprintf(app_context.config->out_test4, "agent_id=%d, item>size=%d,item->size()=%d\n",get_agent_id(),get_agent_id(),item->get_price());
			if (item != NULL)
			{

				//item->get_price();
				if (Eb - item->get_price() > 0)
				{
					//market=399c50
					item->deal(this);//買い手の支払、売り手の金額受領、市場からの削除。会計処理は行わない。
					//Ebの更新はdealの中のbuyer->pay()の中で実施される
					bought_items_number[i]++;
					//bookKeeper->createstockBookSlip(app_context.period_count, 0, 1, item->get_item_type_id(), item->get_price());//2023/5/7荻林追加 関数payで記述している
					//bookKeeper->createstockpriceBookSlip(app_context.period_count, 0, item->get_price(), item->get_item_type_id());//2023/5/7荻林追加　関数payで記述している
					fprintf(app_context.config->out, "consumer %d がitem_type_id=%d 製品価格=%I64d Eb=%I64d 生産者id=%dの製品をmarket=%xから購入しました,bought_items_number[%d]=%d,\n", get_agent_id(), item_type_id, item->get_price(), Eb, item->Get_seller()->Get_agent_id(), market, i, bought_items_number[i]);
					fprintf(app_context.config->out3, "consumer %d がitem_type_id=%d 製品価格=%I64d Eb=%I64d 生産者id=%dの製品をmarket=%xから購入しました,bought_items_number[%d]=%d,\n", get_agent_id(), item_type_id, item->get_price(), Eb, item->Get_seller()->Get_agent_id(), market, i, bought_items_number[i]);
					//	fprintf(app_context.config->out_temporary, "consumer %d がitem_type_id=%d 製品価格=%d Eb=%d 生産者id=%dの製品をmarket=%xから購入しました,bought_items_number[%d]=%d,\n", get_agent_id(), item_type_id, item->get_price(), Eb, item->Get_seller()->Get_agent_id(), market, i, bought_items_number[i]);

				}
				else
				{
					//	printf("consumer %d がitem_type_id=%d 製品価格=%d Eb=%d 生産者id=%dの製品を購入しようとしましたがcash不足のため購入しませんでした\n",get_agent_id(),item_type_id,item->get_price(),Eb,item->Get_seller()->Get_agent_id());

					fprintf(app_context.config->out, "consumer %d がitem_type_id=%d 製品価格=%I64d Eb=%I64d 生産者id=%dの製品を購入しようとしましたがcash不足のため購入しませんでした\n", get_agent_id(), item_type_id, item->get_price(), Eb, item->Get_seller()->Get_agent_id());
					fprintf(app_context.config->out3, "consumer %d がitem_type_id=%d 製品価格=%I64d Eb=%I64d 生産者id=%dの製品を購入しようとしましたがcash不足のため購入しませんでした\n", get_agent_id(), item_type_id, item->get_price(), Eb, item->Get_seller()->Get_agent_id());
					//fprintf(app_context.config->out_temporary, "consumer %d がitem_type_id=%d 製品価格=%d Eb=%d 生産者id=%dの製品を購入しようとしましたがcash不足のため購入しませんでした\n", get_agent_id(), item_type_id, item->get_price(), Eb, item->Get_seller()->Get_agent_id());

					failed_items.insert(item_type_id); // 候補から除外して次へ
				}
			}//if(item!=NULL)の終わり	
			else {
				//商品が存在しない場合
				//printf("no item.[%d]\n", item_type_id);
				fprintf(app_context.config->out, "period= %d no item.[%d] for consumer %d to buy\n", app_context.period_count, item_type_id, get_agent_id());
				fprintf(app_context.config->out_test4, "period= %d no_item.[%d] for consumer %d to buy\n", app_context.period_count, item_type_id, get_agent_id());

				failed_items.insert(item_type_id); // 候補から除外して次へ
			}
			//delete item;
		}//while(Eb>0)の終わり


		//-------------------------------

	}//buy_retailsの終わり
	*/


/*

	int select_item_type_id_for_maximize_utility() {
		//vector <int> bought_items_numberはAgent.hで宣言してある。
		double max = 0.0;
		int item_type_idkk = 0;
		int item_type_id = 0;
		RetailMarketNavigator* market_navi = app_context.retail_market_navi_for_customer;
		max = utility_weight[0] * pow((double(bought_items_number[0])+1),-2);
		item_type_idkk = app_context.config->item_type_idk[0];
		for (unsigned int i = 0; i < app_context.config->item_type_idk.size(); i++) {
		
			if (max < utility_weight[i] * pow((double(bought_items_number[i])+1), -2)) {
				max = utility_weight[i] * pow((double(bought_items_number[i])+1), -2);
				item_type_idkk = app_context.config->item_type_idk[i];
			//		RetailMarket* market = market_navi->get_retail_market( item_type_idkk);
			//		Item* item = market->select_one(item_type_idkk);
			//	if (Eb - item->get_price() > 0) {
				
			//	}
			}

		}
		return item_type_idkk;
	}
	
    void buy_retails() {
		int item_type_id = 0;
		//消費者は毎期１種類の製品種を購入する。その製品種は毎期異なる。
        // 購入製品種は消費者毎に固有の値とするため、setupに移行、ここではコメント化20191123荻林
        RetailMarketNavigator* market_navi = app_context.retail_market_navi_for_customer;		
       //-------------------
		reset_bought_items_number();//購入済み製品数を0にリセットする、関数はAgent/hで定義済み20210627
		int i=0;//item_type_id vectorの添え字
		//fprintf(app_context.config->out_temporary, "before while period=%d agent_id=%d\n", app_context.period_count, agent_id);
	//	printf("start of buy_retails() agent_id=%d \n", get_agent_id());
		//Eb: Agentで変数宣言している。
		while (Eb > 0)
		{
			//効用最大となる製品種を選択する。
			item_type_id = select_item_type_id_for_maximize_utility();//関数はconsumer.hで実装
			for (unsigned int j = 0; j < app_context.config->item_type_idk.size(); j++) {
			if (app_context.config->item_type_idk[j] == item_type_id) i = j;//item_type_idkのitem_type_idkの添え字をiとする
		    }
			fprintf(app_context.config->out3, "agent_id=%d がmax_utility_id=%d　を選択しました i=%d\n", agent_id, item_type_id,i);
			//RetailMarket* market = market_navi->get_retail_market(agent_id, item_type_id);		//CustomerMarketNavigator所管　現時点ではAreamarketの参照　  20120524記　高島
			RetailMarket* market = market_navi->get_retail_market( item_type_id);
																								//	printf("inside buy_retails of consumer before select_one period=%d agent_id=%d agent_type=%d \n", app_context.period_count, get_agent_id(), get_agent_type());
			Item* item = market->select_one(item_type_id);		//現在RetailMarket継承のAreaMarketの所管、ItemMapから当該ItemIDのItemSetを取り出す　  20120524記　高島
			//printf("after select_one agent_id=%d agent_type=%d address of Item=%x Eb=%I64d price=%I64d \n", get_agent_id(),
			//	get_agent_type(),item,Eb,item->get_price());
		//	printf("after select_one agent_id=%d agent_type=%d \n", get_agent_id(), get_agent_type());													//fprintf(app_context.config->out_test4, "agent_id=%d, item>size=%d,item->size()=%d\n",get_agent_id(),get_agent_id(),item->get_price());
			if (item != NULL) 
			{

				//item->get_price();
				if(Eb-item->get_price()>0) 
				{
					//market=399c50
					item->deal(this);//買い手の支払、売り手の金額受領、市場からの削除。会計処理は行わない。
									 //Ebの更新はdealの中のbuyer->pay()の中で実施される
					bought_items_number[i]++;
					//bookKeeper->createstockBookSlip(app_context.period_count, 0, 1, item->get_item_type_id(), item->get_price());//2023/5/7荻林追加 関数payで記述している
					//bookKeeper->createstockpriceBookSlip(app_context.period_count, 0, item->get_price(), item->get_item_type_id());//2023/5/7荻林追加　関数payで記述している
					fprintf(app_context.config->out, "consumer %d がitem_type_id=%d 製品価格=%I64d Eb=%I64d 生産者id=%dの製品をmarket=%xから購入しました,bought_items_number[%d]=%d,\n", get_agent_id(), item_type_id, item->get_price(), Eb, item->Get_seller()->Get_agent_id(),market,i,bought_items_number[i]);
					fprintf(app_context.config->out3, "consumer %d がitem_type_id=%d 製品価格=%I64d Eb=%I64d 生産者id=%dの製品をmarket=%xから購入しました,bought_items_number[%d]=%d,\n", get_agent_id(), item_type_id, item->get_price(), Eb, item->Get_seller()->Get_agent_id(),market, i, bought_items_number[i]);
				//	fprintf(app_context.config->out_temporary, "consumer %d がitem_type_id=%d 製品価格=%d Eb=%d 生産者id=%dの製品をmarket=%xから購入しました,bought_items_number[%d]=%d,\n", get_agent_id(), item_type_id, item->get_price(), Eb, item->Get_seller()->Get_agent_id(), market, i, bought_items_number[i]);

				}
				else 
				{
				//	printf("consumer %d がitem_type_id=%d 製品価格=%d Eb=%d 生産者id=%dの製品を購入しようとしましたがcash不足のため購入しませんでした\n",get_agent_id(),item_type_id,item->get_price(),Eb,item->Get_seller()->Get_agent_id());
					fprintf(app_context.config->outEB, "consumer %d がitem_type_id=%d price=%I64d Eb=%I64d sellar tried to buy products id=%d, but could noy buy because of available money の製品を購入しようとしましたがcash不足のため購入しませんでした\n", get_agent_id(), item_type_id, item->get_price(), Eb, item->Get_seller()->Get_agent_id());

					fprintf(app_context.config->out,"consumer %d がitem_type_id=%d 製品価格=%I64d Eb=%I64d 生産者id=%dの製品を購入しようとしましたがcash不足のため購入しませんでした\n",get_agent_id(),item_type_id,item->get_price(),Eb,item->Get_seller()->Get_agent_id());
					fprintf(app_context.config->out3,"consumer %d がitem_type_id=%d 製品価格=%I64d Eb=%I64d 生産者id=%dの製品を購入しようとしましたがcash不足のため購入しませんでした\n", get_agent_id(), item_type_id, item->get_price(),Eb, item->Get_seller()->Get_agent_id());
					//fprintf(app_context.config->out_temporary, "consumer %d がitem_type_id=%d 製品価格=%d Eb=%d 生産者id=%dの製品を購入しようとしましたがcash不足のため購入しませんでした\n", get_agent_id(), item_type_id, item->get_price(), Eb, item->Get_seller()->Get_agent_id());

					break;
				}
			}//if(item!=NULL)の終わり	
			else {
				//商品が存在しない場合
				//printf("no item.[%d]\n", item_type_id);
				fprintf(app_context.config->outEB, "period= %d no_item.[%d] for consumer %d to buy\n", app_context.period_count, item_type_id, get_agent_id());

				fprintf(app_context.config->out,"period= %d no item.[%d] for consumer %d to buy\n", app_context.period_count,item_type_id,get_agent_id());
				fprintf(app_context.config->out_test4, "period= %d no_item.[%d] for consumer %d to buy\n", app_context.period_count,item_type_id, get_agent_id());
				break;
			}
			//delete item;
		}//while(Eb>0)の終わり


		//-------------------------------
  
	}//buy_retailsの終わり
*/
/*
* //AIによる新しいロジックに変更　20260625荻林
//	buy_retailsのAIによる新しいロジック　20260107荻林
//int select_item_type_id_for_maximize_utility(const std::set<int>& excluded_ids = {}) {
	int select_item_type_id_for_maximize_utility(const std::set<int>&excluded_ids = std::set<int>()) {

	double max_utility = -1.0;
	int best_item_type_id = -1;

	for (unsigned int i = 0; i < app_context.config->item_type_idk.size(); i++) {

		int current_id = app_context.config->item_type_idk[i];

		// 除外リストに入っている場合はスキップ
		if (excluded_ids.count(current_id)) continue;

		// 効用計算
	
	//	double current_utility =
	//		utility_weight[i] * pow(double(bought_items_number[i]) + 1.0, -2.0);
	
	// pow()の重い計算を1度だけ実行
		double utility_base = double(bought_items_number[i]) + 1.0;
		double current_utility = utility_weight[i] * (1.0 / (utility_base * utility_base));

		if (current_utility > max_utility) {
			max_utility = current_utility;
			best_item_type_id = current_id;
		}
	}

	return best_item_type_id;
}
*/
//AIによる新しいロジックに変更　20260625荻林	
	// 除外リストをunordered_setに
	int select_item_type_id_for_maximize_utility(const std::unordered_set<int>& excluded_ids = {}) {
		double max_utility = -1.0;
		int best_item_type_id = -1;

		const auto& item_type_idk = app_context.config->item_type_idk;
		for (unsigned int i = 0; i < item_type_idk.size(); i++) {
			int current_id = item_type_idk[i];
			if (excluded_ids.find(current_id) != excluded_ids.end()) continue;

			double utility_base = double(bought_items_number[i]) + 1.0;
			double current_utility = utility_weight[i] * (1.0 / (utility_base * utility_base));

			if (current_utility > max_utility) {
				max_utility = current_utility;
				best_item_type_id = current_id;
			}
		}
		return best_item_type_id;
	}



/*
//AIによる新しいロジックに変更　20260625荻林
void buy_retails() {

	RetailMarketNavigator* market_navi =
		app_context.retail_market_navi_for_customer;

	// -----------------------------------------
	// item_type_id → index のマップを構築（重要）
	// -----------------------------------------
	std::unordered_map<int, int> item_index;
	for (int k = 0; k < app_context.config->item_type_idk.size(); k++) {
		item_index[app_context.config->item_type_idk[k]] = k;
	}

	reset_bought_items_number();

	std::set<int> failed_items;
	//fprintf(app_context.config->out_Eb_retailes, "period=,%d,agent_id=,%d, Eb_beginning=,%I64d,\n",
	//	app_context.period_count,get_agent_id(),Eb);
	while (Eb > 0) {

		// 効用最大の item_type_id を選択（除外リスト対応）
		int item_type_id = select_item_type_id_for_maximize_utility(failed_items);
		if (item_type_id == -1) break;
		int ncon=0;
		// -----------------------------------------
		// item_type_id に対応する index を取得（安全）
		// -----------------------------------------
		if (!item_index.count(item_type_id)) {
			
		//	fprintf(app_context.config->out_trace_buy,
		//		"ERROR: item_type_id=,%d, に対応する index が見つかりません\n",
		//		item_type_id);
			
			failed_items.insert(item_type_id);
			ncon ++ ;
		//	fprintf(app_context.config->out_trace_buy, "period=,%d,max_utility=,%d,itemなし%d度目,agent_id=,%d,agent_type=,%d,Eb=,%I64d,\n ",app_context.period_count,item_type_id,ncon,get_agent_id(),get_agent_type(),Eb);
			continue;
		}

		int i = item_index[item_type_id];
		
	//	fprintf(app_context.config->out_trace_buy,
	//		"period=,%d,agent_id=,%d, がmax_utility_id=,%d, を選択しました i=,%d,Eb=,%lld,\n",
	//		app_context.period_count,agent_id, item_type_id, i,Eb);
		
		// 市場取得
		RetailMarket* market = market_navi->get_retail_market(item_type_id);

		// 商品選択
		Item* item = market->select_one(item_type_id);

		if (item != NULL) {

			// 購入可能か？
			if (Eb - item->get_price() > 0) {

				// 取引実行
				item->deal(this);

				bought_items_number[i]++;
				
			//	fprintf(app_context.config->out_trace_buy,
			//		"period=,%d,consumer, %d, がitem_type_id=,%d, 製品価格=,%lld, Eb_remained=,%lld, "
			//		"生産者id=,%d,の製品をmarket=,%p,から購入しました, "
			//		"bought_items_number[,%d,]=,%d,\n",
			//		app_context.period_count,get_agent_id(), item_type_id, item->get_price(), Eb,
			//		item->Get_seller()->Get_agent_id(), market, i,
			//		bought_items_number[i]);
				
			}
			else {

				// 現金不足
			
			//	fprintf(app_context.config->out_trace_buy,
			//		"period=,%d,consumer, %d, がitem_type_id=,%d, 製品価格=,%lld, Eb=,%lld, "
			//		"生産者id=,%d,の製品を購入しようとしましたがcash不足\n",
			//		app_context.period_count,get_agent_id(), item_type_id, item->get_price(), Eb,
			//		item->Get_seller()->Get_agent_id());
			/
				failed_items.insert(item_type_id);
				break;
			}

		}
		else {

			// 商品が市場に存在しない
	//		fprintf(app_context.config->out_trace_buy,

	//			"period=,%d, no item.[,%d,] for consumer, %d, to buy\n",
	//			app_context.period_count, item_type_id, get_agent_id());
			failed_items.insert(item_type_id);
		}
	}


}//buy_retailsの終わり
	//------------
	*/
	//AIによる新しいロジック　20260625荻林	
void buy_retails() {
	RetailMarketNavigator* market_navi = app_context.retail_market_navi_for_customer;

	// item_type_id → index のマップを1回だけ構築
	static std::unordered_map<int, int> item_index;
	if (item_index.empty()) {
		for (int k = 0; k < app_context.config->item_type_idk.size(); k++) {
			item_index[app_context.config->item_type_idk[k]] = k;
		}
	}

	reset_bought_items_number();

	std::unordered_set<int> failed_items;
	while (Eb > 0) {
		int item_type_id = select_item_type_id_for_maximize_utility(failed_items);
		if (item_type_id == -1) break;

		auto it = item_index.find(item_type_id);
		if (it == item_index.end()) {
			failed_items.insert(item_type_id);
			continue;
		}
		int i = it->second;

		RetailMarket* market = market_navi->get_retail_market(item_type_id);
		Item* item = market->select_one(item_type_id);

		if (item != NULL) {
			if (Eb - item->get_price() > 0) {
				item->deal(this);
				bought_items_number[i]++;
			}
			else {
				//2026/10/1 ogibayashi: この品目が買えなかった(予算不足)だけで、break(全品目の購入を中断)していたのを
				//continueに変更。在庫切れ(item==NULL)の場合と同じく、この品目だけ候補から除外して他の(より安い)品目を
				//残りの予算で買いに行けるようにする。これがないと、価格が上がった品目で予算が足りなくなった時点で
				//その期の買い物を丸ごと止めてしまい、価格が下がって在庫過多になっている他の品目に需要が回らず、
				//価格による需給調整(高い品目は買われなくなる→値下がる、安い品目は需要が回る→値上がる)が機能しない。
				failed_items.insert(item_type_id);
				continue;
			}
		}
		else {
			failed_items.insert(item_type_id);
		}
	}
}//buy_retailsの終わり

public:
    Consumer(int agent_id,int agent_type) : Agent(agent_id,agent_type) {
		fprintf(app_context.config->out,"consumer id=%d が生成されました。agent_type=%d\n",get_agent_id(),get_agent_type());
	}
    virtual ~Consumer() {}
	int Get_agent_id() { return agent_id; };
	int Get_AgentType() { return agent_type; }
	//void set_paid_tax_amount(long long paid_tax_amount) {
	//	this->paid_tax_amount = paid_tax_amount;
	//}	
	//long long get_paid_tax_amount() {
	//	return paid_tax_amount;
	//}	
	/*//void set_worker_type(int worker_type) {
		this->worker_type = worker_type;
	}
	int get_worker_type() { return worker_type; }
	*/
    void set_up() {
		//fprintf(app_context.config->out_test0, "Consumer %d のsetup開始\n", get_agent_id());
		//define_agent_type(1);
		fprintf(app_context.config->out_agent_address,"agent_id=,%d,address=,%x,\n",get_agent_id(),unsigned (this));
			//fixed_wage=long long (RandomGenerator::random_int(app_context.config->consumer_fixed_wage_min,app_context.config->consumer_fixed_wage_max));
			fixed_wage=long long (RandomGenerator::random_int(app_context.config->consumerParameter->fixed_wage_min,app_context.config->consumerParameter->fixed_wage_max));
			//fixed_wage = long long(double(fixed_wage) * 2.0);
			//fixed_wage = long long (double (fixed_wage) * 1.2);
		//	fixed_wage = long long(double(fixed_wage) * 1.8);
			//fixed_wage = long long(double(fixed_wage) * 0.1);//20260113にコメントにする。（0.1倍をやめた）
			//fixed_wage = long long(double(fixed_wage) * 0.8);
			//fixed_wage = long long(double(fixed_wage) * 0.9);
			//if (get_agent_type() == 3100 || get_agent_type() == 3110 || get_agent_type() == 3120)fixed_wage = fixed_wage * 2;
			//initial_cash=RandomGenerator::random_int(30000,50000);
			initial_cash = RandomGenerator::random_int(app_context.config->consumerParameter->init_cash_min, app_context.config->consumerParameter->init_cash_max);
			const_income = fixed_wage;
			//initial_cash=fixed_wage*10000;
			//initial_deposit = RandomGenerator::random_int(30000, 50000);
			initial_deposit = RandomGenerator::random_int(app_context.config->consumerParameter->init_cash_min, app_context.config->consumerParameter->init_cash_max);

			deposit=initial_cash;
			bookKeeper->set_initia_cash(initial_cash);
			bookKeeper->set_initial_deposit(initial_deposit);
			bookKeeper->createcashBookSlip(app_context.period_count,initial_cash,0);
			bookKeeper->createbankBookSlip(app_context.period_count, initial_deposit, 0);
			   /*この時点では引数に関係なく以下のようにセットされる。   
					cashbookslip->income=0;
					cashbookslip->balance=initial_cash;
					cashbookslip->spending=0;
				*/
			fprintf(app_context.config->out,"period= %d consumer %d の所持金=%I64dです\n",app_context.period_count,get_agent_id(), initial_cash);
			fprintf(app_context.config->out,"period= %d bookKeeper経由のconsumer %d のinitial_cash=%I64d, spending=%I64d balance=%I64d\n"
				,app_context.period_count,get_agent_id(),bookKeeper->get_initial_cash(),bookKeeper->get_cashbookslip()->spending, bookKeeper->get_cashbookslip()->balance); 
			//rwd=0.01;
			//rwd=0.001;
			//rwd=RandomGenerator::random_double(0,0.05);
			//rwd = RandomGenerator::random_double(0,0.1)
			//rwd=0.0;
			//rwd = app_context.config->consumer_rwd;
			//rpc = app_context.config->consumer_rpc;
			 rwd = app_context.config->consumerParameter->rwd;
			rpc = app_context.config->consumerParameter->rpc;
			//rwd = 0.5;
//			rwd = 0.3;
		    //rwd = 0.7;
			//rpc = 0.7;
			//rpc = 1.0;
			//rpc = 1.5;
			// rpc=2.0;
			//income=int(deposit*rwd);
			//income=int(fixed_wage*0.2);
			//income=fixed_wage;
			//set_income(income);
			 Eb0 = 0;
		
			//Eb0=3000+int(income*rpc)+int(deposit*rwd);
			//Eb0=int(income*rpc)+int(deposit*rwd);//20210726 
			
			// これはplan_expenses()に持ってくる
			/*　
			Eb0 = long long(double(income) * rpc) + long long(double(deposit) * rwd);//20230731
			//Eb0=int( (income+int (deposit*rwd))*rpc);
			//Eb0=bookKeeper->get_cashbookslip()->income*rpc+int(deposit*rwd);
			Eb=Eb0;
			if(app_context.config->ncase == 0) Eb = const_income;//ncase=0 CR simulation, ncase=1 CRW simulation 
		   // cash=income+int(deposit*rwd)
			cash = income + long long(double(deposit) * rwd);;
			//deposit=deposit-int(deposit*rwd);
			deposit -= long long(double(deposit) * rwd);
			asset_end= deposit+cash;
			*/

			//bookKeeper->createjournalSlip(app_context.period_count, 0, "現金", cash, "初期資産投入", cash, "資本金", 2);
			//bookKeeper->createjournalSlip(app_context.period_count, 0, "預金", deposit, "初期資産投入", deposit, "資本金", 2);
			bookKeeper->createjournalSlip(app_context.period_count, 0, "現金", initial_cash, "初期資産投入", initial_cash, "資本金", 2);
			bookKeeper->createjournalSlip(app_context.period_count, 0, "預金", initial_deposit, "初期資産投入", initial_deposit, "資本金", 2);
			expenses=0;
			/*購入する製品種の決定*/
			//購入製品種はエージェント固有の値としランダムに決定する20191123荻林 コメント化20210627
			//item_type_id = RandomGenerator::random_int(1, app_context.config->item_type_id_count) * 10;
			//fprintf(app_context.config->out,"consumer %d の初期所持金=%dです 購入製品種id=%d\n",get_agent_id(),cash,item_type_id);
			fprintf(app_context.config->out,"period= %d consumer %d の初期所持金=%I64dです 購入製品種はその都度決めます\n",app_context.period_count,get_agent_id(),cash);
			setupRolls();
			//購入した製品の数を0にセットする＝bought_itmes_number のvectorを定義する
			//reset_bought_items_number();  agent.hのコンストラクタに移行
		//	set_utility_weight_biased();//item_type_idkの数だけutility_weightを定義する。値はretailer製品を0.5-1乱数、他を0-1乱数で決定する。20210627
			//set_utility_weight_biased03();
			set_utility_weight_random();	//item_type_idkの数だけutility_weightを定義する。値はランダムに決定する。20210627
			//fprintf(app_context.config->out_test0, "inside setup() Consumer %d agent_type=,%d,", get_agent_id(),get_agent_type());
			
			fprintf(app_context.config->out_utility_weight, "agent_no=,%d,", agent_id);
			for (unsigned int i = 0; i < app_context.config->item_type_idk.size(); i++) {
				fprintf(app_context.config->out_utility_weight, "utility_weight[%d]=,%7.3f,",i,utility_weight[i]);
			//	fprintf(app_context.config->out_test0, "utility_weight[%d]=,%7.3f,", i, utility_weight[i]);
			}
			fprintf(app_context.config->out_utility_weight, "\n");
			//fprintf(app_context.config->out_test0, "\n");
			
	}
	
	void setupRolls(){
		buyDealer=app_context.rollAggregator->getMarketBuyDealer();
	}
	/*int receive_wages(int wage){
		return wage;
	}*/
	void receive_wages(long long wage, int agent_id){
		//printf("consumer, inside receive_wages() wage=%d\n",wage);
		//引数のagent_idはemployerのagent_id
		//int balance;
	    //balance=bookKeeper->get_cashbookslip()->balance;
		//int previous_balance = bookKeeper->get_cashkbookSlip_balance(app_context.period_count);
		bookKeeper->createcashBookSlip(app_context.period_count,wage,0);
		//bookKeeper->createbankBookSlip(app_context.period_count, wage, 0);
		bookKeeper->createjournalSlip(app_context.period_count, 0, "現金", wage, "固定給受取", wage, "給与収入", 3);
		//bookKeeper->createjournalSlip(app_context.period_count, 0, "預金", wage, "固定給受取", wage, "給与収入", 3);
		//income+=wage;
		consumer_income_received += wage;
		set_income(consumer_income_received);
		//income = consumer_income_received;//収入が複数回あった場合でも収入がある度にincomeの値を更新する（期末に受け取る、それを用いて翌期に消費する
		income_count++;
		//set_income(income);
		set_var_income(income);
			//fprintf(app_context.config->out_test,"inside receive_wages,period=,%d, income=,%d,income_count=,%d,wage=,%d,\n",app_context.period_count,income,income_count,wage);
		cash+=income	;
		fprintf(app_context.config->retailMarketData->out_income_expenses,"inside receive_wage, period=,%d,consumer_id=,%d,income=,%I64d,cash=,%I64d,wage=,%I64d,\n",app_context.period_count,this->get_agent_id(),income,cash,wage);
	//	fprintf(app_context.config->out3,"receive wage()が呼ばれました consumer_id=,%d,income=,%I64d,wage=,%I64d,\n",this->get_agent_id(),income, wage);
	/*
		fprintf(app_context.config->out_trace_buy, "period=,%d,consumer_id=,%d,income=,%I64d,を受け取りましたget_var_income()=,%lld,cash=,%I64d,wage=,%I64d,var_income=,%lld\n", 
			app_context.period_count, this->get_agent_id(), income,get_var_income(), cash, wage,get_var_income());		
	*/
			//set_cash(cash+wage);
		//fprintf(app_context.config->out_test0, "after receive_wages in consumer , period=,%d,agent_id=,%d, agent_type()=,%d,get_income()=,%I64d,wage=,%lld,fixed_wage=,%lld, \n",
		//	app_context.period_count, get_agent_id(), get_agent_type(), get_income(), wage, get_fixed_wage());
		//fprintf(app_context.config->out_Eb," inside receive_wages(), period=, % d, agent_i, % d, agent_type()=, % d, get_income()=, % I64d, consumer_income_received=,%lld,wage=, % lld, fixed_wage=, % lld, \n",
		//	app_context.period_count, get_agent_id(), get_agent_type(), get_income(), consumer_income_received, wage,get_fixed_wage());
	}
	long long get_fixed_wage(){
		return fixed_wage;
	}
	void plan_expenses(long long income){
		//long long min_consumption=4000;
		long long min_consumption = app_context.config->consumerParameter->min_consumption;	
		double tax_rate_income = app_context.config->governmentParameter->tax_rate_income;
		//consuemr->do_kishu()で呼ばれる　　Ebの決定
		//rwd=0なら、Eb=income*rpc
		//long long  income;
		//printf("starting of plan_expenses() agent_id=%d agent_type=%d,\n", get_agent_id(), get_agent_type());
		/*int cash_max_on_hand = get_fixed_wage() * 1.2;
		int cash_balance = bookKeeper->get_cashbookSlip_balance();
		int withdrawal_amount=cash_balance-cash_max_on_hand;
		if (withdrawal_amount < 0) {
			//手持ち資金が少なければ預金から引き出す。
			bookKeeper->createjournalSlip(app_context.period_count, 0, "現金", withdrawal_amount, "預金引き出し", withdrawal_amount, "預金", 0);
			bookKeeper->createcashBookSlip(app_context.period_count,withdrawal_amount,0);
			bookKeeper->createbankBookSlip(app_context.period_count, 0, withdrawal_amount);
		}
		else {
			//手持ち現金が、多ければ余剰分を預金に預け入れる
			bookKeeper->createjournalSlip(app_context.period_count, 0, "預金", withdrawal_amount, "預金預け入れ", withdrawal_amount, "現金", 0);
			bookKeeper->createcashBookSlip(app_context.period_count, 0,withdrawal_amount);
			bookKeeper->createbankBookSlip(app_context.period_count,  withdrawal_amount,0);
		}
		//Eb0=3000+int(income*rpc)+int(deposit*rwd);
		//int deposit;
		//deposit = bookKeeper->get_bankbookSlip_balance();
		//Eb0=int (income*rpc)+int (deposit*rwd);
		//if (deposit > 0) Eb0 = int(income * rpc) + int(deposit * rwd);//Eb0=bookKeeper->get_cashbookslip()->income*rpc+int(deposit*rwd);//消費予算	
		//else Eb0 = income * rpc;
		if (income < cash_max_on_hand) Eb0 = income * rpc;
		else Eb0 = cash_max_on_hand * rpc;
		if (Eb0 > 0) Eb = Eb0;
		else Eb = 0;
		int bank_balance=bookKeeper->get_bankbookSlip_balance();
		if(Eb>0 && cash_balance<Eb0)

		*/
		//printf("before cash_balance\n");
		long long cash_balance=0;
		long long wage1 = 0;
		//if (app_context.period_count == 0) cash_balance = bookKeeper->get_cashbookslip()->balance;
		//else cash_balance= bookKeeper->get_cashbookSlip_balance_kishu();
		//printf("after long long cash_balance=0\n");
		long long amount_withdrawal= long long (rwd * bookKeeper->get_bankbookslip()->balance);
		// income = fixed_wage;
		//income = get_income();  //incomeは期首のdefine_employee_wages(),define_executive_wages()で決定されている。
		//Eb0の決定	
		//Eb0 = long long(double(income) * rpc);//rpcはAgent.hで宣言、Consumerのsetupで初期化されている。	
////////original consumption function
		/*
		if(app_context.period_count==0) Eb0 = min_consumption+long long((double(fixed_wage)*(1-tax_rate_income)-double (min_consumption)) * rpc) + amount_withdrawal;//20230731
		//else Eb0 = long long(double(get_var_income()) * rpc) + amount_withdrawal;//20230731 get_income()は期首にreceive_wages()で決定されている。	
		else Eb0 = min_consumption+long long((double(get_income())-double(min_consumption)) * rpc) + amount_withdrawal;//20230731 get_income()は期首にreceive_wages()で決定されている。	
		*/
///////consumprtion function revised 20290927 ogibayashi
		if (app_context.period_count == 0)
			Eb0 = min_consumption + long long(double(fixed_wage) * (1 - tax_rate_income) * rpc) + amount_withdrawal;//20230731
		//else Eb0 = long long(double(get_var_income()) * rpc) + amount_withdrawal;//20230731 get_income()は期首にreceive_wages()で決定されている。	
		else Eb0 = min_consumption + long long(double(get_income()) * rpc) + amount_withdrawal;//20230731 get_income()は期首にreceive_wages()で決定されている。	

		/*
		fprintf(app_context.config->out_trace_buy, "period=,%d,consumer_id=,%d,get_income=,%I64d,get_var_income()=,%lld,deposit=,%I64d,rwd=,%6.3f,rpc=,%6.3f,amount_withdrawal=,%I64d,inside plan_expenses()\n",
			app_context.period_count, this->get_agent_id(), get_income(), get_var_income(), bookKeeper->get_bankbookslip()->balance, rwd, rpc, amount_withdrawal);
   */
		Eb0 += Eb; //Eb0は前期からの繰越分を含む	
		if(app_context.config->areamarketParameter->case_wihtout_expense_initial_deposit_and_cash!=1)Eb0 += amount_withdrawal;
		//printf("Eb0= %I64d income= %I64d agent_type=%d, amount_withdrawal=%I64d address of bookKeeper=%x\n", Eb0,income,get_agent_type(),amount_withdrawal, unsigned(this->bookKeeper));
		//預金引き出し率分の額を無条件に引き出しておく
		//bookKeeper->withdraw_bankdeposit(amount_withdrawal);
		//printf("after bookKeeper->withdraw_bankdeposit amount_withdrawal=%I64d\n",amount_withdrawal);
		if (app_context.config->areamarketParameter->case_wihtout_expense_initial_deposit_and_cash != 1) {
			if (rpc > 1) bookKeeper->withdraw_bankdeposit(long long(double(income) * (rpc - 1)));
		}
		
		/*
		bookKeeper->createcashBookSlip(app_context.period_count, amount_withdrawal, 0);
		bookKeeper->createbankBookSlip(app_context.period_count, 0, amount_withdrawal);
		bookKeeper->createjournalSlip(app_context.period_count, 0, "現金", amount_withdrawal, "預金引き出し", amount_withdrawal, "預金", 0);
		*/
		//cash_balance = bookKeeper->get_cashbookslip()->balance;//預金引き出し後の現金残高
		//printf("cash_balance-Eb0= %I64d \n", cash_balance - Eb0);
		if (app_context.config->areamarketParameter->case_wihtout_expense_initial_deposit_and_cash != 1) {
			if (cash_balance - Eb0 < 0) {
				//Eb = cash_balance;//20230731コメント化
				//cashがEb0に対して不足であれば不足分だけ預金を引き出す
				//預金引き出しは無条件にやめる
				//bookKeeper->withdraw_bankdeposit(Eb0 - cash_balance);//20230715 関数作成

				//bookKeeper->createjournalSlip(app_context.period_count, 0, "現金", Eb0-cash_balance, "預金引き出し", Eb0-cash_balance, "預金", 0);//20230611修正
				//bookKeeper->createcashBookSlip(app_context.period_count, Eb0-cash_balance,0);
				//bookKeeper->createbankBookSlip(app_context.period_count, 0,Eb0-cash_balance);
				//Eb = bookKeeper->get_cashbookslip()->balance;//預金引き出し後の現金残高
				//Eb = long long(double(income) * rpc);//20230801
				Eb = Eb0;
				if (app_context.config->ncase == 0) Eb = fixed_wage;//ncase=0 CR simulation, ncase=1 CRW simulation 

			}
			else {
				Eb = Eb0;
				//Eb = cash_balance;//20230731コメント化
				 //Eb = long long(double(income) * rpc);//20230801
				//cash_balance-Ebを預金する
				//bookKeeper->createjournalSlip(app_context.period_count, 0, "預金", Eb-cash_balance, "預金預け入れ", Eb-cash_balance, "現金", 0);これは間違い20220626
				//bookKeeper->createcashBookSlip(app_context.period_count, 0, Eb-cash_balance);
				//bookKeeper->createbankBookSlip(app_context.period_count, Eb-cash_balance, 0);
				//預金預け入れ
				/*
				bookKeeper->createjournalSlip(app_context.period_count, 0, "預金", cash_balance - Eb, "預金預け入れ", cash_balance - Eb, "現金", 0);//20220626修正
				bookKeeper->createcashBookSlip(app_context.period_count, 0, cash_balance-Eb);
				bookKeeper->createbankBookSlip(app_context.period_count, cash_balance-Eb, 0);
				*/
			}
		}//if(app_context.config->areamarketParameter->case_wihtout_expense_initial_deposit_and_cash != 1)の終わり

	
		//if (get_agent_id() < 3)fprintf(app_context.config->out_test, "agent_id=,%d,income=,%d,depost=,%d,rpc=%6.3f,rwd=%6.3f,EB0=,%d,\n", get_agent_id(), income, deposit, rpc, rwd, Eb0);
		if (get_agent_id() < 3)fprintf(app_context.config->out_test, "period=,%d,agent_id=,%d,income=,%I64d,cash_balance=,%I64d,amount_withdrawal=,%I64d,rpc=,%6.3f,rwd=,%6.3f,EB0=,%I64d,Eb=,%I64d,\n"
			, app_context.period_count,get_agent_id(), income, cash_balance,amount_withdrawal, rpc, rwd, Eb0,Eb);
		//printf("after else\n");
		/*
		Eb0 = income * rpc;
		if (Eb0 > 0) Eb = Eb0;
		else Eb = 0;
		*/
		//以上20220526荻林
		// else Eb0=bookKeeper->get_cashbookslip()->income*rpc;
		//Eb0 += 3000;//added on the date 20210726
		//printf(" before calculation of cash in plan_expenses\n");
		if (app_context.config->areamarketParameter->case_wihtout_expense_initial_deposit_and_cash != 1) {
			cash = income + int(deposit * rwd);
			//printf("after calculation of cash   cash=%I64d \n", cash);
			fprintf(app_context.config->retailMarketData->out_income_expenses, " kishu,period=,%d,consumer_id=,%d,income=,%I64d,cash=,%I64d,deposit=,%I64d,int(deposit*rwd)=,%d,Eb0=,%I64d,\n", app_context.period_count, this->get_agent_id(), income, cash, deposit, int(deposit * rwd), Eb0);
			deposit = deposit - long long(double(deposit) * rwd);
		}
		else {
			cash = income;
			deposit = deposit;
		}
		//printf("after calculation of deposit deposit=%I64d \n", deposit);
		if (app_context.config->ncase == 0) Eb = fixed_wage;//ncase=0 CR simulation, ncase=1 CRW simulation
	}
	void do_kishu() {
		initialize_consumer_income_received();
		/*
		fprintf(app_context.config->out_trace_buy, "period=,%d,consumer_id=,%d,agent_type=,%d,income=,%lld,get_income()=,%lld,inside do_kishu()before plan_expenses, before accounting_treatment\n",
			app_context.period_count, get_agent_id(), get_agent_type(), income, get_income());
		*/
			//if(get_agent_id()==10) fprintf(app_context.config->out_test0, "start of  Consumer-do_kishu() ,period=,%d,consumer_id=,%d,agent_type=,%d,income=,%lld,get_income()=,%lld,fixed_wage=,%lld,inside do_kishu()before plan_expenses, before accounting_treatment\n",
		//	app_context.period_count, get_agent_id(), get_agent_type(), income, get_income(), get_fixed_wage());
		//printf("before accounting tratmnt at kishu() agnt_type=%d\n",get_agent_type());
		accounting_treatment_at_kishu(); //Agent.hで定義済み
		//bookKeeper->createcashBookSlip(app_context.period_count, 0, 0);//前期繰越用 //income, spending
		//bookKeeper->createbankBookSlip(app_context.period_count, 0, 0);//前期繰越用 //depoist, withdrawal
		//printf("period= %d end of accounting treatment at do_kishu -- consumer %d agent_type=%d\n",app_context.period_count,agent_id,get_agent_type());
		//fprintf(app_context.config->out, "perod= %d do_kishu -- consumer %d\n", app_context.period_count, agent_id);
		/*  Enterprise->do_kishu()でpay_tax()を呼んでいるため、ここではコメント化  20251218
		if (get_agent_type() == 3100) {
			pay_tax();	//個人事業主の所得税支払　20251215荻林追加
		}
		*/
		/*
		fprintf(app_context.config->out_trace_buy, "period=,%d,consumer_id=,%d,agent_type=,%d,income=,%lld,get_income()=,%lld,inside do_kishu()before plan_expenses\n", 
			app_context.period_count, get_agent_id(), get_agent_type(),income, get_income());
		*/
			/*
		//incomeの値は、period=0に対しては、set_up()で、
		　　　　period>0に対しては従業員、経営者については、define_epmployee__wage_list,define_executive_wage_list()で決める。
			　　但し、period>0, 個人事業主経営者については、
		if (app_context.period_count == 0) {
			if (get_agent_type() == 31) income = 0;
			else income = get_fixed_wage();
		}
		else {
			if (get_agent_type() == 31) income = 0;//失業者の所得は0とする。
			else {}   //incomeは前期末にreceive_wages()で決定されている。	
		}
		*/
       // plan_expenses(get_income());

		//printf("after plan_exspenses_in consumer do_kishu()...\n");
		asset_begining=asset_end;
		//printf("after asset\begining\n");
		/*Eb = Eb0;
		if (app_context.config->ncase == 0) Eb = const_income;
		*/
		income_count=0;
		//fprintf(app_context.config->out_test,"in kishu,period=,%d,consumer_id=,%d, income=,%d,\n",app_context.period_count,this->get_agent_id(),income);
		//income=0;
		expenses=0;
		
		/*if(app_context.period_count==0){
			//set_cash(cash);
		}
		else{
			int wage = 0;
			//wage=receive_wages(fixed_wage);
			//set_cash(cash+wage);
			Eb=income+int (deposit*rwd);
			deposit-=int (deposit*rwd);
			income=0;
		}*/
		int size_of_stock_10 = 0;
		int size_of_stock_20 = 0;
		//size_of_stock_10=app_context.retail_market_navi_for_customer->get_retail_market(get_agent_id(),10)->get_ITEM_SET_size(10);
		//size_of_stock_20 = app_context.retail_market_navi_for_customer->get_retail_market(get_agent_id(),20)->get_ITEM_SET_size(20);
		size_of_stock_10=app_context.retail_market_navi_for_customer->get_retail_market(10)->get_ITEM_SET_size(10);
		size_of_stock_20 = app_context.retail_market_navi_for_customer->get_retail_market(20)->get_ITEM_SET_size(20);
		//printf("before fprintf\n");
		//fprintf(app_context.config->out,"consumer %d の所持金=%dです\n",get_agent_id(),cash);
		//fprintf(app_context.config->out_Eb,"inside kishu, period=,%d,income=,%d,deposit=,%d,Eb=,%d,\n",app_context.period_count,income, deposit, Eb); 
		//fprintf(app_context.config->out, "period= %d consumer %d の期首所持金=%I64dです\n", app_context.period_count,get_agent_id(), bookKeeper->get_cashbookSlip_balance());
		//fprintf(app_context.config->out_Eb, "inside kishu, period=,%d,agent_id=,%d,agent_type=,%d,income=,%I64d,現金残高=,%I64d,預金残高=,%I64d,Eb=,%I64d,size_of_stock_10=,%d,size_of_stock_20=,%d,\n", app_context.period_count, get_agent_id(),get_agent_type(),income,bookKeeper->get_cashbookSlip_balance(),bookKeeper->get_bankbookSlip_balance(), Eb,size_of_stock_10,size_of_stock_20);
		//pay_tax();	今は意味のない存在のなので一時的に消去  20120523記　高島
		//printf("end of consumer_kishu() agent_id=%d agent_type=%d \n",get_agent_id(), get_agent_type());
	//	if(get_agent_id()==10) fprintf(app_context.config->out_test0, "end of Consumer-do_kishu(), period=,%d,consumer_id=,%d,agent_type=,%d,income=,%lld,get_income()=,%lld,fixed_wge=,%lld,inside do_kishu()before plan_expenses, before accounting_treatment\n",
	//		app_context.period_count, get_agent_id(), get_agent_type(), income, get_income(),get_fixed_wage());
	}
	
    void do_kimatsu() {
		//if (get_agent_type() != 31) buy_retails();			//最終消費財の購買　自関　  20120524記　高島 無職者は消費行動しない
        //printf("do_kimatsu -- consumer %d\n",get_agent_id());
		//fprintf(app_context.config->out,"do_kimatsu -- consumer　%d\n",get_agent_id());
	//	fprintf(app_context.config->out_test0, "start of inside consumer_>do_kimatsu(),period=,%d,consumer_id=,%d,agent_type=,%d,income=,%lld,inside do_kimatsu() before accounting_treatment\n",
	//		app_context.period_count, get_agent_id(), get_agent_type(), get_income());
		//fprintf(app_context.config->out_test,"in kimatsu, period=,%d,consumer_id=,%d,income=,%d,\n",app_context.period_count,this->get_agent_id(),income);
		/*cash += income - expenses;
		asset_end=asset_begining+income-expenses;
		deposit+=cash;  //cashはすべて預金する
		cash=0;
		*/
		//pay_tax();	//消費者の税金支払　20251215荻林追加	
		int cash_balance=0;
		cash_balance = bookKeeper->get_cashbookslip()->balance;
	/*	if (cash_balance - Eb0 < 0) {
			//預金引き出し
			bookKeeper->createjournalSlip(app_context.period_count, 0, "現金", Eb0 - cash_balance, "預金引き出し", Eb0 - cash_balance, "預金", 0);//20230611修正
			bookKeeper->createcashBookSlip(app_context.period_count, Eb0 - cash_balance, 0);
			bookKeeper->createbankBookSlip(app_context.period_count, 0, Eb0 - cash_balance);

		}
		*/
		if (cash_balance - Eb0>0) {
			//預金預け入れ
		//	bookKeeper->deposit_bankaccount(cash_balance - Eb0);
			/*
			bookKeeper->createjournalSlip(app_context.period_count, 0, "預金", cash_balance - Eb0, "預金預け入れ", cash_balance - Eb0, "現金", 0);//20220626修正
			bookKeeper->createcashBookSlip(app_context.period_count, 0, cash_balance - Eb0);
			bookKeeper->createbankBookSlip(app_context.period_count, cash_balance - Eb0, 0);
			*/
		}
		

		cash = bookKeeper->get_cashbookSlip_balance();
		deposit = bookKeeper->get_bankbookSlip_balance();
		fprintf(app_context.config->retailMarketData->out_income_expenses,"kimatsu,period=,%d,consumer_id=,%d,income=,%I64d,expenses=,%I64d,Eb0=,%I64d,Eb=,%I64d,\n",app_context.period_count,this->get_agent_id(),income,expenses,Eb0,Eb);
	//	fprintf(app_context.config->out_Eb,"inside kimatsu, period=,%d,agent_id=,%d,income=,%I64d,deposit=,%I64d,Eb=,%I64d,\n",app_context.period_count,get_agent_id(),income, deposit, Eb); 
		//printf("end of do_kimatsu of consumer \n");
	//	fprintf(app_context.config->out_test0, "end of inside consumer_>do_kimatsu(),period=,%d,consumer_id=,%d,agent_type=,%d,income=,%lld,inside do_kimatsu() after accounting_treatment\n",
	//		app_context.period_count, get_agent_id(), get_agent_type(), get_income());
	}

    void action() {

		//fprintf(app_context.config->out_Eb, "before plan_expenses in action, period=,%d,agent_id=,%d,income=,%I64d,deposit=,%I64d,Eb=,%I64d,\n"
		//	, app_context.period_count, get_agent_id(), income, deposit, Eb);
		fprintf(app_context.config->out_consumer_income_Eb_tax, "before and after plan expenses in consumer action,period=,%d,consumer_id=,%d,income=,%lld,Eb=,%lld,",
			app_context.period_count, get_agent_id(), get_income(), Eb);
		plan_expenses(get_income());
		fprintf(app_context.config->out_consumer_income_Eb_tax, " new_Eb=,%lld,tax=,%lld, cash=,%lld,deposit=,%lld,\n",
			 Eb,get_paid_tax_amount(),get_cash(),get_deposit());
		//fprintf(app_context.config->out_Eb, "after plan_expenses before buy_retailes in action, period=,%d,agent_id=,%d,income=,%I64d,deposit=,%I64d,Eb=,%I64d,\n", app_context.period_count, get_agent_id(), income, deposit, Eb);
       //printf("action -- consumer %d \n",get_agent_id());
		//fprintf(app_context.config->out,"action -- consumer %d\n",get_agent_id());
		//printf("before buy_retails period=%d agent_id=%d agent_type=%d \n",app_context.period_count,get_agent_id(),get_agent_type());
		if (get_agent_type() != 31) buy_retails();			//最終消費財の購買　自関　  20120524記　高島 無職者は消費行動しない
    }
	
    /**
     *  支払
     *
     *  @param item 商品
     */

	/*
	void pay_tax(long long income) {
    double tax_rate_inocome = app_context.config->governmentParameter->tax_rate_income;

    auto agent_list = app_context.governmentAggregatorNavigator
                        ->get_agent_aggregator()
                        ->get_agent_list();

    if (!agent_list.empty()) {
        Agent* ag = agent_list[0];
        long long tax_amount = static_cast<long long>(income * tax_rate_income);
        //ag->receive_tax(tax_amount);
		ag->receive_tax(tax_amount, get_agent_id(),get_agent_type());	
    }
}


	
	
	*/





	void pay_tax(long long profit){
		/*
		//if(get_agent_type()==3100) printf("consumer %d is going to pay tax income=%d\n", get_agent_id(), income);
		double tax_rate_income = app_context.config->governmentParameter->tax_rate_income;
		//printf("consumer %d tax_rate_income=%6.3f\n", get_agent_id(), tax_rate_income);
		vector <Agent*> agent_list = app_context.governmentAggregatorNavigator->get_agent_aggregator()->get_agent_list();
		//printf("consumer %d found government ag list size=%d to pay tax\n", get_agent_id(), agent_list.size());
		if (!agent_list.empty()) {
			Agent* ag = agent_list[0];
			long long tax_amount = static_cast<long long>(double(profit) * tax_rate_income);
			//ag->receive_tax(tax_amount);
			ag->receive_tax(tax_amount, get_agent_id(),get_agent_type());
		}
		income -= static_cast<long long>(double(profit) * tax_rate_income);//税引き後所得に修正
		//printf("consumer %d finished pay tax income=%I64d\n", get_agent_id(), income);
		//Agent* ag;
		//printf("consumer %d is going to pay tax income=%I64d before specify government ag\n", get_agent_id(), income);
		//ag = app_context.governmentAggregatorNavigator->get_agent_aggregator()->get_agent_list()[0];
		//ag = app_context.government_aggregator->get_agent_list()[0];
		//printf("consumer %d found government ag %d to pay tax\n", get_agent_id(), ag->get_agent_id());
		//ag->receive_tax(long long(double(income) * tax_rate_income));
	//	app_context.government_aggregator->get_agent_list()[0]->receive_tax(long long( double(income) * tax_rate_income));
	//	app_context.government_aggregator = new GovernmentAggregator(1);


			/*
			long long tax_amount = 0;
			tax_amount=long long(double(income)*0.1);//所得の10%を税金として支払う
			if(tax_amount>0){
				//int balance;
				//balance=bookKeeper->get_cashbookslip()->balancae;
				//int previous_balance = bookKeeper->get_cashkbookSlip_balance(app_context.period_count);
				bookKeeper->createcashBookSlip(app_context.period_count,0,tax_amount);
				//bookKeeper->createbankBookSlip(app_context.period_count, 0, tax_amount);
				bookKeeper->createjournalSlip(app_context.period_count, 0, "租税公課", tax_amount, "所得税等支払", tax_amount, "現金", 0);
				//bookKeeper->createjournalSlip(app_context.period_count, 0, "租税公課", tax_amount, "所得税等支払", tax_amount, "預金", 0);
				cash-=tax_amount;
				fprintf(app_context.config->out," period= %d consumer %d が所得税等として%I64d を支払ました.所持金残は%I64d です\n",app_context.period_count,get_agent_id(),tax_amount,cash);
				fprintf(app_context.config->retailMarketData->out_income_expenses,"consumer pay tax, period=,%d,consumer_id=,%d,income=,%I64d,expenses=,%I64d,tax_amount=,%I64d,cash=,%I64d,\n",app_context.period_count,get_agent_id(),income,expenses,tax_amount,cash);
			}
			*/
		
	}
    void pay(Item* item) {
      //  printf("pay -- consumer\n");
		// TODO
		//int balance;
		// balance=bookKeeper->get_cashbookslip()->balance;
		//int previous_balance = bookKeeper->get_cashkbookSlip_balance(app_context.period_count);
		bookKeeper->createcashBookSlip(app_context.period_count,0,item->get_price());
        cash -= item->get_price();
		Eb-=item->get_price();
		expenses+=item->get_price();
		/*
		fprintf(app_context.config->out," period= %d consumer %dがproducer %d に商品種 %d 代金として価格%I64d を支払ます.所持金残は%I64d です\n",
			app_context.period_count,get_agent_id(),item->Get_seller()->Get_agent_id(),item->get_item_type_id(),item->get_price(),cash);
	*/
	//item->Get_seller()->receive_payments(item);//item->deal()に移動　20200305荻林
		/*
        fprintf(app_context.config->out_producer_consumer_income_expenses,"bookKeeper of consumer, period=, %d, agent_id=, %d, income=, %I64d, expenses=, %I64d, balance=, %I64d, agent_id=, %d,at_item->buyer->pay() in consumer pay(),\n",
			app_context.period_count, get_agent_id(), bookKeeper->get_cashbookslip()->income, bookKeeper->get_cashbookslip()->spending, bookKeeper->get_cashbookslip()->balance, item->Get_seller()->Get_agent_id());
		*/
			//仕訳伝票作成
		bookKeeper->createjournalSlip(app_context.period_count, 0,"製品", item->get_price(), "製品購入", item->get_price(), "現金",0);
		bookKeeper->createstockBookSlip(app_context.period_count, 0, 1, item->get_item_type_id(), item->get_price());
		bookKeeper->createstockpriceBookSlip(app_context.period_count, 0, item->get_price(), item->get_item_type_id());
    }
};

