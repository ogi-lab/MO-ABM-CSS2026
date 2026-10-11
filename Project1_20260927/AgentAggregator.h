#pragma once

#include <algorithm>
#include <vector>
#include <map>
#include "Agent.h"
#include "RandomGenerator.h"
#include "SimulatorConfig.h"
#include "SimulatorContext.h"
#include "Globals3.h"
//#include"GovernmentAggregatorNavigator.h"
//#include "BookKeeper.h"

extern SimulatorContext app_context;
using namespace std;

/**
 *  エージェント集合
 */
//class AgentAggregator : public Agent {

class AgentAggregator{
    vector<Agent*> agent_list;
	//std::map<int, Agent*> agent_map;
	vector<long long > ave_price_list_k;//retailer製品の期首平均製品価格リスト
	                        //vector<int> item_type_idkと順番が対応している
	vector<long long> ave_price_list_w;//wholesaler製品の期首平均価格リスト
	                       //vector<int> item_type_idkwと順番が対応している
	//long long withholdings0;
	//void set_withholdings0(long long w) {withholdings0 = w; }
	//long long get_withholdings0() { return withholdings0; }

protected:
	int agent_type = 0;
    /**
     * エージェント数取得
     *
     * @return エージェント数
     */
    virtual int get_agent_count() = 0;	//エージェントの設定時の数、各数は子クラスの同名関数よりappconfigより取得  20120523記　高島

    /**
     * エージェント生成
     *
     * @param index 生成時の連番
     * @return エージェントインスタンス
     */
    virtual Agent* create_new_agent(int index) = 0;//各子クラスで実装
public:
   // AgentAggregator(int agent_id,int agent_type) : Agent(agent_id,agent_type){}
	AgentAggregator(int agent_type){this->agent_type=agent_type;}
    virtual ~AgentAggregator(){}
	int get_agent_type(){return agent_type;}
	vector <Agent*> get_agent_list(){

		return agent_list;
	}
	/*void print_economy_debug(FILE* out) {
		if (!out) return;
		long long total_Eb = 0;
		long long total_income = 0;
		long long total_expenses = 0;
		for (unsigned int i = 0; i < agent_list.size(); i++) {
			Agent* a = agent_list[i];
			total_Eb += a->get_Eb();
			total_income += a->get_income();
			total_expenses += a->get_expenses();
		}
		// 政府エージェント群を別に集計（app_context から取得）
		long long gov_Eb = 0;
		if (app_context.governmentAggregatorNavigator) {
			std::vector<Agent*> gov_agents = app_context.governmentAggregatorNavigator->get_agent_aggregator()->get_agent_list();
			for (size_t gi = 0; gi < gov_agents.size(); ++gi) {
				Agent* g = gov_agents[gi];
				if (g) gov_Eb += g->get_Eb();
			}
		}

		long long private_Eb = total_Eb - gov_Eb;
		fprintf(out, "period=,%d, total_income=,%lld, total_expenses=,%lld, total_Eb=,%lld, private_Eb=,%lld, gov_Eb=,%lld\n",
			app_context.period_count, total_income, total_expenses, total_Eb, private_Eb, gov_Eb);
	}
	*/
	Agent* get_agent(int agent_id){
		int agent_size = 0;
		int ic = 0;
		ic=0;
		agent_size=agent_list.size();
		for(int i=0;i<agent_size;i++){
			if(agent_list[i]->get_agent_id()==agent_id){
				ic=i;break;
			}
		}//forの終わり
		//fprintf(app_context.config->out_agent_address,"inside agent_aggregator,i=,%d,agent_id=,%d,address=,%x,\n",ic,agent_list[ic]->get_agent_id(),agent_list[ic]);
		return agent_list[ic];
	}
	void renew_utility_weight() {
		for (int i = 0; i < agent_list.size(); i++) {
			agent_list[i]->renew_utility_weight();
		}
	}
	/*
	long long get_ave_price_in_market(int item_type_id) {
		long long ave_price_in_market = 0;
		//毎期首に市場の平均価格を格納しておく。
		//Enterprise.hのdo_kishu()で呼ばれる（agent_type==2131 || 2132の場合のみ）
		//ave_price_in_market is the average of price in market at_kishu(i.e. before production)
		int market_type = 0;
		int period = app_context.period_count;
		if (get_agent_type() == 43) {
			market_type = 1;
			ave_price_in_market = app_context.retail_market_navi_for_customer->get_retail_market( item_type_id)->get_ave_price(market_type, item_type_id);
			//if (period == 21) fprintf(app_context.config->out_test12, "period=,%d,agent_id=,%d,market_type=,%d,ave_price_in_market=,%I64d,item_type_id=,%d,inside define ave_price in market() by producer,\n", app_context.period_count, get_agent_id(), market_type, ave_price_in_market, item_type_id);
		}
		else if (get_agent_type() == 44) {
			market_type = 2;
			ave_price_in_market = app_context.material_market_navi_for_retailer->get_retail_market( item_type_id)->get_ave_price(market_type, item_type_id);
			//if (period == 351 || period == 352 || period == 353 || period == 354) fprintf(app_context.config->out_test12, "period=,%d,agent_id=,%d,market_type=,%d,ave_price_in_market=,%I64d,item_type_id=,%d,inside define ave_price_in market,\n", app_context.period_count, get_agent_id(), market_type, ave_price_in_market, item_type_id);
		}
		//bookKeeper->set_ave_price_in_market(ave_price_in_market);
		return ave_price_in_market;
	
	}
	*/
	/*
	void define_ave_price_list_in_market() {
		//Aggregatorが管轄しているagent集合に関わる製品集合の期首（前期末）の平均製品価格をvectorに格納する
		long long ave_price_in_market = 0;
		if (get_agent_type() == 43) {
			//vectorの初期化
			if(ave_price_list_k.size()>0) for (int i = 0; i < ave_price_list_k.size(); i++)ave_price_list_k.pop_back();
			for (int i = 0; i < app_context.config->item_type_id_count; i++) {
				ave_price_in_market = get_ave_price_in_market(app_context.config->item_type_idk[i]);
				ave_price_list_k.push_back(ave_price_in_market);
			}
			for (int i = 0; i < agent_list.size(); i++) {
				agent_list[i]->set_ave_price_list(ave_price_list_k);
			}
		}
		else if (get_agent_type() == 44) {
			//vectorの初期化
			if(ave_price_list_w.size()>0) for (int i = 0; i < ave_price_list_w.size(); i++)ave_price_list_w.pop_back();
			for (int i = 0; i < app_context.config->item_type_id_count_w; i++) {
				ave_price_in_market = get_ave_price_in_market(app_context.config->item_type_idkw[i]);
				ave_price_list_w.push_back(ave_price_in_market);
			}
			for (int i = 0; i < agent_list.size(); i++) {
				agent_list[i]->set_ave_price_list(ave_price_list_w);
			}
		}
		else {}
	}
	*/
	
