
#pragma once
#include <stdio.h>
#include <map>
#include <set>
#include <vector>
#include "Enterprise.h"
#include "RandomGenerator.h"
#include "Seller.h"
#include "SimulatorApp.h"
#include "SimulatorContext.h"
#include "Shipment.h"
#include "ShipmentNavigator.h"
#include "SimulatorConfig.h"
#include "RetailMarket.h"
#include "RetailMarketNavigator.h"
#include "DataAnalyzer.h"
#include "DataAnalyzerNavigator.h"
#include "ConsumerAggregatorNavigator.h"
#include "toukei.h"
#include <math.h>
#include "Item.h"
#include "PredForItemBySeq.h"
#include "AccountData.h"

using namespace std;
extern SimulatorContext app_context;

// 生産者区分
const int PRODUCER_TYPE_RETAILER = 1;
const int PRODUCER_TYPE_WHOLESELLER_NO_MATERIALS = 2;
const int PRODUCER_TYPE_WHOLESELLER_NORMAL = 3;
const int PRODUCER_TYPE_EQUIPMENT_MAKER = 4;

struct SoldItem{
	int item_type_id = 0;
	long long price = 0;
	int period = 0;
};
struct SbsHistory{
	//期末の供給個数、売却個数、在庫個数、価格の記録データ
	int period = 0;
	int item_type_id = 0;
	int number_supplied = 0;
	int number_bought = 0;
	int number_stocks = 0;
	long long price = 0;
};
/** 生産者 */
class Producer : public Enterprise, public Seller, public Buyer {
	int producer_type = 0;
public:
	int Get_agent_id() { return agent_id; };
	int Get_AgentType() { return agent_type; }
private:
	struct SoldItem* soldItem;//period, price, item_type_idをセットにした売却情報
	vector <SoldItem*> memSold;
	struct SbsHistory* sbsHistory;
	vector <SbsHistory*> memHistory;
	ShipmentNavigator* shipment_navigator;
	/** 在庫(key:商品種ID, value:商品集合) */
	map<int, set<Item*, PredForItemBySeq>*> stock_map;

	/**
	 * 指定した商品種IDに対応する在庫集合を返す
	 *
	 * @param item_type_id 商品種ID
	 * @return 在庫集合
	 */
	set<Item*, PredForItemBySeq>* get_item_set(int item_type_id) {

		map<int, set<Item*, PredForItemBySeq>*>::iterator it = stock_map.find(item_type_id);

		// setがmap内になければ作成して登録して返す
		if (it == stock_map.end()) {
			set<Item*, PredForItemBySeq>* new_item_set = new set<Item*, PredForItemBySeq>();
			stock_map.insert(pair<int, set<Item*, PredForItemBySeq>*>(item_type_id, new_item_set));
			return new_item_set;
		}
		return it->second;
	}

	DataAnalyzer* get_dataAnalyzer() {
		DataAnalyzerNavigator* data_navi;
		//2026/10/1 ogibayashi: item_type_idkはretailer+soleproprietorの製品群(retailMarket)。
		//soleproprietor(2130)もretailer(2131)と同じくretailMarket側のDataAnalyzerを見るべき(2132=wholesalerのみmaterialMarket)。
		if (Get_AgentType() == 2131 || Get_AgentType() == 2130) {
			data_navi = app_context.dataAnalyzer_navi_for_retailMarket;
			//fprintf(app_context.config->out_temporary, "inside producer get_dataAnalyzer retailMarketが選択されました\n");
		}
		else {
			data_navi = app_context.dataAnalyzer_navi_for_materialMarket;
			//fprintf(app_context.config->out_temporary, "inside producer get_dataAnalyzer retailMarketが選択されました\n");
		}
		return data_navi->get_dataAnalyzer();
		//DataAnalyzer* dataAnalyzer=data_navi->get_dataAnalyzer();
		//return dataAnalyzer;
	}
	Toukei toukei;
	/**原料購入*/

protected:

	int qmax = 0;//production capacity（個数
	int qmin = 0;//値はretailerのsetupで設定
	long long min_price = 0;//production genka
	//vector<long long >ave_price_list; 全てのitem_type_idのave_priceを持たせようとしたがやめた20240507
	long long ave_price_in_market;//period=1以降の各期首に自分が製造するitem_type_idについてデータを格納しておく。
	int size_of_stock_in_market = 0;
	int q_count = 0;
	double qcount_round_carry = 0.0;//丸め誤差を次期に持ち越すための状態変数(2026/10/1 ogibayashi)
	void set_q_count(int q_count) { this->q_count = q_count; }
	int get_q_count() { return q_count; }
	int new_qcount = 0;
	void set_new_qcount(int new_qcount) { this->new_qcount = new_qcount; }
	int get_new_qcount() { return new_qcount; }
	void set_qmax(int qmax) { this->qmax = qmax; }
	int get_qmax() { return qmax; }
	void set_qmin(int qmin) { this->qmin = qmin; }
	int get_qmin() { return qmin; }
	int price_up_flag = 0;
	int price_down_flag = 0;
	long long price = 0;
	int item_type_id = 0;//生産する製品種(生産者毎に一種類、固有）
	int get_item_type_id() { return item_type_id; }
	int item_type_id_to_buy = 0;//生産に必要な原料種　retailerはsetupでランダムに決定する	
	int get_item_type_id_to_buy() { return item_type_id_to_buy; }
	//int item_type_idk;//materialMarketの製品種（生産者wholesaler, 購入者retailer)
	long long direct_labor_cost = 0;
	long long direct_material_fee = 0;
	long long genka_per_item = 0;
	int size_shiped = 0;
	long long get_genka_per_item() { return genka_per_item; }
	int get_size_shiped() { return size_shiped; }
	void define_direct_labor_cost() {}
	void define_direct_material_fee() {}
	//virtual void accounting_treatment_at_production(vector<Item*> items) {}
	virtual long long get_direct_material_fee() { return 0; }
	void set_direct_material_fee(long long direct_materila_fee) {
		//retailerはvoid buy_materials()でこの関数を呼び出す。
		this->direct_material_fee = direct_material_fee;
	}
	//int get_direct_magterial_fees() { return direct_material_fees; }
	double producer_demand_delta = 0.0;//需要基準量決定ルールにおけるsigmaの係数2012_6_3荻林
	//retailer他各種producerのset_up()でパラメータ読み込みで定義20191123荻林		
   //但し現状20240303現在はパラメータ値に関係なく1.65としている
	double min_qcount_ratio = 0.0;//retailer,wholesalerのsetupで定義
	double producer_price_alpha = 0.0;//価格改定時の価格上下降率2012_6_3荻林
	//retailer他各種producerのset_up()でパラメータ値読み込みで定義20191123荻林
	double price_delta = 0.0;
	virtual void define_price_delta() = 0;//各種producerで定義、set_up()で呼び出す
	long long fixed_cost = 0;//item_type_id毎の固定費（item_type_idに関わらず一定、乱数で決定2012_6_3荻林）
	long long variable_cost = 0;//製品1個毎の変動費2012_6_3荻林追加
	vector <int >employee_list;

	long long get_price() { return price; }
	int  define_and_get_item_type_id_to_produce() {
		if (get_agent_type() == 2132) {
			int ijk = RandomGenerator::random_int(1, app_context.config->item_type_idkw.size());
			item_type_id = app_context.config->item_type_idkw[ijk - 1];
		}
		else {
			if (app_context.config->areamarketParameter->case_monopolistic == 0) {
				//競争小売市場
				int ijk = RandomGenerator::random_int(1, app_context.config->item_type_idk.size());
				item_type_id = app_context.config->item_type_idk[ijk - 1];
			}
			else {
				//独占小売市場
				if (get_agent_type() == 2131) {
					//retailerの場合
					int ijk = RandomGenerator::random_int(1, app_context.config->item_type_idkr.size());
					item_type_id = app_context.config->item_type_idkr[ijk - 1];
		
				}
				else if (get_agent_type() == 2130) {
					int ijk = RandomGenerator::random_int(1, app_context.config->item_type_idks.size());
					item_type_id = app_context.config->item_type_idks[ijk - 1];
				}
				else printf("該当する生産者タイプがありませんでした\n");
			} //中elseの終わり
		}//外else の終わり
		return item_type_id;
	}//関数の終わり


