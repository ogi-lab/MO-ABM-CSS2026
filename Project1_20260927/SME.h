#pragma once
#include "Producer.h"


extern SimulatorContext app_context;

/** リテイラー */
class SME : public Producer {

	//vector <int> employee_wage_list;
	long long direct_material_fee = 0;
	int item_type_id_buy = 0;// item_type_id of material to buy
	int define_and_get_item_type_id_buy() {
		int item_type_id = get_item_type_id();//生産する品種
		int ijk = 0;
		//生産品種ごとに対応する原料品種を決定する場合は以下のルーチンを使用する	
		for (int i = 0; i < app_context.config->item_type_idkr.size(); i++) {
			if (item_type_id == app_context.config->item_type_idkr[i]) {
				ijk = i;
				break;
			}
		}
		return app_context.config->item_type_idkw[ijk];
	}

	void buy_materials() {
		direct_material_fee = 0;
		//printf("start of buy materials in retailer.h \n");
		//if (app_context.config->ncase = !0) {//ncase=0;//ncase=0 CR simulation, ncase=1 CRW simulation
			//if (app_context.config->wholesaler_count > 0) {//wholesalerが存在する場合はwholesalerから購入することにする　20210905記　荻林
				/*  購入対象の市場を定義する　右辺の実体はSimulatorAppのset_up_marketで定義されている　*/
			RetailMarketNavigator* market_navi = app_context.material_market_navi_for_retailer;//SimulatorAppのset_up_market内でnewされている
			/* 購入するitem_type_idを定義する。vector <int> item_type_idkwの中から ランダムに一種類選択することにする*/
			//	int item_type_id_buy;//class属性変数に移行20230605荻林
				//ランダムに決定する。
		
			//生産品種ごとに対応する原料品種を決定する場合は以下のルーチンを使用する	
			item_type_id_to_buy = get_item_type_id_to_buy();//set_upで決定済みのものを取得する	

			//item_type_id_buy = item_type_id;
			//fprintf(app_context.config->out_temporary, "inside buy_material of Retailer  address of market=%x\n",market);
		/**購入する個数は生産する製品個数と同じとする*/
			int q_count = get_q_count();
			if (agent_id == 509) fprintf(app_context.config->out509, "period=,%d,q_count=,%d,inside buy matewrials(),\n", app_context.period_count, q_count);
			//int expected_sales = int(q_count * get_price()*0.7);
			int expected_sales = int(q_count * get_price() * 0.3);
			//int n = 1;
			int n = 0;//購入できた原料の数
			//if (get_agent_id() == 160) printf("inside buy_materials q_count = % d\n", q_count);
			// if (get_agent_id() == 478 || get_agent_id() == 488 || get_agent_id() == 512)
			//	 fprintf(app_context.config->out_test10, "period=%d agent_id=%d 購入前　q_count=%d,  item_type_id_buy=%d balance=%I64d\n"
			//		 , app_context.period_count,get_agent_id(),q_count, item_type_id_buy,bookKeeper->get_bankbookslip()->balance);
			//while (n <= q_count) {
			// printf("before while in buy_materials()\n"); 
			 //RetailMarket* market = market_navi->get_retail_market(agent_id, item_type_id_buy);//navigatorに従ってmaterialMarketが選択される
			RetailMarket* market = market_navi->get_retail_market(item_type_id_buy);//navigatorに従ってmaterialMarketが選択される
			if (agent_id == 509) fprintf(app_context.config->out509, "period=,%d,q_count aimed=,%d, before_agent_id=,%d,to buy, item_type_id_buy=,%d,size=,%d,\n", app_context.period_count, q_count, get_agent_id(), item_type_id_buy, market->get_ITEM_SET_size(item_type_id_buy));
			//q_cont=0（生産計画の時点で予定生産個数=0のケース）の時は以下のルーチンは通らないので注意
			while (n < q_count) {
				
				//	 printf("inside while  before market->select_one()\n");
				if (item_type_id_buy == 60) {
					if (app_context.period_count == 20) fprintf(app_context.config->out509, "period=,%d, before_agent_id=,%d,to buy, item_type_id_buy=,%d,size=,%d,\n", app_context.period_count, get_agent_id(), item_type_id_buy, market->get_ITEM_SET_size(item_type_id_buy));
				}
				//if (agent_id == 5090)fprintf(app_context.config->out509, "item_type=id=,%d,size=,%d");
				//if(agent_id==509)  market->materialMarket_print_all_items(2);
			
				Item* item = market->select_one(item_type_id_buy);
				//if (item != NULL && (item->get_price() * n) < expected_sales) {
		   //	 printf("after market->selecct_one() before if(Item==NULL) \n");
				if (item == NULL && app_context.period_count == 0) {
					//if (get_agent_id() == 478 || get_agent_id() == 488 || get_agent_id() == 512)
					fprintf(app_context.config->out_test10, "period=%d agent_id=%d 購入前　item=NULL でした　n＝%d q_count=%d,  item_type_id_buy=%d\n"
						, app_context.period_count, get_agent_id(), n, q_count, get_item_type_id_to_buy());
					fprintf(app_context.config->materialMarketData->areamarket2, "//-----period= %d agent_id=%d item_type_id_buy=%d \n", app_context.period_count, get_agent_id(), get_item_type_id_to_buy());
					// market->materialMarket_print_all_items(2);
					fprintf(app_context.config->materialMarketData->areamarket2, "//-----after print_all_items \n");

				}

				//	 printf("after if(Item==NULL)\n");
				if (item != NULL) {
					//	 printf("balance=%I64d n=%d q_count=%d period=%d agent_id=%d \n", bookKeeper->get_bankbookslip()->balance,n,q_count,app_context.period_count,get_agent_id());
					if (bookKeeper->get_bankbookslip()->balance > item->get_price() || bookKeeper->get_cashbookslip()->balance > item->get_price()) {
						//購入するのは預金残高又は現金残高が商品価格より大きい場合に限る・
						// 但し、現金残高が商品価格未満なら商品価格分を預金から引き出す。
						if (bookKeeper->get_cashbookslip()->balance < item->get_price()) {
							bookKeeper->withdraw_bankdeposit(item->get_price());
						}
						//	 printf("before item->deal(%x)direct_material_fee=%I64d  \n",this,direct_material_fee);
						item->deal(this);//買い手の支払、売り手の金額受領、市場からの削除。会計処理は行わない。
						direct_material_fee += item->get_price();
						n++;
						if (agent_id == 509)fprintf(app_context.config->out509, "period=%d,n=,%d,q_count=%d,after n++,\n", app_context.period_count, n, q_count);
						//	 printf("after item->deal(this) n=%d \n", n);
					}
					else break;
				}
				else {
					fprintf(app_context.config->out_test7, "period=,%d,agent_id=,%d,number of bought material items=,%d,\n", app_context.period_count, get_agent_id(), n);
					break;
				}

			}
		
			set_direct_material_fee(direct_material_fee);
			if (agent_id == 509) fprintf(app_context.config->out509, "period=,%d,n=,%d,q_count=,%d,before renew_qcont in buy materils,\n", app_context.period_count, n, q_count);;
			if (n < q_count) renew_qcount(n);//購入できた原料が予定生産量より少なければ予定生産量を改定する
			if (agent_id == 509) fprintf(app_context.config->out509, "period=,%d,n=,%d,q_count=,%d,after renew_qcont in buy materils,\n", app_context.period_count, n, q_count);;
	//	}//if (app_context.config->wholesaler_count>0) {の終わり
		//else {}
	}
	
