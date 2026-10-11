#pragma once

#include "Agent.h"
#include "AgentAggregator.h"
#include "Retailer.h"
#include "SimulatorConfig.h"
#include "SimulatorContext.h"

extern SimulatorContext app_context;

/** リテイラー集合 */
class RetailerAggregator : public AgentAggregator {
protected:
    int get_agent_count() {
        return app_context.config->retailer_count;
    }

    Agent* create_new_agent(int index) {
        return new Retailer(app_context.agent_id_seq++,2131);
    }
public:
    RetailerAggregator(int agent_type) : AgentAggregator(agent_type){}
    ~RetailerAggregator(){}
};

