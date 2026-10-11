#pragma once

#include "Agent.h"
#include "AgentAggregator.h"
#include "SoleProprietor.h"
#include "SimulatorConfig.h"
#include "SimulatorContext.h"

extern SimulatorContext app_context;

/** リテイラー集合 */
class SoleProprietorAggregator : public AgentAggregator {
	protected:
    int get_agent_count() {
        return app_context.config->soleproprietor_count;
    }

    Agent* create_new_agent(int index) {
        return new SoleProprietor(app_context.agent_id_seq++,2130);
    }
public:
    //SoleProprietorAggregator(int agent_id,int agent_type) : AgentAggregator(agent_id,agent_type){}
	SoleProprietorAggregator(int agent_type) : AgentAggregator(agent_type){}
    ~SoleProprietorAggregator(){}
};