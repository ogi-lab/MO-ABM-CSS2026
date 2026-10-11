#pragma once
#include "AgentAggregator.h"

class AgentAggregatorNavigator {
public:
	virtual ~AgentAggregatorNavigator() {}
	virtual AgentAggregator* get_agent_aggregator() = 0;
	//virtual AgentAggregator* get_agent_aggregator() {}
};