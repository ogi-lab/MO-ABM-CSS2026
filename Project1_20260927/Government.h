#pragma once
#include "Agent.h"
#include <stdio.h>
#include "SimulatorConfig.h"
#include "SimulatorContext.h"
#include "DataAnalyzer.h"
#include "DataAnalyzerNavigator.h"
#include "AccountData.h"
#include <set>
#include <unordered_map>

extern SimulatorContext app_context;

/** リテイラー */

class Government : public Agent, public Buyer {
	double tax_rate_income = 0.0; //所得税率
	double tax_rate_corporate = 0.0; //法人税率
	double subsidy_ratio = 0.0; //補助金率
	double market_purchasing_ratio = 0.0; //市場購入比率
	long long tax_revenue_income_received = 0;
	long long tax_revenue_corporate_received = 0;
	long long total_tax_income_received = 0;
	long long total_transfer_payment_made = 0;
	long long total_tax_corporate_received = 0;
	long long withholdings0 = 0;
	void set_withholdings(long long w) { withholdings0 = w; }
	 	long long get_withholdings0() { return app_context.withholdings0; }

	//	buy_retailsのAIによる新しいロジック　20260107荻林
	int select_item_type_id_for_maximize_utility(const std::set<int>& excluded_ids = std::set<int>()) {

		double max_utility = -1.0;
		int best_item_type_id = -1;

		for (unsigned int i = 0; i < app_context.config->item_type_idk.size(); i++) {

			int current_id = app_context.config->item_type_idk[i];

			// 除外リストに入っている場合はスキップ
			if (excluded_ids.count(current_id)) continue;

			// 効用計算
			double current_utility =
				utility_weight[i] * pow(double(bought_items_number[i]) + 1.0, -2.0);

			if (current_utility > max_utility) {
				max_utility = current_utility;
				best_item_type_id = current_id;
			}
		}// forの終わり

		return best_item_type_id;
	}

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
	//	fprintf(app_context.config->out_Eb_retailes_government, "period=,%d,agent_id=,%d, Eb_beginning=,%I64d,\n",
	//		app_context.period_count, get_agent_id(), Eb);
		while (Eb > 0) {

			// 効用最大の item_type_id を選択（除外リスト対応）
			int item_type_id = select_item_type_id_for_maximize_utility(failed_items);
			if (item_type_id == -1) break;
			int ncon = 0;
			// -----------------------------------------
			// item_type_id に対応する index を取得（安全）
			// -----------------------------------------
			if (!item_index.count(item_type_id)) {
			/*
				fprintf(app_context.config->out_trace_buy,
					"ERROR: item_type_id=,%d, に対応する index が見つかりません\n",
					item_type_id);
			*/
				failed_items.insert(item_type_id);
				ncon++;
		//		fprintf(app_context.config->out_trace_buy, "government, period=,%d,max_utility=,%d,itemなし%d度目,agent_id=,%d,agent_type=,%d,Eb=,%I64d,\n ", app_context.period_count, item_type_id, ncon, get_agent_id(), get_agent_type(), Eb);
				continue;
			}

			int i = item_index[item_type_id];
      /*
			fprintf(app_context.config->out_trace_buy,
				"government,period=,%d,agent_id=,%d, がmax_utility_id=,%d, を選択しました i=,%d,Eb=,%lld,\n",
				app_context.period_count, agent_id, item_type_id, i, Eb);
		*/

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
			/*
					fprintf(app_context.config->out_trace_buy,
						"government, period=,%d,consumer, %d, がitem_type_id=,%d, 製品価格=,%lld, Eb_remained=,%lld, "
						"生産者id=,%d,の製品をmarket=,%p,から購入しました, "
						"bought_items_number[,%d,]=,%d,\n",
						app_context.period_count, get_agent_id(), item_type_id, item->get_price(), Eb,
						item->Get_seller()->Get_agent_id(), market, i,
						bought_items_number[i]);
			*/
				}
				else {

					// 現金不足
			/*
					fprintf(app_context.config->out_trace_buy,
						"government,period=,%d,consumer, %d, がitem_type_id=,%d, 製品価格=,%lld, Eb=,%lld, "
						"生産者id=,%d,の製品を購入しようとしましたがcash不足\n",
						app_context.period_count, get_agent_id(), item_type_id, item->get_price(), Eb,
						item->Get_seller()->Get_agent_id());
			*/
					failed_items.insert(item_type_id);
					break;
				}

			}
			else {

				// 商品が市場に存在しない
				/*
				fprintf(app_context.config->out_trace_buy,

					"period=,%d, no item.[,%d,] for consumer, %d, to buy\n",
					app_context.period_count, item_type_id, get_agent_id());
				*/
				failed_items.insert(item_type_id);
			}
		}


	}//buy_retailsの終わり
