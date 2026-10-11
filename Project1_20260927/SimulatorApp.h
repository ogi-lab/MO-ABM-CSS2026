#pragma once
#pragma once
#include <iostream>
#include <stdio.h>
#include "RetailerAggregator.h"
#include "WholesalerAggregator.h"
#include "ConsumerAggregator.h"
#include "ConsumerAggregatorNavigator.h"
#include "EquipmentMakerAggregator.h"
#include "SoleProprietorAggregator.h"
#include "BankAggregator.h"
#include "GovernmentAggregator.h"
#include "GovernmentAggregatorNavigator.h"
#include "EnterpriseManager.h"
#include "AreaMarket.h"
#include "CustomerMarketNavigator.h"
#include "GovernmentMarket.h"
#include "MarketSyncMarketLisnter.h"
#include "ShipmentNavigator.h"
#include "ShipmentNavigatorImpl.h"
#include "SimulatorConfig.h"
#include "SimulatorContext.h"
#include "MarketDataAnalyzer.h"
#include "DataAnalyzerNavigator.h"
#include "MarketDataAnalyzerNavigator.h"
#include "RollAggregator.h"
#include "Accountant.h"
#include "LaborMarket.h"
extern SimulatorContext app_context;

/** シミュレータアプリケーション */
class SimulatorApp {
	AreaMarket* retailMarket;					/**< 最終消費財市場 */
	AreaMarket* materialMarket;					/**< 原料市場 */
	GovernmentMarket* governmentMarket;			/**< 政府購買市場*/
	MarketSyncMarketListener* syncListener;		/**< 市場同期市場取引リスナ*/
	//MarketDataAnalyzer* marketdataAnalyzer;		/**<市場データ解析者*/
	ConsumerAggregator* consumer_aggregator;
	GovernmentAggregator* government_aggregator;
     //Note: retailerAggregator, wholesalerAggregatorはapp_contextに保有させている。
	void initialize_withholdings0() { app_context.withholdings0 = 0; }
	 /** 初期設定 */
    void set_up() {
		
        
		printf("--SimulatorApp_set_up--\n");
		RandomGenerator::set_seed();//基本500
		printf("--set_up randomGenerator--\n");
		app_context.accountant=new Accountant();  //create_agents()の前に生成しておく
		printf("--set_up_accountant--\n");
		//app_context.config->rg->set_seed();
		fprintf(app_context.config->out,"----Start----\n");
        printf("-- app set_up_market --\n");
		fprintf(app_context.config->out,"----app set_up----\n");
		set_up_market();
		printf("end of set_up_market\n");
	//	set_up_dataAnalyzer();//set_up_market()の後に書くこと
      
		
    }
	void set_up_Accountant(){
		//create_agent()の後に呼ばれること
		//この関数は実際は使わない。bookKeeperはagent[i]->createBookKeeper()の中のvoid set_bookKeeper(BookKeeper* bookKeeper)によってsetされる。
		app_context.accountant->setup();//各agentのbookKeeperの集合を保有させる
	}
		/**データアナライザ生成*/
	void set_up_dataAnalyzer(){
   /* AreaMarketに移管20210815
		marketdataAnalyzer=new MarketDataAnalyzer();
		//retailMarket->set_dataAnalyzer(marketdataAnalyzer);
		app_context.dataAnalyzer_navi_for_retailMarket = new MarketDataAnalyzerNavigator(*marketdataAnalyzer); //app_context内にアドレス参照を渡して生成
		//printf("address of marketdataAnalyzer =%x in SimulatorApp\n",marketdataAnalyzer);
		marketdataAnalyzer->set_up();	//boughtRecord.set_up();suppliedRecord.set_up();stockRecord.set_up();areamarketRecord.set_up();
	*/
	}

