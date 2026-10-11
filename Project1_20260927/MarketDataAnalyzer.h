#pragma once
#include <map>
#include <string>
#include <iostream>
#include <map>
#include <set>
#include "Globals.h"
#include "Item.h"
#include "PredForItemByPrice.h"
#include "SimulatorContext.h"
#include <vector>
#include "SimulatorConfig.h"
#include <stdio.h>

extern SimulatorContext app_context;
using namespace std;

class MarketDataAnalyzer: public DataAnalyzer{

	vector <map<int,ITEM_SET*>> maps;
	int market_type = 0;
	map<int, ITEM_SET*> item_map_bought;  // キーがint、値がITEM_SET*のmap
	int dataAnalyzer_type = 0;
	struct BoughtItem* boughtItem;
	struct SuppliedItem* suppliedItem;
	struct StockItem* stockItem;
	struct AreaMarketItem* areamarketItem;
	
	//boughtItem=new BoughtItem;
	//BoughtRecord* boughtrecord;
	//boughtrecord=new BoughtRecord;
	BoughtRecord boughtRecord;
	SuppliedRecord suppliedRecord;
	StockRecord stockRecord;
	AreaMarketRecord areamarketRecord;
	
	void set_boughtItem(struct BoughtItem* boughtItem) {
		boughtRecord.set_boughtItem(boughtItem);
	}
	void set_suppliedItem(struct SuppliedItem* suppliedItem) {
		suppliedRecord.set_suppliedItem(suppliedItem);
	}
	void set_stockItem(struct StockItem* stockItem) {
		stockRecord.set_stockItem(stockItem);
	}
	ITEM_SET* get_item_set(int item_type_id){
		//item_map_bought=maps[app_context.period_count];
		map<int,ITEM_SET*>::iterator it=item_map_bought.find(item_type_id);
		//setがmap内になければ作成して登録して返す
		if(it==item_map_bought.end()){
			ITEM_SET* new_item_set=new ITEM_SET();
			item_map_bought.insert(pair<int,ITEM_SET*>(item_type_id,new_item_set));
			return new_item_set;
		}
		return it->second;
	}

	public:
		MarketDataAnalyzer() {
			//fprintf(app_context.config->out_temporary, "MarketDataAnalyzerのオブジェクトが生成されました\n");
			//fprintf(app_context.config->out_temporary, "address of boughtrecord =%x\n",&boughtRecord);
		};
		void set_market_type(int market_type) {
			this->market_type = market_type;
			boughtRecord.set_market_type(market_type);
			suppliedRecord.set_market_type(market_type);
			stockRecord.set_market_type(market_type);
			areamarketRecord.set_market_type(market_type);
		}
		int get_market_type() { return market_type; }
	void set_up(){
		
		boughtRecord.set_up();
		suppliedRecord.set_up();
		stockRecord.set_up();
		//printf("before areamarketrecord_set_up\n");
		areamarketRecord.set_up();
		//printf("after areamarketrecord_set_up\n");
		
	}
	void set_dataAnalyzer_type(int dataAnalyzer_type) {
		//fprintf(app_context.config->out_temporary, "inside set dataAnalyzer_type() dataAnalyzer_type=%d \n", dataAnalyzer_type);
		this->dataAnalyzer_type = dataAnalyzer_type;
		boughtRecord.set_dataAnalyzer_type(dataAnalyzer_type);
		suppliedRecord.set_dataAnalyzer_type(dataAnalyzer_type);
		stockRecord.set_dataAnalyzer_type(dataAnalyzer_type);
		areamarketRecord.set_dataAnalyzer_type(dataAnalyzer_type);
	}
	int get_dataAnalyzer_type() { return dataAnalyzer_type; }
	void do_kishu(){
		//create_map();
		suppliedRecord.do_kishu();
		boughtRecord.do_kishu();
		stockRecord.do_kishu();


	}

	void create_map(){
		int i = 0;
		//map<int, ITEM_SET*> item_map_bought;  // キーがint、値がITEM_SET*のmap]
		for(i=0;i<app_context.config->item_type_id_count;i++){
			ITEM_SET* new_item_set=new ITEM_SET();
			item_map_bought.insert(pair<int,ITEM_SET*>(i,new_item_set));
			//pairのintをiとしているので意味がない,intをitem_type_idとしたものはget_item_setで作られる
			//item_type_id_count個だけのmapが生成される、中身はまだない
			//中身はConsumerの購買毎に追加される(Item->deal()経由で）
		}
	}

