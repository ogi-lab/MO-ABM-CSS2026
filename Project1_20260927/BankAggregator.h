#pragma once

#include "Agent.h"
#include "AgentAggregator.h"
#include "Bank.h"
#include "SimulatorConfig.h"

extern SimulatorContext app_context;

/** リテイラー集合 */
class BankAggregator : public AgentAggregator {
protected:
    int get_agent_count() {
        return app_context.config->bank_count;
    }

    Agent* create_new_agent(int index) {
        return new Bank(app_context.agent_id_seq++,2141);
    }
public:
    BankAggregator(int agent_type) : AgentAggregator(agent_type){}
    ~BankAggregator(){}
};

