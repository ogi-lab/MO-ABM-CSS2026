#pragma once
//#include "Agent.h" 
#include <vector>
#include "Globals2.h"//vector <int> UNEMPLOYED_LIST; //agent_idのlistをグローバル変数として宣言している
//#include "AgentAggregatorNavigator.h"
//#include "ConsumerAggregatorNavigator.h"
//#include "SimulatorContext.h"
//#include "ConsumerAggregator.h"
//#include "SimulatorConfig.h"
//#include "LaborMarket.h"
//using namespace std;
extern SimulatorContext app_context;
class HR
{
	//Human Resources Department
	vector <int> employee_list;//agent_idのlist
	vector <int> employee_address_list;
	vector<long long > employee_wage_list;
	vector <int> executive_list;
	vector <int> executive_address_list;
	vector<long long > executive_wage_list;
protected:
	int agent_id = 0;
	int agent_type = 0;

	//vector <Agent*> list;
	//vector <Agent*> unemployed_list;
	int n_employee = 0;
	int n_executive = 0;
public:
	HR(int agent_id, int agent_type) {
		this->agent_id = agent_id;	//HRを保有しているagentのid
		this->agent_type = agent_type;//HRを保有しているagentのagent_type
	}
	int get_agent_id() { return agent_id; }
	int get_agent_type() { return agent_type; }
	int get_n_employee() { return n_employee; }
	int get_n_executive() { return n_executive; }
	void set_n_employee(int n) { n_employee = n; }
	void set_n_executive(int ne) { n_executive = ne; }
	int get_employee(int ik) {
		if (employee_list.size() > unsigned(ik)) return employee_list[ik];
		else return 0;
	}
	int get_executive(int ik){
		if (executive_list.size() > unsigned(ik)) return executive_list[ik];
		else return 0;
	}
	void set_employee_wage_list(vector <long long > employee_wage_list) {
		this->employee_wage_list = employee_wage_list;
	}
	void set_executive_wage_list(vector<long long> executive_wage_list) {
		this->employee_wage_list = executive_wage_list;
	}
	/*void define_employee_wage_list(int total_sales) {
		double rw_max = 0.07;
		double rw_min = 0.05;
		double rate = 0.0;
		int wage = 0;
		int fixed_wage = 0;
		Agent* ag;
		for (int i = 0; i < employee_list.size(); i++) {
			int employee_id = 0;
			Agent* ag;
			//rate=RandomGenerator::random_double(rate_min,rate_max);
			rate = RandomGenerator::random_double(rw_min, rw_max);
			ag = app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(employee_list[i]);
			fprintf(app_context.config->out3, "address of ag= %x, address of employee_list[%d]=%x) %x,\n", ag, i, employee_list[i]);
			fixedgg_wage = ag->get_fixed_wage();
			//fixed_wage=app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(employee_list[i]->get_agent_id())->get_fixed_wage();
			//employee_id=app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(employee_list[i]->get_agent_id())->get_agent_id();
			employee_id = employee_list[i];
			//wage=int (total_sales*rate)+int(fixed_wage*0.5);
			//wage=int (total_sales*rate)+int (fixed_wage*0.5);
			
			if (total_sales < 0) wage = fixed_wage;
			else wage = fixed_wage + int(total_sales * rate);
			employee_wage_list.push_back(wage);
		}
	}*/
	/*Agent* get_employee(int ik){
		if(employee_list.size()>ik) return employee_list[ik];
		else return 0;
	}*/
	void set_up(){
		//consumerの場合のagent_typeはSimulatorAppのrenew_consumer_type()で決定する
	//	printf("before hire_employee agent_id=%d agent_type=%d \n", get_agent_id(),get_agent_type());
		//従業員の雇用
		hire_employee();//employeeの初期のagent_id集合を決定する	
	//	printf("before hire_executive agent_id=%d agent_type=%d \n", get_agent_id(),get_agent_type());
		//経営者の
		hire_executive();//executiveの初期のagent_id集合を決定する
		printf("end of hire_executive agent_id=%d agent_type=%d\n", get_agent_id(), get_agent_type());
	}
	void hire_employee(){
		//初期雇用者数の設定
		//agent_type;
			 /* = 1 GOVERNMENT
			= 2130  SOLEPROPRIETOR
			= 2131 RETAILER
			= 2132  WHOLESALER
			= 2133  EQUIPMENT MAKER
			= 2141  BANK
			= 31 CONSUMER
			= 310 Soleproprietor_worker
			= 3100 Soleproprietor_exective
			= 311 Retailer_worker
			= 3110 Retailer_exective
			= 312 Wholesaler_worker
			= 3120 Wholesaler_exective
			= 41 GOVERNMENT AGGREGATOR
			= 42　CONSUMER AGGREGATOR
			= 43  RETAILER AGGREGATOR
			= 44  WHOLESALER AGGREGATOR
			= 45  EQUIPMENT AGGREGATOR
			= 46 BANK AGGREGATOR
			= 47 ENTERPRISE MANAGER
			= 48 SOLEPROPRIETOR AGGREGATOR
			*/

	    //int n=int (double( app_context.config->consumer_count)/app_context.config->retailer_count);
		int n = 0;
		if (agent_type == 2131) n = app_context.config->retailer_worker_count;
		else if (agent_type == 2132) n = app_context.config->wholesaler_worker_count;
		else if (agent_type == 2130) n = app_context.config->soleproprietor_worker_count;
		set_n_employee(n);	
		int mk = 0, m = 0;
		//employeeのagent_idを決定する
		//	unemployed_list=app_context.labormarket->get_unemployed_list();
		for(int ik=0;ik<get_n_employee();ik++){
			mk=UNEMPLOYED_LIST.size();	//agent_idのリスト　採用前は全員が無就労者
			      //LaborMarketのset_up()で定義される
			m=RandomGenerator::random_int(0,mk-1);
//			if (agent_type == 2131) app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(UNEMPLOYED_LIST[m])->set_agent_type(311);
//			if (agent_type == 2132) app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(UNEMPLOYED_LIST[m])->set_agent_type(312);
			employee_list.push_back(UNEMPLOYED_LIST[m]);//m番目のagent_idをemployee_listに加える
			fprintf(app_context.config->out_agent_address,"inside agent_aggregator, retailer_id=,%d n_employee=,%d,employee_id=,%d,employee_address=,%x, \n", agent_id,get_n_employee(), get_employee(ik), get_employee(ik));	
		//	fprintf(app_context.config->out3,"inside agent_aggregator, employer_id=,%d,employer_type=,%d,n_employee=,%d,employee_id=,%d,employee_address=,%x,employee_list.size=,%d,UNEMPLOYED_LIST.size=,%d, \n",agent_id,agent_type,get_n_employee(),get_employee(ik), get_employee(ik),employee_list.size(),UNEMPLOYED_LIST.size());
			//printf("agent_id=%d agent_type=%d employee_list[%d]=%d\n", get_agent_id(),get_agent_type(),ik, employee_list[ik]);
			//	fprintf(app_context.config->out3,"inside agent_aggregator, retailer_id=,%d n_employee=,%d,employee_id=,%d,employee_address=,%x, \n"
		//		,agent_id,get_n_employee(),get_employee(ik), app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(get_employee(ik)));
//AgentAggregatorNavigator* consumerAggregatorNavigator;
			UNEMPLOYED_LIST.erase(UNEMPLOYED_LIST.begin()+m);//m番目のagent_idをリストから除く
		}
		//消費者数が450、retailerが80、wholesalerが15、b_retailer=5、n_wholesaler=3の場合、retailerが雇処理後で50人の非雇用者がUNEMPLOYED_LISTに含まれている
		//app_context.labormarket->set_unemployed_list(unemployed_list);
	}
	vector <int> get_employee_list(){
		return employee_list;
	}
	vector <long long > get_employee_wage_list() {
		return employee_wage_list;
	}
	vector<int> get_executive_list() {
		return executive_list;
	}
	vector<long long> get_executive_wage_list() {
		return executive_wage_list;
	}
	