    /** 市場生成 */
    void set_up_market() {

        // 政府購買市場
        governmentMarket = new GovernmentMarket();						//政府用の購買市場の生成  20120523記　高島
        syncListener = new MarketSyncMarketListener(*governmentMarket);	//政府購買市場とリテイラー製品市場の情報共有ob  20120523記　高島

        // 最終消費財市場
        // インスタンス数を増した場合は、必ずすべてにsyncListenerをセットすること
		//printf("--CtoB市場retailMarket生成、set_up--\n");
        retailMarket = new AreaMarket();//アドレス=399c50
		//retailMarket->set_up();
		retailMarket->retailMarket_set_up();
		fprintf(app_context.config->out,"retailMarketが生成されました、アドレス=%x\n",unsigned (retailMarket));
        retailMarket->addMarketListener(syncListener);
        // 原料を扱う市場（TODO 今は使用者がまだいない）

		//printf("--BtoB市場materialMarket生成、set_up--\n");
		materialMarket = new AreaMarket();
		//printf("after materialMarket\n");
		//materialMarket->set_up();
		//printf("before materalmarket_et_up\n");
		materialMarket->materialMarket_set_up();
		fprintf(app_context.config->out, "materialMarketが生成されました、アドレス=%x\n", unsigned (materialMarket));
		//printf("after materialMarket_set_up\n");
		//
		//retailMarket->create_data_analyzer();

        // 消費者購買用の市場ナビゲータ//アドレス＝399e68
        app_context.retail_market_navi_for_customer = new CustomerMarketNavigator(*retailMarket);
		fprintf(app_context.config->out,"retail_market_navi_for_customer=%x\n",unsigned (app_context.retail_market_navi_for_customer));
        // 出荷ナビゲータ//アドレ＝399ea0
        app_context.shipment_navi_for_retailer = new ShipmentNavigatorImpl(*retailMarket);
 		fprintf(app_context.config->out_temporary,"shipment_navi_for_retailerが生成されました、アドレス=%x\n",unsigned (app_context.shipment_navi_for_retailer));
		app_context.shipment_navi_for_soleproprietor = new ShipmentNavigatorImpl(*retailMarket);
		fprintf(app_context.config->out_temporary, "shipment_navi_for_retailerが生成されました、アドレス=%x\n", unsigned(app_context.shipment_navi_for_retailer));

		app_context.material_market_navi_for_retailer = new CustomerMarketNavigator(*materialMarket);// retailerの原料市場の設定20210905記　荻林
		fprintf(app_context.config->out, "material_market_navi_for_retailer=%x\n",unsigned (app_context.material_market_navi_for_retailer));

		app_context.shipment_navi_for_wholesaler = new ShipmentNavigatorImpl(*materialMarket);
		//fprintf(app_context.config->out2,"address of shipment_navi_for_retailer %x 	in SimulatorApp\n",&app_context.shipment_navi_for_retailer);
		//printf("end of set_up_market\n");
    }



