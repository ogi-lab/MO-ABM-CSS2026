#pragma once

#include "Enterprise.h"

extern SimulatorContext app_context;

/** リテイラー */
class Bank : public Enterprise {
public:
    Bank(int agent_id,int agent_type)
        : Enterprise(agent_id,agent_type) {
			fprintf(app_context.config->out,"bank id=%d が生成されました agent_type=%d\n",get_agent_id(), get_agent_type());
	}
    ~Bank(){}

    void set_up() {
        //define_agent_type(3);
		/**初期所持金の決定*/
		set_cash(RandomGenerator::random_int(app_context.config->retailerParameter->init_cash_min,app_context.config->retailerParameter->init_cash_max));
	//	fprintf(app_context.config->out,"bank %d の初期所持金=%dです item_type_id=%d qmax=%d\n",get_agent_id(),cash,item_type_id,qmax);
	    fprintf(app_context.config->out,"bank %d の初期所持金=%I64dです \n",get_agent_id(),cash);
	}
	/**　給与支払い*/
	void pay_wages(){}
   /** 納税 */
    //void pay_tax() {}
    /** 購買 */
    void buy_retails() {}
    /** 決算 */
    void kessan() {}
	/**給与支払い以外の期中行動*/
	void other_actions(){}
	//void transferstocks_to_StockRecord(){};
	void other_kimatsu_actions(){}
	void other_kishu_actions() {}


};

