#pragma once

#include "Producer.h"
#include "SimulatorContext.h"

extern SimulatorContext app_context;

/** リテイラー */
class Wholesaler : public Producer {
	//Producerは、Enterprise, Seller, Buye を継承している
	void buy_materials() {
		//printf("inside buy_materials_wholesaler\n");
	}
	//void pay_wages() {}

	/*int get_direct_labor_cost() {
		
		//hr->define_employee_wage_list(total_sales);
		//employee_wage_list = hr->get_employee_wage_list();
		int direct_labor_cost=0;
		for (int i = 0; i < employee_wage_list.size(); i++) {
			direct_labor_cost += employee_wage_list[i];
		}
		return direct_labor_cost;
	}*/
	long long get_direct_material_fee() {
		return  0;
	}
	/*int get_wage(int agent_id) {
		int wage = 0;
		int index = 0;
		vector <int> employee_list;
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
   /* Wholesaler(int agent_id)
        : Producer(agent_id, PRODUCER_TYPE_WHOLESELLER_NORMAL) {}*/
	Wholesaler(int agent_id,int agent_type): Producer(agent_id,agent_type){
		fprintf(app_context.config->out,"Wholesaler id=%d が生成されました。agent_type=%d \n",get_agent_id(), get_agent_type());
	}
    ~Wholesaler(){}

