#pragma once

#include "Agent.h"
#include "AgentAggregator.h"
#include "Wholesaler.h"
#include "SimulatorConfig.h"

extern SimulatorContext app_context;

/** ホールセラー集合 */
class WholesalerAggregator : public AgentAggregator {
//protected:
    int get_agent_count() {
        return app_context.config->wholesaler_count;
    }

    Agent* create_new_agent(int index) {
        return new Wholesaler(app_context.agent_id_seq++,2132);
    }
public:
	//WholesalerAggregator(int agent_type){this->agent_type=agent_type;}
	WholesalerAggregator(int agent_type):AgentAggregator(agent_type){}
    ~WholesalerAggregator(){}
};

