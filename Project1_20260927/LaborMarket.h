#pragma once
#include "Globals2.h"
//#include "Agent.h"
//#include "SimulatorContext.h"
//#include  "ConsumerAggregator.h"
extern SimulatorContext app_context;
using namespace std;
class LaborMarket{
	unsigned int n_employed = 0;
	unsigned int n_unemployed = 0;
	unsigned int agentlist_size = 0;
	//vector <Agent*> employed_list;
	//vector <Agent*> unemployed_list;
public:
	LaborMarket(){}
	
	/*int get_agentlist_size(){
		return app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent_list().size();
	}*/

	void setup(){
		n_employed=0;
		n_unemployed=app_context.config->consumer_count;
		for(unsigned int i=0;i<n_unemployed;i++){
			//初期条件として、全ての消費者をunemployed_listに格納する
			UNEMPLOYED_LIST.push_back(app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent_list()[i]->get_agent_id());
		//	fprintf(app_context.config->out3,"unemployed_id=%d address=%x\n",app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent_list()[i]->get_agent_id(),unsigned (app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent_list()[i]));
		}
		/*for(unsigned int i=0;i<get_agentlist_size();i++){
			//初期条件として、全ての消費者をunemployed_listに格納する
			unemployed_list.push_back(app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent_list()[i]);
			fprintf(app_context.config->out3,"unemployed_id=%d\n",app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent_list()[i]->get_agent_id());
		}*/
	}
	/*vector <Agent*> get_unemployed_list(){
		return unemployed_list;
	}*/
	/*void set_unemployed_list(vector <Agent*> list){
		unemployed_list=list;	
	}*/
	
};