    /** エージェント生成 */
	void create_agents() {
		//各種Agent集合をnewして、set_up()を実行すると各種メンバーオブジェクトがnnewされる
		//printf("start create_agents\n");
		//各種役割集合RollAggregatorの生成
		app_context.rollAggregator = new RollAggregator();
		//消費者集合の生成
		//printf("-- create agents -agent_id_seq=%d-\n",app_context.agent_id_seq);
		fprintf(app_context.config->out, "-- create agents -agent_id_seq=%d\n", app_context.agent_id_seq);
		consumer_aggregator = new ConsumerAggregator(42);
		fprintf(app_context.config->out, "consumer_aggregator agent_type=%d\n", consumer_aggregator->get_agent_type());
		consumer_aggregator->set_up();//各消費者生成,各消費者set_up()
		app_context.consumerAggregatorNavigator = new ConsumerAggregatorNavigator(*consumer_aggregator);
		//printf("end of create consumerAggregator\n");
		//労働市場の生成
		app_context.labormarket = new LaborMarket();
		app_context.labormarket->setup();//全ての消費者をunemployed_listに加える
		
		//政府集合の生成
		if (app_context.config->government_count != 0) {
			//app_context.governmentAggregatorNavigator->get_agent_aggregator() = new GovernmentAggregator(1);
			government_aggregator = new GovernmentAggregator(41);
			app_context.governmentAggregatorNavigator = new GovernmentAggregatorNavigator(*government_aggregator);
			printf("end of create government\n");
			//fprintf(app_context.config->out_trace_tax_income, "inside SimulatorApp create government before setup,\n");	
			government_aggregator->set_up();
			//	fprintf(app_context.config->out_trace_tax_income, "inside SimulatorApp create government after setup,\n");
		}

		//銀行集合の生成
		if (app_context.config->bank_count !=0) {
		app_context.bank_aggregator = new BankAggregator(46);
		fprintf(app_context.config->out, "bank_aggregator agent_type=%d\n", app_context.bank_aggregator->get_agent_type());
		app_context.bank_aggregator->set_up();//各銀行生成,各銀行set_up()
		}
		
		//ﾘﾃｲﾗ-集合の生成
		//printf("end of create bank\n");
		if (app_context.config->retailer_count != 0) {
			app_context.retailer_aggregator = new RetailerAggregator(43);
			fprintf(app_context.config->out, "retailer_aggregator agent_type=%d\n", app_context.retailer_aggregator->get_agent_type());
		//	printf("before retailer->setup\n");
			app_context.retailer_aggregator->set_up();//各ﾘﾃｲﾗｰ生成,各ﾘﾃｲﾗｰset_up(),set_up()でcreate_Hr(),Hrのset_up()でhire_emplyee()が呼び出される
		//	printf("end of create retailers\n");
			renew_consumer_type(app_context.retailer_aggregator, 311, 3110);
		}
		
		//ﾎｰﾙｾﾗ集合の生成
		if (app_context.config->wholesaler_count != 0) {
			//printf("before create_wholeseller\n");
			app_context.wholesaler_aggregator = new WholesalerAggregator(44);
			//printf("after create wholeseller\n");
			fprintf(app_context.config->out, "wholesaler_aggregator agent_type=%d \n", app_context.wholesaler_aggregator->get_agent_type());
			app_context.wholesaler_aggregator->set_up();//各ﾎｰﾙｾﾗｰ生成,各ﾎｰﾙｾﾗｰset_up(),set_up()でcreate_Hr(),Hrのset_up()でhire_emplyee()が呼び出される

			//printf("end of create wholesaler\n
			printf("before renew_conwsumer_type\n");
			renew_consumer_type(app_context.wholesaler_aggregator, 312, 3120);
			printf("end of renew_consumer_type for wholesaler_worker\n");
		}
		//個人事業主の生成
		if (app_context.config->soleproprietor_count != 0) {
			app_context.soleproprietor_aggregator = new SoleProprietorAggregator(48);
			//printf("end of creating soleproprietor_aggregator\n");
			fprintf(app_context.config->out, "soleproprietor_aggregator agent_type=%d\n", app_context.soleproprietor_aggregator->get_agent_type());
			app_context.soleproprietor_aggregator->set_up();//各個人事業主生成,各個人事業主set_up()
			//printf("end of soleproprietor_aggregator->set_up()\n");
			renew_consumer_type(app_context.soleproprietor_aggregator, 310, 3100);
			//printf("end of renew_consumer_type for solproprietor_worker\n");
		}

		//設備製造者集合の生成
		if (app_context.config->equipmentmaker_count != 0) {
			app_context.equipmentmaker_aggregator = new EquipmentMakerAggregator(45);
			//printf("after create equipmentmakeraggregator\n");
			fprintf(app_context.config->out, "equipmentmaker_aggregator agent_type=%d\n", app_context.equipmentmaker_aggregator->get_agent_type());
			app_context.equipmentmaker_aggregator->set_up();//各設備製造者生成,各設備製造者set_up()
			//printf("end of equipmentmaker_aggregator->set_up()\n");
		}

		//倒産起業管理者の生成
		app_context.enterprise_manager=new EnterpriseManager(app_context.agent_id_seq++,47);
		fprintf(app_context.config->out,"enterprise_manager agent_type=%d\n",app_context.enterprise_manager->get_agent_type());
		app_context.enterprise_manager->set_up();//set_up()
		//printf("end of create_agents\n");
		
		print_consumer_type();
	//	if(app_context.config->ncase!=0) consumer_aggregator->renew_utility_weight();
			//retailerはsoleproprietor製品を、soleproprietorはretailer製品を優先的に購入するケース、これはやめたのでコメントのままでよい
		print_consumer_utility_weight();

    }

