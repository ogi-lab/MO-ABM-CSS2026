#include "SimulatorConfig.h"
#include "SimulatorContext.h"
//#include "Item.h"
//class Item;
using namespace std;
extern SimulatorContext app_context;
struct AreaMarketItem{
	int buyer_agent_no = 0;
	int seller_agent_no = 0;
	int item_type_id = 0;
	int price = 0;
	int period = 0;
};
class AreaMarketRecord{

	vector <AreaMarketItem*> memItem;
		int *price_ave;
		int *sum;
		int *n;
		int previous_size = 0;
		int dataAnalyzer_type = 0;
		int market_type = 0;
public:
	void set_dataAnalyzer_type(int dataAnalyzer_type) { this->dataAnalyzer_type = dataAnalyzer_type; }
	int get_dataAnalyzer_type() { return dataAnalyzer_type; }
	void set_market_type(int market_type) { this->market_type = market_type; }
	int get_market_type() { return market_type; }
	AreaMarketRecord(){
	//	boughtItem=new BoughtItem;//コンストラクタ内でnewするのはダメ
	}
	~AreaMarketRecord(){
		delete [] n;
		delete [] price_ave;
		delete [] sum;
	};
	
	void set_up(){

		
		//printf("inside set_up before if\n");
		if (market_type == 1) {
			//printf("market_type=%d\n", market_type);
			//areamarketにある製品の平均価格、配列だけset_upで製品種数分newしておく
			price_ave = new int[app_context.config->item_type_id_count];
			sum = new int[app_context.config->item_type_id_count];
			//期に購入された製品の個数
			n = new int[app_context.config->item_type_id_count];
			fprintf(app_context.config->retailMarketData->out_toukei_areamarket_ave, "period,item_type_id,number of items,average price,\n");
			fprintf(app_context.config->retailMarketData->out_toukei_areamarket, "期末在庫商品リスト,\nperiod,item_type_id,price,seller_id,\n ");
		}
		else if (market_type == 2) {
			printf("market_type=%d\n", market_type);
			//期末にareamarketにある製品の平均価格、配列だけ期首にnewしておく
			price_ave = new int[app_context.config->item_type_id_count_w];
			sum = new int[app_context.config->item_type_id_count_w];
			//期に購入された製品の個数
			n = new int[app_context.config->item_type_id_count_w];
			fprintf(app_context.config->materialMarketData->out_toukei_areamarket_ave, "period,item_type_id,number of items,average price,\n");
			fprintf(app_context.config->materialMarketData->out_toukei_areamarket, "期末在庫商品リスト,\nperiod,item_type_id,price,seller_id,\n ");
		}
		else {}
	}
	void do_kishu() {
		previous_size = memItem.size();
	}
	void print_memItem(int i){
		printf("memItem[%d]->price=%d\n",i,memItem[i]->price);
	}
	AreaMarketItem* get_item(int i){
		return memItem[i];
	}
	void set_areamarketItem(struct AreaMarketItem* areamarketItem){
		//this->boughtItem=boughtItem;
		memItem.push_back(areamarketItem);
	}
	int get_sizeofmemItem(){
		return memItem.size();
	}
	void print_areamarketitems(int period) {
		//期末在庫市場商品情報をプリントする、item_type_id毎、seller毎の商品価格をプリントアウトする　　　20210807荻林記//
		int i; int j;
		int size = 0;
		size = memItem.size();
		fprintf(app_context.config->out_temporary, "inside printa_areamarket size=%d\n", size);
		//fprintf(app_context.config->out,"[areamarketRecord] ----all items----買われた製品の集合 %d 期 \n",app_context.period_count);
		//fprintf(app_context.config->out," period  item_type_id  price  buyer_id  seller_id\n");

		/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
		if (market_type == 1) {
			for (i = 0; i < app_context.config->item_type_id_count; i++) {
				sum[i] = 0; price_ave[i] = 0; n[i] = 0;
			}
		//	if (size > previous_size) {
				for (i = 0; i < size; i++) {
					//printf("inside for before if i=%d period=%d\n",i,period);
					if (memItem[i]->period == period) {
						//printf("inside if before fprint\n");
						fprintf(app_context.config->retailMarketData->out_toukei_areamarket, " %d , %d , %d , %d , \n",
							memItem[i]->period,
							memItem[i]->item_type_id,
							memItem[i]->price,
							//memItem[i]->buyer_agent_no,
							memItem[i]->seller_agent_no);
						for (j = 0; j < app_context.config->item_type_id_count; j++) {
							if (memItem[i]->item_type_id == app_context.config->item_type_idk[j]) {
								sum[j] += memItem[i]->price;
								n[j]++;
							}
							else {}
						}//for(j=0....の終わり
					}//if(memItem[i]->period.....の終わり)
				}//for(i=0;.....の終わり
				for (j = 0; j < app_context.config->item_type_id_count; j++) {

					if (size > 0 && n[j] != 0) {
						price_ave[j] = int(double(sum[j]) / n[j]);
						fprintf(app_context.config->retailMarketData->out_toukei_areamarket_ave, "%d , %d ,%d , %d ,,", app_context.period_count, app_context.config->item_type_idk[j], n[j], price_ave[j]);
					}
					else fprintf(app_context.config->retailMarketData->out_toukei_areamarket_ave, "%d , %d ,%d ,,", app_context.period_count, app_context.config->item_type_idk[j], n[j]);

				}//for(j=0.....の終わり
				fprintf(app_context.config->retailMarketData->out_toukei_areamarket_ave, "\n");
			}//if(market_type==....の終わり
			/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
			/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
			else if (market_type == 2) {
				for (i = 0; i < app_context.config->item_type_id_count_w; i++) {
					sum[i] = 0; price_ave[i] = 0; n[i] = 0;
				}
				for (i = 0; i < size; i++) {
					//printf("inside for before if i=%d period=%d\n",i,period);
					if (memItem[i]->period == period) {
						//printf("inside if before fprint\n");
						fprintf(app_context.config->materialMarketData->out_toukei_areamarket, " %d , %d , %d , %d , \n",
							memItem[i]->period,
							memItem[i]->item_type_id,
							memItem[i]->price,
							//memItem[i]->buyer_agent_no,
							memItem[i]->seller_agent_no);
						for (j = 0; j < app_context.config->item_type_id_count_w; j++) {
							if (memItem[i]->item_type_id == app_context.config->item_type_idkw[j]) {
								sum[j] += memItem[i]->price;
								n[j]++;
							}
							else {}
						}
					}
				}//for(i=0....の終わり
				for (j = 0; j < app_context.config->item_type_id_count_w; j++) {
					//printf("inside for (j = j = % d\n", j);
					if (size > 0 && n[j] != 0) {
						price_ave[j] = int(double(sum[j]) / n[j]);
						fprintf(app_context.config->materialMarketData->out_toukei_areamarket_ave, "%d , %d ,%d , %d ,,", app_context.period_count, app_context.config->item_type_idkw[j], n[j], price_ave[j]);
					}
					else fprintf(app_context.config->materialMarketData->out_toukei_areamarket_ave, "%d , %d ,%d ,,,", app_context.period_count, app_context.config->item_type_idkw[j], n[j]);

				}//for(j=0....の終わり
				fprintf(app_context.config->materialMarketData->out_toukei_areamarket_ave, "\n");
			}//else ifの終わり
			else {}
			/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
		}
		void print_areamarketitems(int agent_id, int period) {
			//生産者を指定して期に購入された製品情報をプリントアウトする
			int i = 0;
			int size = 0;
			size = memItem.size();
			//	fprintf(app_context.config->out,"[areamarketRecord] ----all items----市場の期末製品の集合 %d 期 period = %d agent_id=%d \n",app_context.period_count,period,agent_id);
			//	if(size==0) fprintf(app_context.config->out,"市場の期末製品集合は空です\n");
			//	fprintf(app_context.config->out," period  item_type_id  price  buyer_id  seller_id\n");
			if (size > 0) {
				for (i = 0; i < size; i++) {
					if (memItem[i]->period == period && memItem[i]->seller_agent_no == agent_id) {
						fprintf(app_context.config->out, " %d  %d  %d  %d   %d\n",
							memItem[i]->period,
							memItem[i]->item_type_id,
							memItem[i]->price,
							memItem[i]->buyer_agent_no,
							memItem[i]->seller_agent_no);
					}
				}
			}
		}
		int number_of_areamarket_items(int agent_id, int period, int item_type_id) {
			//生産者および製品種を指定して、期末の市場製品の個数を返す
			int i = 0, size = 0;
			size = memItem.size();
			int sum = 0;
			//printf("agent_id=%d period=%d item_type_id=%d size=%d\n",agent_id,period,item_type_id,size);
		//	fprintf(app_context.config->out,"agent_id=%d period=%d item_type_id=%d\n",agent_id,period,item_type_id);
			if (size > 0) {
				for (i = 0; i < size; i++) {
					if (memItem[i]->period == period) {
						if (memItem[i]->seller_agent_no == agent_id && memItem[i]->item_type_id == item_type_id) {
							sum++;
						}

					}
				}//for(i=0...の終わり
				return sum;
			}//if(size>0....の終わり
			else {
				return 0;
			}
		}
};