    /** 初期設定 */
    void set_up() {
		//printf("start of agentAggregator set_up agent_type=%d agent_count=%d\n",agent_type,get_agent_count());
        int agent_size = get_agent_count();
        agent_list = vector<Agent*>(agent_size);
		//createBookKeeper();
		//fprintf(app_context.config->out_temporary, "");
		if(get_agent_type()==41) printf("before for inside agent_aggregator->set_up() agent_type=%d agent_size=%d\n",get_agent_type(),agent_size);
        for (int i=0; i < agent_size; i++) {
			int agentid = 0;
			//printf("i=%d before create_agent\n",i);
            agent_list[i] = create_new_agent(i);	//各子クラスで実装（各agent_typeを決定する）
			//printf("after create_agent\n");
			agentid=agent_list[i]->get_agent_id();
			//printf("before createBookKeeper() agent[%d]->get_agent_id()=%d\n",i,agentid);
			agent_list[i]->createBookKeeper();
		//	printf("in AgentAggregator->setup() i=%d after crateBookKeeper()\n",i);
			//if(agent_list[i]->get_agent_type()==2131){ //retailerだけがHrを保有する
			if (agent_list[i]->get_agent_type() == 2131 || agent_list[i]->get_agent_type() ==2132 || agent_list[i]->get_agent_type()==2130) {
				agent_list[i]->createHR();//Agent.hで実装、hrをnewしhr->set_up()を呼び出す,set_up()でhire_employee()が呼び出される
			}
		  // printf("before agent_list->set_up() after createBookKeeper() agent_type=%d\n",agent_list[i]->get_agent_type());
			agent_list[i]->set_up();	//hr, bookKeeper以外の初期状態設定
		//	printf("end of agent_list->set_up\n");
			agent_map[agentid]=agent_list[i];
		//	map<int, Agent*>::iterator it = agent_map.find(agent_id);
		//	if(it==agent_map.end()){
		//	agent_map.insert(pair<int, Agent*>(agentid,agent_list[i]));
		//	}
			//[agentid]=agent_list[i];
		//if(get_agent_type()==44) fprintf(app_context.config->out_agent_type,"")
		}
		






		/* for (int i=0; i < agent_size; i++) {
			agent_list[i]->createBookKeeper();
			//agent_list[i]->setBookKeeper(new BookKeeper(agentid));
			if(agent_list[i]->get_agent_type()==2131){
				agent_list[i]->createHR();
			}
			agent_list[i]->set_up();				//各子クラス保有関数  20120523記　高島
		//	fprintf(app_context.config->out_agent_address,"agent_id=,%d,address=,%x,\n",agent_list[i]->get_agent_id(),agent_list[i]);
        }*/
		//printf("end of agent_aggregator set_up agent_type=%d\n",agent_type);
    }
	