	void renew_consumer_type(AgentAggregator* aggregator, int agent_type,int agent_type2){
	     //agent_type: worker_type, agent_type2: executive_type
		int size = aggregator->get_agent_list().size();
		for (int i = 0; i < size; i++) {
			
			Agent* ag;
			ag = aggregator->get_agent_list()[i];
			int size_of_employee = ag->get_HR()->get_employee_list().size();
			 //HR* hr はAgent.hクラスが保有。

			for (int j = 0; j < size_of_employee; j++) {
				int size_of_consumer = consumer_aggregator->get_agent_list().size();
				//consumerの中からemployee_listにあるagent_idをもつconsumerを探し出して、そのagent_typeをworker_typeに変更する
				for (int k = 0; k < size_of_consumer; k++) {
					if (consumer_aggregator->get_agent_list()[k]->get_agent_id() == ag->get_HR()->get_employee_list()[j]) {
						consumer_aggregator->get_agent_list()[k]->set_agent_type(agent_type);
						consumer_aggregator->get_agent_list()[k]->get_bookKeeper()->set_agent_type(agent_type);
						//consumer_aggregator->get_agent_list()[k]->renew_utility_weight();
					}

				}
			}//for(j=の終わり
			int size_of_executive = ag->get_HR()->get_executive_list().size();
			for (int j = 0; j < size_of_executive; j++) {
				int size_of_consumer = consumer_aggregator->get_agent_list().size();
				//consumerの中からexecutive_listにあるagent_idをもつconsumerを探し出して、そのagent_typeをexecutive_typeに変更する
				for (int k = 0; k < size_of_consumer; k++) {
					if (consumer_aggregator->get_agent_list()[k]->get_agent_id() == ag->get_HR()->get_executive_list()[j]) {
						consumer_aggregator->get_agent_list()[k]->set_agent_type(agent_type2);
						consumer_aggregator->get_agent_list()[k]->get_bookKeeper()->set_agent_type(agent_type2);
						//consumer_aggregator->get_agent_list()[k]->renew_utility_weight();
					}

				}
			}//for(j=の終わり
		}//for(i=の終わり
		  
	}
	
	void print_consumer_type() {
		int size = consumer_aggregator->get_agent_list().size();
		for (int i=0; i < size; i++) {
			fprintf(app_context.config->out_consumer_type, "agent_id=,%d,  agent_type=,%d,\n", consumer_aggregator->get_agent_list()[i]->get_agent_id(), consumer_aggregator->get_agent_list()[i]->get_agent_type());
		}

	}
	void print_consumer_utility_weight() {
		for (int i = 0; i < consumer_aggregator->get_agent_list().size(); i++) {
			Agent* ag;
			ag = consumer_aggregator->get_agent_list()[i];
			ag->print_utility_weight();
		}
		
		

	}
    /** シミュレーション実行 */
    void run() {
        printf("-- run --\n");
		fprintf(app_context.config->out,"-- run --\n");
		printf("before initialize withholdings0\n");	
		initialize_withholdings0();
		printf("after initialize withholdings0\n");
        for (int i=0; i < app_context.config->run_period_count; i++) 
		{
			printf("i_period %d\n",i);	//  20120523記　高島
            app_context.period_count = i;
			printf("i= %d before process_period\n",i);
           process_period();
			printf("i= %d after process_period\n",i);
        }
    }