	//　給与支払
	  //最下層オブジェクトへ移管
	void print_min_price() {
		//fprintf(app_context.config->out_min_price, "period=,%d, agent_id=,%d, agent_type=,%d,min_price=,%lld,inside function,\n", app_context.period_count, get_agent_id(), get_agent_type(),min_price);
	}
	void other_actions() {
		//fprintf(app_context.config->out_min_price, "period=,%d, agent_id=,%d, agent_type=,%d,min_price=,%lld,inside other_actins(),\n", app_context.period_count, get_agent_id(), get_agent_type(),min_price);

		//給与支払以外の中期行動
		//printf("before plan_produce()\n");
	//	if (agent_id == 509) fprintf(app_context.config->out509, "period=,%d,q_count=,%d, before plan_produce,\n", app_context.period_count, q_count);
		plan_produce(); //生産計画　process_producing()の中から外に出す　20210905記 
		//printf("aftr plan_produce()\n");
						//fprintf(app_context.config->out_temporary, "before buy_material period=%d agent_id=%d\n ",app_context.period_count,get_agent_id());
	//	if (agent_id == 509) fprintf(app_context.config->out509, "period=,%d,q_count=,%d, after plan_produce,\n", app_context.period_count, q_count);
		if(app_context.config->ncase!=0) buy_materials(); //private関数としてvirtual に宣言20210905記　荻林
	//	if (agent_id == 509) fprintf(app_context.config->out509, "period=,%d,q_count=,%d, after buy_materialse,\n", app_context.period_count, q_count);
		process_producing();	//生産行動　自関　  20120523記　高島
	//	if (agent_id == 509) fprintf(app_context.config->out509, "period=,%d,q_count=,%d, after process_producing,\n", app_context.period_count, q_count);
		process_payment();						//給与以外の支払い　但し現在は未実装　  20120524記　高島　20210910荻林記
		//printf("before expanding scale period_count=%d\n",app_context.period_count);
		process_expanding_scale();				//設備投資　　但し現在は未実装	  20120524記　高島 20210910荻林記
	//	printf("end of otehr actions()\n");
	}
	long long get_direct_labor_cost() {

		//hr->define_employee_wage_list(total_sales);
		//employee_wage_list = hr->get_employee_wage_list();
		long long direct_labor_cost = 0;
		for (unsigned int i = 0; i < employee_wage_list.size(); i++) {
			direct_labor_cost += employee_wage_list[i];
		}
		return direct_labor_cost;
	}
	/*int get_direct_material_fee() {
		if (get_agent_type() == 2132) direct_material_fee=0;
		return  direct_material_fee;
	}*/
	void accounting_treatment_at_production(vector<Item*> items) {
		//producrerのcreate_products()で呼ばれる
		int size = items.size();//生産個数
		//原価計算
		//直接材料費＝０
		//直接労務費
		long long direct_labor_cost_per_item = 0;
		long long direct_material_fee_per_item = 0;
		long long total_sales = 0;
		long long genka = 0;
		long long imported_inventory = 0;//輸入品の在庫評価額
		long long imported_inventory_per_item=50;	
		long long capitalized_labor_cost = 0;//当期生産分に配分する労務費（在庫に組み込む分）
		long long idle_capacity_cost = 0;//操業度差異（余剰人員コスト）。当期の損益に直接計上する
		if (app_context.period_count > 0) total_sales = bookKeeper->get_totalsales(app_context.period_count - 1);
		else total_sales = 0;
		//define_employee_wage_list(total_sales);//従業員給与は各期首に前期売上高に応じて決められている、ここではemployee_wage_listから支払予定給与の合計を取り出すだけでよい
		//int direct_labor_cost;
		size_shiped = size;
	//	if (get_agent_id() == 509) fprintf(app_context.config->out509, "  period=,%d, size=,%d,inside accounting at production,\n", app_context.period_count, size);
		if (size_shiped > 0) {
			long long price = items[0]->get_price();//生産製品の価格
			long long torihikigaku = size * price;//当月完成高が全て売れた場合の売上額
			direct_labor_cost = get_direct_labor_cost(); //producer.hで定義、employee_wage_listから支払予定給与の合計を取り出す
			//	fprintf(app_context.config->out_test8,"period=,%d,agent_id=,%d,agent_type=,%d,direct_labor_cost=,%I64d \n",app_context.period_count,get_agent_id(),get_agent_type(),get_direct_labor_cost());
			direct_labor_cost_per_item = long long(double(direct_labor_cost) / size);
			direct_material_fee = get_direct_material_fee();
			direct_material_fee_per_item = long long(double(direct_material_fee) / size);
			//20260914修正：労務費は雇用人数で決まり生産数量に関係なく一定額発生するため、
			//生産量がqmaxより著しく少ない期にその全額を少数の製品に配分すると1個当たり原価が
			//異常に高騰し、その在庫がまとめて売却された際に実体を伴わない一時的な大幅損失
			//（period=51等で発生したwholesalerの利益急落）を生む。このため「通常操業(qmax)時の
			//1個当たり労務費」を基準に実際生産数量分だけを在庫化(capitalized_labor_cost)し、
			//残り(idle_capacity_cost=余剰人員コスト)は当期の損益に直接計上する。20260914荻林
			if (qmax > 0) {
				capitalized_labor_cost = long long(double(direct_labor_cost) / qmax * size);
				if (capitalized_labor_cost > direct_labor_cost) capitalized_labor_cost = direct_labor_cost;
			}
			else capitalized_labor_cost = direct_labor_cost;
			idle_capacity_cost = direct_labor_cost - capitalized_labor_cost;
			genka = capitalized_labor_cost + direct_material_fee;
			genka_per_item = long long(double(genka) / size);
		//	fprintf(app_context.config->out_test8, "period=,%d,agent_id=,%d,agent_type=,%d,size_shiped=,%d,item_type_id=%d direct_labor_cost=,%I64d, genka_per_item=,%I64d,direct_labor_cconst_per_item=,%I64d,direct_material_cost_per_item=,%I64d,\n",
		//		app_context.period_count, get_agent_id(), get_agent_type(), size, item_type_id, get_direct_labor_cost(), genka_per_item, direct_labor_cost_per_item, direct_material_fee_per_item);
			//genka = direct_material_fee + direct_labor_cost;
			/*
			if (get_agent_type() == 2130) //soleproprietor
			{
				imported_inventory = imported_inventory_per_item * size;
				genka += imported_inventory;
				genka_per_item = long long(double(genka) / size);
			}
			*/

		}


		else {
			//printf("size_shiped=0です,size_shiped=%d 出荷製品1個当たり原価が計算できません agent_type=%d agent_id=%d\n", size_shiped, get_agent_type(),get_agent_id());
			direct_material_fee_per_item = 0; direct_material_fee = 0;
			direct_labor_cost_per_item = 0; direct_labor_cost = 0;
			genka_per_item = 0; genka = 0;
			imported_inventory = 0;
			capitalized_labor_cost = 0; idle_capacity_cost = 0;
		}
		if (items.size() > 0) {
			for (unsigned int i = 0; i < items.size(); i++) {
				items[i]->set_genka_per_item(genka_per_item);
				items[i]->set_imported_inventory_per_item(imported_inventory_per_item);
			}
		}
		//printf("befoe creating TBSlip\n");
		//-------------------仕訳データの作成------------------------------
		//genka_per_item = int(double(genka) / size_shiped);
		//printf("inside accounting treatment at production in wholesaler size_shiped = %d\n", size_shiped);
		//bookKeeper->createjournalSlip(app_context.period_count, 0, "製品", torihikigaku, "製品生産", torihikigaku, "売上原価", 1);
		/*
		if (get_agent_type() == 2130) //solproprietor
		{
			bookKeeper->createjournalSlip(app_context.period_count, 0, "輸入品在庫", imported_inventory, "材料輸入", imported_inventory, "現金",0);
			bookKeeper->createjournalSlip(app_context.period_count, 0, "当月完成高", imported_inventory, "製品生産", imported_inventory, "輸入品在庫", 0);
		}
		*/
		bookKeeper->createjournalSlip(app_context.period_count, 0, "当月完成高", direct_material_fee, "製品生産", direct_material_fee, "直接材料費", 1);
		//20260914修正：在庫化するのはcapitalized_labor_costのみ。idle_capacity_cost分は「直接労務費」の
		//借方残高として残り、操業度差異（余剰人員コスト）として当期の損益にそのまま反映される。
		bookKeeper->createjournalSlip(app_context.period_count, 0, "当月完成高", capitalized_labor_cost, "製品生産", capitalized_labor_cost, "直接労務費", 1);
		bookKeeper->createjournalSlip(app_context.period_count, 0, "製品在庫", genka, "製品生産", genka, "当月完成高", 0);
	//	fprintf(app_context.config->out_test11, "period=%d agent_id=%d agent_type=%d direct_material_fee= %l64d,direct_labor_cost=%l64d, genka= %l64d,size=%d \n",
	//		app_context.period_count, get_agent_id(), get_agent_type(), get_direct_material_fee(), get_direct_labor_cost(), get_genka_per_item() * size, size);
		//printf("after creating TBSlip\n");
		//-------------------仕訳データの作成終わり
		//bookKeeper->createjournalSlip(app_context.period_count, 1, "直接労務費", direct_labor_cost, "製品生産", direct_labor_cost, "未払給与",2);
	//	if (agent_id == 509) fprintf(app_context.config->out509, "period=,%d,inside accounting at production, size_shiped=,%d,\n,", app_context.period_count, size_shiped);
		if (size_shiped > 0) {
			bookKeeper->createstockBookSlip(app_context.period_count, items.size(), 0, items[0]->get_item_type_id(), items[0]->get_price());
			bookKeeper->createstockpriceBookSlip(app_context.period_count, genka, 0, items[0]->get_item_type_id());
			bookKeeper->creategenkaBookSlip(app_context.period_count, size_shiped, direct_labor_cost, direct_material_fee, genka, items[0]->get_item_type_id());
	//		fprintf(app_context.config->out_test9, "period=,%d, agent_id=,%d,agent_type=,%d,size=,%d,direct_labor_cost_per_item=,%I64d,direct_material_fee_per_item=,%I64d,genka_per_item=,%I64d,\n",
	//			app_context.period_count, get_agent_id(), get_agent_type(), size, direct_labor_cost_per_item, direct_material_fee_per_item, genka_per_item);
			/*
			if (app_context.period_count == 2 && agent_id == 542) fprintf(app_context.config->wholesalerData->out_stockpricebook,
				"peiod=,%d, agent_id=,%d,genka_per_item=,%I64d,size=,%d,karikata=,%I64d, kashikata=0,karikata=kashikata=<%I64d,\n",
				app_context.period_count, get_agent_id(), genka_per_item, size, genka_per_item * size, get_genka_per_item());
				*/
		}
		else {
			//size_shiped=0
			//	bookKeeper->creategenkaBookSlip(app_context.period_count, size_shiped, direct_labor_cost_per_item, direct_material_fee_per_item, genka_per_item, items[0]->get_item_type_id());
		//	fprintf(app_context.config->out_test, "inside account treatment at production, , period=,%d, size_shiped=,%d,agent_id=,%d\n",
		//		app_context.period_count, size_shiped, get_agent_id());
			bookKeeper->creategenkaBookSlip(app_context.period_count, size_shiped, direct_labor_cost_per_item, direct_material_fee_per_item, genka_per_item, 0);
		}
		//printf("after the end of accounting_treatment_at production\n");
	}
	/** 納税 */
	//void pay_tax() {}
	/** 購買 */
	void buy_retails() {}
	virtual void buy_materials() {};//20210905記　荻林

	/** 決算 */
	void kessan() {
	//	fprintf(app_context.config->out3, "inside kessan() in producer period=%d producer_id= %d, total_sales=%I64d\n", app_context.period_count, get_agent_id(), bookKeeper->get_cashbookslip()->income);
	}