/*
	int select_item_type_id_for_maximize_utility() {
		//vector <int> bought_items_numberはAgent.hで宣言してある。
		double max = 0.0;
		int item_type_idkk = 0;
		int item_type_id = 0;
		RetailMarketNavigator* market_navi = app_context.retail_market_navi_for_customer;
		max = utility_weight[0] * pow((double(bought_items_number[0]) + 1), -2);
		item_type_idkk = app_context.config->item_type_idk[0];
		for (unsigned int i = 0; i < app_context.config->item_type_idk.size(); i++) {

			if (max < utility_weight[i] * pow((double(bought_items_number[i]) + 1), -2)) {
				max = utility_weight[i] * pow((double(bought_items_number[i]) + 1), -2);
				item_type_idkk = app_context.config->item_type_idk[i];
				//		RetailMarket* market = market_navi->get_retail_market( item_type_idkk);
				//		Item* item = market->select_one(item_type_idkk);
				//	if (Eb - item->get_price() > 0) {

				//	}
			}

		}
		return item_type_idkk;
	
	}
	*/
	/*
	void buy_retails() {
		int item_type_id = 0;
		//消費者は毎期１種類の製品種を購入する。その製品種は毎期異なる。
		// 購入製品種は消費者毎に固有の値とするため、setupに移行、ここではコメント化20191123荻林
		RetailMarketNavigator* market_navi = app_context.retail_market_navi_for_customer;
		//-------------------
		reset_bought_items_number();//購入済み製品数を0にリセットする、関数はAgent/hで定義済み20210627
		int i = 0;//item_type_id vectorの添え字
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
					fprintf(app_context.config->out, "government %d がitem_type_id=%d 製品価格=%I64d Eb=%I64d 生産者id=%dの製品をmarket=%xから購入しました,bought_items_number[%d]=%d,\n", get_agent_id(), item_type_id, item->get_price(), Eb, item->Get_seller()->Get_agent_id(), market, i, bought_items_number[i]);
					fprintf(app_context.config->out3, "government %d がitem_type_id=%d 製品価格=%I64d Eb=%I64d 生産者id=%dの製品をmarket=%xから購入しました,bought_items_number[%d]=%d,\n", get_agent_id(), item_type_id, item->get_price(), Eb, item->Get_seller()->Get_agent_id(), market, i, bought_items_number[i]);
					//	fprintf(app_context.config->out_temporary, "consumer %d がitem_type_id=%d 製品価格=%d Eb=%d 生産者id=%dの製品をmarket=%xから購入しました,bought_items_number[%d]=%d,\n", get_agent_id(), item_type_id, item->get_price(), Eb, item->Get_seller()->Get_agent_id(), market, i, bought_items_number[i]);

				}
				else
				{
					//	printf("consumer %d がitem_type_id=%d 製品価格=%d Eb=%d 生産者id=%dの製品を購入しようとしましたがcash不足のため購入しませんでした\n",get_agent_id(),item_type_id,item->get_price(),Eb,item->Get_seller()->Get_agent_id());

					fprintf(app_context.config->out, "consumer %d がitem_type_id=%d 製品価格=%I64d Eb=%I64d 生産者id=%dの製品を購入しようとしましたがcash不足のため購入しませんでした\n", get_agent_id(), item_type_id, item->get_price(), Eb, item->Get_seller()->Get_agent_id());
					fprintf(app_context.config->out3, "consumer %d がitem_type_id=%d 製品価格=%I64d Eb=%I64d 生産者id=%dの製品を購入しようとしましたがcash不足のため購入しませんでした\n", get_agent_id(), item_type_id, item->get_price(), Eb, item->Get_seller()->Get_agent_id());
					//fprintf(app_context.config->out_temporary, "consumer %d がitem_type_id=%d 製品価格=%d Eb=%d 生産者id=%dの製品を購入しようとしましたがcash不足のため購入しませんでした\n", get_agent_id(), item_type_id, item->get_price(), Eb, item->Get_seller()->Get_agent_id());

					break;
				}
			}//if(item!=NULL)の終わり	
			else {
				//商品が存在しない場合
				//printf("no item.[%d]\n", item_type_id);
				fprintf(app_context.config->out, "period= %d no item.[%d] for consumer %d to buy\n", app_context.period_count, item_type_id, get_agent_id());
				fprintf(app_context.config->out_test4, "period= %d no_item.[%d] for consumer %d to buy\n", app_context.period_count, item_type_id, get_agent_id());
				break;
			}
			//delete item;
		}//while(Eb>0)の終わり


		//-------------------------------

	}//buy_retailsの終わり


	*/