	void do_kimatu_notmaps()		//mapsベクターを保有しないバージョン　創　201205278 高島
	{
		//ここで購入された商品情報をBoughtRecordクラスへ引き渡す
	/*	map関係部分をすべて削除　2012_5_29記　荻林
		fprintf(app_context.config->out,"購入された商品情報をmaps<vector*>とBoughtRecordクラスへ引き渡す\n");
		transfer_all_items();

		delete_item_map_bought();

		if(item_map_bought.empty())
			fprintf(app_context.config->out,"空です in do_kimtasu\n");
		else	fprintf(app_context.config->out,"要素数は%dです in do_kimtsu\n",item_map_bought.size());

	*/
		//data_recordのプリントアウト
		print_boughtitems(app_context.period_count);
		print_supplieditems(app_context.period_count);
		print_stockitems(app_context.period_count);
		//print_stockitems(app_context.period_count);
	}
	int get_ave_price(int period,int item_type_id){
		//printf("before int ave_price\n");
		int ave_price=boughtRecord.get_ave_price(period,item_type_id);
		//printf("inside get_ave_price, ave_price=%d\n",ave_price);
		return ave_price;
	}
	void delete_item_map_bought()		//item_map_boughtを毎期削除し、削除過程でItem obも同時に削除　創　201205278 高島
	{									//このコード自体はitem_map_boughtと含めて後日別クラスに分離の可能性があるため、do_kimatuから独立
		map<int, ITEM_SET*>::iterator itmap=item_map_bought.begin();
		Item *del_item;
        for (itmap=item_map_bought.begin(); itmap!= item_map_bought.end(); itmap++) 
		{
            ITEM_SET* item_set_bought = itmap->second;
            ITEM_SET::iterator it;
	        for ( it=item_set_bought->begin() ; it != item_set_bought->end(); it++ )
			{
				del_item=(*it);
//			   (*it)->delete_self();
			   delete del_item;
			}	
        }

		item_map_bought.clear();//vectorに格納した後は毎期mapを空にする
	}

	void do_kimatsu(){
		//printf("start of marketdatAnalyzer->do_kimatsu\n");
		//ここで購入された商品情報をBoughtRecordクラスへ引き渡す
	//	fprintf(app_context.config->out,"購入された商品情報をmaps<vector*>とBoughtRecordクラスへ引き渡す\n");
		//fprintf(app_context.config->out_temporary, "inside do_kimatsu before transfer_all_items period=%d\n",app_context.period_count);
		transfer_all_items();
		maps.push_back(item_map_bought);//mapをvecorにpushbackする
		item_map_bought.clear();//vectorに格納した後は毎期mapを空にする
		if(item_map_bought.empty())
			fprintf(app_context.config->out,"空です in do_kimtasu\n");
		else	fprintf(app_context.config->out,"要素数は%dです in do_kimtsu\n",item_map_bought.size());


		//data_recordのプリントアウト
		//printf("before print out data_records\n");
		fprintf(app_context.config->out_temporary, "before print_boughtitems period=%d\n", app_context.period_count);
		//printf("before print_boughtitems period=%d\n",app_context.period_count);
		print_boughtitems(app_context.period_count);
		//printf("end of print_boughtitems\n");
		print_supplieditems(app_context.period_count);
		//printf("address of suppliedItem=%x suppliedRecord=%x market_type=%d\n", suppliedItem, suppliedRecord, get_market_type());
		fprintf(app_context.config->out_temporary, "after print_supplieditems period=%d\n", app_context.period_count);
		//printf("end of supplieditems before print_stockitems in MarketDataAnalyzer.h\n");
		print_stockitems(app_context.period_count);
		//printf("end of marketdataAnalyzer->do_kimatsu\n");
	}

