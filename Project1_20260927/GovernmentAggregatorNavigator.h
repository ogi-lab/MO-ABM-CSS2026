#pragma once
// GovernmentAggregatorNavigator

#include "AgentAggregatorNavigator.h"

// 前方宣言（完全定義はここでは不要）
class AgentAggregator;

class GovernmentAggregatorNavigator : public AgentAggregatorNavigator {
    AgentAggregator& governmentAggregator;
public:
    GovernmentAggregatorNavigator(AgentAggregator& government_aggregator) : governmentAggregator(government_aggregator) {}
    virtual ~GovernmentAggregatorNavigator() {}
    AgentAggregator* get_agent_aggregator() {
        return &governmentAggregator;
    }
};


