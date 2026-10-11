#pragma once
#include <vector>
#include "SimulatorConfig.h"
#include "SimulatorContext.h"
//#include "Item.h"
//class Item;
using namespace std;
extern SimulatorContext app_context;
struct BoughtItem{
	int buyer_agent_no = 0;
	int seller_agent_no = 0;
	int item_type_id = 0;
	int price = 0;
	int period = 0;
};
struct AvePrice{
	int item_type_id = 0;
	int price_ave = 0;
	int period = 0;
};
class BoughtRecord{
	struct AvePrice* aveprice;
	vector <BoughtItem*> memItem;
	vector <AvePrice*> memAvePrice;
	int *price_ave;
	int *sum;
	int *n;
	int previous_size = 0;//期首のmemItemサイズ 2012_5_29記　荻林
	int dataAnalyzer_type = 0;
	int market_type = 0;

public:
	void set_dataAnalyzer_type(int dataAnalyzer_type) { this->dataAnalyzer_type = dataAnalyzer_type; }
	int get_dataAnalyzer_type() { return dataAnalyzer_type; }
	void set_market_type(int market_type) { this->market_type = market_type; }
	int get_market_type() { return market_type; }
	BoughtRecord(){
	//	boughtItem=new BoughtItem;//コンストラクタ内でnewするのはダメ
	}
	~BoughtRecord(){
		delete [] n;
		delete [] price_ave;
		delete [] sum;
	}
	void set_up() {
		//期に購入された製品の平均価格
		price_ave = new int [app_context.config->item_type_id_count];
		sum = new int[app_context.config->item_type_id_count];
		//期に購入された製品の個数
		n = new int[app_context.config->item_type_id_count];
		if (market_type == 1) {
		fprintf(app_context.config->retailMarketData->out_toukei_bought_ave, "period,item_type_id,number of bought items,average price,\n");
		fprintf(app_context.config->retailMarketData->out_toukei_bought, "period,item_type_id,price,buyer_id,seller_id,\n");//2012_5_29記　荻林
		fprintf(app_context.config->retailMarketData->out2_toukei_bought, "period,item_type_id,price,buyer_id,seller_id,ncount_bought,\n");//2019_11_23記　荻林
		//previous_size = 0;(初期はmemItem.size=0のため不要
		}
		else if (market_type == 2) {
			fprintf(app_context.config->materialMarketData->out_toukei_bought_ave, "period,item_type_id,number of bought items,average price,\n");
			fprintf(app_context.config->materialMarketData->out_toukei_bought, "period,item_type_id,price,buyer_id,seller_id,\n");//2012_5_29記　荻林
			fprintf(app_context.config->materialMarketData->out2_toukei_bought, "period,item_type_id,price,buyer_id,seller_id,ncount_bought,\n");//2019_11_23記　荻林
			//previous_size = 0;(初期はmemItem.size=0のため不要
		}
		else {}
	}
	void do_kishu(){
		previous_size=memItem.size();
		//period_count=0の時はmemItem.size=0,よってprevious_size=0
		//前期末購入がゼロの場合は後にvectorでサイズオーバになるので要注意
	}
	void print_memItem(int i){
		printf("memItem[%d]->price=%d\n",i,memItem[i]->price);
	}
	BoughtItem* get_item(int i){
		return memItem[i];
	}
	void set_boughtItem(struct BoughtItem* boughtItem){
		//item->deal(buyer)からDataAnalyzer経由で商品が購入されるたびに呼び出される
		//this->boughtItem=boughtItem;
		memItem.push_back(boughtItem);
	}
	void set_AvePrice(int period, int price_ave, int item_type_id){
		aveprice=new struct AvePrice();
		aveprice->item_type_id=item_type_id;
		aveprice->period=period;
		aveprice->price_ave=price_ave;
		memAvePrice.push_back(aveprice);
	}
	int get_ave_price(int period, int item_type_id){
		int size = 0;
		size=memAvePrice.size();
		//printf("inside get_ave_price after size size=%d\n",size);
		int price_ave = 0;
		price_ave=0;
		struct AvePrice* aveprice;
		//printf("inside get_ave_price before for \n");
		for(int i=0;i<size;i++){
			aveprice=memAvePrice[i];
		//	printf("i=%d period=%d ave_price_item_type_id=%d\n",i,aveprice->period,aveprice->item_type_id);
			if(aveprice->item_type_id==item_type_id && aveprice->period==period){
				price_ave=aveprice->price_ave;
			}
		}
		return price_ave;
	}
	int get_sizeofmemItem(){
		return memItem.size();
	}
	void print_boughtitems(int period) {
	//	printf("beginning of print_boughtitems(%d)\n", period);
		//期に購入された製品情報（個々の製品およびitem_type_id毎の平均価格をプリントアウトする
		//fprintf(app_context.config->out_temporary,"begining of boughtrecord->print_boughtitems(period) period=%d\n",app_context.period_count);
		int i; int j;
		int size = 0;
		int ncount = 0;
		int buyer_id = 0;
		int seller_id = 0;
		size = memItem.size();

		for (i = 0; i < app_context.config->item_type_id_count; i++) {
			sum[i] = 0; price_ave[i] = 0; n[i] = 0;
		}
		//fprintf(app_context.config->out,"[boughtRecord] ----all items----買われた製品の集合 %d 期 \n",app_context.period_count);
		//fprintf(app_context.config->out_temporary, "[boughtRecord] ----all items----買われた製品の集合 %d 期 previous_size=%d size=%d\n", app_context.period_count, previous_size, size);
		//fprintf(app_context.config->out," period  item_type_id  price  buyer_id  seller_id\n");
	//	printf("before difning buyer and seller seize=%d previous_size= %d \n", size, previous_size);
		ncount = 0;

		/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
		if (market_type == 1) {
		if (size - previous_size > 0) {
			buyer_id = memItem[previous_size]->buyer_agent_no;//最初の購入製品の買主のbuyer_id
			seller_id = memItem[previous_size]->seller_agent_no;//最初の購入製品の売主のseller_id
		//	printf("befor for after defining buyer and seller previous_size= %d size= %d\n",previous_size,size);
			for (i = previous_size; i < size; i++) {
				if (memItem[i]->period == period) {
					//printf("i=%d inside if before fprint\n",i);
					//今期購入された商品を一つづつout_toukei_boughtに書きだす。
					fprintf(app_context.config->retailMarketData->out_toukei_bought, " %d , %d , %d , %d ,  %d,\n",
						memItem[i]->period,
						memItem[i]->item_type_id,
						memItem[i]->price,
						memItem[i]->buyer_agent_no,
						memItem[i]->seller_agent_no);
					//--------------------------------------20191123荻林記

					if (memItem[i]->buyer_agent_no == buyer_id && memItem[i]->seller_agent_no == seller_id) {
						//同じbuyer、sellerの製品購入数を算出する。
						ncount++;
						//fprintf(app_context.config->out2_toukei_bought,"inside if(i<size-1) ,i=%d, size-1=%d, ncount=%d,buyer_id=%d,seller_id=%d\n",
						//	  i,size-1,ncount, buyer_id,seller_id);
						if (i == size - 1) {
							//最後まで同一buyer、sellerの場合には最後に書きだす。
							//ncount++;
							fprintf(app_context.config->retailMarketData->out2_toukei_bought, " %d , %d , %d , %d, %d, %d,\n",
								memItem[i]->period,
								memItem[i]->item_type_id,
								memItem[i]->price,
								memItem[i]->buyer_agent_no,
								memItem[i]->seller_agent_no,
								ncount);
						}
					}
					else {
						//	fprintf(app_context.config->out2_toukei_bought,"inside else, i=%d, size-1=%d, ncount=%d,\n",i,size-1,ncount);
							//当初のbuyer、sellerでない場合には、一つ前の数とデータをを書き出す。
						fprintf(app_context.config->retailMarketData->out2_toukei_bought, " %d , %d , %d , %d, %d, %d,\n",
							memItem[i - 1]->period,
							memItem[i - 1]->item_type_id,
							memItem[i - 1]->price,
							memItem[i - 1]->buyer_agent_no,
							memItem[i - 1]->seller_agent_no,
							ncount);
						//buyer又はsellerのidが当初と異なる場合には新たにそれを定義する。
						if (memItem[i]->buyer_agent_no != buyer_id) buyer_id = memItem[i]->buyer_agent_no;
						if (memItem[i]->seller_agent_no != seller_id) seller_id = memItem[i]->seller_agent_no;
						ncount = 1;
						if (i == size - 1) {
							//ncount++;
							fprintf(app_context.config->retailMarketData->out2_toukei_bought, " %d , %d , %d , %d, %d, %d,\n",
								memItem[i]->period,
								memItem[i]->item_type_id,
								memItem[i]->price,
								memItem[i]->buyer_agent_no,
								memItem[i]->seller_agent_no,
								ncount);
						}
					}


					/*if(i==size-1) {
						//ncount++;
							fprintf(app_context.config->out2_toukei_bought," %d , %d , %d , %d, %d, %d,\n" ,
							memItem[i]->period,
							memItem[i]->item_type_id,
							memItem[i]->price,
							memItem[i]->buyer_agent_no,
							memItem[i]->seller_agent_no,
							ncount);
					}*/

					//--------------------------------------------------
					for (j = 0; j < app_context.config->item_type_id_count; j++) {
						if (memItem[i]->item_type_id == app_context.config->item_type_idk[j]) {
							//item_tpe_id毎に集計する
							sum[j] += memItem[i]->price;
							n[j]++;
						}
						else {}
					}
				}//if(memItem[i]->period==periodの終わり
				else printf("boughtRecord dataに異常があります period=%d\n", period);

			}//for(i=previous_size;i<size;i++)の終わり
			//printf("end of for i=previous_dize;i<size;i++)");

		}
	//	printf("after if (size-preivous_size>0 before for(j=0;....\n");
		for (j = 0; j < app_context.config->item_type_id_count; j++) {
		//	printf("inside for in boughrecord (j=0;...,n[%d]=%d sum[%d]=%d\n ", j, n[j],j,sum[j]);
			if (n[j] != 0) {
				price_ave[j] = int(double(sum[j]) / n[j]);
				set_AvePrice(period, price_ave[j], app_context.config->item_type_idk[j]);
				//fprintf(app_context.config->out_toukei_bought_ave,"%d , %d ,%d , %d ,\n",app_context.period_count,(j+1)*10,n[j],price_ave[j]);
				fprintf(app_context.config->retailMarketData->out_toukei_bought_ave, "%d , %d ,%d , %d ,%d,,"
					, app_context.period_count, app_context.config->item_type_idk[j], n[j], price_ave[j], get_ave_price(period, app_context.config->item_type_idk[j]));
				//printf("period_count=%d period=%d item_type_id= %d n[j]=%d , price_ave=%d ,price_ave=%d,\n", app_context.period_count, period, app_context.config->item_type_idk[j], n[j], price_ave[j], get_ave_price(period, app_context.config->item_type_idk[j]));
			}
			else {
				fprintf(app_context.config->retailMarketData->out_toukei_bought_ave, "%d , %d ,%d ,,,,", app_context.period_count, app_context.config->item_type_idk[j], n[j]);
			//	printf( "period=%d , item_tpe_idkw[%d]=%d ,n[%d]=%d ,,,,", app_context.period_count, j,app_context.config->item_type_idk[j], j,n[j]);
			}
		}
		fprintf(app_context.config->retailMarketData->out_toukei_bought_ave, "\n");
	}
		/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
		/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
		else if (market_type == 2) {
		if (size - previous_size > 0) {
			buyer_id = memItem[previous_size]->buyer_agent_no;//最初の購入製品の買主のbuyer_id
			seller_id = memItem[previous_size]->seller_agent_no;//最初の購入製品お売主のseller_id
			//printf("after defining buyer and seller\n");
			for (i = previous_size; i < size; i++) {
				if (memItem[i]->period == period) {
					//printf("inside if before fprint\n");
					//今期購入された商品を一つづつout_toukei_boughtに書きだす。
					fprintf(app_context.config->materialMarketData->out_toukei_bought, " %d , %d , %d , %d ,  %d,\n",
						memItem[i]->period,
						memItem[i]->item_type_id,
						memItem[i]->price,
						memItem[i]->buyer_agent_no,
						memItem[i]->seller_agent_no);
					//--------------------------------------20191123荻林記

					if (memItem[i]->buyer_agent_no == buyer_id && memItem[i]->seller_agent_no == seller_id) {
						//同じbuyer、sellerの製品購入数を算出する。
						ncount++;
						//fprintf(app_context.config->out2_toukei_bought,"inside if(i<size-1) ,i=%d, size-1=%d, ncount=%d,buyer_id=%d,seller_id=%d\n",
						//	  i,size-1,ncount, buyer_id,seller_id);
						if (i == size - 1) {
							//最後まで同一buyer、sellerの場合には最後に書きだす。
							//ncount++;
							fprintf(app_context.config->materialMarketData->out2_toukei_bought, " %d , %d , %d , %d, %d, %d,\n",
								memItem[i]->period,
								memItem[i]->item_type_id,
								memItem[i]->price,
								memItem[i]->buyer_agent_no,
								memItem[i]->seller_agent_no,
								ncount);
						}
					}
					else {
						//	fprintf(app_context.config->out2_toukei_bought,"inside else, i=%d, size-1=%d, ncount=%d,\n",i,size-1,ncount);
							//当初のbuyer、sellerでない場合には、一つ前の数とデータをを書き出す。
						fprintf(app_context.config->materialMarketData->out2_toukei_bought, " %d , %d , %d , %d, %d, %d,\n",
							memItem[i - 1]->period,
							memItem[i - 1]->item_type_id,
							memItem[i - 1]->price,
							memItem[i - 1]->buyer_agent_no,
							memItem[i - 1]->seller_agent_no,
							ncount);
						//buyer又はsellerのidが当初と異なる場合には新たにそれを定義する。
						if (memItem[i]->buyer_agent_no != buyer_id) buyer_id = memItem[i]->buyer_agent_no;
						if (memItem[i]->seller_agent_no != seller_id) seller_id = memItem[i]->seller_agent_no;
						ncount = 1;
						if (i == size - 1) {
							//ncount++;
							fprintf(app_context.config->materialMarketData->out2_toukei_bought, " %d , %d , %d , %d, %d, %d,\n",
								memItem[i]->period,
								memItem[i]->item_type_id,
								memItem[i]->price,
								memItem[i]->buyer_agent_no,
								memItem[i]->seller_agent_no,
								ncount);
						}
					}


					/*if(i==size-1) {
						//ncount++;
							fprintf(app_context.config->out2_toukei_bought," %d , %d , %d , %d, %d, %d,\n" ,
							memItem[i]->period,
							memItem[i]->item_type_id,
							memItem[i]->price,
							memItem[i]->buyer_agent_no,
							memItem[i]->seller_agent_no,
							ncount);
					}*/

					//--------------------------------------------------
					for (j = 0; j < app_context.config->item_type_id_count_w; j++) {
						if (memItem[i]->item_type_id == app_context.config->item_type_idkw[j]) {
							//item_tpe_id毎に集計する
							sum[j] += memItem[i]->price;
							n[j]++;
						}
						else {}
					}
				}//if(memItem[i]->period==periodの終わり
				else printf("boughtRecord dataに異常があります period=%d\n", period);

			}//for(i=previous_size;i<size;i++)の終わり

		}
		for (j = 0; j < app_context.config->item_type_id_count_w; j++) {
			//fprintf(app_context.config->out_temporary, "n[%d]=%d\n",j, n[j]);
			if (n[j] != 0) {
				price_ave[j] = int(double(sum[j]) / n[j]);
				set_AvePrice(period, price_ave[j], app_context.config->item_type_idkw[j]);
				//fprintf(app_context.config->out_toukei_bought_ave,"%d , %d ,%d , %d ,\n",app_context.period_count,(j+1)*10,n[j],price_ave[j]);
				fprintf(app_context.config->materialMarketData->out_toukei_bought_ave, "%d , %d ,%d , %d ,%d,,", 
					app_context.period_count, app_context.config->item_type_idkw[j], n[j], price_ave[j], get_ave_price(period, app_context.config->item_type_idkw[j]));
				//printf("period_count=%d period=%d item_type_id= %d n[j]=%d , price_ave=%d ,price_ave=%d,\n", app_context.period_count, period, app_context.config->item_type_idkw[j], n[j], price_ave[j], get_ave_price(period, app_context.config->item_type_idkw[j]));
			}
			else fprintf(app_context.config->materialMarketData->out_toukei_bought_ave, "%d , %d ,%d ,,,,", app_context.period_count, app_context.config->item_type_idkw[j], n[j]);

		}
		fprintf(app_context.config->materialMarketData->out_toukei_bought_ave, "\n");
		}
		/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
	}//関数の終わり
	void print_boughtitems(int agent_id, int period){
		//生産者を指定して期に購入された製品情報をプリントアウトする
			int i = 0;
		int size = 0;
		size=memItem.size();
//		fprintf(app_context.config->out,"[boughtRecord] ----all items----買われた製品の集合 %d 期 period = %d agent_id=%d \n",app_context.period_count,period,agent_id);
		if(size==0) fprintf(app_context.config->out,"買われた製品集合は空です\n");
//		fprintf(app_context.config->out," period  item_type_id  price  buyer_id  seller_id\n");
		if(size>0){
			for(i=previous_size;i<size;i++){
				if(memItem[i]->period==period && memItem[i]->seller_agent_no == agent_id){
				fprintf(app_context.config->out," %d  %d  %d  %d   %d\n",
				memItem[i]->period,
				memItem[i]->item_type_id,
				memItem[i]->price,
				memItem[i]->buyer_agent_no,
				memItem[i]->seller_agent_no);
				}
			}
		}
	}
	int number_of_bought_items(int agent_id,int period,int item_type_id){
		//生産者および製品種を指定して、その期に購入された製品の個数を返す
		int i = 0, size = 0;
		size=memItem.size();
		int sum=0;
		//printf("agent_id=%d period=%d item_type_id=%d size=%d\n",agent_id,period,item_type_id,size);
		fprintf(app_context.config->out,"agent_id=%d period=%d item_type_id=%d\n",agent_id,period,item_type_id);
		if(size>0){
			for(i=0;i<size;i++){
				if(memItem[i]->period==period){
					if(memItem[i]->seller_agent_no==agent_id && memItem[i]->item_type_id==item_type_id){
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