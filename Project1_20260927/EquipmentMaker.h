#pragma once

#include "Producer.h"

extern SimulatorContext app_context;

/** リテイラー */
class EquipmentMaker : public Producer {
	void buy_materials() {}
	//void pay_wages() {}
	//void other_kishu_actions() {}
public:
   /* EquipmentMaker(int agent_id)
        : Producer(agent_id, PRODUCER_TYPE_EQUIPMENT_MAKER) {}*/
	EquipmentMaker(int agent_id,int agent_type):Producer(agent_id,agent_type){
		fprintf(app_context.config->out,"Equipment Maker id=%d が生成されました。agent_type=%d\n",get_agent_id(), get_agent_type());
	}
    ~EquipmentMaker(){}

    void set_up() {
        set_shipment_navigator(app_context.shipment_navi_for_retailer);
		/**最大生産能力*/
		qmax=RandomGenerator::random_int(app_context.config->retailerParameter->qmax_min,app_context.config->retailerParameter->qmax_max);
		qmax = int (qmax * pow(5, 0.75));
		/**初期生産量の決定*/
		//q_count=0.7*qmax;
		q_count=qmax;
		producer_demand_delta = 1.65;
		//producer_demand_delta=RandomGenerator::random_double(app_context.config->producer_demand_delta_min,app_context.config->producer_demand_delta_max);
		//delta=2;
		//printf("agent_id=%d delta=%f\n",agent_id,producer_demand_delta);
		/**生産する製品種の決定*/
		//printf("before define item_type_id of equipmentmaker agent_id=%d\n",get_agent_id());
		item_type_id= RandomGenerator::random_int(1, app_context.config->item_type_id_count) * 10;
				//alpha=RandomGenerator::random_double(0.01,0.05);
		//printf("after define item_type_id of equipmentmaker  agernt_id=%d\n", get_agent_id());
		producer_price_alpha=app_context.config->producer_price_alpha;
		define_price_delta();
		//printf("aftger define_price_delta\n");
		fixed_cost=RandomGenerator::random_int(app_context.config->retaielr_fixedcost_min,app_context.config->retaielr_fixedcost_max);
		variable_cost=RandomGenerator::random_int(app_context.config->retailer_variablecost_min,app_context.config->retailer_variablecost_max);
		/**初期価格の設定*/
		int min_price = 0;
		min_price=100;
		price=RandomGenerator::random_int(int(double(min_price)/15),20)*20 ;

		/**初期所持金の決定*/
		int cash = RandomGenerator::random_int(app_context.config->retailerParameter->init_cash_min, app_context.config->retailerParameter->init_cash_max);
		set_cash(cash);
		int initial_cash = cash;
		initial_deposit= RandomGenerator::random_int(app_context.config->retailerParameter->init_cash_min, app_context.config->retailerParameter->init_cash_max);
		fprintf(app_context.config->out,"equipmentmaker %d の初期所持金=%dです item_type_id=%d qmax=%d\n",get_agent_id(),cash,item_type_id,qmax);
		bookKeeper->createcashBookSlip(app_context.period_count, initial_cash, 0);
		bookKeeper->createbankBookSlip(app_context.period_count, initial_deposit, 0);
		//printf("end of set_up of agent_id=%d\n", get_agent_id());
	}
	void define_price_delta(){
	}
};

