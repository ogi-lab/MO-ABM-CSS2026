#pragma once

#include "Agent.h"
#include "AgentAggregator.h"
#include "EquipmentMaker.h"
#include "SimulatorConfig.h"

extern SimulatorContext app_context;

/** ホールセラー集合 */
class EquipmentMakerAggregator : public AgentAggregator {
protected:
    int get_agent_count() {
        return app_context.config->equipmentmaker_count;
    }

    Agent* create_new_agent(int index) {
        return new EquipmentMaker(app_context.agent_id_seq++,2133);
    }
public:
    //EquipmentMakerAggregator(int agent_id,int agent_type) : AgentAggregator(agent_id,agent_type){}
	EquipmentMakerAggregator(int agent_type) : AgentAggregator(agent_type){}
    ~EquipmentMakerAggregator(){}
};

