#pragma once
#include <vector>
#include "SimulatorConfig.h"
#include "SimulatorContext.h"
//#include "Item.h"
//class Item;
using namespace std;
extern SimulatorContext app_context;
struct StockItem{
	int buyer_agent_no = 0;
	int seller_agent_no = 0;
	int item_type_id = 0;
	int price = 0;
	int period = 0;
};
class StockRecord{

	vector <StockItem*> memItem;

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
	
	StockRecord(){
	//	boughtItem=new BoughtItem;//コンストラクタ内でnewするのはダメ
	}
	~StockRecord(){
		delete [] n;
		delete [] price_ave;
		delete [] sum;
	}
	void set_up(){
		//在庫製品の平均価格
		price_ave = new int[app_context.config->item_type_id_count];
		sum = new int[app_context.config->item_type_id_count];
		//在庫製品の個数
		n = new int[app_context.config->item_type_id_count];
		if (market_type == 1) {
			fprintf(app_context.config->retailMarketData->out_toukei_stock_ave, "period,item_type_id,number of stock items,average price,\n");
			fprintf(app_context.config->retailMarketData->out_toukei_stock, "period,item_type_id,price,seller_id,\n");
			fprintf(app_context.config->retailMarketData->out2_toukei_stock, "period,item_type_id,price,seller_id,ncount_stock,\n");
		}
		else if (market_type == 2) {
			fprintf(app_context.config->materialMarketData->out_toukei_stock_ave, "period,item_type_id,number of stock items,average price,\n");
			fprintf(app_context.config->materialMarketData->out_toukei_stock, "period,item_type_id,price,seller_id,\n");
			fprintf(app_context.config->materialMarketData->out2_toukei_stock, "period,item_type_id,price,seller_id,ncount_stock,\n");
		}
		else {}
	}
	void do_kishu(){
		previous_size=memItem.size();
	}
	void set_stockItem(struct StockItem* stockItem){
		//printf("period=%d buyer_id=%d seller_id=%d item_type_id=%d\n",app_context.period_count,stockItem->buyer_agent_no,stockItem->seller_agent_no,stockItem->item_type_id);
		memItem.push_back(stockItem);
	}
	int get_sizeofmemItem(){
		return memItem.size();
	}
	void print_stockitems(int period){
		//期に購入された製品情報（個々の製品およびitem_type_id毎の平均価格をプリントアウトする
		int i;int j;
		int size = 0;
		int ncount = 0;
		int seller_id = 0;
		size=memItem.size();
		//printf("inside stockRecord period=%d market_type=%d size=%d previouus_size=%d\n", period, get_market_type(), size, previous_size);

		for(i=0;i<app_context.config->item_type_id_count;i++){
			sum[i]=0;price_ave[i]=0;n[i]=0;
		}
	//	fprintf(app_context.config->out,"[stockRecord] ----all items----在庫製品の集合 %d 期 \n",app_context.period_count);
	//	fprintf(app_context.config->out," period  item_type_id  price  buyer_id  seller_id\n");
		//fprintf(app_context.config->out_temporary, "inside print_stockitems  period=%d market_type=%d ",app_context.period_count,get_market_type());
	//	printf("inside print_stockitems  period=%d market_type=%d \n", app_context.period_count, market_type);
		ncount=0;
		//printf("before memItem[previous_size] previous_size	=%d size of memItem=%d \n",previous_size, memItem.size());
		/**::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
		//printf("before if( get_market_type()==1)\n ");
		if (get_market_type() == 1) {
		//	printf("inside market_type=1 market_type=%d\n",get_market_type());
			if (size > 0 && size > previous_size) {
				seller_id = memItem[previous_size]->seller_agent_no;
			//	printf("before for (i = ) in if(get_market_type()==1),size=%d, previous_size=%d seller_id=%d\n",size, previous_size,seller_id);
				for (i = previous_size; i < size; i++) {
					/*
					printf("inside for (i= previous_size) in StockRecord.print_stockItems(period) i=%d size=%d seller_id=%d period=%d memItem[i]->period=%d memItem[i]->item_type_id=%d,seller_agent_no=%d,\n",i,size,seller_id,period,
						memItem[i]->period,memItem[i]->item_type_id,memItem[i]->seller_agent_no);
					*/				
				if (memItem[i]->period == period) {
						fprintf(app_context.config->retailMarketData->out_toukei_stock, " %d , %d , %d , %d ,\n",
							memItem[i]->period,
							memItem[i]->item_type_id,
							memItem[i]->price,
							memItem[i]->seller_agent_no);
						//--------------------------------------20191123荻林記

						if (memItem[i]->seller_agent_no == seller_id) {
							ncount++;
							//fprintf(app_context.config->out2_toukei_stock,"inside if(i<size-1) ,i=%d, size-1=%d, ncount=%d,buyer_id=%d,seller_id=%d\n",
							//	  i,size-1,ncount, buyer_id,seller_id
							if (i == size - 1) {
								//ncount++;
								fprintf(app_context.config->retailMarketData->out2_toukei_stock, " %d , %d , %d , %d, %d, \n",
									memItem[i]->period,
									memItem[i]->item_type_id,
									memItem[i]->price,
									memItem[i]->seller_agent_no,
									ncount);
							}
						}
						else {
							//	fprintf(app_context.config->out2_toukei_stock,"inside else, i=%d, size-1=%d, ncount=%d,\n",i,size-1,ncount);
							fprintf(app_context.config->retailMarketData->out2_toukei_stock, " %d , %d , %d , %d, %d, \n",
								memItem[i - 1]->period,
								memItem[i - 1]->item_type_id,
								memItem[i - 1]->price,
								memItem[i - 1]->seller_agent_no,
								ncount);
							if (memItem[i]->seller_agent_no != seller_id) seller_id = memItem[i]->seller_agent_no;
							ncount = 1;
							if (i == size - 1) {
								//ncount++;
								fprintf(app_context.config->retailMarketData->out2_toukei_stock, " %d , %d , %d , %d, %d, \n",
									memItem[i]->period,
									memItem[i]->item_type_id,
									memItem[i]->price,
									memItem[i]->seller_agent_no,
									ncount);
							}
						}

						/*if(i==size-1) {
							//ncount++;
								fprintf(app_context.config->out2_toukei_stock," %d , %d , %d , %d, %d, %d,\n" ,
								memItem[i]->period,
								memItem[i]->item_type_id,
								memItem[i]->price,
								memItem[i]->seller_agent_no,
								ncount);
						}*/
						//--------------------------------------------------
						for (j = 0; j < app_context.config->item_type_id_count; j++) {
							if (memItem[i]->item_type_id == app_context.config->item_type_idk[j]) {
								sum[j] += memItem[i]->price;
								n[j]++;
							}
							else {}
						}
					}
					else printf("stockRecord dataに異常があります period=%d market_type=%d size=%d previouus_size=%d\n", period, get_market_type(), size, previous_size);
				}//for(i=0...no owari 
				//printf("after  for seller_id=%d\n",seller_id);
			}	//if(size>0)の終わり
			for (j = 0; j < app_context.config->item_type_id_count; j++) {
				//printf("inside for(j=0, j=%d, item_type_id_count=%d \n", j, app_context.config->item_type_id_count);
					if(n[j]!=0) {
					//	printf("n[% d] = % d sum[%d]=%d\n", j, n[j],j,sum[j]);
						price_ave[j] = int(double(sum[j]) / n[j]);
						fprintf(app_context.config->retailMarketData->out_toukei_stock_ave, "%d , %d ,%d , %d ,,", app_context.period_count, app_context.config->item_type_idk[j], n[j], price_ave[j]);
					}
					else {
						//printf("n[%d]=%d\n", j, n[j]);
						fprintf(app_context.config->retailMarketData->out_toukei_stock_ave, "%d , %d ,%d ,,,,", app_context.period_count, app_context.config->item_type_idk[j], n[j]);
					}
			}
			fprintf(app_context.config->retailMarketData->out_toukei_stock_ave, "\n");
			//printf("before else if (get_market_type()==2)\n ");
		}
			/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
		/**::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
	
		else if (get_market_type() == 2) {
			//printf("inside market_type=2 market_type=%d\n\n", get_market_type());
			fprintf(app_context.config->out_temporary, "inside stock record  period=%d market_type=%d size=%d previous_size=%d\n",app_context.period_count,market_type,size,previous_size);
			//printf("inside stock record  period=%d market_type=%d size=%d previous_size=%d\n", app_context.period_count, market_type, size, previous_size);

			if (size > 0 && size > previous_size) {
				seller_id = memItem[previous_size]->seller_agent_no;
				for (i = previous_size; i < size; i++) {
					if (memItem[i]->period == period) {
						fprintf(app_context.config->materialMarketData->out_toukei_stock, " %d , %d , %d , %d ,\n",
							memItem[i]->period,
							memItem[i]->item_type_id,
							memItem[i]->price,
							memItem[i]->seller_agent_no);
						//--------------------------------------20191123荻林記

						if (memItem[i]->seller_agent_no == seller_id) {
							ncount++;
							//fprintf(app_context.config->out2_toukei_stock,"inside if(i<size-1) ,i=%d, size-1=%d, ncount=%d,buyer_id=%d,seller_id=%d\n",
							//	  i,size-1,ncount, buyer_id,seller_id
							if (i == size - 1) {
								//ncount++;
								fprintf(app_context.config->materialMarketData->out2_toukei_stock, " %d , %d , %d , %d, %d, \n",
									memItem[i]->period,
									memItem[i]->item_type_id,
									memItem[i]->price,
									memItem[i]->seller_agent_no,
									ncount);
							}
						}
						else {
							//	fprintf(app_context.config->out2_toukei_stock,"inside else, i=%d, size-1=%d, ncount=%d,\n",i,size-1,ncount);
							fprintf(app_context.config->materialMarketData->out2_toukei_stock, " %d , %d , %d , %d, %d, \n",
								memItem[i - 1]->period,
								memItem[i - 1]->item_type_id,
								memItem[i - 1]->price,
								memItem[i - 1]->seller_agent_no,
								ncount);
							if (memItem[i]->seller_agent_no != seller_id) seller_id = memItem[i]->seller_agent_no;
							ncount = 1;
							if (i == size - 1) {
								//ncount++;
								fprintf(app_context.config->materialMarketData->out2_toukei_stock, " %d , %d , %d , %d, %d, \n",
									memItem[i]->period,
									memItem[i]->item_type_id,
									memItem[i]->price,
									memItem[i]->seller_agent_no,
									ncount);
							}
						}

						/*if(i==size-1) {
							//ncount++;
								fprintf(app_context.config->out2_toukei_stock," %d , %d , %d , %d, %d, %d,\n" ,
								memItem[i]->period,
								memItem[i]->item_type_id,
								memItem[i]->price,
								memItem[i]->seller_agent_no,
								ncount);
						}*/
						//--------------------------------------------------
						for (j = 0; j < app_context.config->item_type_id_count_w; j++) {
							if (memItem[i]->item_type_id == app_context.config->item_type_idkw[j]) {
								sum[j] += memItem[i]->price;
								n[j]++;
							}
							else {}
						}
					}
					else printf("stockRecord dataに異常があります period=%d market_type=%d size=%d previouus_size=%d\n", period,get_market_type(),size,previous_size);
				}
			}	//if(size>0)の終わり
			for (j = 0; j < app_context.config->item_type_id_count_w; j++) {
				//if (size > 0) {
					//if (n[j] == 0) fprintf(app_context.config->materialMarketData->out_toukei_stock_ave, "%d , %d ,%d , ,,", app_context.period_count, app_context.config->item_type_idk[j], n[j]);
					if(n[j]!=0)  {
						price_ave[j] = int(double(sum[j]) / n[j]);
						fprintf(app_context.config->materialMarketData->out_toukei_stock_ave, "%d , %d ,%d , %d ,,", app_context.period_count, app_context.config->item_type_idkw[j], n[j], price_ave[j]);
					}
					else fprintf(app_context.config->materialMarketData->out_toukei_stock_ave, "%d , %d ,%d ,,,,", app_context.period_count, app_context.config->item_type_idkw[j], n[j]);

		//		}
				//else if (size == 0) fprintf(app_context.config->materialMarketData->out_toukei_stock_ave, "%d , %d ,%d , ,,", app_context.period_count, app_context.config->item_type_idk[j], n[j]);
			}
			fprintf(app_context.config->materialMarketData->out_toukei_stock_ave, "\n");
			printf("after else if (get_market_type()==2)\n ");
		}
		else {}
		/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
	}
	void print_stockitems(int agent_id, int period){
		//生産者を指定して在庫製品情報をプリントアウトする
			int i = 0;
		int size = 0;
		size=memItem.size();
		//fprintf(app_context.config->out,"[stockRecord] ----all items----在庫製品の集合 %d 期 period = %d agent_id=%d \n",app_context.period_count,period,agent_id);
		//if(size==0) fprintf(app_context.config->out,"在庫製品集合は空です\n");
		//fprintf(app_context.config->out," period  item_type_id  price  buyer_id  seller_id\n");
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
	int number_of_stock_items(int agent_id,int period,int item_type_id){
		//生産者および製品種を指定して、在庫製品の個数を返す
		int i = 0, size = 0;
		size=memItem.size();
		int sum=0;
		//fprintf(app_context.config->out,"agent_id=%d period=%d item_type_id=%d\n",agent_id,period,item_type_id);
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