	/** 生産行動 */
	void process_producing() {
		//生産計画　自関　  20120523記　高島
//fprintf(app_context.config->out_temporary, "inside process_producing period=%d agent_id=%d agent_type=%d q_count=%d\n", app_context.period_count, get_agent_id(), get_agent_type(), q_count);

//if(q_count>0) create_products();	//実際の生産、市場への出荷　自関　  20120523記　高島
//printf("before create_products()\n");
		create_products(q_count); //create_productsの関数形修正、会計処理が生産量0の場合にも行われるようにするため 20220923
		//printf("after create_products()\n");
	}

	int get_number_of_stocks(int item_type_id) {
		set<Item*, PredForItemBySeq>* stocks = get_item_set(item_type_id);
		return stocks->size();
		//市場に商品が存在しなければsize=0

	}

	void ave_sigma(int data[], int n, double* ave, double* sigma) {
		int i; double ave1, sigma1;
		double sum; sum = 0.0;
		for (i = 0; i < n; i++)	sum += double(data[i]);

		ave1 = sum / n;

		/**偏差平方差、分散、標準偏差、標準誤差*/
		double s; s = 0.0;
		for (i = 0; i < n; i++)	s += (data[i] - (ave1)) * (data[i] - (ave1));
		if (n <= 1) sigma1 = 0;
		else sigma1 = sqrt(s / (n - 1));
		*ave = ave1;
		*sigma = sigma1;
	}
	int number_of_soldItem(int period, int item_type_id) {
		//2012_6_3荻林作成　指定期の売れた個数を返す
		int i = 0;
		int sum = 0;
		sum = 0;
		int size = 0;
		size = memSold.size();
		for (i = 0; i < size; i++) {
			if (memSold[i]->period == period && memSold[i]->item_type_id) {
				sum++;
			}
		}
		return sum;
	}
	/*int number_of_stockItem(int period,int item_type_id){
		//2012_6_3荻林作成　指定期の売れた個数を返す
		int i = 0;
		int sum = 0;
		sum=0;
		int size = 0;
		size=memSold.size();
		for(i=0;i<size;i++){
			if(memSold[i]->period==period && memSold[i]->item_type_id){
				sum++;
			}
		}
		return sum;
	}
	*/
	void ave_sigma_soldItem(double* ave, double* sigma) {
		//critical_period_for_demand分の購入された商品の平均とシグマを計算する
//----------2012_5_28
		int n = 0, i = 0, j = 0;
		int* data;
		double ave1 = 0.0, sigma1 = 0.0;
		if (app_context.period_count > 0) {
			//DataAnalyzer* dataAnalyzer=app_context.dataAnalyzer_navi_for_retailMarket->get_dataAnalyzer();
			//前の期に売れた製品の数
			//number_of_bought_items=dataAnalyzer->number_of_bought_items(agent_id,app_context.period_count-1,item_type_id);

			//number_of_bought_items=get_dataAnalyzer()->number_of_bought_items(agent_id,app_context.period_count-1,item_type_id);	
			//	if(double(number_of_bought_items)/qmax<0.2) price_down_flag++;

		//	if (app_context.period_count <= app_context.config->critical_period_for_demand) {
			if (app_context.period_count <= app_context.config->producerParameter->critical_period_for_demand) {
			n = app_context.period_count;
				j = 0;
			}
			else {
				//n = app_context.config->critical_period_for_demand;
				n = app_context.config->producerParameter->critical_period_for_demand;
				j = app_context.period_count - n;
			}
			data = new int[n];
			for (i = 0; i < n; i++) {
				data[i] = number_of_soldItem(j + i, item_type_id);
				//period-10+0～period-10+9
				//data[i]=get_dataAnalyzer()->number_of_bought_items(agent_id,j+i,item_type_id);
				//printf("data[%d]=%d n=%d period=%d agent_id=%d\n",j+i,data[i],n,app_context.period_count,agent_id);
			}
			ave_sigma(data, n, &ave1, &sigma1);	//分散の計算　自関　、ave,sigmaは参照渡して値を戻す  20120523記　高島
			//-----------------------------
			//-----------------------------
			delete[] data;
			//------------------------------
		}

		//----------2012_5_28
			//～～～～～～～～～別関数への移動候補  20120523記　高島
		*ave = ave1;
		*sigma = sigma1;
	}
	/*	void ave_sigma_stockItem(double *ave,double* sigma){
			//critical_period_for_demand分の購入された商品の平均とシグマを計算する
	//----------2012_5_28
			int n = 0, i = 0, j = 0;
			int *data;
			double ave1 = 0.0, sigma1 = 0.0;
			if(app_context.period_count>0){
				//DataAnalyzer* dataAnalyzer=app_context.dataAnalyzer_navi_for_retailMarket->get_dataAnalyzer();
				//前の期に売れた製品の数
				//number_of_bought_items=dataAnalyzer->number_of_bought_items(agent_id,app_context.period_count-1,item_type_id);

				//number_of_bought_items=get_dataAnalyzer()->number_of_bought_items(agent_id,app_context.period_count-1,item_type_id);
				//	if(double(number_of_bought_items)/qmax<0.2) price_down_flag++;

				if(app_context.period_count<=app_context.config->critical_period_for_demand) {
							n=app_context.period_count;
							j=0;
					}
				else{
						n=app_context.config->critical_period_for_demand;
						j=app_context.period_count-n;
				}
						data=new int[n];
						for(i=0;i<n;i++){
							data[i]=number_of_stockItem(j+i,item_type_id);
							//data[i]=get_dataAnalyzer()->number_of_bought_items(agent_id,j+i,item_type_id);
							//printf("data[%d]=%d n=%d period=%d agent_id=%d\n",j+i,data[i],n,app_context.period_count,agent_id);
						}
						ave_sigma(data,n,&ave1,&sigma1);	//分散の計算　自関　、ave,sigmaは参照渡して値を戻す  20120523記　高島
						//-----------------------------
						//-----------------------------
			delete [] data;
						//------------------------------
			}

		//----------2012_5_28
			//～～～～～～～～～別関数への移動候補  20120523記　高島
			*ave=ave1;
			*sigma=sigma1;
		}
		*/
		/** 生産計画 */
	void plan_produce() {
		int item_type_id = 0;
		AccountData* accountData = NULL;
		if (agent_type == 31 || agent_type==310 || agent_type == 311 || agent_type == 312 || agent_type==3100 || agent_type==3110 || agent_type==3120) accountData = app_context.config->consumerData;
		else if (agent_type == 2131) accountData = app_context.config->retailerData;
		else if (agent_type == 2132) accountData = app_context.config->wholesalerData;
		else if (agent_type == 2130) accountData = app_context.config->soleproprietorData;
		else {}

	//	printf("period=%d start of plan_produce()\n",app_context.period_count);
		//～～～～～セットアップ時の項目に移動候補　  20120523記　高島
		if (app_context.period_count == 0) {
			//fprintf(app_context.config->out," up_flag=%d down_flag=%d \n",price_up_flag,price_down_flag);
			price_down_flag = 0;
			price_up_flag = 0;
		}
		//～～～～～セットアップ時の項目に移動候補　  20120523記　高島

		//～～～～～～～～～～別関数への移動候補　  20120523記　高島
		/*生産品種の決定*/
		item_type_id = get_item_type_id();	//set_up()で決定した品種を生産する。
		//wholesalerの場合は毎回ランダムに決定する。
		/*
		if(get_agent_type()==2132){
			int ijk = RandomGenerator::random_int(1, app_context.config->item_type_idkw.size());
			item_type_id = app_context.config->item_type_idkw[ijk - 1];	
		}
		*/
		//if(double(size)/qmax<0.2) price_up_flag++;
		/**生産量の決定*/
		int new_qcount = 0;
		//new_qcount=renew_qcount_decision(q_count);
		new_qcount = renew_qcount_decision_revised(q_count, item_type_id);//line525 ODD記載のロジック荻林20201020
	//	new_qcount = q_count;//生産量一定ロジック202604053

		set_new_qcount(new_qcount);
		renew_qcount(new_qcount);//line 615で定義　new_qcount!=q_countならq_countをnew_qcountに更新する
	//	if (agent_id == 509) fprintf(app_context.config->out509, "period=,%d,inside_plan_produce, new_qcount=,%d,qcount=,%d,\n", app_context.period_count, new_qcount, q_count);
		/**価格の決定*/
		//price=min_price*2;
		long long new_price = 0;
		//new_price=renew_price_decision(price);
		//printf("before new_price= in plan_produce\n");
		new_price = renew_price_decision_revised(price, item_type_id);//703行目　ODD記載のロジック荻林20201020
				//create_products()ではnew_priceをprice_delta分ふらして出荷する
				// price_deltaは各子クラスのset_up()で定義される　パラメータで指定
		//	printf("after new_price= i plan_produce\n");
		/**在庫の評価 */
	//	fprintf(app_context.config->out, "period=%d, producer %d 生産前の在庫の評価\n", app_context.period_count, get_agent_id());
		print_stocks();
	//	printf("period=%d after print_stocks()\n",app_context.period_count);
		/**市場在庫品の価格の改定*/
		renew_price(new_price);//new_price!=priceならrenew_price_in_market(new_price, item_type_id)を呼出し市場在庫製品の価格を改定する
	//	printf("after renew_price\n");
		if (get_agent_type() == 2131 || get_agent_type() == 2132 || get_agent_type()==2130) {
		
			//fprintf(accountData->out_quantity_priceBook, "inside fprint agent_type=,%d,\n",get_agent_type());
	//		printf("beforeprint_out_quantity_priceBook \n");
		//	fprintf(accountData->out_quantity_priceBook,"period=,%d, agent_id=,%d,q_count=,%d,new_qcount=,%d,item_type_id=,%d,\n", app_context.period_count, get_agent_id(), q_count, new_qcount, item_type_id);
	//		printf("why fprintf(accountData->out_quantity_priceBook  below causes an error?\n");
			/*
			fprintf(accountData->out_quantity_priceBook,
				"period=,%d, agent_id=,%d,q_count=,%d,new_qcount=,%d,item_type_id=,%d,price=,%I64d,new_price=,%I64d,item_type_id=,%d,\n", app_context.period_count, get_agent_id(), q_count, new_qcount, item_type_id, price, new_price, item_type_id
			);
			*/
				
	//		printf("after fprint\n");
		}
	//	printf(" end of plan_produce()\n");
		/*
		if (get_agent_id() == 478 || get_agent_id() == 488 || get_agent_id() == 512)
			fprintf(app_context.config->out_test10, "inside producer plan_produce() period=%d agent_id=%d q_count=%d, new_qcount=%d, item_type_id=%d\n"
				,app_context.period_count,get_agent_id(),q_count, new_qcount,item_type_id);
		*/
	}
	void print_stocks() {
		//plan_produce()の中で呼ばれる
		//在庫の認識：下記(*it)で在庫製品の情報をゲットできる
		set<Item*, PredForItemBySeq>* stocks = get_item_set(item_type_id);
	//	fprintf(app_context.config->out, "period=%d,在庫製品情報 in print_stocks producer %d stock_size=%d\n", app_context.period_count, get_agent_id(), stocks->size());
		//fprintf(app_context.config->out_temporary, "inside print_stocks() peirod=%d agent_id=%d item_type_id=%d number of stocks in producer size=%d,\n", app_context.period_count, get_agent_id(),item_type_id, stocks->size());
		set<Item*, PredForItemBySeq> ::iterator it;
		for (it = stocks->begin(); it != stocks->end(); it++) {
			// cout << (*it)->get_item_type_id() << "-" << (*it)->get_price() << endl;
		//	fprintf(app_context.config->out, " %d  - %I64d seller id=%d\n", (*it)->get_item_type_id(), (*it)->get_price(), (*it)->Get_seller()->Get_agent_id());
		}
	}
	int renew_qcount_decision(int q_count) {
		/**生産量の決定
			q_count:前期の生産量
			new_qcount:今期の生産量
								   */
		int new_qcount = 0;
		//int size;
		//double ave,sigma;
		//size=get_number_of_stocks(item_type_id);
		if (app_context.period_count > 0) {
			//new_qcount = ruledemand_qcount_decision(q_count, item_type_id);
			new_qcount=rulestock_qcount_decision(q_count,item_type_id);
			//if(new_qcount<int(double(qmax)*0.5)) new_qcount=int(double(qmax)*0.5);	//生産計画量が生産限界の一定以下の場合は生産限界の一定値へ（0.5の部分はパラメータ化）  20120523記　高島
			//else if(new_qcount>qmax) new_qcount=qmax;
			//if(new_qcount>qmax) new_qcount=qmax;
			//-------------------20191123コメント化荻林
				/*if(new_qcount<=int(double(fixed_cost)/(price-variable_cost))){
				new_qcount=int(double(fixed_cost)/(price-variable_cost));
				if(new_qcount>qmax) new_qcount=0;//qmax生産しても固定費分を稼げないので生産中止
			}*/
			//---------------------
			if (new_qcount > qmax) new_qcount = qmax;
		//	if (q_count != 0) fprintf(app_context.config->out_q_price, "agent_id=,%d,fixed_cost=,%I64d,variable_cost=,%I64d,q_count=,%d,price=%I64d,qmax=,%d,new_qcount=,%d,minq=,%d,minprice=,%I64d,\n", agent_id, fixed_cost, variable_cost, q_count, price, qmax, new_qcount, int(double(fixed_cost) / (price - variable_cost)), variable_cost + fixed_cost / q_count);
		}
		else new_qcount = q_count;
		return new_qcount;
	}
	int renew_qcount_decision_revised(int qcount, int item_type_id) {

		//2020/9/2　　　ODD記載のロジックに改定　荻林
		//生産量の決定
		  //  q_count:前期の生産量
		//	new_qcount:今期の生産量
		 //
		int new_qcount = 0;
		int size = get_number_of_stocks(item_type_id);
		if (app_context.config->case_const_qcount == 0) {
			//case_const_qcountが0の場合はqcount=constant	
			new_qcount = qcount;
			if (app_context.period_count > 0 && size > 0) {
				new_qcount = new_qcount - size;
				if (new_qcount < 0) new_qcount = 0;
			}
		//	fprintf(app_context.config->out_size, "period=,%d, agent_id=,%d, new_qcount=,%d, size=,%d,item_type_id=,%d,\n", app_context.period_count, get_agent_id(),new_qcount,size,item_type_id );
		//	if (size > 0) new_qcount = new_qcount - size;
		}
		/*
		else if (app_context.period_count == 0) {
			new_qcount = qcount * app_context.config->producerParameter->initial_quantity_ratio;
		}
		*/
		else if(app_context.period_count > 0) {
				new_qcount = ruledemand_qcount_decision_revised(qcount, item_type_id);
			//	fprintf(app_context.config->out_qcount_decision, "inside inside else if, period=,%d, agent_id=,%d,case_const_qcount=,%d,qcount=,%d,new_qcount=,%d,\n", app_context.period_count, get_agent_id(), app_context.config->case_const_qcount, qcount, new_qcount);
			}
			//	if(app_context.period_count>0) new_qcount = rulestock_qcount_decision(q_count, item_type_id);
		else {
			new_qcount = qcount;
		//	if (size > 0) new_qcount = new_qcount - size;
		}
		//fprintf(app_context.config->out_qcount_decision, "inside renew_qcount_decision, period=,%d, agent_id=,%d,case_const_qcount=,%d,qcount=,%d,new_qcount=,%d,\n", app_context.period_count, get_agent_id(),app_context.config->case_const_qcount,qcount,new_qcount);
		return new_qcount;
	}
	void add_soldItem(Item* item) {
		soldItem->item_type_id = item->get_item_type_id();
		soldItem->price = item->get_price();
		soldItem->period = app_context.period_count;
		memSold.push_back(soldItem);
	}
	int ruledemand_qcount_decision_revised(int q_count, int item_type_id) {
		//int critical_stock = int(double(get_qmax(item_type_id)) * app_context.config->producer_critical_stock_ratio);
		//int critical_stock = 0;
		//int critical_stock = int(double(get_qmax(item_type_id)) * app_context.config->critical_stock_ratio);
		//int critical_stock = int(double(get_qmax(item_type_id)) * app_context.config->producerParameter->critical_stock_ratio_quantity);
		int critical_stock = int(double(q_count) * app_context.config->producerParameter->critical_stock_ratio_quantity);

		//2020/9/2　　　ODD記載のロジックに改定　荻林
		long long fixed_wage = get_fixed_wage();
		long long profit = bookKeeper->get_profit();
		int size = 0;
		double ave = 0.0, sigma = 0.0;
		int new_qcount=0;
		//int qsi;//base amount of production 
		double qsi = 0.0;//20230821
		double beta = 0.0;
		//beta = 0.05;
		beta = app_context.config->producerParameter->beta;//producer_production_adjustment_coefficient
		//過去の売り上げ実績と前期の在庫量に基づき当期生産量を決定する。	
			//size=get_number_of_stocks(item_type_id);
		/*
		if (app_context.period_count > 0) size = bookKeeper->get_number_of_stocks(app_context.period_count - 1, item_type_id);
		else size = 0;
		*/
		size = bookKeeper->get_size_of_stock_in_market();
		//fprintf(app_context.config->out_temporary, "peirod=%d agent_id=%d item_type_id=%d number of stocks in producer size=%d,\n", app_context.period_count, get_agent_id(),item_type_id, size);
		ave_sigma_soldItem(&ave, &sigma);
		//qsi=int (ave)+int(producer_demand_delta*sigma);//目標在庫量
		qsi = ave + producer_demand_delta * sigma;//20230821
//		if (size == 0) new_qcount = int(qsi * (1 + beta));
	/*
		if (size <= 0) new_qcount = int(qsi * (1 + beta)) - size;
		else if (size > critical_stock)  new_qcount = int(qsi * (1 - beta)) - size;
		else new_qcount = q_count-size;
		if (new_qcount > qmax) new_qcount = qmax;
		else if (new_qcount < 0) new_qcount = 0;
	*/	
	
//Ogibayahi original logic for quantity	
		//2026/10/1 ogibayashi: beta(5%程度)による調整量が生産量の絶対水準が小さい(10個未満等)ときに
		//四捨五入で消えてq_countが不動点に固定される問題への対処。切り捨てた(あるいは切り上げた)小数分を
		//qcount_round_carryに保存し次期の計算に加算することで、複数期にわたって蓄積させ丸めの情報を失わないようにする。
		bool carry_reset = false;
		double raw_qcount = 0.0;
		if (critical_stock == 0) {
			if (size <= 0) raw_qcount = qsi * (1 + beta)-size;
			else  raw_qcount = qsi * (1 - beta)-size;
			//fprintf(app_context.config->out_trace_supply_sell, "period=,%d,agent_id=,%d, item_type_id=,%d, size=,%d,critical_stock=,%d,beta=,%lf,qsi=,%lf,new_qcount=,%d,qcount=,%d,\n", app_context.period_count, get_agent_id(), item_type_id, size, critical_stock, beta, qsi, new_qcount, q_count);

		}
		else {
			if (size <= critical_stock) 	raw_qcount = qsi * (1 + beta)-size;
			//		else new_qcount = int(qsi * (1 - beta)) - size;
			else if (size >= qmax - critical_stock) raw_qcount = qsi * (1 - beta)-size;
			else { raw_qcount = double(q_count); carry_reset = true; }//変化させない分岐ではcarryを積ませない
		}
		double combined_qcount = raw_qcount + qcount_round_carry;
		new_qcount = int(combined_qcount + 0.5);//四捨五入
		qcount_round_carry = carry_reset ? 0.0 : (combined_qcount - double(new_qcount));//切り落とした(あるいは切り上げた)分を次期に持ち越す
		if (new_qcount > qmax) { new_qcount = qmax; qcount_round_carry = 0.0; }//設備増産選択肢は入っていないので、この条件をとる20210917　天井でcarryもリセット
		else if (new_qcount < qmin) { new_qcount = qmin; qcount_round_carry = 0.0; }
	//takashima original logic for quantity
/*
		if(size==0) new_qcount=int(qsi*(1+beta)+0.5);
		else {
			new_qcount=int(qsi*(1-beta)+0.5)-size;
		}
		if (new_qcount > qmax) new_qcount = qmax;//設備増産選択肢は入っていないので、この条件をとる20210917
	*/	
//         新たな生産量の決定に際して、既存の在庫個数は考慮しない
		//new_qcount = new_qcount - size;//新たな生産量の決定に際して既存の在庫個数を考慮する
		if (new_qcount < 0) { new_qcount = 0; qcount_round_carry = 0.0; }
	//	fprintf(app_context.config->out_trace_supply_sell, "period=,%d,agent_id=,%d, item_type_id=,%d, size=,%d,critical_stock=,%d,beta=,%lf,qsi=,%lf,new_qcount=,%d,qcount=,%d,\n", app_context.period_count, get_agent_id(), item_type_id, size, critical_stock, beta, qsi, new_qcount, q_count);

	//	fprintf(app_context.config->out_qcount_decision, "period=,%d,agent_id=,%d, item_type_id=,%d, size=,%d,new_qcount=,%d,qcount=,%d,\n",app_context.period_count,get_agent_id(),item_type_id,size,new_qcount, q_count);
		//20240213　下記生産制限を追加（生産赤字額が固定給与以上なら生産しない）
		//if (profit + fixed_wage*employee_wage_list.size()*1000000 < 0) new_qcount = 0;
	//	if (profit < -1e6) new_qcount = 0;
		//else if(new_qcount<int(qmax*0.5)) new_qcount=int(qmax*0.5);//20191123新規記入荻林
		//else if(new_qcount<qmin) new_qcount=qmin;//20200903新規記入荻林
		//fprintf(app_context.config->out_qmax, "period=,%d,agent_id=,%d,ave=,%lf,sigma=,%lf,qsi=,%lf,size=,%d,critical_stock=,%d,beta=,%lf,qcount=.%d,new_qcount=%d,\n", app_context.period_count, get_agent_id(), ave, sigma, qsi, size, critical_stock, beta, q_count, new_qcount);
		return new_qcount;
	}
	void renew_qcount(int new_qcount) {
		//if (new_qcount != q_count) q_count = new_qcount;
		set_q_count(new_qcount);
	}
	void renew_price(long long new_price) {
		//plan_procude()で呼ばれる
		if (new_price != price) {
			renew_price_in_market(new_price, item_type_id);//AreaMarketに出荷されたItemの価格を改定する
		}
		price = new_price;
	}


