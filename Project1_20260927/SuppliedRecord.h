#pragma once
#include <vector>
#include "SimulatorConfig.h"
#include "SimulatorContext.h"
//#include "Item.h"
//class Item;
using namespace std;
extern SimulatorContext app_context;
struct SuppliedItem {
	int seller_agent_no = 0;
	int item_type_id = 0;
	int price = 0;
	int period = 0;
	int qmax = 0;//production capacity of item_type_id of seller_agent_no
};
class SuppliedRecord {
	vector <SuppliedItem*> memItem;
	int* price_ave;
	int* sum;
	int* n;
	int* nqmax;
	int previous_size = 0;//期首のmemItemサイズ 2012_5_29記　荻林
	int dataAnalyzer_type = 0;
	int market_type = 0;
	int dimension_max = maxof(app_context.config->item_type_id_count_w, app_context.config->item_type_id_count);
	/*
		//製品の平均価格、配列だけset_upで製品種数分newしておく
		price_ave = new int[app_context.config->item_type_id_count_w];
		sum = new int[app_context.config->item_type_id_count_w];
		//期に購入された製品の個数
		n = new int[app_context.config->item_type_id_count_w];
	*/
	int maxof(int a, int b) { if (a > b) return a; else return b; }
public:
	void set_dataAnalyzer_type(int dataAnalyzer_type) { this->dataAnalyzer_type = dataAnalyzer_type; }
	int get_dataAnalyzer_type() { return dataAnalyzer_type; }
	void set_market_type(int market_type) { this->market_type = market_type; }
	int get_market_type() { return market_type; }
	SuppliedRecord() {
		//	boughtItem=new BoughtItem;//コンストラクタ内でnewするのはダメ
	}
	~SuppliedRecord() {
		delete[] n;
		delete[] price_ave;
		delete[] sum;
		delete[] nqmax;
	}

