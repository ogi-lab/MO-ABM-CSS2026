#pragma once
#include "AgentAggregatorNavigator.h"
class AgentAggregator;
//#include "ConsumerAggregator.h"
class ConsumerAggregatorNavigator :public AgentAggregatorNavigator{
	AgentAggregator& consumerAggregator;
public:
	ConsumerAggregatorNavigator(AgentAggregator& consumer_aggregator):consumerAggregator(consumer_aggregator){
	//printf("address of consumerAggregator in ConsumerAggregatorNavigator=%x\n",&consumerAggregator);
	}
	AgentAggregator* get_agent_aggregator(){
		return &consumerAggregator;
	}
};