    void set_up() {
		//printf("before shipment_navi_for wholesaler\n");
        set_shipment_navigator(app_context.shipment_navi_for_wholesaler);
        //printf("after shipment_navi_for wholesaler\n");
		/**最大生産能力*/
		//以下）とりあえずretailerのset_up()の内容と同じ（所持金まで）2012_12_24荻林
		//qmax=RandomGenerator::random_int(app_context.config->wholesaler_qmax_min,app_context.config->wholesaler_qmax_max );
		//qmax, qminをパラメータで設定
		qmax = RandomGenerator::random_int(app_context.config->wholesalerParameter->qmax_min, app_context.config->wholesalerParameter->qmax_max);
		qmin = RandomGenerator::random_int(app_context.config->wholesalerParameter->qmin_min, app_context.config->wholesalerParameter->qmin_max);
		
		/*qmax, qminをreteilrのそれと連動させる、パラメータの値は使わない*/
		/*
		qmax = int(double(app_context.config->retailerParameter->qmax_max) *double( app_context.config->retailer_count) /double( app_context.config->wholesaler_count));
		qmin = int(double(app_context.config->retailerParameter->qmax_min) * double(app_context.config->retailer_count) / double(app_context.config->wholesaler_count));
		*/
		fprintf(app_context.config->out_test, "agent_id=,%d, qmax=,%d,\n",  get_agent_id(), qmax);
		//qmax = qmax * pow(5, 0.75)*10;
		//qmax = qmax * 10;
		min_qcount_ratio = 0.2;
		//qmin = int(double(qmax) * min_qcount_ratio);
		producer_demand_delta = 1.65;
		/**初期生産量の決定*/
		//q_count=0.7*qmax;
		q_count=qmax;
		q_count = int(double(qmax) * (app_context.config->producerParameter->initial_quantity_ratio));
		//q_count = int(double(qmax) * (app_context.config->producerParameter->initial_quantity_ratio)*1.5/5.25);
		producer_demand_delta = 1.65;
		//producer_demand_delta=RandomGenerator::random_double(app_context.config->producer_demand_delta_min,app_context.config->producer_demand_delta_max);
		//delta=2;
		//printf("agent_id=%d delta=%f\n",agent_id,producer_demand_delta);
		/**生産する製品種（一種類、生産者毎に固有）の決定*/

		if (app_context.config->item_type_idkw.size() == 0) printf("app_context.config->item_type_idkw.size() = 0です,生産品種数>0を記入して下さい。\n");
		/*
		//int ijk = RandomGenerator::random_int(1, app_context.config->item_type_id_count);
		int ijk = RandomGenerator::random_int(1,app_context.config->item_type_idkw.size());
		//int ijk = int (RandomGenerator::random_double(1-0.5, app_context.config->item_type_idkw.size())+0.5);
		item_type_id = app_context.config->item_type_idkw[ijk - 1];
		*/
		item_type_id = define_and_get_item_type_id_to_produce();
		get_bookKeeper()->set_item_type_id(item_type_id);
		//売れた製品に応じて生産する品種を変える場合は以下を使用	

		//printf("inside whoelseller setup before fprintf\n");
		fprintf(app_context.config->materialMarketData->item_type_id_for_production, "agent_id=,%d ,item_type_id=,%d,qmax=,%d,\n", get_agent_id(), item_type_id, qmax);
		//printf("after wholeseller setup\n");
		//item_type_id= RandomGenerator::random_int(1, app_context.config->item_type_id_count) * 10;
		fprintf(app_context.config->out_test, "     agent_id=,%d, final qmax=,%d, item_type_id=,%d,\n", get_agent_id(), qmax,item_type_id);
				//alpha=RandomGenerator::random_double(0.01,0.05);
		producer_price_alpha=app_context.config->producer_price_alpha;
		define_price_delta();
		fixed_cost=RandomGenerator::random_int(app_context.config->retaielr_fixedcost_min,app_context.config->retaielr_fixedcost_max);
		variable_cost=RandomGenerator::random_int(app_context.config->retailer_variablecost_min,app_context.config->retailer_variablecost_max);
		//雇用従業員のagent_type設定
		int size_employee_list = employee_list.size();
		for (int i = 0; i < size_employee_list; i++) {
			app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(employee_list[i])->set_agent_type(311);
		}
		/**初期価格の設定*/
		//int min_price = app_context.config->wholesalerParameter->price_min;
		//int max_price = app_context.config->wholesalerParameter->price_max;
		int min_price_min = app_context.config->wholesalerParameter->min_price_min;
		int min_price_max = app_context.config->wholesalerParameter->min_price_max;

		//int min_price = int(800*0.5) ;//20230807　200に変更 20240116800*0.5
		//int max_price = int(1200*0.5);//20230807　300に変更202401161000*0.5
		//int min_price = 200;//20230815  800*0.5に変更;
		//int max_price = 300;//20230815　　1200*0.5に変更
		//int min_price = 400;
		//int max_price = 600;
		//int min_price;
		//min_price=100;
	//	price=RandomGenerator::random_int(int(double(min_price)/15),20)*20 ;
		price = RandomGenerator::random_int(app_context.config->wholesalerParameter->price_min, app_context.config->wholesalerParameter->price_max);

		min_price = long long(RandomGenerator::random_int(min_price_min, min_price_max));
		/**初期所持金の決定*/
		initial_cash =long long (RandomGenerator::random_int(app_context.config->wholesalerParameter->init_cash_min,app_context.config->wholesalerParameter->init_cash_max));
        cash=initial_cash;
		if (app_context.config->areamarketParameter->case_wihtout_expense_initial_deposit_and_cash!= 1) {
			initial_deposit = long long(RandomGenerator::random_int(app_context.config->wholesalerParameter->init_cash_min, app_context.config->wholesalerParameter->init_cash_max));
		}
				else {
			initial_deposit = 0;
		}	
		deposit = initial_deposit;
		set_cash(cash);
		set_deposit(deposit);
		bookKeeper->set_initia_cash(cash);
		bookKeeper->createcashBookSlip(app_context.period_count,initial_cash,0);
		bookKeeper->createbankBookSlip(app_context.period_count, initial_deposit, 0);
		bookKeeper->createjournalSlip(app_context.period_count, 0, "現金", cash, "初期資産投入", cash, "資本金", 2);
		bookKeeper->createjournalSlip(app_context.period_count, 0, "預金", deposit, "初期資産投入", cash, "資本金", 2);
		fprintf(app_context.config->out,"wholesaler %d の初期所持金=%I64dです item_type_id=%d qmax=%d\n",get_agent_id(),cash,item_type_id,qmax);
		fprintf(app_context.config->out,"bookKeeper経由のwholesaler　%dのcash=%I64d, spending=%I64d balance=%I64d\n",bookKeeper->get_agent_id(),bookKeeper->get_cashbookslip()->income,bookKeeper->get_cashbookslip()->spending,bookKeeper->get_cashbookslip()->balance);
		fprintf(app_context.config->out,"bookKeeper経由　period=%d income=%I64d\n",bookKeeper->get_cashbookslip()->period,bookKeeper->get_cashbookslip()->income);

	}
	/*void set_employee_type() {   //SimulatorAppのrenew_consumer_type()にて実行しているので削除
		//雇用従業員のagent_type設定
		int size_employee_list = employee_list.size();
		for (int i = 0; i < size_employee_list; i++) {
			app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(employee_list[i])->set_agent_type(312);
		}
	}*/
	void define_price_delta() {
		price_delta = app_context.config->wholesalerParameter->price_delta;
	}
	