    /** 期首処理 */
    void do_kishu() {
	//	if (app_context.period_count == 0) set_withholdings0(0);
		//printf("inside do_kishu() before fprintf agent_type\n");	
		//printf("inside do_kishu() before fo agent_type=%d\n", get_agent_type());
		//管轄するagentが関係するmarketの平均価格を計算して各agentの属性変数vectorに格納する.
		//各agentの属性変数は、vector<longlong>ave_price_list0
	/*
		if (app_context.period_count > 0) {
			if (get_agent_type() == 43 || get_agent_type() == 44) define_ave_price_list_in_market();
			     //agentが関係する市場の平均価格リストを格納する
		}
	*/	
	//	fprintf(app_context.config->out3, "start of do_kishu() of agentaggregator_type=%d\n",get_agent_type());
		//printf( "start of do_kishu() before for  agentaggregator_type=%d\n", get_agent_type());
        for (int i=0; i < int(agent_list.size()); i++) {
		 // printf("period=%d inside for in agent_aggregator aggregator_type= %d before print_agent_type agent_list.size()=%d i=%d agent_id=%d agent_type=%d\n",app_context.period_count,get_agent_type(),int(agent_list.size()),i, agent_list[i]->get_agent_id(),agent_list[i]->get_agent_type());
			agent_list[i]->do_kishu();
			if(i==0)fprintf(app_context.config->out_min_price, "inside kishu\n");
			agent_list[i]->print_min_price();
			//printf("inside for after do_kisyu() i=%d agent_id=%d \n",i,agent_list[i]->get_agent_id());
			if (app_context.period_count == 0) agent_list[i]->print_agent_type();
			//printf("inside for after print_agent_type()\n");
        }
		   //if(	app_context.period_count==0){
		    // set_withholdings0(app_context.withholdings0);
		      //Agent* ag;
		     //ag = app_context.governmentAggregatorNavigator->get_agent_aggregator()->get_agent_list()[0];
		     //ag->set_withholdings(app_context.withholdings0);
		     //}
		//printf("inside do kishu() after for in agent_aggregator agent_type=%d\n",get_agent_type());
    }

    /** 行動 */
    void action() {
		
	//	fprintf(app_context.config->out3, "start of do_action() of agentaggregator_type=%d\n", get_agent_type());

        // 並びをランダムにシャッフルする
        // RandomGenerator内部で共有しているmt19937エンジンでシャッフルする
        RandomGenerator::shuffle(agent_list.begin(), agent_list.end());
		fprintf(app_context.config->out,"shuffle\n");
	//printf("before agent_list[i]->action() agent_type%d size=%d agent_type=%d \n",get_agent_type(),agent_list.size(),agent_list[0]->get_agent_type());
		//fprintf(app_context.config->out_temporary, "period=%d action agent_type=%d agent_list.size()=%d\n",app_context.period_count,get_agent_type(),agent_list.size());
        for (int i=0; i <int( agent_list.size()); i++) {
			//if(i==0) printf("inside action() in AgentAggregator before action of agnt i= %d agent_id = % d agent_type= %d\n", i,agent_list[i]->get_agent_id(),agent_list[i]->get_agent_type());
            agent_list[i]->action();
			if(i==0)fprintf(app_context.config->out_min_price, "inside action\n");
			agent_list[i]->print_min_price();
			//consumer以外はEnterprizeで定義
		//	if (i == 0) printf("inside action() in AgentAggregator after action of agent\n");
		/*	if (app_context.period_count == 0)
				fprintf(app_context.config->out_test10, "agent_list[%d] agent_id=%d agent_type=%d\n", i, agent_list[i]->get_agent_id(),agent_list[i]->get_agent_type());
		*/
		//	if (i == int(agent_list.size() - 1))fprintf(app_context.config->out_test10, "last agent to do_action agent_id= %d agent_type=%d item_type_id_buy=%d\n", agent_list[i]->get_agent_id(), agent_list[i]->get_agent_type());
        }

		//printf("after agent_list[i]->action() agent_type=%d size=%d agent_type=%d \n",get_agent_type(),agent_list.size(),agent_list[0]->get_agent_type());
    }