	/*int get_direct_labor_cost() {

		//hr->define_employee_wage_list(total_sales);
		//employee_wage_list = hr->get_employee_wage_list();
		int direct_labor_cost = 0;
		for (int i = 0; i < employee_wage_list.size(); i++) {
			direct_labor_cost += employee_wage_list[i];
		}
		return direct_labor_cost;
	}*/
	long long get_direct_material_fee() {
		return  direct_material_fee;
	}
	/*int get_wage(int agent_id) {
		int wage = 0;
		int index = 0;
		vector<int>employee_list;
		employee_list = hr->get_employee_list();
		for (int i; i < employee_list.size(); i++) {
			if (employee_list[i] == agent_id) {
				index = i;
				wage = employee_wage_list[i];
			}
		}
	}
	*/

public:
	/* Retailer(int agent_id)
		 : Producer(agent_id, PRODUCER_TYPE_RETAILER) {}*/
	SME(int agent_id, int agent_type) :Producer(agent_id, agent_type) {
		//fprintf(app_context.config->out,"retailer id=%d が生成されました。agent_type=%d\n",get_agent_id(), get_agent_type());
		//fprintf(app_context.config->out_temporary, "retailer id=%d が生成されました。agent_type=%d\n", get_agent_id(), get_agent_type());
	}
	~SME() {}
	/*int get_item_type_id_buy() {
		//producer.hで定義されている
		return item_type_id_buy;//で of material to buy
	}
	`*/