	int ruledemand_qcount_decision(int q_count, int item_type_id) {
		int size = 0;
		double ave = 0.0, sigma = 0.0;
		int new_qcount = 0;

		size = get_number_of_stocks(item_type_id);
		ave_sigma_soldItem(&ave, &sigma);
		new_qcount = int(ave) + int(producer_demand_delta * sigma) - size;//producer_demand_deltaはretailerのsetupで定義している
		//new_qcount=int(ave)+int(sigma);
		//new_qcount=int(ave)+int(sigma)-size;//20191123新規記入荻林
		//if(new_qcount<qmin) new_qcount=qmin;//20191123新規記入荻林
		if (new_qcount < qmin) new_qcount = qmin;
		if (new_qcount > qmax) new_qcount = qmax;
		//set_SbsHistory(item_type_id,q_count,ave,size,price);

		return new_qcount;
	}
	int rulestock_qcount_decision(int q_count, int item_type_id) {
		//生産量決定　その時点の在庫量によって決定2012_6_3荻林作成
		int size = 0;
			int critical_stock=int(app_context.config->producerParameter->critical_stock_ratio_quantity*double(qmax));
		size = get_number_of_stocks(item_type_id);
		double beta = 0.0;
		//beta = 0.05;
		beta = 0.05;
		int new_qcount = 0;
		if (size > qmax - critical_stock) new_qcount = int(double(q_count) * (1 - beta));
		else if (size < critical_stock) new_qcount = int(double(q_count) * (1 + beta));
		else new_qcount = q_count;
		/*
		if (size > int(double(qmax) * 0.8)) {
			new_qcount = int(double(q_count) * (1 - beta * 2));
		}
		else if (size > int(double(qmax) * 0.5)) {
			new_qcount = int(double(q_count) * (1 - beta));
		}
		//else if (size<int(double(qmax) * 0.05) && size >= 1) {
		else if(size<int(double (qmax)*0.2)){
			new_qcount = int(double(q_count) * (1 + beta));
		}
		//else if (size < 1) {
		//else{
		//	new_qcount = int(double(q_count) * (1 + beta * 2));
		//}
		else { new_qcount = q_count; }
		//if(new_qcount<int(qmax*0.5)) new_qcount=int(qmax*0.5);//20191123新規記入荻
		*/
		return new_qcount;
	}
	int renew_price_decision(int price) {
		//価格決定
		int size = 0;
		size = get_number_of_stocks(item_type_id);
		//alphaはretailerのsetup()で定義
		if (size > int(double(qmax) * 0.8)) {
			price = int(double(price) * (1 - producer_price_alpha * 2));
		}
		else if (size > int(double(qmax) * 0.5)) {
			price = int(double(price) * (1 - producer_price_alpha));
		}
		else if (size<int(double(qmax) * 0.05) && size >= 1) {
			price = int(double(price) * (1 + producer_price_alpha));
		}
		else if (size < 1) {
			price = int(double(price) * (1 + producer_price_alpha * 2));
		}
		else {}
		/*if(price<=variable_cost+int(double(fixed_cost)/qmax)) {
			price=variable_cost+int(double(fixed_cost)/qmax)+10;
		}*/	//20191123コメント化荻林
		return price;
	}
	long long renew_price_decision_revised(long long price, int item_type_id) {
		//fprintf(app_context.config->out_min_price, "period=,%d, agent_id=,%d, min_price=,%lld,price=,%lld,inside renew_price_decision_revised,\n",app_context.period_count,get_agent_id(),min_price,price);
		//2020/10/20 ODD記載のロジック
		//2020/9/1　改定　荻林　前期の平均市場価格を考慮して当期の価格を決定する
		//期首の市場平均価格はAreamarketのget_ave_price_in_market(item_type_id)で取得できる
		long long fixed_wage = get_fixed_wage();
		long long profit = bookKeeper->get_profit();
		int size = get_number_of_stocks(item_type_id);
		long long price_ave=0;
		// if (app_context.period_count > 0) price_ave = ave_price_in_market;
		if (app_context.period_count > 0) {
			//price_ave = get_dataAnalyzer()->get_ave_price(app_context.period_count - 1, item_type_id);//市場平均価格
		//price_ave = get_bookKeeper()->get_ave_price_in_market();

			if (agent_type != 2132) price_ave = app_context.retail_market_navi_for_customer->get_retail_market(item_type_id)->get_ave_price(item_type_id);
			else price_ave = app_context.material_market_navi_for_retailer->get_retail_market(item_type_id)->get_ave_price(item_type_id);
			//2026/10/1 ogibayashi: 上のget_ave_price(item_type_id)は「現在市場に残っている(売れ残っている)在庫」の平均価格であり、
			//完売(size==0)が続く局面では在庫そのものが存在しないため常に0が返り、price_ave>0の分岐が一度も使われず価格が凍結する。
			//在庫ベースの平均価格が得られない(0)場合は、直近期の実際の成約価格の平均(get_dataAnalyzer())で代替する。
			if (price_ave <= 0) {
				price_ave = long long(get_dataAnalyzer()->get_ave_price(app_context.period_count - 1, item_type_id));
			}
		}

			long long new_price = 0;
		double alpha=0.01;
		long long critical_price_low = long long(double(price_ave) * (1 - alpha));
		long long critical_price_high = long long(double(price_ave) * (1 + alpha));
		int critical_stock =int( double(get_qmax(item_type_id)) * app_context.config->producerParameter->critical_stock_ratio_price);
	//	fprintf(app_context.config->error_message, "period=,%d,critical_stock=,%d,qmax=,%d,critical_ratio=,%lf\n", app_context.period_count, critical_stock,get_qmax(item_type_id),app_context.config->producerParameter->critical_stock_ratio_price);
		new_price = 0;
		if (app_context.period_count == 0) {
			new_price = price;
			//printf("before else period=%d in renew price desicion revised\n",app_context.period_count);
		}
		else {
			//printf("inside else  period=%d in renew price desicion revised\n",app_context.period_count);
			//printf("inside renew_price_decision_revised before price_ave=  period=%d\n", app_context.period_count);
			//price_ave = ave_price_in_market;//if文の上、変数定義のところに移動する
			//ave_pride_in_marketはEneterprise.hのdo_kishu()の中のdefine_ave_price_in_market()で定義されている。
			// その時点でもし商品がなければprice_ave=0;
		//	price_ave=get_dataAnalyzer()->get_ave_price(app_context.period_count-1,item_type_id);//市場平均価格
//ogibayashi_originallogic
/*
			if (price_ave > 0) {
				//marketに商品がある場合
				if (get_q_count() >= get_qmax()) {
					if (size == 0) new_price = long long(double(price) * (1 + producer_price_alpha));
					else if (price < price_ave)  new_price = price;// new_price=price_ave;
					else if (price > price_ave) new_price = long long(double(price) * (1 - producer_price_alpha));
					else { new_price = price; }
					fprintf(app_context.config->out_ave_price, "market商品ありq=qmax,period=,%d,agent_id=,%d,size=,%d,price=,%I64d,price_ave=,%I64d,new_price=,%I64d,item_type_id=,%d,qcount=,%d,qmax=,%d,new_qcount=,%d,\n",
						app_context.period_count, agent_id, size, price, price_ave, new_price,get_item_type_id(),get_q_count(),get_qmax(),get_new_qcount());
				}
				else {
					new_price = price;
					fprintf(app_context.config->out_ave_price, "market商品ありq=qmax,period=,%d,agent_id=,%d,size=,%d,price=,%I64d,price_ave=,%I64d,new_price=,%I64d,item_type_id=,%d,qcount=,%d,qmax=,%d,new_qcount=,%d,\n",
						app_context.period_count, agent_id, size, price, price_ave, new_price, get_item_type_id(), get_q_count(), get_qmax(), get_new_qcount());

				}
			}
			else {
				//new_price = price;
				//marketに商品がない場合	
				if(get_q_count()>=get_qmax())new_price = long long(double(price) * (1 + producer_price_alpha));
				else new_price = price;	
			   	//new_price = price;
				fprintf(app_context.config->out_ave_price, "market商品なしq=qmax,period=,%d,agent_id=,%d,size=,%d,price=,%I64d,price_ave=,%I64d,new_price=,%I64d,item_type_id=,%d,qcount=,%d,qmax=,%d,new_qcount=,%d,\n",
					app_context.period_count, agent_id, size, price, price_ave, new_price, get_item_type_id(), get_q_count(), get_qmax(), get_new_qcount());
					}
	
			fprintf(app_context.config->out_trace_supply_sell, "period=,%d,agent_id=,%d,item_type_id=,%d, price=,%I64d,price_ave=,%I64d,new_price=,%I64d,size=,%d,critical_stock=,%d,producer_price_alpha=%lf,\n ",
				app_context.period_count, get_agent_id(), item_type_id, price, price_ave, new_price, size, critical_stock, producer_price_alpha);
*/
//ogibayahi revised logic1 for price 2026/4/10
    
			if(price_ave>0){
				if (size == 0) {
					if (get_q_count() >= get_qmax()) new_price = long long(double(price) * (1 + producer_price_alpha));
					else if(price>price_ave) new_price = long long(double(price) * (1 - producer_price_alpha));
					else new_price = price;	
				}
					
				else {
					if(get_q_count()>=get_qmax()) new_price = price;
					else new_price = long long(double(price) * (1 - producer_price_alpha));
				}
			}
			else {
				new_price = price;
			}	
			if (new_price < min_price) new_price = min_price;
			//fprintf(app_context.config->out_min_price,"period=,%d,agent_id=,%d,min_price=,%lld,new_price=,%lld,price=,%lld,\n",app_context.period_count,get_agent_id(),min_price, new_price,price);
	
		//ogibayashi revised logic2 for price 2026/4/10
	/*
			long long price_ave_minus = price_ave;// long long(double(price_ave) * (1 - producer_price_alpha));
			long long price_ave_plus = price_ave;// long long(double(price_ave) * (1 + producer_price_alpha));

			if (price_ave > 0) {
				if (size == 0) {
					if (get_q_count() >= get_qmax()) new_price = long long(double(price) * (1 + producer_price_alpha));
					else if (price > price_ave_plus) new_price = long long(double(price) * (1 - producer_price_alpha));
					else if (price < price_ave_minus) new_price = long long(double(price) * (1 + producer_price_alpha));
					else new_price = price;
				}

				else {
					if (get_q_count() >= get_qmax()) new_price = price;
					else if(price>price_ave_plus) new_price = long long(double(price) * (1 - producer_price_alpha));
					else if(price<price_ave_minus) new_price = long long(double(price) * (1 + producer_price_alpha));
					else new_price = price;
				}
			}
			else {
				new_price = price;
			}
	*/	
			//ogibayashi revised logic3 for price 2026/4/10
	/*
			//long long price_ave_minus = price_ave;// long long(double(price_ave) * (1 - producer_price_alpha));
			//long long price_ave_plus = price_ave;// long long(double(price_ave) * (1 + producer_price_alpha));

			if (price_ave > 0) {
				if (size == 0) {
					if (get_q_count() >= get_qmax()) new_price = long long(double(price) * (1 + producer_price_alpha));
					else if (price > price_ave) new_price = long long(double(price) * (1 - producer_price_alpha));
					//else if (price < price_ave_minus) new_price = long long(double(price) * (1 + producer_price_alpha));
					else new_price = price;
				}

				else {
					if (get_q_count() >= get_qmax()) new_price = price;
					else if (price > price_ave) new_price = long long(double(price) * (1 - producer_price_alpha));
					else if (price < price_ave) new_price = long long(double(price) * (1 + producer_price_alpha));
					else new_price = price;
				}
			}
			else {
				new_price = price;
			}

*/


//takshima AI original logic
/*
 			if (size == 0) new_price = long long(double(price) * (1 + producer_price_alpha));
			else {
				double diff = double(price_ave) - double(price);
				new_price = long long(double(price) + producer_price_alpha * diff);
			}
*/
//takshima original logic
/*
			if (size == 0) { new_price = long long(double(price) * (1 + producer_price_alpha));
				fprintf(app_context.config->out_ave_price, "size=0,period=,%d,agent_id=,%d,size=,%d,price=,%I64d,price_ave=,%I64d,new_price=,%I64d,item_type_id=,%d,qcount=,%d,qmax=,%d,new_qcount=,%d,\n",
					app_context.period_count, agent_id, size, price, price_ave, new_price, get_item_type_id(), get_q_count(), get_qmax(), get_new_qcount());
					}
				
			else if (price > price_ave) {
				new_price = long long(double(price) * (1 - producer_price_alpha));
				fprintf(app_context.config->out_ave_price, "size>0,price<ave_price,period=,%d,agent_id=,%d,size=,%d,price=,%I64d,price_ave=,%I64d,new_price=,%I64d,item_type_id=,%d,qcount=,%d,qmax=,%d,new_qcount=,%d,\n",
					app_context.period_count, agent_id, size, price, price_ave, new_price, get_item_type_id(), get_q_count(), get_qmax(), get_new_qcount());
			}



			else {
				new_price = price;
				fprintf(app_context.config->out_ave_price, "size>0 else,period=,%d,agent_id=,%d,size=,%d,price=,%I64d,price_ave=,%I64d,new_price=,%I64d,item_type_id=,%d,qcount=,%d,qmax=,%d,new_qcount=,%d,\n",
					app_context.period_count, agent_id, size, price, price_ave, new_price, get_item_type_id(), get_q_count(), get_qmax(), get_new_qcount());
			}
	*/		
//---------------------------------/
//AI推奨ロジック
			/*
  if (price_ave > 0) {

    if (size == 0) {
        // 在庫ゼロ → 強めの値上げ（現行仕様）
        new_price = long long(double(price) * (1 + producer_price_alpha));

    } else {
        // 平均価格に向けて双方向に調整
		//price<price_aveの時はdif>0　　new_priceは価格をあげる、price>price_aveの時はdif<0, new_priceは価格を下げる、
        double diff = double(price_ave) - double(price);
        new_price = long long(double(price) + producer_price_alpha * diff);
    }

} else {
    // 市場に商品がない → 値上げ（現行仕様）
    new_price = long long(double(price) * (1 + producer_price_alpha));
}
*/
//	  fprintf(app_context.config->out_ave_price, "%d,%d,size=,%d,%I64d,%I64d,%I64d,\n", app_context.period_count, agent_id, size, price, price_ave, new_price);
  //fprintf(app_context.config->out_trace_supply_sell, "period=,%d,agent_id=,%d,item_type_id=,%d, price=,%I64d,price_ave=,%I64d,new_price=,%I64d,size=,%d,critical_stock=,%d,producer_price_alpha=%lf,\n ",
//	  app_context.period_count, get_agent_id(), item_type_id, price, price_ave, new_price, size, critical_stock, producer_price_alpha);


// ---------------------------------
		//ロジック変更
		/*
			if (price_ave > 0) {
//				if (size = 0) {
				if (size <= critical_stock) {
				//	if (price < price_ave) new_price = long long(double(price) * (1 + producer_price_alpha));
					//if (price < price_ave) {
					  if(price<critical_price_low){
						//if(size==0 || size<get_stocks(app_context.period_count-1,item_type_id))
							new_price = long long(double(price) * (1 + producer_price_alpha));
							if (get_agent_id() == 102) fprintf(app_context.config->out_renewprice_test17, "period=,%d,agent_id=,%d,size=,%d,price=,%I64d,price_ave=,%I64d,new_price=,%I64d,\n", app_context.period_count,  get_agent_id(),size, price, price_ave,new_price);
					  }//if(price<critical_price_low)の終わり
					else {
						new_price = price;
						if (get_agent_id() == 102) fprintf(app_context.config->out_renewprice_test17, "period=,%d,agent_id=,%d,size=,%d,price=,%I64d,price_ave=,%I64d,new_price=,%I64d,\n", app_context.period_count, get_agent_id(), size,price, price_ave, new_price);
					}
					  fprintf(app_context.config->out_trace_supply_sell, "period=,%d,agent_id=,%d, price=,%I64d,price_ave=,%I64d,new_price=,%I64d,size=,%d,critical_stock=,%d,price_ave>0,\n ",
						  app_context.period_count, get_agent_id(),price, price_ave, new_price, size, critical_stock);
				}//if (size <critical_stock)の終わり
				else if (size > get_qmax(item_type_id) - critical_stock) {
					//if (price > price_ave) {
					  if(price>critical_price_high){
						//if(size==qmax || size>get_stocks(app_context.period_count-1,item_type_id))
						new_price = long long(double(price) * (1 - producer_price_alpha));
						if (get_agent_id() == 102) fprintf(app_context.config->out_renewprice_test17, "period=,%d,agent_id=,%d,size=,%d,price=,%I64d,price_ave=,%I64d,new_price=,%I64d,\n", app_context.period_count,  get_agent_id(),size, price, price_ave,new_price);
					}
					else {
						new_price = price;
						if (agent_id == 102) fprintf(app_context.config->out_renewprice_test17, "period=,%d,agent_id=,%d,size=,%d,price=,%I64d,price_ave=,%I64d,new_price=,%I64d,\n", app_context.period_count, get_agent_id(),size, price, price_ave, new_price);
					}
				}//else if (size > get_qmax(item_type_id) - critical_stock)の終わり
				else new_price = price;
				fprintf(app_context.config->error_message, "period=,%d,price=,%I64d,item_type_id=,%d,price_ave=,%I64d,new_price=,%I64d,size=,%d,,critical_stock=,%d,\n ",
					app_context.period_count,price,item_type_id,price_ave,new_price,size,critical_stock);
	
			}//if (price_ave > 0)の終わり
			else {
				new_price = price;
				fprintf(app_context.config->out_trace_supply_sell, "period=,%d,item_type_id=,%d,agent_id=,%d, price=,%I64d,price_ave=,%I64d,new_price=,%I64d,size=,%d,critical_stock=,%d,\n ",
					app_context.period_count, item_type_id,get_agent_id(), price, price_ave, new_price, size, critical_stock);

			}
			*/
		}//if(app_context.period_count==0){} else { の終わり

		return new_price;
	}
	void define_size_of_stock_in_market() {
		//Enterprise.hのdo_kishu()で呼ばれる（agent_type==2131 || 2132の場合のみ）
		if (get_agent_type() == 2131 || get_agent_type()==2130) {
			//size_of_stock_in_market = app_context.retail_market_navi_for_customer->get_retail_market(get_agent_id(), item_type_id)->get_size_of_stock_in_market(item_type_id, get_agent_id());
			size_of_stock_in_market = app_context.retail_market_navi_for_customer->get_retail_market(item_type_id)->get_size_of_stock_in_market(item_type_id, get_agent_id());
		}
		else if (get_agent_type() == 2132) {
			//	printf("before size_of_stock_in_market in the void define_size\of_stock_in_market() in producer.h\n");
			//size_of_stock_in_market = app_context.material_market_navi_for_retailer->get_retail_market(get_agent_id(), item_type_id)->get_size_of_stock_in_market(item_type_id, get_agent_id());
			size_of_stock_in_market = app_context.material_market_navi_for_retailer->get_retail_market(item_type_id)->get_size_of_stock_in_market(item_type_id, get_agent_id());

		}
		bookKeeper->set_size_of_stock_in_market(size_of_stock_in_market);
	}
	void define_ave_price_in_market() {
		//Areamarketで定義しているのでproducerでは必要ない。
		//market

		//毎期首に市場の平均価格(自分が製造するitem_type_idの製品の）を格納しておく。
		//SimulatorAppの期首にAreaMarketの関数として期首に計算される。それをproducerの属性変数として格納しておく
		//Enterprise.hのdo_kishu()で呼ばれる（agent_type==2131 || 2132の場合のみ）
		//ave_price_in_market is the average of price in market at_kishu(i.e. before production)
		// 20240215下記をget_ave_price(item_type_id)に変更する
		
		int market_type = 0;
		int period = app_context.period_count;
		if (get_agent_type() == 2131 || get_agent_type()==2130) {
			market_type = 1;
			//ave_price_in_market = app_context.retail_market_navi_for_customer->get_retail_market(get_agent_id(), item_type_id)->get_ave_price(market_type, item_type_id);
			ave_price_in_market = app_context.retail_market_navi_for_customer->get_retail_market( item_type_id)->get_ave_price(item_type_id);
		//	if (period == 21) fprintf(app_context.config->out_test12, "period=,%d,agent_id=,%d,market_type=,%d,ave_price_in_market=,%I64d,item_type_id=,%d,inside define ave_price in market() by producer,\n", app_context.period_count, get_agent_id(), market_type, ave_price_in_market, item_type_id);
		}
		else if (get_agent_type() == 2132) {
			market_type = 2;
			//ave_price_in_market = app_context.material_market_navi_for_retailer->get_retail_market(get_agent_id(), item_type_id)->get_ave_price(market_type, item_type_id);
			ave_price_in_market = app_context.material_market_navi_for_retailer->get_retail_market(item_type_id)->get_ave_price(item_type_id);
		//	if (period == 21) fprintf(app_context.config->out_test12, "period=,%d,agent_id=,%d,market_type=,%d,ave_price_in_market=,%I64d,item_type_id=,%d,inside define ave_price in market() by producer,\n", app_context.period_count, get_agent_id(), market_type, ave_price_in_market, item_type_id);
		//	if (period == 351 || period == 352 || period == 353 || period == 354) fprintf(app_context.config->out_test12, "period=,%d,agent_id=,%d,market_type=,%d,ave_price_in_market=,%I64d,item_type_id=,%d,inside define ave_price_in market,\n", app_context.period_count, get_agent_id(), market_type, ave_price_in_market, item_type_id);
		}
	
	//	ave_price_in_market=get_ave_price(item_type_id);
		bookKeeper->set_ave_price_in_market(ave_price_in_market);
		//fprintf(app_context.config->out_ave_price, "inside define_ave_price_in market,period=,%d,agent_id=,%d, item_type_id=,%d,ave_price_in_market=,%I64d,\n", app_context.period_count, get_agent_id(), item_type_id, ave_price_in_market);

	}
	void renew_price_in_market(long long new_price,int item_type_id){
		//producerのrenew_price(long long new_price)で呼ばれる(new_price!=priceの時のみ）
		//AreaMarketに出荷されたItemの価格を改定する
		set<Item*, PredForItemBySeq>* stocks = get_item_set(item_type_id);
		//fprintf(app_context.config->out,"in print_stocks producer %d stock_size=%d\n",get_agent_id(),stocks->size());
		set<Item*, PredForItemBySeq> ::iterator it;
		/**価格改定*/
		int nt=0;int ng=0;
		long long new_price0 = 0;
		for (it = stocks->begin(); it != stocks->end(); it++)
		{
			//価格改定をItem経由で行う
			nt++;
			/*
			if (nt == stocks->size())
				fprintf(app_context.config->out2, "period_count=%d price=%I64d seller_id=%d new_price=%I64d nt=%d価格付け替えます\n", app_context.period_count, (*it)->get_price(), (*it)->Get_seller()->Get_agent_id(), new_price, nt);
		*/
		long long old_price = (*it)->get_price();
			if (price_delta == 0) new_price0 = new_price;
			else  new_price0 = long long(RandomGenerator::random_double(double(new_price) * (1.0 - price_delta), double(new_price) * (1.0 + price_delta)));
			//double new_price0 = RandomGenerator::random_double(double (new_price) * 0.95, double (new_price) * 1.05);
		    //(*it)->Get_destination()->notify_price_changed((*it), new_price);
			(*it)->Get_destination()->notify_price_changed((*it), new_price0);
			if (new_price > old_price) {
				/*	(*it)->Get_seller()->createjournalSlip(app_context.period_count, 0, "製品在庫", new_price - old_price, "価格改定", new_price - old_price, "価格改定差益", 1);
					(*it)->Get_seller()->createjournalSlip(app_context.period_count, 1, "価格改定差益", new_price - old_price, "価格改定", new_price - old_price, "売上", 3);
			*/
			//20230717	(*it)->Get_seller()->createjournalSlip(app_context.period_count, 0, "製品在庫", new_price - old_price, "価格改定", new_price - old_price, "減耗費",1);
			//	(*it)->Get_seller()->createjournalSlip(app_context.period_count, 1, "価格改定差益", new_price - old_price, "価格改定", new_price - old_price, "売上", 3);
			}
			else {

				/*
				(*it)->Get_seller()->createjournalSlip(app_context.period_count,1, "価格改定差損", new_price - old_price, "価格改定", new_price - old_price, "製品在庫", 0);
				(*it)->Get_seller()->createjournalSlip(app_context.period_count,3, "売上", new_price - old_price, "価格改定", new_price - old_price, "価格改定差損", 1);
				*/
				//20230717(*it)->Get_seller()->createjournalSlip(app_context.period_count,1,"減耗費",old_price-new_price, "価格改定", old_price-new_price, "製品在庫",0);
			}
			
		}
		//fprintf(app_context.config->out2,"period_count=%d price=%d seller_id=%d new_price=%d nt=%d個　価格付け替えます\n",app_context.period_count,(*it)->get_price(),(*it)->Get_seller()->Get_agent_id(),new_price,nt);
		
              // cout << (*it)->get_item_type_id() << "-" << (*it)->get_price() << endl;
			 //  fprintf(app_context.config->out," %d  - %d seller id=%d\n",(*it)->get_item_type_id(),(*it)->get_price(),(*it)->Get_seller()->Get_agent_id());		
	}
	void createjournalSlip(int period, int Dr_code, string Dr_title, long long dabit, string description, long long credit, string Cr_title, int Cr_code) {
		bookKeeper->createjournalSlip(period, Dr_code, Dr_title, dabit, description, credit, Cr_title, Cr_code);
		//bookKeper->createjornalSlipの第２引数がDr_codeであるべきところがCr_codeとなっていたので修正した。20230607
		//この関数は減耗費の仕訳のみに使われている。
	}
    /** 経費処理 */
    void process_payment() {
		//pay_wages();
	}