    /** 期末処理 */
    void do_kimatsu() {
	//	fprintf(app_context.config->out3, "start of do_kimatsu() of agentaggregator_type=%d\n", get_agent_type());
		//printf("start of do_kimatsu in AgentAggregator.h before for, period =%d, agent_type=%d, agent_list.size()=%d\n",app_context.period_count,get_agent_type(),agent_list.size());
		for (int i=0; i<int(agent_list.size()); i++) {
			//if(agent_list[i]->get_agent_type()==2130 || agent_list[i]->get_agent_type()==310) 
				//printf("start of for i= %d agent_id =%d agent_type= %d \n",i,agent_list[i]->get_agent_id(), agent_list[i]->get_agent_type());
			//printf("before agent_list[%d]->do_kimatsu() agent_id=%d agent_type=%d,\n",i,agent_list[i]->get_agent_id(), agent_list[i]->get_agent_type());
			agent_list[i]->do_kimatsu();
			//printf("after agent_list[%d]->do_kimatsu() agent_id= %d \n", i,agent_list[i]->get_agent_id());
			if(get_agent_type()!=41) accounting_treatment_at_kimatsu(agent_list[i]);

			
		//	printf("end of for i= %d agent_id =%d agent_type= %d \n", i, agent_list[i]->get_agent_id(), agent_list[i]->get_agent_type());
        }//for(i==0,...の終わり
		//
		// 
		// 
		 //printf("end of for before print_accountData()\n");
		//if (get_agent_type() == 44)printf("end of agents kimatsu\n");
		//printf("before print_accountData\n");
		if(get_agent_type()!=41) print_accountData();// Consumer
		//printf("after print_accountData\n");
		//printf("if(agent_type==43)\n");// RetailerAggregator
		if(agent_type==43){//RetailerAggregator
			//printf("before printtotalsalesave\n");
			app_context.accountant->print_totalsalesave_retailer();
			//printf("after printtotalsalesave\n");
		   // app_context.accountant->print_cashBook_retailer(app_context.period_count,2131);
			//printf("after print cashbook\n");
		}
		if (agent_type == 44) {//WholesaerAggregator
			//printf("before printtotalsalesave\n");
			app_context.accountant->print_totalsalesave_wholesaler();
			//printf("after printtotalsalesave\n");
			//app_context.accountant->print_cashBook_wholesaler(app_context.period_count, 2132);
			//printf("after print cashbook\n");
		}
		if (agent_type == 48) {
			app_context.accountant->print_totalsalesave_soleproprietor();
		}
		//printf("after(if(agent_id==43 agent_type=%d\n",get_agent_type());
		//printf("end of do_kimatsu aggregator_type=%d\n",get_agent_type());
		//出荷producer数比率のプリントアウト
		//printf("before get_size_shiped agent_type=%d\n",agent_type);
		int nretailer = 0; int nwholesaler = 0; int nsoleproprietor=0; double ratio;
		for (int i = 0; i<int(agent_list.size()); i++) {
			if (agent_type == 43) {
				if (agent_list[i]->get_size_shiped() > 0) nretailer++;
			}
			if (agent_type == 44) {
				if (agent_list[i]->get_size_shiped() > 0) nwholesaler++;
			}
			if (agent_type == 48) {
				if (agent_list[i]->get_size_shiped() > 0) nsoleproprietor++;
			}
		}
		if (agent_type == 43) ratio = double(nretailer) / app_context.config->retailer_count;
		else if (agent_type == 44) ratio = double(nwholesaler) / app_context.config->wholesaler_count;
		else if (agent_type == 48) ratio = double(nsoleproprietor) / app_context.config->soleproprietor_count;
		if(agent_type==43 || agent_type==44 || agent_type==48 )fprintf(app_context.config->out_test13, "period=,%d,agentaggregator_type=,%d, agent_type=,%d,ratio of shiped agent=,%f,\n ", app_context.period_count, agent_type,agent_list[0]->get_agent_type(), ratio);
		//printf("end of do_kimatsu agent_type= %d\n",agent_type);
	//	print_economy_debug(app_context.config->out_economy_debug);
 }//do_kimatsu()の終わり
 void accounting_treatment_at_kimatsu(Agent* ag) {
	 ag->get_bookKeeper()->create_title_list();
	 //printf("after create_title_list()\n");
	 ag->get_bookKeeper()->createGL_Slip();//この関数の中でTB_tableもcreateされる
	 //printf("after createGL_Slip()\n");
	 ag->get_bookKeeper()->createcashBookTotal(app_context.period_count);
	 //printf("after createcashBookTotal \n");
	 ag->get_bookKeeper()->createbankBookTotal(app_context.period_count);
	 ag->get_bookKeeper()->creategenkaBookTotal(app_context.period_count);
	 //20230131コメント化　if (ag->get_agent_type() == 2131 || ag->get_agent_type() == 2132) {
		 //fprintf(app_context.config->wholesalerTotal->out_stockBook, "before createstockBookTotal in side do_kimatsu in agentAggregator \n");
	 //printf("before createstockBookTotal \n");
	 ag->get_bookKeeper()->createstockBookTotal(app_context.period_count);
	 ag->get_bookKeeper()->createstockpriceBookTotal(app_context.period_count);
	 if (ag->get_agent_type() == 2131) {
		 ag->get_bookKeeper()->creatematerialBookTotal(app_context.period_count);

		 ag->get_bookKeeper()->creatematerialpriceBookTotal(app_context.period_count);
	 }
	 if (ag->get_agent_type() == 2130 || ag->get_agent_type() == 2131 || ag->get_agent_type() == 2132) {
		 ag->get_bookKeeper()->creategenkaBookTotal(app_context.period_count);
	 }


	 //printf("after createstockBookTotal \n");
			 //20230131コメント化}
			 //20230131コメント化　else {
				 //consumer用
				 //ag->get_bookKeeper()->createstockBookTotal(app_context.period_count);
				 //ag->get_bookKeeper()->createstockpriceBookTotal(app_context.period_count);
			 //20230131コメント化}
	 if (app_context.period_count < 360) {
		 //	printf("before print_title_list()\n");
		 ag->get_bookKeeper()->print_title_list();
		 //	printf("before print_cashbook\n");
		 ag->get_bookKeeper()->print_cashbook(app_context.period_count);
		 //	printf("before print_cashbooktotal\n");
		 ag->get_bookKeeper()->print_cashbooktotal(app_context.period_count); //printf("after print_cashbook\n");
		 //	printf("before print_bankbook\n");
		 ag->get_bookKeeper()->print_bankbook(app_context.period_count); //printf("after print_bankbook \n");
		 ag->get_bookKeeper()->print_bankbooktotal(app_context.period_count);// printf("after print_bankbooktotal\n");
		 //if (ag->get_agent_type() == 2131 || ag->get_agent_type() == 2132|| ag->get_agent_type() == 31 || ag->get_agent_type() == 311 || ag->get_agent_type() == 312) {
	 //if (ag->get_agent_type() == 2131 || ag->get_agent_type() == 2132){
	 //		printf("before print_stockbook\n");
		 ag->get_bookKeeper()->print_stockbook(app_context.period_count);
		 //	printf("before print_stockpricebook.csv\n");
		 ag->get_bookKeeper()->print_stockpricebook(app_context.period_count);
		 //	printf("after print_stockpricebook.csv\n");
		 ag->get_bookKeeper()->print_stockbooktotal(app_context.period_count);
		 //	printf("after print_stockbooktotal\n");
		 ag->get_bookKeeper()->print_stockpricebooktotal(app_context.period_count);

		 //	printf("aafter print_stockpricebooktotal\n");
		 if (ag->get_agent_type() == 2131) {
			 ag->get_bookKeeper()->print_materialbook(app_context.period_count);
			 ag->get_bookKeeper()->print_materialpricebook(app_context.period_count);
			 ag->get_bookKeeper()->print_materialbooktotal(app_context.period_count);
			 ag->get_bookKeeper()->print_materialpricebooktotal(app_context.period_count);

		 }
		 if (ag->get_agent_type() == 2130 || ag->get_agent_type() == 2131 || ag->get_agent_type() == 2132) {
			 //	printf("before print_genkabook agent_id=%d \n",ag->get_agent_id());
			 fprintf(app_context.config->out_genka_test15, "period=,%d,agent_id=,%d, agent_type=,%d,\n", app_context.period_count, ag->get_agent_id(), ag->get_agent_type());
			 ag->get_bookKeeper()->print_genkabook(app_context.period_count);
			 ag->get_bookKeeper()->print_genkabooktotal(app_context.period_count);

			 //		printf("after  print_genkabook agent_id=%d \n", ag->get_agent_id());
		 }

		 //}
	 }//if(<360の終わり
	 //printf("after print stockBook \n");
 //printf("after print_cashbook,print_bankbook, print_stockbook,in agentaggregator->do_kimatsu()\n");
 //if (get_agent_type() == 43) printf("after print_title_list\n");
	 if (app_context.period_count < 360) {
		 //if (ag->get_agent_id() == 540 || ag->get_agent_id() == 15 || ag->get_agent_id() == 460) {
	 //	if (ag->get_agent_type()==2132 || ag->get_agent_type() == 2131 || ag->get_agent_type() == 31 || ag->get_agent_type() == 311 || ag->get_agent_type() == 312) {
		 //	printf("before print_journal()");
	//	 ag->get_bookKeeper()->print_journal();
		 //	printf("before print_TB_table\n");
			 //if(ag->get_agent_id()==545 || ag->get_agent_id()==455) ag->get_bookKeeper()->print_TB_table();
		 ag->get_bookKeeper()->print_TB_table();
		 //	if(ag->get_agent_type()==2131 || ag->get_agent_type()==2132) ag->get_bookKeeper()->print_TB_table();
		 //	printf("after print_TB_table\n");

	 }

	 //int agent_id1 = 460;			int agent_id0 = 15;			int agent_id2 = 540;
	 //ag->get_bookKeeper()->clear_journal();
	 //ag->get_bookKeeper()->clear_title_list();
	 //ag->get_bookKeeper()->clear_TB_table();//clearするのはaccountantの所、必要データを収録した後とする





 }

