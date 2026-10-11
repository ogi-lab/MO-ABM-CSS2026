#pragma once
#include "Agent.h"
#include <stdio.h>
#include "SimulatorConfig.h"
#include "SimulatorContext.h"
#include "DataAnalyzer.h"
#include "DataAnalyzerNavigator.h"

extern SimulatorContext app_context;

/** 倒産企業管理者 */
class EnterpriseManager : public Agent {
public:
    EnterpriseManager(int agent_id,int agent_type) : Agent(agent_id,agent_type){
		fprintf(app_context.config->out,"EnterpriseManager が生成されました。 agent_id=%d, agent_type=%d,\n",get_agent_id()	,get_agent_type());
	}
    ~EnterpriseManager(){}

    void set_up() {
     
		/**初期所持金の決定*/
		//set_cash(RandomGenerator::random_int(app_context.config->retailer_init_cash_min,app_context.config->retailer_init_cash_max));
		//fprintf(app_context.config->out,"retailer %d の初期所持金=%dです item_type_id=%d qmax=%d\n",get_agent_id(),cash,item_type_id,qmax);
	
	}
	void do_kishu(){}
	void action(){}
	void do_kimatsu(){}
	void transferstocks_to_StockRecord(){};
};