    /** 設備投資 */
    void process_expanding_scale() {}

    /** 生産 */
    void create_products(int q_count) {
        //一度に複数個出荷できるように改訂（2012_3_30、白木氏作成）
        // ダミー出荷
		//item_type_id=10;
	//	printf("start of  create_products agent_id=%d agent_type=%d \n",get_agent_id(),get_agent_type());
			int item_count = q_count;
			int seller_type = 0;
			vector<Item*> items = vector<Item*>(item_count);//vectorの大きさが分かっている時はpush_backは使わない　　//生産計画量分のベクタを確保する  20120524記　高島
		//	if (agent_id == 509)fprintf(app_context.config->out509, "period=,%d,q_count=,%d,item_count=,%d,items.size()=,%d,inside creates_products(),\n", app_context.period_count, q_count, item_count, items.size());
			if (q_count > 0){
		//	fprintf(app_context.config->out, "time= %d,producer %d が製品%d個出荷します\n", app_context.period_count, get_agent_id(), item_count);
			//fprintf(app_context.config->out_temporary, "time= %d,producer %d agent_type=%d item_type_id=%d が製品%d個出荷します\n", app_context.period_count, get_agent_id(), Get_AgentType(),item_type_id,item_count);
			seller_type = Get_AgentType();
			long long price_max; long long price_min; long long price0;
			if (get_agent_type() == 2130) {
		//		printf("inside creat_products, q_count=%d agent_id=%d", q_count, get_agent_id());
			}
			//double price_delta;producer.hの属性変数に移動2024年３月5日　荻林
			//if (get_agent_type() == 2131) price_delta = app_context.config->retailer_price_delta;
			//else if (get_agent_type() == 2132) price_delta = app_context.config->wholesaler_price_delta;
			//price_deltaの定義はproducer.hの関数として宣言するvirtual void define_price_delta()=0;実装は各種生産者
			//他の生産者タイプをモデルに入れる場合にはここで定義すること
			//将来は各producerタイプのset_up()で定義する方式に変更した2024年３月５日
		//	fprintf(app_context.config->out_test14, "period=,%d,agent_id=,%d,price_delta=,%lf,price=,%I64d,inside create products,\n", app_context.period_count,get_agent_id(), price_delta,price);
			for (int i = 0; i < item_count; i++) {
				if(price_delta==0) price0=price;
				else price0 = long long (RandomGenerator::random_double(double(price) * (1.0 - price_delta), double(price) * (1.0 + price_delta)));			
				//price0 = long long(RandomGenerator::random_double(double(price) * 0.95, double(price) *1.05));
				//if (Get_Agent_type() == 2131)
				//items[i] = new Item(item_type_id, price);		
				//price_delta>0なら価格を前後price_delta*100%ずつ振らす
				items[i] = new Item(item_type_id, price0);
				//アイテムを生成する、生成時にコンストラクタでIDと価格を格納  20120524記　高島
				//else items[i] = new Item(item_type_idk, price);
						//fprintf(app_context.config->out,"item_type_id=%d price=%d\n",item_type_id,price);
			//	fprintf(app_context.config->out_trace_supply_sell, "period=,%d,agent_id=,%d,item_index=,%d, item_type_id=,%d, price=,%I64d,inside create products for loop,\n",
			//		app_context.period_count, get_agent_id(), i, item_type_id, price0);
				}
		//	if (get_agent_id() == 540) fprintf(app_context.config->out_test, "period=,%d,item_count=,%d, price=,%I64d, item_type_id=,%d,\n", app_context.period_count, item_count, items[0]->get_price(), items[0]->get_item_type_id());

			//if(Get_Agent_type()==2131)
				Shipment* shipment = shipment_navigator->get_shipment(agent_id, item_type_id);		//現在のshipmentはsinglemarketshipment　  20120523記　高島
			//else Shipment* shipment = shipment_navigator->get_shipment(agent_id, item_type_idk);
				shipment->ship(items, this);//関数shipはSingleMarketShipmentで実装している
			//	fprintf(app_context.config->out_trace_supply_sell, "time=, %d,producer, %d, が製品,%d,個出荷しました, item_type_id=,%d,price0=,%lld,\n", app_context.period_count, get_agent_id(), item_count,item_type_id,price0);


			/*
			if(seller_type==2131) fprintf(app_context.config->out_temporary,"retailer_id=%d agent_type=%d が商品を出荷しました、item_type_id=%d出荷先（shipment）=%x、shipment_navi_for_retailer=%x\n",get_agent_id(),get_agent_type(),item_type_id,shipment,app_context.shipment_navi_for_retailer);
			else if(seller_type==2132) fprintf(app_context.config->out_temporary, "wholesaler_id=%d agent_type=%d が商品を出荷しました、item_type_id=%d 出荷先（shipment）=%x、shipment_navi_for_wholeseller=%x\n", get_agent_id(), get_agent_type(),item_type_id,shipment, app_context.shipment_navi_for_retailer);
			*/
			
			}//if(q_count>0)の終わり
    /*
			if (get_agent_id() == 489 || get_agent_id() == 490 || get_agent_id() == 491) {
				fprintf(app_context.config->out_test, "inside create_products(), period =, %d, agent_id =, %d, q_count =, %d,items.size()=, %d,\n ",
					app_context.period_count, get_agent_id(), q_count, items.size());
			}
	*/
		//printf("before_accoungint teatment\n");
		//printf("before accounting treatment at production(items) in create products(int q_count) agent_id=%d agent_type=%d \n",get_agent_id(),get_agent_type());
			//if (agent_id == 509) fprintf(app_context.config->out509, "period=,%d, items.size() =,%d, inside_accounting_treatmen_at_production, \n,",app_context.period_count, items.size());
			accounting_treatment_at_production(items);
		//printf("after accounting treatment at production(items) period_cont=%d agent_id=%d agent_type=%d \n\n",app_context.period_count,get_agent_id(),get_agent_type());
		items.clear();
		
    }
//virtual void accounting_treatment_at_production(vector<Item*> items) {	}
void transferstocks_to_StockRecord(){
	//printf("period=%d agent_id=%d 
	// 
	// 
	// inside producer transferstocks_to_stockrecord\n",app_context.period_count,get_agent_id());
		//関数呼び出しはEnterprizeのdo_kimatsu()
		set<Item*, PredForItemBySeq>* stocks = get_item_set(item_type_id);
		set<Item*, PredForItemBySeq> ::iterator it;
		for ( it=stocks->begin() ; it != stocks->end(); it++ ){
   		//   fprintf(app_context.config->out," %d  - %d seller id=%d\n",(*it)->get_item_type_id(),(*it)->get_price(),(*it)->Get_seller()->Get_agent_id());
		   get_dataAnalyzer()->set_stockItem(app_context.period_count,(*it)->get_item_type_id(),int ((*it)->get_price()),(*it)->Get_seller()->Get_agent_id());
		//dataAnalyzer->set_stockItem(app_context.period_count,(*it)->get_item_type_id(),(*it)->get_price(),(*it)->Get_seller()->Get_agent_id());
		
		}
	}
	int get_qmax(int item_type_id){
		return qmax;
	}
	void set_soldItem(int period, int item_type_id, long long price){
		//2012_6_3荻林作成、売れた製品情報をproducerに持たせる
		soldItem =new SoldItem;
		soldItem->period=period;
		soldItem->item_type_id=item_type_id;
		soldItem->price=price;
		memSold.push_back(soldItem);
	}
public:
	int Get_Agent_type() { return agent_type; }
	long long get_ave_price_in_market() {
		//if(app_context.period_count==21) fprintf(app_context.config->out_test12, "inside get_ave_price_in_market() of producer.h \n");
		return ave_price_in_market;
	}
	void pay(Item* item) {}
 /*   Producer(int agent_id, int producer_type)
      : Enterprise(agent_id), producer_type(producer_type) {}*/
	Producer(int agent_id, int agent_type)
      : Enterprise(agent_id, agent_type) {}
    virtual ~Producer() {}