	void print_accountData() {	
		if (get_agent_type() == 42) {
			print_accountData_agent_type(311, app_context.config->retailMarketData);
			print_accountData_agent_type(310, app_context.config->retailMarketData);
			print_accountData_agent_type(312, app_context.config->materialMarketData);
			print_accountData_base(31);
		}
	}
	void print_accountData_agent_type(int agent_type, MarketData* marketData) {
		long long sum_income = 0, sum_expenses = 0, sum_Eb0 = 0;
		long long income_ave = 0;
		long long expenses_ave = 0;
		long long Eb0_ave = 0;
		sum_income = 0;
		sum_expenses = 0;
		sum_Eb0 = 0;
		int n = 0;
		//retaler_workerの計算
		for (int i = 0; i<int(agent_list.size()); i++) {
			if (agent_list[i]->get_agent_type() == agent_type) {
				sum_income += agent_list[i]->get_income();
				sum_expenses += agent_list[i]->get_expenses();
				sum_Eb0 += agent_list[i]->get_Eb0();
				n++;
			}
		}
		if (n > 0) {
			income_ave = long long (double(sum_income) / n);
			expenses_ave = long long (double(sum_expenses) / n);
			Eb0_ave = long long (double(sum_Eb0) / n);
			fprintf(marketData->out_income_expenses_ave, "period=,%d,n=,%d,Eb0_ave=,%I64d,income_ave=,%I64d, expenses_ave=,%I64d,\n", app_context.period_count, n, Eb0_ave, income_ave, expenses_ave);
		}
	}
	void print_accountData_base(int agent_type) {
		long long sum_income; long long  sum_expenses;long long sum_Eb0;
		long long income_ave = 0;
		long long expenses_ave = 0;
		long long Eb0_ave = 0;
		sum_income = 0;
		sum_expenses = 0;
		sum_Eb0 = 0;
		int n = 0;
		for (int i = 0; i<int(agent_list.size()); i++) {
			if (agent_list[i]->get_agent_type() == agent_type) {
				sum_income += agent_list[i]->get_income();
				sum_expenses += agent_list[i]->get_expenses();
				sum_Eb0 += agent_list[i]->get_Eb0();
				n++;
			}
		}
		if (n > 0) {
			income_ave = int(double(sum_income) / n);
			expenses_ave = int(double(sum_expenses) / n);
			Eb0_ave = int(double(sum_Eb0) / n);
			fprintf(app_context.config->out_income_expenses_ave, "period=,%d,n=,%d,Eb0_ave=,%I64d,income_ave=,%I64d, expenses_ave=,%I64d,\n", app_context.period_count, n, Eb0_ave, income_ave, expenses_ave);
		}
	}
};