	void receive_payments(Item* item) {
	//	fprintf(app_context.config->out, "inside receive_payments in Wholesaler.h\n");
		// printf("receive_payments -- producer\n");
		//int genka_per_item;//製品1個当たりの原価
		//if (get_size_shiped() == 0) printf("size_shiped=0です,size_shiped=%d 出荷製品1個当たり原価が計算できません\n",get_size_shiped());
		//genka_per_item = get_genka()/ get_size_shiped();
		 // TODO
		total_sales += long long (item->get_price());
		cash += long long (item->get_price());
		//int  balance = bookKeeper->get_cashbookslip()->balance;
		//int previous_balance = bookKeeper->get_cashkbookSlip_balance(app_context.period_count);
		bookKeeper->createcashBookSlip(app_context.period_count, item->get_price(), 0);
		bookKeeper->createstockBookSlip(app_context.period_count, 0, 1, item->get_item_type_id(), item->get_price());
		//bookKeeper->createstockpriceBookSlip(app_context.period_count, 0, item->get_genka_per_item(), item->get_item_type_id());
		bookKeeper->createstockpriceBookSlip(app_context.period_count, 0, item->get_price(), item->get_item_type_id());
	//	fprintf(app_context.config->out, "receive_payments -- producer %d for item_type=%d price=%I64d resultant cash=%I64d\n", get_agent_id(), item->get_item_type_id(), item->get_price(), cash);
	//	fprintf(app_context.config->out, "bookKeeper経由 income=%I64d spending=%I64d  balance=%I64d\n", bookKeeper->get_cashbookslip()->income, bookKeeper->get_cashbookslip()->spending, bookKeeper->get_cashbookslip()->balance);
	//	fprintf(app_context.config->out_producer_consumer_income_expenses, "bookKeeper of producer 経由, period=, %d, agent_id=, %d, income=, %I64d, spending=, %I64d, balance=, %I64d, agent_id=, %d,\n", app_context.period_count, get_agent_id(), bookKeeper->get_cashbookslip()->income, bookKeeper->get_cashbookslip()->spending, bookKeeper->get_cashbookslip()->balance, item->get_buyer()->Get_agent_id());
		bookKeeper->createjournalSlip(app_context.period_count, 0,"現金", item->get_price(), "商品売上", item->get_price(), "売上",3);
		bookKeeper->createjournalSlip(app_context.period_count, 1, "売上原価",item->get_genka_per_item() ,"商品売上", item->get_genka_per_item(), "製品在庫", 0);
		if(get_agent_id()==540) fprintf(app_context.config->out_test, "priod=,%d,agent_id=,%d,price=,%I64d,item_type=,%d,\n ", app_context.period_count, get_agent_id(), item->get_price(), item->get_item_type_id());
	}
	/*void createjournalSlip(int period, int Dr_code, string Dr_title, long long dabit, string description, long long credit, string Cr_title, int Cr_code) {
		bookKeeper->createjournalSlip(period, Cr_code, Dr_title, dabit, description, credit, Cr_title, Cr_code);
	}*/
};