    void set_shipment_navigator(ShipmentNavigator* shipment_navigator) {
        this->shipment_navigator = shipment_navigator;
    }
	void create_SbsHistory(int item_type_id,int number_stocks){
//		void create_SbsHistory(int item_type_id, int number_supplied, int number_bought, int number_stocks, int price) {
		sbsHistory=new SbsHistory;
		sbsHistory->period=app_context.period_count;
		sbsHistory->item_type_id=item_type_id;
		//sbsHistory->number_bought;
		//sbsHistory->number_supplied;
		sbsHistory->number_stocks = number_stocks;
		//sbsHistory->price;
		memHistory.push_back(sbsHistory);
		/*
		if (get_agent_id() == 150) {
			fprintf(app_context.config->out_size_test16, "agent_id=,%d,item_type_id=.%d,period=,%d, nuber_stocks=,%d,", get_agent_id(), item_type_id, app_context.period_count, number_stocks);
			if (app_context.period_count > 0) fprintf(app_context.config->out_size_test16, "previoous_size=,%d,\n", get_stocks(app_context.period_count - 1, item_type_id));
			else fprintf(app_context.config->out_size_test16, "\n");
		}
		*/
	}
	int get_stocks(int period, int item_type_id) {
		//periodの期末在庫数を返す
		int size = memHistory.size();
		int stock = 0;
		if (size > 0) {
			for (int i = 0; i < size; i++) {
				if (memHistory[i]->period == period && memHistory[i]->item_type_id == item_type_id) stock = memHistory[i]->number_stocks;
			}
		}
		else stock = 0;
		return stock;
	}