	void set_up() {

		//製品の平均価格、配列だけset_upで製品種数分newしておく
		price_ave = new int[dimension_max];
		sum = new int[dimension_max];
		//期に購入された製品の個数
		n = new int[dimension_max];
		nqmax = new int[dimension_max];
		MarketData* marketData = NULL;

		if (market_type == 1) {
			marketData = app_context.config->retailMarketData;
			//	fprintf(app_context.config->out_temporary, "dataAnalyzertype=%d\n", dataAnalyzer_type);
		}
		else if (market_type == 2) {
			marketData = app_context.config->materialMarketData;
		} //else {}
		fprintf(marketData->out_toukei_supplied_ave, "period,item_type_id,number of items supplied,average_price,qmax,\n");
		fprintf(marketData->out_toukei_supplied, "period,item_type_id,price, seller_agent_no,qmax,\n");
		fprintf(marketData->out2_toukei_supplied, "period,item_type_id,price, seller_agent_no,qmax,ncount_supplied,\n");
	}
	void do_kishu() {
		previous_size = memItem.size();
	}
	void print_memItem(int i) {
		printf("memItem[%d]->price=%d\n", i, memItem[i]->price);
	}
	SuppliedItem* get_item(int i) {
		return memItem[i];
	}
	void set_suppliedItem(struct SuppliedItem* suppliedItem) {
		//this->boughtItem=boughtItem;
		memItem.push_back(suppliedItem);
		//fprintf(app_context.config->out_temporary, "inside set_suppliedItem in SuppliedRecord size of memItem=%d\n",memItem.size());
	}
	int get_sizeofmemItem() {
		return memItem.size();
	}
	void print_supplieditems(int period) {
		//fprintf(app_context.config->out_temporary, "period=%d market_type=%d inside print_supplieditems\n ",period,market_type);
		int i; int j;
		int size = 0;
		int* agent_no;
		int ncount = 0;
		int seller_id = 0;
		//printf("begining of print_supplieditems market_type=%d\n", market_type);
		//if(app_context.config->item_type_id_count_w > app_context.config->item_type_id_count) agent_no = new int[app_context.config->item_type_id_count_w];
		//else agent_no = new int[app_context.config->item_type_id_count];
		agent_no = new int[dimension_max];
		//printf("end of if\n");
		/*if (market_type == 1) 	agent_no = new int[app_context.config->item_type_id_count];
		else if(market_type==2) agent_no = new int[app_context.config->item_type_id_count_w];
		else { agent_no = new int[app_context.config->item_type_id_count]; }
		*/
		size = memItem.size();
		//fprintf(app_context.config->out_temporary, "inside print_supplieditems in suppliedrecord before if(market_type==1) period=%d size=%d previous_size=%d market_type=%d\n", app_context.period_count, size, previous_size, market_type);

		//fprintf(app_context.config->out,"[suppliedRecord] ----all items----供給された製品の集合 %d 期 \n",app_context.period_count);
		//fprintf(app_context.config->out," period  item_type_id  price   seller_id\n");
		//printf("inside print_supplieditems size=%d, previous_size=%d\n",size,previous_size);
		ncount = 0;
		/*int sizeOfperiod=0;
		for(i=previous_size;i<size;i++){
			fprintf(app_context.config->out2_toukei_supplied," %d , %d , %d , %d, %d, %d,%d, %d,%d,\n" ,
						memItem[i]->period,
						memItem[i]->item_type_id,
						memItem[i]->price,
						memItem[i]->seller_agent_no,
						memItem[i]->qmax,
						sizeOfperiod,
						i,
						previous_size,
						size);
			sizeOfperiod++;}
			*/
			/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
			//printf("before if(market_type==1)\n");
		MarketData* marketData = NULL;
		if (market_type == 1) {
			marketData = app_context.config->retailMarketData;
			app_context.config->item_type_id_count0 = app_context.config->item_type_id_count;
			app_context.config->item_type_idk0 = app_context.config->item_type_idk;
		}
		else if (market_type == 2) {
			marketData = app_context.config->materialMarketData;
			app_context.config->item_type_id_count0 = app_context.config->item_type_id_count_w;
			app_context.config->item_type_idk0 = app_context.config->item_type_idkw;
		}
		//printf("before for\n");
		for (i = 0; i < app_context.config->item_type_id_count0; i++) {
			sum[i] = 0;// printf("after sum i=%d\n",i);
			price_ave[i] = 0; //printf("after price_ave i=%d\n",i);
			n[i] = 0; nqmax[i] = 0; //printf("after n i=%d\n,",i);
			agent_no[i] = 0;
		}

		//printf("if (size>previous_size)\n");
		if (size > previous_size) {
			seller_id = memItem[previous_size]->seller_agent_no;
			for (i = previous_size; i < size; i++) {
				//printf("inside for before if i=%d period=%d\n",i,period);
				if (memItem[i]->period == period) {
					//printf("inside if before fprint\n");
					fprintf(marketData->out_toukei_supplied, " %d , %d , %d , %d,%d,\n",
						memItem[i]->period,
						memItem[i]->item_type_id,
						memItem[i]->price,
						memItem[i]->seller_agent_no,
						memItem[i]->qmax);
					//--------------------------------------20191123荻林記
					if (memItem[i]->seller_agent_no == seller_id) {
						ncount++;
						//fprintf(app_context.config->out2_toukei_supplied,"inside if(i<size-1) ,i=%d, size-1=%d, ncount=%d,seller_id=%d,\n",
						//	  i,size-1,ncount,seller_id);
						if (i == size - 1) {
							//ncount++;
							fprintf(marketData->out2_toukei_supplied, " %d , %d , %d , %d, %d, %d,\n",
								memItem[i]->period,
								memItem[i]->item_type_id,
								memItem[i]->price,
								memItem[i]->seller_agent_no,
								memItem[i]->qmax,
								ncount);
							ncount = 0;
						}
					}
					else {
						//fprintf(app_context.config->out2_toukei_supplied,"inside else, i=%d, size-1=%d, ncount=%d,\n",i,size-1,ncount);
						fprintf(marketData->out2_toukei_supplied, " %d , %d , %d , %d, %d, %d,\n",
							memItem[i - 1]->period,
							memItem[i - 1]->item_type_id,
							memItem[i - 1]->price,
							memItem[i - 1]->seller_agent_no,
							memItem[i - 1]->qmax,
							ncount);
						if (memItem[i]->seller_agent_no != seller_id) seller_id = memItem[i]->seller_agent_no;
						ncount = 1;
						if (i == size - 1) {
							//ncount++;
							fprintf(marketData->out2_toukei_supplied, " %d , %d , %d , %d, %d, %d,\n",
								memItem[i]->period,
								memItem[i]->item_type_id,
								memItem[i]->price,
								memItem[i]->seller_agent_no,
								memItem[i]->qmax,
								ncount);
							ncount = 0;
						}
					}


					/*if(i==size-1) {
						//ncount++;
							fprintf(app_context.config->out2_toukei_bought," %d , %d , %d , %d, %d, %d,\n" ,
							memItem[i]->period,
							memItem[i]->item_type_id,
							memItem[i]->price,
							memItem[i]->seller_agent_no,
							memItem[i]->qmax,
							ncount);
					}*/

					//--------------------------------------------------
					for (j = 0; j < app_context.config->item_type_id_count0; j++)
					  {
						//printf("inside supplied record, j=%d item_type_id_count0=%d item_type_id_count=%d \n", j, app_context.config->item_type_id_count0, app_context.config->item_type_id_count);
						if (memItem[i]->item_type_id == app_context.config->item_type_idk0[j]) {
							//if(qmax[j]==0) qmax0[j]=memItem[i]->qmax;
							if (memItem[i]->seller_agent_no != agent_no[j]) {
								//各sellerのqmaxを１回だけカウントしてnqmax[]に加える
								nqmax[j] += memItem[i]->qmax;
								agent_no[j] = memItem[i]->seller_agent_no;
							}
							sum[j] += memItem[i]->price;
							n[j]++;
						}
						else {}
					}
				}
				else printf("suppliedRecord dataに異常があります period=%d\n", period);
			}
		}
		delete[] agent_no;
	//	printf("inside supplied record, before for(j=..<item_type_id_count0\n");
		for (j = 0; j < app_context.config->item_type_id_count0; j++) {
			if (n[j] != 0) {
				price_ave[j] = int(double(sum[j]) / n[j]);
				fprintf(marketData->out_toukei_supplied_ave, "%d , %d ,%d , %d ,%d,,", app_context.period_count, app_context.config->item_type_idk0[j], n[j], price_ave[j], nqmax[j]);
			}
			else fprintf(marketData->out_toukei_supplied_ave, "%d , %d ,%d ,,,,", app_context.period_count, app_context.config->item_type_idk0[j], n[j]);

		}
		fprintf(marketData->out_toukei_supplied_ave, "\n");

		/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
		//fprintf(app_context.config->out_temporary, "inside print supplied items in Supplied recored period=%d  \n", app_context.period_count);
	}