public:
   // Government(int agent_id,int agent_type) : Agent(agent_id,agent_type){}
	Government(int agent_id, int agent_type) : Agent(agent_id, agent_type) {
		fprintf(app_context.config->out, "consumer id=%d が生成されました。agent_type=%d\n", get_agent_id(), get_agent_type());
	}
    virtual ~Government(){}
	int Get_agent_id() { return agent_id; };
	int Get_AgentType() { return agent_type; }
	
	double get_tax_rate_income() { return tax_rate_income; }
	double get_tax_rate_corporate() { return tax_rate_corporate; }	
	double get_subsidy_ratio() { return subsidy_ratio; }
	double get__market_purchasing_ratio() { return market_purchasing_ratio; }
	long long get_tax_revenue_income_received() { return tax_revenue_income_received; }
	long long get_tax_revenue_corporate_received() { return tax_revenue_corporate_received; }
	void set_tax_revenue_income_received(long long amount) { tax_revenue_income_received += amount; }	
	void set_tax_revenue_corporate_received(long long amount) { tax_revenue_corporate_received += amount; }	
	void initialize_tax_revenue_income() { tax_revenue_income_received = 0; }
	void initialize_income() { income = 0; }	
    void set_up() {
		tax_revenue_corporate_received = 0;
		initialize_tax_revenue_income();
		tax_revenue_income_received = 0;	
		fprintf(app_context.config->out_test, "government id=%d がセットアップされました。agent_type=%d\n", get_agent_id(), get_agent_type());	
	//	fprintf(app_context.config->out_trace_tax_income, "inside government_set_up,\n");
		/**初期所持金の決定*/
		//set_cash(RandomGenerator::random_int(app_context.config->retailer_init_cash_min,app_context.config->retailer_init_cash_max));
		//fprintf(app_context.config->out,"retailer %d の初期所持金=%dです item_type_id=%d qmax=%d\n",get_agent_id(),cash,item_type_id,qmax);
		initial_cash = RandomGenerator::random_int(app_context.config->governmentParameter->init_cash_min, app_context.config->governmentParameter->init_cash_max);
		initial_deposit = RandomGenerator::random_int(app_context.config->governmentParameter->init_cash_min, app_context.config->governmentParameter->init_cash_max);	
		cash = initial_cash;
		deposit = initial_deposit;
		//bookKeeper->createjournalSlip(app_context.period_count, 0, "現金", initial_cash, "初期資産投入", initial_cash, "資本金", 2);
		//bookKeeper->createjournalSlip(app_context.period_count, 0, "預金", initial_deposit, "初期資産投入", initial_deposit, "資本金", 2);
		//パラメータの設定
		tax_rate_income = app_context.config->governmentParameter->tax_rate_income;
		tax_rate_corporate = app_context.config->governmentParameter->tax_rate_corporate;
		subsidy_ratio = app_context.config->governmentParameter->firm_subsidy_ratio;
		market_purchasing_ratio = app_context.config->governmentParameter->market_purchasing_ratio;
		//購入商品効用の重み付け設定();
		//set_utility_weight();
		set_utility_weight_random();
	}
	void plan_expenses() {
		//long long fixed_wage = 8232 * 600;

		//long long income=0;
		if (app_context.period_count == 0) {
			//income = fixed_wage * get_tax_rate_income(); //初期収入は固定給とする
		//	income = get_withholdings0();
			income = app_context.withholdings0;
		//	income = 3439441 * get_tax_rate_income(); //20260129 荻林	
			fprintf(app_context.config->out_test7, "period=,%d,withholdings=,%lld, initial income=,%lld, get_tax_rate_income()=,%f,\n",
				app_context.period_count, app_context.withholdings0,income, get_tax_rate_income());
		}
		else {
		//	income = bookKeeper->get_cashBookSlipTotal(app_context.period_count - 1)->income;
		income = get_income();
		}
		//Eb0 =previous_income * market_purchasing_ratio;
		Eb0 = long long (income * market_purchasing_ratio);
		//Eb0 += Eb;
		fprintf(app_context.config->out_test3, "period=,%d, income=,%lld,income_tax_rate=,%f, market_purchasing_ratio=,%f, Eb0=,%lld,previos_Eb=,%lld,"
			, app_context.period_count, income, get_tax_rate_income(), market_purchasing_ratio, Eb0, Eb);
		Eb = Eb0+Eb;
		fprintf(app_context.config->out_test3, " Eb=,%lld,\n", Eb);	
		//Eb = Eb0;
	}
	void do_kishu(){
		//income = 0;
		printf("start of do_kishu() government_kishu agent_id=%d \n", get_agent_id());
	//	accounting_treatment_at_kishu(); //Agent.hで定義済み
		printf("before plan_expenses() in  government agent_id=%d \n", get_agent_id());
		//	plan_expenses();
			printf("after plan_expenses() government agent_id=%d Eb=%I64d \n", get_agent_id(), Eb);
			previous_income = get_income();
			fprintf(app_context.config->out_trace_government, "period=,%d,get_income()=,%lld,previous_income=,%lld,\n", app_context.period_count,get_income(), previous_income);
			initialize_tax_revenue_income();
		
	}
	void action() {
	//	fprintf(app_context.config->out_trace_tax_income, "inside government_action before plan_expenses in government action,\n");
		fprintf(app_context.config->out_trace_government, "period=,%d,government_action start,Eb=,%lld,income=,%lld\n", app_context.period_count, Eb,get_income());
		plan_expenses();
		fprintf(app_context.config->out_trace_government, "period=,%d,government_action after plan_expenses,Eb=,%lld,Eb0=,%lld, income=,%lld,\n", app_context.period_count, Eb,Eb0,get_income());
	//	fprintf(app_context.config->out_trace_tax_income, "inside government_action after plan_expenses in government action,\n");
		buy_retails();
	//	fprintf(app_context.config->out_trace_tax_income, "inside government_action after buy_retailes,\n");
		initialize_income();
	}
	void do_kimatsu(){
	//	if (app_context.period_count > 0)		buy_retails();
	//	fprintf(app_context.config->out_trace_tax_income, "inside government_do_kimatsu() after buy_retailes,\n");
	}
	void other_kimatsu_actions(){};
	//void transferstocks_to_StockRecord(){};
	//void receive_tax(long long tax_amount) {
	void receive_tax(long long tax_amount,int agent_id,int agent_type) {
		  //Enterprise.hのpay_tax（）で呼ばれる
		  // agent_id,agent_typeは税金を支払った企業や個人のagent_id,agent_type
		//bookKeeper->createcashBookSlip(app_context.period_count, tax_amount, 0);
		//cash += tax_amount;
		//Eb += tax_amount;
		//income += tax_amount;
		tax_revenue_income_received += tax_amount;
		income = tax_revenue_income_received; //税収が入金される度にincomexの値を更新する
		fprintf(app_context.config->out_tax_revenue_received, "period=,%d, payer's_id=, %d payer's type=,%d,received tax_amount=,%lld, total_tax_revenue_income_received=,%lld,income=,%lld\n",
			app_context.period_count, agent_id, agent_type,tax_amount, tax_revenue_income_received,income);	
	//	fprintf(app_context.config->out, " period= %d government %d が税金として価格%I64d を受け取ます.所持金残は%I64d です\n", app_context.period_count, get_agent_id(), tax_amount, cash);
		//仕訳伝票作成
		//bookKeeper->createjournalSlip(app_context.period_count, 0, "現金", tax_amount, "税金受領", tax_amount, "税金収入", 0);
		//if(app_context.period_count>0) fprintf(app_context.config->out_trace_tax_income,"period=,%d, tax_amount=,%lld, income=,%lld,tax_revenue_income_received=,%lld,inside receive_tax in government.h\n",
		//	app_context.period_count, tax_amount, income,tax_revenue_income_received);
		/*
		AccountData* accountData = NULL;
		accountData = app_context.config->governmentData;
		if(app_context.period_count>0)fprintf(accountData->out_income_taxRevenue, "period=,%d, tax_amount=,%lld,income=%lld,\n", app_context.period_count, tax_amount, income);
		*/
	}
	void pay(Item* item) {
		bookKeeper->createcashBookSlip(app_context.period_count, 0, item->get_price());
		cash -= item->get_price();
		Eb -= item->get_price();
		expenses += item->get_price();
	//	fprintf(app_context.config->out, " period= %d consumer %dがproducer %d に商品種 %d 代金として価格%I64d を支払ます.所持金残は%I64d です\n", app_context.period_count, get_agent_id(), item->Get_seller()->Get_agent_id(), item->get_item_type_id(), item->get_price(), cash);
		//item->Get_seller()->receive_payments(item);//item->deal()に移動　20200305荻林
		//fprintf(app_context.config->out_producer_consumer_income_expenses, "bookKeeper of consumer, period=, %d, agent_id=, %d, income=, %I64d, expenses=, %I64d, balance=, %I64d, agent_id=, %d,\n", app_context.period_count, get_agent_id(), bookKeeper->get_cashbookslip()->income, bookKeeper->get_cashbookslip()->spending, bookKeeper->get_cashbookslip()->balance, item->Get_seller()->Get_agent_id());
		//仕訳伝票作成
		//bookKeeper->createjournalSlip(app_context.period_count, 0, "製品", item->get_price(), "製品購入", item->get_price(), "現金", 0);
		bookKeeper->createstockBookSlip(app_context.period_count, 0, 1, item->get_item_type_id(), item->get_price());
		bookKeeper->createstockpriceBookSlip(app_context.period_count, 0, item->get_price(), item->get_item_type_id());
	}
};