	void set_up() {
		//printf("start of retailer->set_up() agent_id=%d\n",get_agent_id());
		//define_agent_type(2);
		set_shipment_navigator(app_context.shipment_navi_for_retailer);//producerで定義
		/**最大生産能力*/
		//qmax=RandomGenerator::random_int(app_context.config->retailer_qmax_min,app_context.config->retailer_qmax_max );
		qmax = RandomGenerator::random_int(app_context.config->retailerParameter->qmax_min, app_context.config->retailerParameter->qmax_max);

		//qmax = qmax * pow(5, 0.75);
		min_qcount_ratio = 0.2;//変数名はProducerで定義
		//min_qcount_ratio=0.0;
		qmin = int(double(qmax) * min_qcount_ratio);
		//qmin=0;
		//producer_demand_delta=1.0;
		producer_demand_delta = 1.65;//変数名はproducerで定義
		//producer_demand_delta=1.28;
		/**初期生産量の決定*/
		q_count = qmax;

		q_count = int(double(qmax) * (app_context.config->producerParameter->initial_quantity_ratio));
		//q_count=qmax;
		//producer_demand_delta=RandomGenerator::random_double(app_context.config->producer_demand_delta_min,app_context.config->producer_demand_delta_max);		
		printf("agent_id=%d delta=%f\n", agent_id, producer_demand_delta);
		/**生産する製品種（一種類、生産者固有）の決定*/
		//int ijk = RandomGenerator::random_int(1, app_context.config->item_type_id_count);
		if (app_context.config->item_type_idkr.size() == 0) printf("app_context.config->item_type_idkr.size() = 0です,parameterに生産品種数>0を記入して下さい。\n");
		int ijk = RandomGenerator::random_int(1, app_context.config->item_type_idkr.size());
		item_type_id = app_context.config->item_type_idkr[ijk - 1];
		get_bookKeeper()->set_item_type_id(item_type_id);
		printf("item_type_id=%d\n", item_type_id);
		//原料品種の決定
		item_type_id_buy = define_and_get_item_type_id_buy();
		fprintf(app_context.config->retailMarketData->item_type_id_for_production, "agent_id=,%d ,item_type_id=,%d,qmax=,%d,\n", get_agent_id(), item_type_id, qmax);
		//item_type_id= RandomGenerator::random_int(1, app_context.config->item_type_id_count)*10;
				//alpha=RandomGenerator::random_double(0.01,0.05);
		producer_price_alpha = app_context.config->producer_price_alpha;
		define_price_delta();
		fixed_cost = RandomGenerator::random_int(app_context.config->retaielr_fixedcost_min, app_context.config->retaielr_fixedcost_max);
		variable_cost = RandomGenerator::random_int(app_context.config->retailer_variablecost_min, app_context.config->retailer_variablecost_max);
		/**初期価格の設定*/
		int min_price = app_context.config->retailerParameter->price_min;
		int max_price = app_context.config->retailerParameter->price_max;
		//int min_price=800*2;//20230807 40に変更//20230816復活20240116 800*2
		//int max_price=1200*2;// 20230807　80に変更//20230816復活20240116 1000*2
		//int min_price = 40;//20230815　800に変更
		// int max_price = 80;//20230815　1200に変更
		//int min_price = 800;
		//int max_price = 1200;
		//int max_price=300;
		//int max_price =400;
		//int min_price =200;
		//int max_price =100;
		//int min_price =60;
		//min_price=100;
		//price=RandomGenerator::random_int(int(double(min_price)/15),20)*20 ;
		//min_price=50;
		//price=RandomGenerator::random_int(int(double(min_price)/15),10)*20 ;
		price = RandomGenerator::random_int(min_price, max_price);

		/**初期所持金の決定*/
		//initial_cash=long long (RandomGenerator::random_int(app_context.config->retailer_init_cash_min,app_context.config->retailer_init_cash_max));
		initial_cash = long long(RandomGenerator::random_int(app_context.config->retailerParameter->init_cash_min, app_context.config->retailerParameter->init_cash_max));
		cash = initial_cash;
		initial_deposit = long long(RandomGenerator::random_int(app_context.config->retailerParameter->init_cash_min * 1000, app_context.config->retailerParameter->init_cash_max * 1000));
		set_cash(cash);
		bookKeeper->set_initia_cash(initial_cash);
		bookKeeper->set_initial_deposit(initial_deposit);
		bookKeeper->createcashBookSlip(app_context.period_count, initial_cash, 0);
		bookKeeper->createbankBookSlip(app_context.period_count, initial_deposit, 0);
		bookKeeper->createjournalSlip(app_context.period_count, 0, "現金", initial_cash, "初期資産投入", initial_cash, "資本金", 2);
		bookKeeper->createjournalSlip(app_context.period_count, 0, "預金", initial_deposit, "初期資産投入", initial_deposit, "資本金", 2);
		fprintf(app_context.config->out, "retailer %d の初期所持金=%I64dです item_type_id=%d qmax=%d\n", get_agent_id(), cash, item_type_id, qmax);
		fprintf(app_context.config->out, "bookKeeper経由のretailer　%dのcash=%I64d, spending=%I64d balance=%I64d\n", bookKeeper->get_agent_id(), bookKeeper->get_cashbookslip()->income, bookKeeper->get_cashbookslip()->spending, bookKeeper->get_cashbookslip()->balance);
		fprintf(app_context.config->out, "bookKeeper経由　period=%d income=%I64d\n", bookKeeper->get_cashbookslip()->period, bookKeeper->get_cashbookslip()->income);

		/**従業員の初期雇用 HRが実施するので削除した。*/
/*		int nemployee=int (double (app_context.config->consumer_count))/app_context.config->retailer_count;
		for(int i=0;i<nemployee;i++){
			hire_employee((get_agent_id()-app_context.config->consumer_count-5)*nemployee+i+1);
			fprintf(app_context.config->out_agent_address,"iside retailer setup, retailer_id=,%d,employee_id=,%d,employee_id_address=,%x,\n",this->get_agent_id(), employee_list[i]->get_agent_id(),employee_list[i]);
			fprintf(app_context.config->out_Eb,"inside retailer setup,retailer_id=,%d,employee_id=,%d, fixed_wage=,%d,deposit=,%d,income=,%d,Eb=,%d,\n",this->get_agent_id(),employee_list[i]->get_agent_id(),employee_list[i]->get_fixed_wage(),employee_list[i]->get_depoist(),employee_list[i]->get_income(),employee_list[i]->get_Eb());
		}
*/		//hire_employee(get_agent_id()-100);
//printf("end of retailer set_up agent_id=%d\n",get_agent_id());
/*雇用従業員のagent_type設定
int size_employee_list = employee_list.size();
for (int i = 0; i < size_employee_list; i++) {
	app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(employee_list[i])->set_agent_type(311);
}*/
	}
	/*void set_employee_type() { //SimulatorAppのrenew_consumer_type()にて実行しているので削除
		//雇用従業員のagent_type設定
		int size_employee_list = employee_list.size();
		for (int i = 0; i < size_employee_list; i++) {
			app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(employee_list[i])->set_agent_type(311);
		}
	}*/
	void define_price_delta() {
		price_delta = app_context.config->retailerParameter->price_delta;
	}