	void add_item(Item* item){
		//Itemが購入される度にそのItemをmapの中のITEM_SETに格納する
		//この関数はItemクラスから呼び出される
		ITEM_SET* item_set_bought=get_item_set(item->get_item_type_id());
		item_set_bought->insert(item);
	}
	void set_boughtItem(int period, int item_type_id, int price, int seller_id, int buyer_id){
		boughtItem =new BoughtItem;
		boughtItem->period=period;
		boughtItem->item_type_id=item_type_id;
		boughtItem->price=price;
		boughtItem->seller_agent_no=seller_id;
		boughtItem->buyer_agent_no=buyer_id;
		boughtRecord.set_boughtItem(boughtItem);
	}
	void set_suppliedItem(int period, int item_type_id, int price, int seller_id, int qmax){
		//fprintf(app_context.config->out_temporary, "inside set_suppliedItem in MarketDataAnalyzer period=%d seller_id=%d item_type_id=%d price=%d\n",app_context.period_count,seller_id, item_type_id,price);
		suppliedItem =new SuppliedItem;
		suppliedItem->period=period;
		suppliedItem->item_type_id=item_type_id;
		suppliedItem->price=price;
		suppliedItem->seller_agent_no=seller_id;
		suppliedItem->qmax=qmax;
		suppliedRecord.set_suppliedItem(suppliedItem);
	}

	void set_stockItem(int period, int item_type_id, int price, int seller_id){
		//if(market_type==2) printf("period=%d agent_id=%d item_type_id=%d market_type=%d inside marketdataanalyzer set stockItem\n",app_context.period_count, seller_id ,item_type_id,get_market_type());
		stockItem =new StockItem;
		stockItem->period=period;
		stockItem->item_type_id=item_type_id;
		stockItem->price=price;
		stockItem->seller_agent_no=seller_id;
		stockRecord.set_stockItem(stockItem);
	}

	void set_areamarketItem(int period, int item_type_id, int price, int seller_id){
		areamarketItem =new AreaMarketItem;
		areamarketItem->period=period;
		areamarketItem->item_type_id=item_type_id;
		areamarketItem->price=price;
		areamarketItem->seller_agent_no=seller_id;
		areamarketRecord.set_areamarketItem(areamarketItem);
	}

	//TODO　任意の期のmapを取り出して統計解析するコード（関数）を追加すること
	//製品種別の売れた製品の平均価格、価格の最大値・最小値、生産者別平均価格など

	void print_all_items() {
		//maps[count]経由でその期に買われた製品の集合を取り出す
	
		int count = 0;
		count=app_context.period_count;
		//item_map_bought=maps[app_context.period_count];
	//	fprintf(app_context.config->out,"maps[%d]経由で購入された商品情報をプリントする\n",count);
        //cout << "[a]--- 買われた製品の集合 -　in print_all_items---" << endl;
	//	fprintf(app_context.config->out,"[a]--- all items　買われた製品の集合 %d 期----\n",app_context.period_count);
        map<int, ITEM_SET*>::iterator itmap=maps[count].begin();
        for (itmap=maps[count].begin(); itmap!= maps[count].end(); itmap++) {
			//item_type_id_count個だけのmapが呼び出される.各mapにITEM_SETがある
            ITEM_SET* item_set_bought = itmap->second;
            ITEM_SET::iterator it;
			if (item_set_bought->begin() == item_set_bought->end())
				//そのitem_type_idで買われた商品がなければITEM_SETは空
		//		fprintf(app_context.config->out,"no item in in marketadataanalyzer %d\n",itmap->first);
	        for ( it=item_set_bought->begin() ; it != item_set_bought->end(); it++ ){
               cout << (*it)->get_item_type_id() << "-" << (*it)->get_price() << endl;
		//	   fprintf(app_context.config->out," %d  - %I64d seller_id =%d buyer_id=%d \n",(*it)->get_item_type_id(),(*it)->get_price(),(*it)->Get_seller()->Get_agent_id(),(*it)->get_buyer()->Get_agent_id());
			}
           // cout << endl;
        }
		//printf("after the end of for(itmap....\n");
    }


//		一時的な要件による機能であるため後日排除のこと 20120528 削　高島
	void test_q_counter()		//現在のmapsに含まれている製品obの数カウント　20120525創　高島
	{
		int sum=0;
		int count = 0;


		int i = 0;
		for(i=0;i<int(maps.size());i++)
		{
			count=i;
			map<int, ITEM_SET*>::iterator itmap=maps[count].begin();
			for (itmap=maps[count].begin(); itmap!= maps[count].end(); itmap++) {
				//item_type_id_count個だけのmapが呼び出される.各mapにITEM_SETがある
				ITEM_SET* item_set_bought = itmap->second;
				ITEM_SET::iterator it;
				for ( it=item_set_bought->begin() ; it != item_set_bought->end(); it++ ){
				   sum++;
				}
			   // cout << endl;
			}
		}
		//printf("buy_q_counter=%d\n",sum);
	}