	void hire_executive() {
		//executiveのagent_idを決定する
		int ne = 0;
		if (agent_type == 2131) ne = app_context.config->retailer_executive_count;
		else if (agent_type == 2132) ne = app_context.config->wholesaler_executive_count;
		else if (agent_type == 2130) ne = app_context.config->soleproprietor_executive_count;
		set_n_executive(ne);
		int mk = 0, m = 0;
		//executiveの初期設定
		for (int ik = 0; ik < get_n_executive(); ik++) {
			mk = UNEMPLOYED_LIST.size();	//agent_idのリスト　採用前は全員が無就労者
			m = RandomGenerator::random_int(0, mk - 1);
			executive_list.push_back(UNEMPLOYED_LIST[m]);//m番目のagent_idをexecutive_listに加える
	//		fprintf(app_context.config->out_agent_address, "inside agent_aggregator, retailer_id=,%d n_executive=,%d,executive_id=,%d,executive_address=,%x, \n", agent_id, get_n_executive(), get_executive(ik), get_employee(ik));
	//		fprintf(app_context.config->out3, "inside agent_aggregator, employer_id=,%d,employer_type=,%d,n_employee=,%d,employee_id=,%d,employee_address=,%x,employee_list.size=,%d,UNEMPLOYED_LIST.size=,%d, \n", agent_id, agent_type, get_n_employee(), get_employee(ik), get_employee(ik), employee_list.size(), UNEMPLOYED_LIST.size());
			UNEMPLOYED_LIST.erase(UNEMPLOYED_LIST.begin() + m);//m番目のagent_idをリストから除く
		}
		fprintf(app_context.config->out_exexutive_list, "agent_id=,%d,",get_agent_id());
		for(int ik = 0; ik < executive_list.size(); ik++) {
			fprintf(app_context.config->out_exexutive_list, "executive[%d]_id=,%d,", ik,executive_list[ik]);
		}
		fprintf(app_context.config->out_exexutive_list, "\n");		
	}
	/*
	 ITEM_SET* get_item_set(int item_type_id) {

        map<int, ITEM_SET*>::iterator it = item_map.find(item_type_id);

        // setがmap内になければ作成して登録して返す
        if (it == item_map.end()) {
             ITEM_SET* new_item_set = new ITEM_SET();
             item_map.insert(pair<int, ITEM_SET*>(item_type_id, new_item_set));
             return new_item_set;
        }
        return it->second;		//要素の値（Item setを返している）secondはmapにおける要素の値、fistはmapの要素のキー　  20120524記　高島
    }
	*/



	
};