    /** 行動 */
   /* void action() {
      //  printf("action -- producer %d\n",get_agent_id());
		fprintf(app_context.config->out,"period=%d action -- producer %d\n",app_context.period_count,get_agent_id());
       
	
	}*/
	
    /**
     *  支払金額受領
     *
     *  @param item 商品
     */
	/*
    void receive_payments(Item* item) {
       // printf("receive_payments -- producer\n");　　//各子クラスに移した荻林20220305
	
        // TODO
		total_sales+=item->get_price();
        cash += item->get_price();
		int  balance=bookKeeper->get_cashbookslip()->balance;
		bookKeeper->createcashBookSlip(app_context.period_count,item->get_price(),0,balance,item->get_buyer()->get_agent_id());
		fprintf(app_context.config->out,"receive_payments -- producer %d for item_type=%d price=%d resultant cash=%d\n",get_agent_id(),item->get_item_type_id(),item->get_price(),cash);
		fprintf(app_context.config->out,"bookKeeper経由 income=%d spending=%d balance_before=%d balance=%d\n",bookKeeper->get_cashbookslip()->income,bookKeeper->get_cashbookslip()->spending,balance,bookKeeper->get_cashbookslip()->balance);
		fprintf(app_context.config->out_producer_consumer_income_expenses,"bookKeeper of producer 経由, period=, %d, agent_id=, %d, income=, %d, spending=, %d, balance=, %d, agent_id=, %d,\n",app_context.period_count,get_agent_id(),bookKeeper->get_cashbookslip()->income,bookKeeper->get_cashbookslip()->spending,bookKeeper->get_cashbookslip()->balance,item->get_buyer()->get_agent_id());
	}
	*/
    /**
     *  在庫追加
     *
     *  @param item 商品
     */
    void add_stock(Item* item) {
		//SingleMarketShipmentから呼び出される（出荷のたびに）
        set<Item*, PredForItemBySeq>* stocks = get_item_set(item->get_item_type_id());
        stocks->insert(item);
    }

    /**
     *  在庫削除
     *
     *  @param item 商品
     */
    void remove_stock(Item* item){
		//Itemから呼び出される
        set<Item*, PredForItemBySeq>* stocks = get_item_set(item->get_item_type_id());
        stocks->erase(item);
	//	fprintf(app_context.config->out,"period=%d,商品が購入されたことにより生産者の在庫stocksからitemが1個eraseされました。producer %d seller_id= %d stock_size=%d item_type_id=%d,\n",app_context.period_count,get_agent_id(),item->Get_seller()->Get_agent_id(),stocks->size(),item->get_item_type_id());
    }
	void initilize_qcount() {

	}
	void other_kimatsu_actions(){
		create_SbsHistory(item_type_id,get_number_of_stocks(item_type_id));
		//kessan以外の期末アクション
		transferstocks_to_StockRecord();
	}


};