	//他の生産者タイプをモデルに入れる場合にはここで定義すること
	//将来は各producerタイプのset_up()で定義する方式に変更すること
	void pay(Item* item) {
		// printf("inside pay(item) -- retailer\n");
		 // TODO
		//long long balance;
		//balance = bookKeeper->get_cashbookslip()->balance;
		//int previous_balance = bookKeeper->get_cashkbookSlip_balance(app_context.period_count);
		if (bookKeeper->get_cashbookslip()->balance - item->get_price() < 0) {//cash不足なら預金を引き出す
			long long balance = bookKeeper->get_cashbookslip()->balance;
			//while (bookKeeper->get_cashbookslip()->balance-item->get_price() < 0) {
			//現金残高不足なら預金引き出し
			bookKeeper->withdraw_bankdeposit(item->get_price() - balance);
			/*
			bookKeeper->createjournalSlip(app_context.period_count, 0, "現金",item->get_price()-balance, "預金引き出し", item->get_price()-balance, "預金", 0);
			bookKeeper->createcashBookSlip(app_context.period_count, item->get_price()-balance, 0);
			bookKeeper->createbankBookSlip(app_context.period_count, 0, item->get_price()-balance);
			*/
			//}
		}
		bookKeeper->createcashBookSlip(app_context.period_count, 0, item->get_price());
		cash -= item->get_price();// 20220626 コメント化
		Eb -= item->get_price();//20220626 コメント化
		expenses += item->get_price();
	//	fprintf(app_context.config->out, " retailer %dがproducer %d に商品種 %d 代金として価格%I64d を支払ます.所持金残は%I64d です\n", get_agent_id(), item->Get_seller()->Get_agent_id(), item->get_item_type_id(), item->get_price(), cash);
		//item->Get_seller()->receive_payments(item);//item->deal()の中の this->seller->receive_payments(this);に移動した。
	//	fprintf(app_context.config->out_producer_consumer_income_expenses, "bookKeeper of consumer, period=, %d, agent_id=, %d, income=, %I64d, expenses=, %I64d, balance=, %I64d, agent_id=, %d,\n", app_context.period_count, get_agent_id(), bookKeeper->get_cashbookslip()->income, bookKeeper->get_cashbookslip()->spending, bookKeeper->get_cashbookslip()->balance, item->Get_seller()->Get_agent_id());	//仕訳伝票作成
		bookKeeper->createjournalSlip(app_context.period_count, 0, "原料", item->get_price(), "原料購入", item->get_price(), "現金", 0);
		bookKeeper->createjournalSlip(app_context.period_count, 1, "直接材料費", item->get_price(), "原料振替", item->get_price(), "原料", 0);
		//bookKeeper->createjournalSlip(app_context.period_count, 1, "原材料・労務費", item->get_price(), "原料振替", item->get_price(), "原料", 0);

			//仕訳伝票作成
		//printf("seller_id = % dinside pay() berfore bookKeeper\n",item->Get_seller()->Get_agent_id());
		bookKeeper->creatematerialBookSlip(app_context.period_count, 0, 1, item->get_item_type_id(), item->get_price());
		bookKeeper->creatematerialpriceBookSlip(app_context.period_count, 0, item->get_price(), item->get_item_type_id());
		//printf("after bookKeeper\n");
	}
	void receive_payments(Item* item) {
	//	fprintf(app_context.config->out, "inside receive_payments in Retailer.h\n");
		// printf("receive_payments -- producer\n");
		//int genka_per_item;//製品1個当たりの原価
		//if (size_shiped == 0) printf("size_shiped=0です,l出荷製品1個当たり原価が計算できません\n");
		//genka_per_item = genka / size_shiped;
		 // TODO
		total_sales += item->get_price();
		cash += item->get_price();
		//int  balance = bookKeeper->get_cashbookslip()->balance;
		//int previous_balance = bookKeeper->get_cashkbookSlip_balance(app_context.period_count);
		bookKeeper->createcashBookSlip(app_context.period_count, item->get_price(), 0);
		//bookKeeper->createstockBookSlip(app_context.period_count, 0, 1, 0,1*item->get_price(),item->get_item_type_id());
		bookKeeper->createstockBookSlip(app_context.period_count, 0, 1, item->get_item_type_id(), item->get_price());
		//bookKeeper->createstockpriceBookSlip(app_context.period_count, 0, item->get_genka_per_item(), item->get_item_type_id());
		bookKeeper->createstockpriceBookSlip(app_context.period_count, 0, item->get_price(), item->get_item_type_id());
	//	fprintf(app_context.config->out, "receive_payments -- producer %d for item_type=%d price=%I64d resultant cash=%I64d\n", get_agent_id(), item->get_item_type_id(), item->get_price(), cash);
	//	fprintf(app_context.config->out, "bookKeeper経由 income=%I64d spending=%I64d balance=%I64d\n", bookKeeper->get_cashbookslip()->income, bookKeeper->get_cashbookslip()->spending, bookKeeper->get_cashbookslip()->balance);
	//	fprintf(app_context.config->out_producer_consumer_income_expenses, "bookKeeper of producer 経由, period=, %d, agent_id=, %d, income=, %I64d, spending=, %I64d, balance=, %I64d, agent_id=, %d,\n", app_context.period_count, get_agent_id(), bookKeeper->get_cashbookslip()->income, bookKeeper->get_cashbookslip()->spending, bookKeeper->get_cashbookslip()->balance, item->get_buyer()->Get_agent_id());
		bookKeeper->createjournalSlip(app_context.period_count, 0, "現金", item->get_price(), "商品売上", item->get_price(), "売上", 3);
		bookKeeper->createjournalSlip(app_context.period_count, 1, "売上原価", item->get_genka_per_item(), "商品売上", item->get_genka_per_item(), "製品在庫", 0);
	}




};

