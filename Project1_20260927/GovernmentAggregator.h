#pragma once

#include "Agent.h"
#include "AgentAggregator.h"
#include "Government.h"
#include "SimulatorConfig.h"

extern SimulatorContext app_context;

/** リテイラー集合 */
class GovernmentAggregator : public AgentAggregator {
protected:
    int get_agent_count() {
        return app_context.config->government_count;
    }

    Agent* create_new_agent(int index) {
        return new Government(app_context.agent_id_seq++, 1);
    }
public:
    GovernmentAggregator(int agent_type) : AgentAggregator(agent_type) {}
    ~GovernmentAggregator() {}
};