	void print_supplieditems(int agent_id, int period) {
		int i = 0;
		int size = 0;
		size = memItem.size();
		//fprintf(app_context.config->out,"[suppliedRecord] ----all items----供給された製品の集合 %d 期 period = %d agent_id=%d \n",app_context.period_count,period,agent_id);
		//if(size==0) fprintf(app_context.config->out,"買われた製品集合は空です\n");
		//fprintf(app_context.config->out," period  item_type_id  price  seller_id\n");
		if (size > 0) {
			for (i = previous_size; i < size; i++) {
				if (memItem[i]->period == period && memItem[i]->seller_agent_no == agent_id) {
					fprintf(app_context.config->out, " %d  %d  %d   %d\n",
						memItem[i]->period,
						memItem[i]->item_type_id,
						memItem[i]->price,
						memItem[i]->seller_agent_no);
				}
			}
		}
	}
	int number_of_supplied_items(int agent_id, int period, int item_type_id) {
		int i = 0, size = 0;
		size = memItem.size();
		int sum = 0;
		//printf("agent_id=%d period=%d item_type_id=%d size=%d\n",agent_id,period,item_type_id,size);
		//fprintf(app_context.config->out,"agent_id=%d period=%d item_type_id=%d\n",agent_id,period,item_type_id);
		if (size > 0) {
			for (i = 0; i < size; i++) {
				if (memItem[i]->period == period) {
					if (memItem[i]->seller_agent_no == agent_id && memItem[i]->item_type_id == item_type_id) {
						sum++;
					}

				}
			}
			return sum;
		}
		else {
			return 0;
		}
	}
};