    /** 期内処理実行 */
    void process_period() {
        cout << "\n -- process_period [" << app_context.period_count << "]--" << endl;
		//printf("beginning of process period() 20230608 period=%d \n",app_context.period_count);
	//	fprintf(app_context.config->out,"\n-- process_period [%d ]--\n" ,app_context.period_count);
		
		// 
		//期首行動(consumer,wholesaler,retailer,solproprietor, govenmentの順）
	
		materialMarket->get_marketdataAnalyzer()->do_kishu();

		materialMarket->materialMarket_print_all_items(1);//生産開始前期首在庫リスト
		materialMarket->print_ITEM_SET_size(1);
		//printf("before define ave_price \n");
		materialMarket->define_ave_price_in_market();
		//printf("before consumer do kishu\n");
		//consumer_aggregator->do_kishu();		//消費者期首行動 agentaggregator->Consumer所管  20120523記　高島
		//
		//printf("after consumer_aggregator->do_kishu() in simulatorApp\n");
		if(app_context.config->wholesaler_count!=0) app_context.wholesaler_aggregator->do_kishu();
		//printf("after wholesaler_aggregator->do_kishu()\n");
		materialMarket->print_ITEM_SET_size(1);
		
		retailMarket->get_marketdataAnalyzer()->do_kishu();						//3種類のrecordのkishu()  (previous_size=memItem.size();を実行する)
		retailMarket->get_marketdataAnalyzer()->do_kishu();//item_map_boughtの生成
		//printf("after retailMarket->get_marketdataAnalyzer()->do_kisyu()\n");
	//	fprintf(app_context.config->out3,"peirod=%d 生産開始前の在庫情報 in AreaMarket\n",app_context.period_count);
		retailMarket->retailMarket_print_all_items(1);//生産開始前期首在庫リスト
		retailMarket->print_ITEM_SET_size(1);
		retailMarket->define_ave_price_in_market();
		//	printf("period=%d before production at kishu map_size(10)=%d map_size(20)=%d\n", app_context.period_count,retailMarket->get_ITEM_SET_size(10),retailMarket->get_ITEM_SET_size(20));
		//printf("end of kishu of consumer, wholesaler, marketAnalyzer\n");
		//printf("before retaileraggregator->do_kishu()\n");
		if(app_context.config->retailer_count!=0)app_context.retailer_aggregator->do_kishu();		//リテイラー期首行動 agentaggregator->Enterprise所管 20120523記　高島
		//printf("after retaileraggregator->do_kishu() no owari\n");
		if(app_context.config->soleproprietor_count!=0) app_context.soleproprietor_aggregator->do_kishu();
		//printf("after soleproprietor_aggregator->do_kishu()\n");
		// printf("before consumer do kishu\n");
		consumer_aggregator->do_kishu();		//消費者期首行動 agentaggregator->Consumer所管  201
		//printf("before government_aggregator->do_kishu()\n");
		//if (app_context.period_count > 0) government_aggregator->do_kishu();

		//fprintf(app_context.config->out_trace_government, "inside SimulatorApp process_period before government_aggregator->do_kishu(), period=,%d,\n", app_context.period_count);
		if(app_context.config->government_count!=0)government_aggregator->do_kishu();
		//printf("after government_aggregator->do_kishu()\n");
		/*
		materialMarket->get_marketdataAnalyzer()->do_kishu();
		materialMarket->materialMarket_print_all_items(1);//生産開始前期首在庫リスト
		materialMarket->print_ITEM_SET_size(1);
		*/

		//期中行動（wholesaler, retailer, solproprietor, consumer,governmentの順）
		//printf("before wholesaleraggregator->action() \n");
		if(app_context.config->wholesaler_count!=0) app_context.wholesaler_aggregator->action();
		materialMarket->materialMarket_print_all_items(2);//wholesaler生産後購買開始前の在庫リスト
		materialMarket->print_ITEM_SET_size(2);
		//materialMarket->print_ITEM_SET_size(2);
		if(app_context.config->retailer_count!=0) app_context.retailer_aggregator->action();		//RA期中行動 agentaggregator->Producer所管  20120523記　高島	
		//printf("period=%d after production before buying map_size(10)=%d map_size(20)=%d\n", app_context.period_count, retailMarket->get_ITEM_SET_size(10), retailMarket->get_ITEM_SET_size(20));
		//printf("after_retailer_aggregastor->action()\n");														//生産行動、及び未実装の給与支払い、設備投資を含む  20120524記　高島
		//fprintf(app_context.config->out3,"peirod=%d 生産後購買開始前の在庫情報 in AreaMarket\n", app_context.period_count);
		retailMarket->retailMarket_print_all_items(2);//生産後購買開始前の在庫リスト
		retailMarket->print_ITEM_SET_size(2);
		if (app_context.config->soleproprietor_count != 0)	app_context.soleproprietor_aggregator->action();
		retailMarket->retailMarket_print_all_items(2);//生産後購買開始前の在庫リスト
		retailMarket->print_ITEM_SET_size(2);
		//printf("after retailmarket->print ITEM_SET_size\n");
		//printf("after materialMarket->print\n");
		//printf("period=%d before buying map_size(10)=%d map_size(20)=%d\n", app_context.period_count, retailMarket->get_ITEM_SET_size(10), retailMarket->get_ITEM_SET_size(20));

		//printf("before consumeraggregator->acton() after print_All_items(2)\n");
        consumer_aggregator->action();		//CA期中行動　 agentaggregator->Consumer所管   20120524記　高島		

		if(app_context.config->government_count!=0) government_aggregator->action();

		//printf("after government aggergator->action()\n");									//現在購買行動のみ  20120524記　高島
	//	fprintf(app_context.config->out3,"period =%d 購買後の在庫情報 in AreaMarket\n",app_context.period_count);
		//printf("before retailMarket->print_all_items(3) %d\n",app_context.period_count);
		retailMarket->retailMarket_print_all_items(3);//期末の市場にある製品情報をプリントアウトする
		if(app_context.config->ncase!=0)materialMarket->materialMarket_print_all_items(3);//期末の市場にある製品情報をプリントアウトする
		retailMarket->print_ITEM_SET_size(3);
		if(app_context.config->ncase!=0)	materialMarket->print_ITEM_SET_size(3);
		//printf("period=%d after buying at kimatsu map_size(10)=%d map_size(20)=%d\n", app_context.period_count, retailMarket->get_ITEM_SET_size(10), retailMarket->get_ITEM_SET_size(20));

		//printf("after print_all_items(3)\n");
		//printf("before retailMarket->print_all_items\n");
		//printf("end of action period=%d\n",app_context.period_count);
		
		//期末行動（wholesaler, retaileer, solproprietor, consumer,government, retailemarket, marketdataAnalyzerの順）
		// 		
		//printf("period=%d before wholesaler_aggregatore->kimatsu\n",app_context.period_count);
		if(app_context.config->wholesaler_count!=0) app_context.wholesaler_aggregator->do_kimatsu();
		//printf("end of wholesaler_aggregator kimatsu\n");
		//printf("before retailer_aggregator kimatsu\n");
        if(app_context.config->retailer_count!=0) app_context.retailer_aggregator->do_kimatsu();	//CR期末行動 agentaggregator->Enterprise所管  20120523記　高島
														//printf("END OF retailer_aggregator_do_kimatsu");
		printf("before soleproprietor_aggregator_kimatsu\n");
		if(app_context.config->soleproprietor_count!=0)	app_context.soleproprietor_aggregator->do_kimatsu();
		printf("after soleproprietor_aggregator_kimatsu\n");
		fprintf(app_context.config->out_temporary, "before marketAnalyzer->do_kimatsu\n");
		retailMarket->get_marketdataAnalyzer()->do_kimatsu();
		printf("end of retailer kimatsu\n");
	

		// 
		printf("before consumer_aggregator->kimatsu\n");
		consumer_aggregator->do_kimatsu();	//CA期末行動 agentaggregator->Consumer所管   20120524記　高島
		printf("after consumer_aggregator->kimatsu\n");	

		if(app_context.config->government_count!=0) government_aggregator->do_kimatsu();
	printf("end of government_aggregator->kimatsu\n");
		//fprintf(app_context.config->out_temporary, "before marketdataAnalyzer()->do_kimatsu\n");

		printf("end of do_kimatsu() of agents \n");
		if(app_context.config->ncase!=0) materialMarket->get_marketdataAnalyzer()->do_kimatsu();
		fprintf(app_context.config->out_temporary, "after marketdataAnalyzer()->do_kimatsu\n");
		//marketdataAnalyzer->do_kimatu_notmaps();
		//marketdataAnalyzer->print_all_items();//期末に購入された製t品情報をプリントアウトする
		/* 期末のAreaMarket内製品をvector <AreaMarketItem*> areamarketItemsに格納する
		producerのstock情報　vector<StockItem*> stockItemsと同じ内容であることを確認2012_5_3荻林
		*/	
		retailMarket->transfer_to_areamarketItems();
		retailMarket->get_marketdataAnalyzer()->print_areamarketitems(app_context.period_count);
		materialMarket->transfer_to_areamarketItems();
		materialMarket->get_marketdataAnalyzer()->print_areamarketitems(app_context.period_count);
        //governmentMarket->print_all_items();
		//if(app_context.period_count%50==0) printf("period=%d\n",app_context.period_count);
		retailMarket->get_marketdataAnalyzer()->test_q_counter();
		//printf("after q_counter\n");
		printf("before accountant do kimatsu()\n");
		app_context.accountant->do_kimatsu();//各agentのbookKeeperのkimatsu()を呼び出す.Governmentはコメント化しているので含まれない
		printf("period=%d end of process_period() after accountant do kimatsu()\n",app_context.period_count);
    }
	//fprintf("after bookKip_list",agent%d)

public:
    SimulatorApp() {}
    ~SimulatorApp() {
        delete app_context.config;
        delete app_context.shipment_navi_for_retailer;
        delete app_context.shipment_navi_for_wholesaler;
        delete consumer_aggregator;
        delete app_context.retailer_aggregator;
        delete syncListener;
        delete governmentMarket;
        delete retailMarket;
        delete materialMarket;
    }
 
    /** シナリオ実行 */
    void run_scenario() {
        printf("- run simulation. --\n");
        set_up();
		printf("--end of set_up()--\n");
		 create_agents();
		 printf("---end of create_agents()\n");
	   set_up_Accountant();
	   printf("end of set_up_Accontant()\n");
	   fprintf(app_context.config->out, " -run simulation ---\n");     	
        run();
		printf("end of run()\n");
    }
};
