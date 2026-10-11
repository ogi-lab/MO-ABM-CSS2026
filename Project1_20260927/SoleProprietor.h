#pragma once
#include "Producer.h"
extern SimulatorContext app_context;
class SoleProprietor:public Producer{
	//Producerは、Enterprise, Seller, Buye を継承している
	void buy_materials() {}
	//void pay_wages() {}
	//void other_kishu_actions() {}
public:
	SoleProprietor(int agent_id,int agent_type):Producer(agent_id,agent_type){
		fprintf(app_context.config->out,"soleproprietor id=%d が生成されました。agent_type=%d\n",get_agent_id(), get_agent_type());
	}
    ~SoleProprietor(){}
	void set_up() {
		//fprintf(app_context.config->out_test0, "start of set_up() of SoleProprietor agent_id=%d\n", get_agent_id());
		//define_agent_type(2);
		//printf("start of set_up() of SoleProprietor\n");
		set_shipment_navigator(app_context.shipment_navi_for_soleproprietor);//producerで定義
			/**最大生産能力*/
		//qmax = RandomGenerator::random_int(app_context.config->soleproprietor_qmax_min, app_context.config->soleproprietor_qmax_max);
		qmax = RandomGenerator::random_int(app_context.config->soleproprietorParameter->qmax_min, app_context.config->soleproprietorParameter->qmax_max);
		qmin = RandomGenerator::random_int(app_context.config->soleproprietorParameter->qmin_min, app_context.config->soleproprietorParameter->qmin_max);

		set_qmax(qmax);
		set_qmin(qmin);
		//qmax = int (qmax * pow(5, 0.75));
		min_qcount_ratio = 0.2;
		//min_qcount_ratio=0.0;
		//qmin = int(double(qmax) * min_qcount_ratio);
		//set_qmin(qmin);
		//qmin=0;
		//producer_demand_delta=1.0;
		producer_demand_delta = 1.65;
		//producer_demand_delta=1.28;
		/**初期生産量の決定*/
		q_count =int(double(qmax)* (app_context.config->producerParameter->initial_quantity_ratio));
		//q_count=qmax;
		//producer_demand_delta=RandomGenerator::random_double(app_context.config->producer_demand_delta_min,app_context.config->producer_demand_delta_max);		
		//printf("agent_id=%d delta=%f\n", agent_id, producer_demand_delta);
	
		/**生産する製品種の決定*/
		if (app_context.config->item_type_idks.size() == 0) printf("app_context.config->item_type_idks.size() = 0です,生産品種数>0を記入して下さい。\n");
	    /*
		//int ijk = RandomGenerator::random_int(1, app_context.config->item_type_id_count);
		int ijk = RandomGenerator::random_int(1, app_context.config->item_type_idks.size());
		//		item_type_id = app_context.config->item_type_idk[app_context.config->item_type_id_count_r+ijk - 1];
		item_type_id = app_context.config->item_type_idks[ijk - 1];
		//fprintf(app_context.config->out3, "inside set_up() agent_id=%d agent_type=%d item_type_id \n", get_agent_id(),get_agent_type(), item_type_id);
		*/
		item_type_id = define_and_get_item_type_id_to_produce();
		get_bookKeeper()->set_item_type_id(item_type_id);
		//item_type_id= RandomGenerator::random_int(1, app_context.config->item_type_id_count) * 10;
				//alpha=RandomGenerator::random_double(0.01,0.05);
		//printf("end of defining item_type_id for solproprietor\n");
		producer_price_alpha=app_context.config->producer_price_alpha;
		define_price_delta();
		fixed_cost=RandomGenerator::random_int(app_context.config->retaielr_fixedcost_min,app_context.config->retaielr_fixedcost_max);
		variable_cost=RandomGenerator::random_int(app_context.config->retailer_variablecost_min,app_context.config->retailer_variablecost_max);
		/**初期価格の設定*/
		//int min_price=400;
		//int max_price=800;
		//int max_price=300;
		//int max_price =400;
		//int min_price =200;
		//int max_price =100;
		//int min_price =60;
		//min_price=100;
		int min_price = app_context.config->soleproprietorParameter->price_min;
		int max_price = app_context.config->soleproprietorParameter->price_max;
		int min_price_min = app_context.config->soleproprietorParameter->min_price_min;
		int min_price_max = app_context.config->soleproprietorParameter->min_price_max;


		//price=RandomGenerator::random_int(int(double(min_price)/15),20)*20 ;
		//min_price=50;
		//price=RandomGenerator::random_int(int(double(min_price)/15),10)*20 ;
		price=long long (RandomGenerator::random_int(app_context.config->soleproprietorParameter->price_min,app_context.config->soleproprietorParameter->price_max));
		min_price = long long(RandomGenerator::random_int(min_price_min, min_price_max));
		/**初期所持金の決定*/
		cash=long long (RandomGenerator::random_int(app_context.config->soleproprietorParameter->init_cash_min,app_context.config->soleproprietorParameter->init_cash_max));
		initial_cash=cash;
		initial_deposit= long long (RandomGenerator::random_int(app_context.config->soleproprietorParameter->init_cash_min, app_context.config->soleproprietorParameter->init_cash_max));
		set_cash(cash);
		bookKeeper->set_initia_cash(initial_cash);
		bookKeeper->createcashBookSlip(app_context.period_count,initial_cash,0);
		bookKeeper->createbankBookSlip(app_context.period_count, initial_deposit,0);
		
		fprintf(app_context.config->out,"solopropriety %d の初期所持金=%I64dです item_type_id=%d qmax=%d\n",get_agent_id(),cash,item_type_id,qmax);
		fprintf(app_context.config->out,"bookKeeper経由のsolopropriety　%dのcash=%I64d, spending=%I64d balance=%I64d\n",bookKeeper->get_agent_id(),bookKeeper->get_cashbookslip()->income,bookKeeper->get_cashbookslip()->spending,bookKeeper->get_cashbookslip()->balance);
		fprintf(app_context.config->out,"bookKeeper経由　period=%d income=%I64d\n",bookKeeper->get_cashbookslip()->period,bookKeeper->get_cashbookslip()->income);

		/**従業員の初期雇用*/
		/*
		int nemployee=int (double (app_context.config->consumer_count))/app_context.config->retailer_count;
		for(int i=0;i<nemployee;i++){
			hire_employee((get_agent_id()-app_context.config->consumer_count-5)*nemployee+i+1);	
			fprintf(app_context.config->out_agent_address,"iside retailer setup, retailer_id=,%d,employee_id=,%d,employee_id_address=,%x,\n",this->get_agent_id(), employee_list[i]->get_agent_id(),employee_list[i]);
			fprintf(app_context.config->out_Eb,"inside retailer setup,retailer_id=,%d,employee_id=,%d, fixed_wage=,%d,deposit=,%d,income=,%d,Eb=,%d,\n",this->get_agent_id(),employee_list[i]->get_agent_id(),employee_list[i]->get_fixed_wage(),employee_list[i]->get_depoist(),employee_list[i]->get_income(),employee_list[i]->get_Eb());
		}*/
	}
	void define_price_delta() {
		price_delta = app_context.config->soleproprietorParameter->price_delta;
	}
	void receive_payments(Item* item) {
	//	if (item->get_buyer()->Get_AgentType() == 1)fprintf(app_context.config->out_trace_tax_income, "inside receive_payments in Soleproprietor.h, buyer_id=,%d,buyer_type=,%d\n", item->get_buyer()->Get_agent_id(), item->get_buyer()->Get_AgentType());
		total_sales += item->get_price();
		cash += item->get_price();
		bookKeeper->createcashBookSlip(app_context.period_count, item->get_price(), 0);
		bookKeeper->createstockBookSlip(app_context.period_count, 0, 1, item->get_item_type_id(), item->get_price());
		//bookKeeper->createstockpriceBookSlip(app_context.period_count, 0, item->get_genka_per_item(), item->get_item_type_id());
		bookKeeper->createstockpriceBookSlip(app_context.period_count, 0, item->get_price(), item->get_item_type_id());
	//	fprintf(app_context.config->out, "receive_payments -- producer %d for item_type=%d price=%I64d resultant cash=%I64d\n", get_agent_id(), item->get_item_type_id(), item->get_price(), cash);
	//	fprintf(app_context.config->out, "bookKeeper経由 income=%I64d spending=%I64d balance=%I64d\n", bookKeeper->get_cashbookslip()->income, bookKeeper->get_cashbookslip()->spending, bookKeeper->get_cashbookslip()->balance);
/*
		fprintf(app_context.config->out_producer_consumer_income_expenses, "bookKeeper of producer 経由, period=, %d, agent_id=, %d, income=, %I64d, spending=, %I64d, balance=, %I64d, agent_id=, %d,at receive_payment(),\n",
			app_context.period_count, get_agent_id(), bookKeeper->get_cashbookslip()->income, bookKeeper->get_cashbookslip()->spending, bookKeeper->get_cashbookslip()->balance, item->get_buyer()->Get_agent_id());
*/
		if (item->get_buyer()->Get_AgentType() == 1) {
		bookKeeper->createjournalSlip(app_context.period_count, 0, "現金", item->get_price(), "商品売上", item->get_price(), "政府売上", 3);
		//bookKeeper->createjournalSlip(app_context.period_count, 0, "現金", item->get_price(), "商品売上", item->get_price(), "売上", 3);
		bookKeeper->createjournalSlip(app_context.period_count, 1, "売上原価", item->get_genka_per_item(), "商品売上", item->get_genka_per_item(), "製品在庫", 0);
		//bookKeeper->createjournalSlip(app_context.period_count, 3, "売上", item->get_imported_imventory_per_item(), "原料輸入", item->get_imported_imventory_per_item(), "輸出入", 0);
	}
	else {
		bookKeeper->createjournalSlip(app_context.period_count, 0, "現金", item->get_price(), "商品売上", item->get_price(), "売上", 3);
	}bookKeeper->createjournalSlip(app_context.period_count, 1, "売上原価", item->get_genka_per_item(), "商品売上", item->get_genka_per_item(), "製品在庫", 0);
	//bookKeeper->createjournalSlip(app_context.period_count, 3, "売上", item->get_imported_imventory_per_item(), "原料輸入", item->get_imported_imventory_per_item(), "輸出入", 0);
	}
	




};