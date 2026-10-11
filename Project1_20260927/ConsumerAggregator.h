#pragma once

#include "Agent.h"
#include "AgentAggregator.h"
#include "Consumer.h"
#include "SimulatorConfig.h"

extern SimulatorContext app_context;

/** 消費者集合 */
class ConsumerAggregator : public AgentAggregator {
	BuyDealer* buyDealer;
protected:
    int get_agent_count() {
        return app_context.config->consumer_count;
    }

    Agent* create_new_agent(int index) {
        return new Consumer(app_context.agent_id_seq++,31);//agent_type=31が設定される。もしretailer worker or wholesaler workerとなった時は311 or 312に変更される
    }
public:
    //ConsumerAggregator(int agent_id,int agent_type) : AgentAggregator(agent_id,agent_type){}
	ConsumerAggregator(int agent_type) : AgentAggregator(agent_type){}
    ~ConsumerAggregator(){}
	
};