	void transfer_all_items()
	{
		//ItemのデータをboughtItemに移してBoughtRecordにて保存する
		//今はこのmapは使っていない、deal毎にBoughtRecordのmemitemに格納している
		int count = 0;
		count=app_context.period_count;	
        map<int, ITEM_SET*>::iterator itmap=item_map_bought.begin();
		//fprintf(app_context.config->out_temporary, "inside transfer all items after map<>, before for period=%d\n",app_context.period_count);
		//if (item_map_bought.begin() == item_map_bought.end()) fprintf(app_context.config->out_temporary, "map=NULL period=%d\n", app_context.period_count);
		//else fprintf(app_context.config->out_temporary, "map!=NULL period=%d\n", app_context.period_count);
        for (itmap=item_map_bought.begin(); itmap!= item_map_bought.end(); itmap++) 
		{
            ITEM_SET* item_set_bought = itmap->second;
            ITEM_SET::iterator it;
			if (item_set_bought->begin() == item_set_bought->end()) {
			//	fprintf(app_context.config->out, "no item in transfer_all_item%d\n", itmap->first);
				//fprintf(app_context.config->out_temporary, "inside transfer_all_items no item period=%d\n", app_context.period_count);
			}
	        for ( it=item_set_bought->begin() ; it != item_set_bought->end(); it++ ){
				//fprintf(app_context.config->out_temporary, "inside transfer_all_items  inside for  period=%d\n", app_context.period_count);
				boughtItem=new BoughtItem;
				boughtItem->buyer_agent_no=(*it)->get_buyer()->Get_agent_id();
				boughtItem->item_type_id=(*it)->get_item_type_id();
				boughtItem->seller_agent_no=(*it)->Get_seller()->Get_agent_id();
				boughtItem->price=int ((*it)->get_price());
				boughtItem->period=app_context.period_count;
				boughtRecord.set_boughtItem(boughtItem);
				//set_boughtItem(&boughtItem);
			//	boughtRecord.print_memItem(0);
	
               //cout << (*it)->get_item_type_id() << "-" << (*it)->get_price() << endl;
			   //fprintf(app_context.config->out," %d  - %d seller id=%d\n",(*it)->get_item_type_id(),(*it)->get_price(),(*it)->Get_seller()->Get_agent_id());
			}
           // cout << endl;
        }

	}

	void print_boughtitems(){
		int i = 0;
		for(i=0;i<app_context.period_count;i++) print_boughtitems(i);
	}

	void print_boughtitems(int period){
		//printf("begining of print_bought_items(period)\n"); 
	//	fprintf(app_context.config->out,"boughtRecord経由で購入された商品情報をプリントする\n");
		//printf("boughtRecord経由で購入された商品情報をプリントする\n");
		boughtRecord.print_boughtitems(period);
		//printf("after print_boughtitems(period)\n");
	}

	void print_boughtitems(int agent_id,int period){
		boughtRecord.print_boughtitems(agent_id,period);
	}

	int number_of_bought_items(int agent_id,int period,int item_type_id){
		return boughtRecord.number_of_bought_items(agent_id,period,item_type_id);
	}

	void print_supplieditems(int period){
	//	fprintf(app_context.config->out,"suppliedRecord経由で供給された商品情報をプリントする\n");
		//printf("suppliedRecord経由で供給された商品情報をプリントする\n");
		suppliedRecord.print_supplieditems(period);
	}

	int number_of_supplied_items(int agent_id,int period,int item_type_id){
		return suppliedRecord.number_of_supplied_items(agent_id,period,item_type_id);
	}

	void print_stockitems(int period){
	//	fprintf(app_context.config->out,"stockRecord経由で供給された商品情報をプリントする\n");
		//printf("inside print_stockitems in MarketDataAnalyzer.h stockRecord経由で供給された商品情報をプリントする market_type=%d\n",get_market_type());
		//printf("address of stockRecords= %x\n", stockRecord);
		stockRecord.print_stockitems(period);
		//printf("end of print_stockitems\n");
	}

	void print_areamarketitems(int period){
	//	fprintf(app_context.config->out,"aremarketRecord経由で期末市場商品情報をプリントする\n");
		areamarketRecord.print_areamarketitems(period);
	}
	void print_producersHistory(int agent_id){
		for(int i=0;i<app_context.config->run_period_count;i++){

		}
	}
};