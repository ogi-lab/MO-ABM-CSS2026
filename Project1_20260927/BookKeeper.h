#pragma once
#include <string>
#include "Item.h"
#include "PredForItemBySeq.h"
#include "RetailMarket.h"
#include "RetailMarketNavigator.h"
#include "AccountData.h"
#include "SimulatorConfig.h"
using namespace std;
struct journalSlip{ // 仕訳帳の一行分
	int period = 0;
	long long dabit = 0;//借方金額
	long long credit = 0;//貸方金額
	string Dr_title;//借方科目
	string Cr_title;//貸方科目
	int Dr_code = 0;//借方科目区分
	int Cr_code = 0;//貸方科目区分
	string description; //摘要

};
struct accountTitle {//勘定科目
	int period = 0;
	string title;//勘定科目
	long long Dr_total = 0;//借方トータル
	long long Cr_total = 0;//貸方トータル
	long long opening_balance = 0;//期首残高
	long long balance = 0;//期末残高
	int code = 0;  //0 資産科目、1 経費科目 2 負債科目　3 売上科目
};
struct cashBookSlip{ //現金出納帳の一行分
	int period = 0;
	long long previous_balance = 0;
	long long income = 0;
	long long spending = 0;
	long long balance = 0; //残高
	//int agent_id; // 取引相手の id 

};
struct bankBookSlip{ //預金通帳
	int period = 0;
	long long previous_balance = 0;
	long long deposit = 0; //預け入れ
	long long withdrawal = 0; //引き出し
	long long balance = 0; //残高
};
struct stockBookSlip {// 在庫帳
	int period = 0;
	long long previouos_balance = 0;
	long long production = 0;//生産個数
	long long bought = 0;//売却個数
	//int production_price;//生産価格
	//int bought_price;//売上価格
	int item_type_id = 0;
	long long balance = 0;//個数在庫
	//int balance_price;//価格在庫
	long long price = 0;
};
struct materialBookSlip {// 原料帳
	int period = 0;
	long long previouos_balance = 0;
	long long production = 0;//生産個数
	long long bought = 0;//売却個数
	//int production_price;//生産価格
	//int bought_price;//売上価格
	int item_type_id = 0;
	long long balance = 0;//個数在庫
	//int balance_price;//価格在庫
	long long price = 0;
};
struct stockBookTotalSlip {
	int period = 0;
	long long previouos_balance = 0;
	long long production = 0;//生産個数
	long long bought = 0;//売却個数
	//int production_price;//生産価格
	//int bought_price;//売上価格
	int item_type_id = 0;
	long long balance = 0;//個数在庫
	//int balance_price;//価格在庫
	long long price = 0;

};
struct materialBookTotalSlip {
	int period = 0;
	long long previouos_balance = 0;
	long long production = 0;//生産個数
	long long bought = 0;//売却個数
	//int production_price;//生産価格
	//int bought_price;//売上価格
	int item_type_id = 0;
	long long balance = 0;//個数在庫
	//int balance_price;//価格在庫
	long long price = 0;

};
struct stockpriceBookSlip {//在庫帳、金額ベース
	int period = 0;
	long long previous_balance = 0;
	long long production_price = 0;//生産価格
	long long bought_price = 0;//売上価格
	int item_type_id = 0;
	long long balance_price = 0;//価格在庫
};
struct materialpriceBookSlip {//原料帳、金額ベース
	int period = 0;
	long long previous_balance = 0;
	long long production_price = 0;//生産価格
	long long bought_price = 0;//売上価格
	int item_type_id = 0;
	long long balance_price = 0;//価格在庫
};
struct genkaBookSlip {
	int period = 0;
	long long direct_labor_cost = 0;
	long long direct_material_cost = 0;
	long long genka = 0;
	int item_type_id = 0;
	int size_shiped = 0;
};


struct trialBalanceSlip {//試算表
	int period = 0;
	string title;//勘定科目
	long long Dr_total = 0;//各勘定科目毎の借方トータル(各期、各勘定科目毎のトータル）
	long long Cr_total = 0;//各勘定科目毎の貸方トータル
	int code = 0;		//科目コード
	long long opening_balance = 0;//期首残高
	long long closing_balance = 0;//期末残高
};
struct GLSlip {
	vector <accountTitle*> title_list;
};
struct previous_balance {
	int period = 0;
	string title;
	long long balance = 0;
};
class BookKeeper {			//AgentAggregatorの中で各Agentに生成される
	int agent_id = 0;
	int agent_type = 0;
	int item_type_id = 0;//生産する品種番号
	long long ave_price_in_market = 0;//生産する品種番号のマーケット内平均価格
	int size_of_stock_in_market = 0;//生産する品種番号の期首在庫
	long long initial_cash = 0;
	long long initial_deposit = 0;
	long long profit = 0;//当期利益
	struct cashBookSlip* cashbookslip;
	vector <cashBookSlip*> cashBook;//現金出納帳、
	vector <cashBookSlip*> cashBookTotal;//期毎に入金、出金の値を集計したもの、残高は期末の値
	struct bankBookSlip* bankbookslip;  //預金通帳の各入力行
	vector <bankBookSlip*> bankBook;    //預金通帳
	vector <bankBookSlip*> bankBookTotal;

	struct stockBookSlip* stockbookslip;
	vector <stockBookSlip*> stockBook;//在庫帳
	vector <stockBookSlip*> stockBookTotal;//期毎に出荷、売却の値を集計したもの、残高は期末の在庫値

	struct stockpriceBookSlip* stockpricebookslip;
	vector < stockpriceBookSlip* > stockpriceBook;//在庫帳金額ベース
	vector <stockpriceBookSlip*> stockpriceBookTotal;//期毎に出荷、売却の金額ベース値を集計したもの、残高は期末の金額ベース在庫値
	
	struct materialBookSlip* materialbookslip;
	vector <materialBookSlip*> materialBook;//在庫帳
	vector <materialBookSlip*> materialBookTotal;//期毎に出荷、売却の値を集計したもの、残高は期末の在庫値
	struct materialpriceBookSlip* materialpricebookslip;
	vector < materialpriceBookSlip* > materialpriceBook;//在庫帳金額ベース
	vector <materialpriceBookSlip*> materialpriceBookTotal;//期毎に出荷、売却の金額ベース値を集計したもの、残高は期末の金額ベース在庫値

	struct genkaBookSlip* genkabookslip;
	vector <genkaBookSlip*>genkaBook;//原価管理表
	vector <genkaBookSlip*> genkaBookTotal;

	struct journalSlip* journalslip;   //仕訳伝票
	vector <journalSlip*> journal;      //仕訳帳
	struct accountTitle* account_title;//勘定科目構造体
	vector <accountTitle*> title_list; //勘定科目一覧（当該エージェント関連のもののみ）
	//vector<accountTitle*> title_list;
	struct GLSlip* GLslip;
	vector<GLSlip*> GLslip_list;//各period毎のtitle_listのvectorを格納
	struct trialBalanceSlip* trialBalanceslip;//試算表（各勘定科目毎の値）
	vector<trialBalanceSlip*> TB_table;//同一期内試算表のベクトル、勘定科目分だけある。
	struct previous_balance* pr_balance;//直前の残高
	vector <previous_balance*> PR_balance;//勘定科目毎にperiodにおける残高の値
public:
	BookKeeper(int agent_id, int agent_type) {
		this->agent_id = agent_id;
		this->agent_type = agent_type;
		//journalslip = new journalSlip;
		//journalslip->Dr_title = "預金";
		//fprintf(app_context.config->out_test, "agent_id=,%d,%s,\n", get_agent_id(), journalslip->Dr_title.c_str());
	}
	void set_ave_price_in_market(long long ave_price) { ave_price_in_market = ave_price; }
	void set_size_of_stock_in_market(int size_of_stock) {
		size_of_stock_in_market = size_of_stock;
	}
	int get_size_of_stock_in_market() { return size_of_stock_in_market; }
	long long get_ave_price_in_market() { return ave_price_in_market; }

	void set_initia_cash(long long  cash) { initial_cash = cash; }
	long long get_initial_cash() { return initial_cash; }
	void set_initial_deposit(long long deposit) { initial_deposit = deposit; }
	long long get_initial_deposit() { return initial_deposit; }
	void set_profit(long long  profit) {
		//期末に試算表から計算した当期利益を変数profitに格納する
		this->profit = profit;
	}
	long long get_profit() {
		//前期末の当期利益の額を取り出す
		return profit;
	}

	int get_agent_id() { return agent_id; };
	int get_agent_type() { return agent_type; }
	int get_item_type_id(){ return item_type_id; }
	void set_agent_type(int agent_type) { this->agent_type = agent_type; }
	void set_item_type_id(int item_type_id) { 
		this->item_type_id = item_type_id; 
	//	fprintf(app_context.config->out3, "agent_id=%d agent_type=%d item_type_id=%d \n", get_agent_id(), get_agent_type(), item_type_id);
	}
	void create_PR_balance(int period,string title, long long balance) {
		//createTBsSlipでTB_talbeにTBSlipを格納したのちに呼び出される、呼び出されるのは毎期末
		//balance：期末バランス
		pr_balance = new previous_balance;
		pr_balance->period = period;
		pr_balance->title = title;
		pr_balance->balance = balance;//期末残高
		PR_balance.push_back(pr_balance);
		
	}
	long long get_cashBalance(int period) {
		unsigned int size = cashBookTotal.size();
		long long balance = 0;
		unsigned int n=0;
		for (unsigned int i = 0; i < size; i++) {
			if (cashBookTotal[i]->period == period) {
				balance = cashBookTotal[i]->balance;
				n++; break;
			}
		}
		if (n > 0)	return balance;
		else {
			//毎期首に前期繰越を記帳するようにすればこのケースは起こらない
			if (period > 0) return get_cashBalance(period - 1);
			else return 0;//period=0ではcashBookが空ということはありえないので、このケースは起こらない
		}
	}

	long long get_bankBalance(int period) {
		unsigned int size = bankBookTotal.size();
		long long balance = 0;
		unsigned int n = 0;
		for (unsigned int i = 0; i < size; i++) {
			if (bankBookTotal[i]->period == period) {
				balance = bankBookTotal[i]->balance; 
				n++; break;
			}
		}
		if (n > 0)	return balance;
		else {
			//毎期首に前期繰越を記帳するようにすればこのケースは起こらない
			if (period > 0) return get_bankBalance(period - 1);
			else return 0;//period=0ではcashBookが空ということはありえないので、このケースは起こらない
		}
	}
	long long get_stockBalance(int period) {
		unsigned int size = stockBookTotal.size();
		long long balance = 0;
		unsigned int n = 0;
		for (unsigned int i = 0; i < size; i++) {
			if (stockBookTotal[i]->period == period) {
				balance = stockBookTotal[i]->balance;
				n++; break;
			}
		}
		if (n > 0)	return balance;
		else {
			//毎期首に前期繰越を記帳するようにすればこのケースは起こらない
			if (period > 0) return get_stockBalance(period - 1);//生産がなく、在庫分の売上も全くないケース
			else return 0;//period=0ではcashBookが空ということはありえないので、このケースは起こらない
		}
	}
	long long get_stockpriceBalance(int period) {
		unsigned int size = stockpriceBookTotal.size();
		long long balance = 0;
		unsigned int n = 0;
		for (unsigned int i = 0; i < size; i++) {
			if (stockpriceBookTotal[i]->period == period) {
				balance = stockpriceBookTotal[i]->balance_price;
				n++; break;
			}
		}
		if (n > 0)	return balance;
		else {
			//毎期首に前期繰越を記帳するようにすればこのケースは起こらない
			if (period > 0) return get_stockpriceBalance(period - 1);//生産がなく、在庫分の売上も全くないケース
			else return 0;//period=0ではcashBookが空ということはありえないので、このケースは起こらない
		}
	}
	//-------------------
	long long get_materialBalance(int period) {
		unsigned int size = materialBookTotal.size();
		long long balance = 0;
		unsigned int n = 0;
		for (unsigned int i = 0; i < size; i++) {
			if (materialBookTotal[i]->period == period) {
				balance = materialBookTotal[i]->balance;
				n++; break;
			}
		}
		if (n > 0)	return balance;
		else {
			//毎期首に前期繰越を記帳するようにすればこのケースは起こらない
			if (period > 0) return get_materialBalance(period - 1);//生産がなく、在庫分の売上も全くないケース
			else return 0;//period=0ではcashBookが空ということはありえないので、このケースは起こらない
		}
	}
	long long get_materialpriceBalance(int period) {
		unsigned int size = materialpriceBookTotal.size();
		long long balance = 0;
		unsigned int n = 0;
		for (unsigned int i = 0; i < size; i++) {
			if (materialpriceBookTotal[i]->period == period) {
				balance = materialpriceBookTotal[i]->balance_price;
				n++; break;
			}
		}
		if (n > 0)	return balance;
		else {
			//毎期首に前期繰越を記帳するようにすればこのケースは起こらない
			if (period > 0) return get_materialpriceBalance(period - 1);//生産がなく、在庫分の売上も全くないケース
			else return 0;//period=0ではcashBookが空ということはありえないので、このケースは起こらない
		}
	}
	//----------------------------
	long long get_balance(int period, string title) {
		//期末残高を返す
		int size = PR_balance.size();
		long long balance1=0;
		if (size > 0) {
			for (int i = 0; i < size; i++) {
				if (PR_balance[i]->period == period && PR_balance[i]->title == title) {
					balance1= PR_balance[i]->balance;
					break;
				}
			}
		}
		return balance1;
	/*	int size = PR_balance.size();
		if (size == 0) return 0);
		else {
			for (int i = 0; i < size; i++) {
				//if (PR_balance[i]->title == title) return PR_balance[i]->previous_balance;
				if (PR_balance[i]->title == title) return PR_balance[i]->balance;
			}
		}
		*/
	}
	void print_PR_balance() {
		//createGL_Slip()の中で仕訳帳を毎期末に作成した後によばれる
		AccountData* accountData = NULL;
		if (get_agent_type() == 2131) {  accountData = app_context.config->retailerData; }
		else if (get_agent_type() == 2132) { accountData = app_context.config->wholesalerData; }
		else if (get_agent_type() == 2130) { accountData = app_context.config->soleproprietorData; }
		else {}
		int size = PR_balance.size();
		if (size > 0) {
			for (int i = 0; i < size; i++) {
				if (get_agent_type()==2130 || get_agent_type() == 2131 || get_agent_type() == 2132) {
				fprintf(accountData->out_balance, "inside createGL_Slip after createTB_slip, period=,%d, agent_id=,%d,i=,%d,勘定科目=,%s, balance=,%I64d,\n",
					PR_balance[i]->period, get_agent_id(), i, PR_balance[i]->title.c_str(), PR_balance[i]->balance);
				}
			}
			fprintf(app_context.config->wholesalerData->out_balance, ",\n");
		}
		
	}
	void createjournalSlip(int period, int Dr_code, string Dr_title, long long dabit, string description, long long credit, string Cr_title, int Cr_code)
	{
		journalslip = new journalSlip;
		/*
		if (Dr_title == "減耗費" || Cr_title == "減耗費")
			fprintf(app_context.config->out_test11, "period= %d Dr_code = %d Dr_title = %s Cr_title =%s Cr_code = %d, \n", period, Dr_code, Dr_title.c_str(), Cr_title.c_str(), Cr_code);
		*/
		journalslip->period = period;
		journalslip->Dr_code = Dr_code;
		journalslip->Dr_title = Dr_title;
		journalslip->dabit = dabit;
		journalslip->description = description;
		journalslip->credit = credit;
		journalslip->Cr_title = Cr_title;
		journalslip->Cr_code = Cr_code;
		/*
		if (journalslip->Dr_title == "減耗費") journalslip->Dr_code = 1;
		else if (journalslip->Cr_title == "減耗費")journalslip->Cr_code = 1;
		else {}
		*/
		journal.push_back(journalslip);
		

	}
	void createTBSlip(int period, string title, long long debit_total, long long credit_total, int code, long long balance) {
		//createGL_Slip()で呼ばれる
		// 総勘定元帳を計算する過程で、毎期の仕訳帳に基づいて勘定科目毎に期中の各仕訳データの借方、貸方の合計値を計算し、その合計値に基づいて毎期末に作成される
		//引数のbalanceは期首残高＝period-1期の期末残高
		trialBalanceslip = new trialBalanceSlip;
		trialBalanceslip->period = period;
		trialBalanceslip->title = title;
		trialBalanceslip->Dr_total = debit_total;
		trialBalanceslip->Cr_total = credit_total;
		trialBalanceslip->code = code;
		//期首残高の設定
		trialBalanceslip->opening_balance = balance;
		//期末残高の設定
		if (code == 0 || code == 1) trialBalanceslip->closing_balance = trialBalanceslip->opening_balance + debit_total - credit_total;
		if (code == 2 || code == 3) trialBalanceslip->closing_balance = trialBalanceslip->opening_balance + credit_total - debit_total;
			/*if (period == 0) {
			//期首残高の設定
			trialBalanceslip->opening_balance = 0;
			//期末残高の設定
			if (code == 0 || code==1) trialBalanceslip->closing_balance = trialBalanceslip->opening_balance + debit_total - credit_total;
			if (code == 2 || code==3) trialBalanceslip->closing_balance = trialBalanceslip->opening_balance + credit_total - debit_total;
		}
		else {
			//期首残高の設定
			trialBalanceslip->opening_balance = balance;
			//期末残高の設定
			if (code == 0 || code == 1) trialBalanceslip->closing_balance = trialBalanceslip->opening_balance + debit_total - credit_total;
			if (code == 2 || code == 3) trialBalanceslip->closing_balance = trialBalanceslip->opening_balance + credit_total - debit_total;
			
		}
		*/
		TB_table.push_back(trialBalanceslip);
		if (agent_id == 545) fprintf(app_context.config->wholesalerData->out_balance, "inside create_TB_Slip before create_PR_balance, period=,%d, agent_id=,%d,科目=,%s,opening_balance=,%I64d, closing_balance=,%I64d,\n", 
			trialBalanceslip->period, get_agent_id(),trialBalanceslip->title.c_str(), trialBalanceslip->opening_balance, trialBalanceslip->closing_balance);
		create_PR_balance(trialBalanceslip->period, trialBalanceslip->title, trialBalanceslip->closing_balance);

	}
	/*void createcashBookSlip(int period, int income, int spending, int previous_balance) {
		//引数のbalance：前期末残高
		cashbookslip = new cashBookSlip;
		
		
			cashbookslip->period = period;
			cashbookslip->income = income;
			cashbookslip->spending = spending;
			cashbookslip->balance = previous_balance + income - spending;
			//cashbookslip->agent_id = agent_id;
		cashBook.push_back(cashbookslip);
	}*/
	//void createstockBookSlip(int period, int production, int bought, int production_price, int bought_price,int item_type_id) {
	void createstockBookSlip(int period, long long production, long long bought,  int item_type_id, long long price) {
		stockbookslip = new stockBookSlip;
		long long previous_balance = 0;
		//int previous_balance_price;
		int size = stockBook.size();
		if (size == 0) { 
			previous_balance = 0;
			//previous_balance_price = 0;
		}
		else {
			previous_balance = stockBook[size - 1]->balance;
			//previous_balance_price = stockBook[size]->balance_price;
		}
		stockbookslip->period=period;
		stockbookslip->production = production;//生産個数
		stockbookslip->bought=bought;//売却個数
		//stockbookslip->production_price=production_price;
		//stockbookslip->bought_price;
		stockbookslip->item_type_id = item_type_id;
		stockbookslip->price = price;
		stockbookslip->balance = previous_balance + production - bought;
		//stockbookslip->balance_price = previous_balance_price + production_price - bought_price;
		stockBook.push_back(stockbookslip);

	}
	void creatematerialBookSlip(int period, long long production, long long bought, int item_type_id, long long price) {
		materialbookslip = new materialBookSlip;
		long long previous_balance = 0;
		//int previous_balance_price;
		int size = materialBook.size();
		//printf("inside creatematerialBookSlip size=%d", size);
		if (size == 0) {
			previous_balance = 0;
			//previous_balance_price = 0;
		}
		else {
			previous_balance =materialBook[size - 1]->balance;
			//previous_balance_price = stockBook[size]->balance_price;
		}
		materialbookslip->period = period;
		materialbookslip->production = production;//生産個数
		materialbookslip->bought = bought;//売却個数
		//stockbookslip->production_price=production_price;
		//stockbookslip->bought_price;
		materialbookslip->item_type_id = item_type_id;
		materialbookslip->price = price;
		materialbookslip->balance = previous_balance + production - bought;
		//stockbookslip->balance_price = previous_balance_price + production_price - bought_price;
		//printf("before push back\n");
		materialBook.push_back(materialbookslip);

	}
	void createstockpriceBookSlip(int period, long long production_price, long long bought_price, int item_type_id) {
		stockpricebookslip = new stockpriceBookSlip;
		long long previous_balance_price = 0;
		int size = stockpriceBook.size();
		if (size == 0) 		previous_balance_price = 0;
		else 	previous_balance_price = stockpriceBook[size - 1]->balance_price;
		stockpricebookslip->period = period;
		stockpricebookslip->production_price = production_price;//生産個数金額ベース
		stockpricebookslip->bought_price = bought_price;//売却個数
		stockpricebookslip->item_type_id = item_type_id;
		stockpricebookslip->balance_price = previous_balance_price + production_price - bought_price;
	
		stockpriceBook.push_back(stockpricebookslip);

	}
	void creatematerialpriceBookSlip(int period, long long production_price, long long bought_price, int item_type_id) {
		materialpricebookslip = new materialpriceBookSlip;
		long long previous_balance_price = 0;
		int size = materialpriceBook.size();
		if (size == 0) 		previous_balance_price = 0;
		else 	previous_balance_price = materialpriceBook[size - 1]->balance_price;
		materialpricebookslip->period = period;
		materialpricebookslip->production_price = production_price;//生産個数金額ベース
		materialpricebookslip->bought_price = bought_price;//売却個数
		materialpricebookslip->item_type_id = item_type_id;
		materialpricebookslip->balance_price = previous_balance_price + production_price - bought_price;

		materialpriceBook.push_back(materialpricebookslip);

	}
	void creategenkaBookSlip(int period, int size_shiped, long long direct_labor_cost, long long direct_material_cost, long long genka, int item_type_id) {
		genkabookslip = new genkaBookSlip;
		if (size_shiped > 0) {
			genkabookslip->period = period;
			genkabookslip->direct_labor_cost = direct_labor_cost;
			genkabookslip->direct_material_cost = direct_material_cost;
			genkabookslip->genka = genka;
			genkabookslip->item_type_id = item_type_id;
			genkabookslip->size_shiped = size_shiped;
		}
		else {
			genkabookslip->period = period;
			genkabookslip->direct_labor_cost = 0;
			genkabookslip->direct_material_cost = 0;
			genkabookslip->genka = 0;
			genkabookslip->item_type_id = item_type_id;
			genkabookslip->size_shiped = size_shiped;
		}

		genkaBook.push_back(genkabookslip);
	}
	void createcashBookSlip(int period, long long income, long long spending) {
		cashbookslip = new cashBookSlip;
		long long previous_balance = 0;
		int size = cashBook.size();
		if (size == 0) previous_balance = 0;
		else previous_balance = cashBook[size - 1]->balance;

		cashbookslip->period = period;
		cashbookslip->income = income;
		cashbookslip->spending = spending;
		cashbookslip->balance = previous_balance + income - spending;
		cashBook.push_back(cashbookslip);
	}
	
	void createbankBookSlip(int period,long long deposit,long long withdrawal) {
		bankbookslip = new bankBookSlip;
		long long previous_balance = 0;
		int size = bankBook.size();
		if (size == 0) previous_balance = 0;
		else previous_balance = bankBook[size - 1]->balance;

		bankbookslip->period = period;
		bankbookslip->deposit = deposit;
		bankbookslip->withdrawal = withdrawal;
		bankbookslip->balance = previous_balance + deposit - withdrawal;
		bankBook.push_back(bankbookslip);
	}
	void createcashBookTotal(int period) {
		cashbookslip = new cashBookSlip;
		cashbookslip->income = 0;
		cashbookslip->spending = 0;
		int period_max = cashBook[cashBook.size() - 1]->period;
		int ncon = 0;
		//vector <int> previous_balance;
		//previous_balance[0] = 0;
		long long previous_balance = 0;
		int size = cashBookTotal.size();
		/*
		if (size == 0) previous_balance = 0;
		else previous_balance = cashBookTotal[size - 1]->balance;
		*/
		//previous_balanceの定義を変更20220922
		if (period == 0)previous_balance = 0;
		else previous_balance = get_cashBalance(period - 1);

/*			cashbookslip = new cashBookSlip;
			cashbookslip->income = 0;
			cashbookslip->spending = 0;
			*/
			ncon = 0;
			for (unsigned int i = 0; i < cashBook.size(); i++) {
				if (period == cashBook[i]->period) {
					cashbookslip->income += cashBook[i]->income;
					cashbookslip->spending += cashBook[i]->spending;
					cashbookslip->period = cashBook[i]->period;
					ncon++;
				
				}
			}
			if (ncon == 0) {
				//periodの現金出納帳記載がない場合
				bankbookslip->period = period;
			}
			cashbookslip->balance = previous_balance + cashbookslip->income - cashbookslip->spending;
			cashBookTotal.push_back(cashbookslip);
	}
	void creategenkaBookTotal(int period) {
		//printf("start of for in creategenkaBookTotal genkaBook.size=%d\n", genkaBook.size());
		genkabookslip = new genkaBookSlip;
		//-----------------
		genkabookslip->direct_labor_cost = 0;
		genkabookslip->direct_material_cost =0;
		genkabookslip->genka =0;
		genkabookslip->size_shiped = 0;
		//---------------------
		//int period_max = genkaBook[genkaBook.size() - 1]->period;
		int ncon = 0;
		//vector <int> previous_balance;
		//previous_balance[0] = 0;
		//long long previous_balance;
		//printf("start of for in creategenkaBookTotal genkaBook.size=%d", genkaBook.size());
		int size = genkaBookTotal.size();
		/*
		if (size == 0) previous_balance = 0;
		else previous_balance = cashBookTotal[size - 1]->balance;
		*/
		//previous_balanceの定義を変更20220922
		//if (period == 0)previous_balance = 0;
		//else previous_balance = get_genkaBalance(period - 1);
		ncon = 0;
		//printf("start of for in creategenkaBookTotal genkaBook.size=%d", genkaBook.size());
		for (unsigned int i = 0; i < genkaBook.size(); i++) {
			if (period == genkaBook[i]->period) {
				genkabookslip->direct_labor_cost+= genkaBook[i]->direct_labor_cost;
				genkabookslip->direct_material_cost+= genkaBook[i]->direct_material_cost;
				genkabookslip->genka+= genkaBook[i]->genka;
				genkabookslip->size_shiped += genkaBook[i]->size_shiped;
				genkabookslip->period = genkaBook[i]->period;
				genkabookslip->item_type_id = genkaBook[i]->item_type_id;
				ncon++;

			}
		}
		//printf("end of for in creategenkaBookTotal\n");
		if (ncon == 0) {
			//periodの現金出納帳記載がない場合
			genkabookslip->period = period;
		}
		//genkabookslip->balance = previous_balance + cashbookslip->income - cashbookslip->spending;
		genkaBookTotal.push_back(genkabookslip);
	}
	void createbankBookTotal(int period) {
		bankbookslip = new bankBookSlip;
		bankbookslip->deposit = 0;
		bankbookslip->withdrawal= 0;
		int period_max = bankBook[bankBook.size() - 1]->period;
		int ncon = 0;
		//vector <int> previous_balance;
		//previous_balance[0] = 0;
		long long previous_balance = 0;
		int size = bankBookTotal.size();
		/*
		if (size == 0) previous_balance = 0;
		else previous_balance = bankBookTotal[size - 1]->balance;
		*/
		//previous_balanceの定義を変更20220922
		if (period == 0)previous_balance = 0;
		else previous_balance = get_bankBalance(period - 1);

		/*bankbookslip = new cashBookSlip;
		cashbookslip->income = 0;
		cashbookslip->spending = 0;
		*/
		ncon = 0;
		for (unsigned int i = 0; i < bankBook.size(); i++) {
			if (period == bankBook[i]->period) {
				bankbookslip->deposit += bankBook[i]->deposit;
				bankbookslip->withdrawal += bankBook[i]->withdrawal;
				bankbookslip->period = bankBook[i]->period;
				ncon++;

			}
		}
		if (ncon ==0 ) {
			//periodの預金通帳記載がない場合
			bankbookslip->period = period;
		}
		bankbookslip->balance = previous_balance + bankbookslip->deposit - bankbookslip->withdrawal;
		bankBookTotal.push_back(bankbookslip);
	}
	void createstockBookTotal(int period) {
		//期毎に各出荷と売却を集計した在庫帳
		//fprintf(app_context.config->wholesalerTotal->out_stockBook, "inside BookKeeper->createstockBookTotal, \n");
		//printf("inside createstockBookTotal  size=%d,period =%d,\n", stockBook.size(),period);
		long long previous_balance = 0;
		int size = stockBookTotal.size();
		if (stockBook.size() == 0) {
			stockbookslip = new stockBookSlip;
			stockbookslip->production = 0;
			stockbookslip->bought = 0;
			stockbookslip->period = period;
			stockbookslip->balance = 0;
			stockbookslip->previouos_balance = 0;
			previous_balance = 0;
			stockbookslip->price = 0;
			}
		else {
			stockbookslip = new stockBookSlip;
			stockbookslip->production = 0;
			stockbookslip->bought = 0;
			stockbookslip->price = 0;
			//printf("period=%d, stockBook.size=%d \n", period, stockBook.size());
			int period_max = stockBook[stockBook.size() - 1]->period;
			int ncon = 0;
			//vector <int> previous_balance;
			//previous_balance[0] = 0;
			

			//previous_balanceの定義を変更20220922
			if (period == 0) previous_balance = 0;
			else previous_balance = get_stockBalance(period - 1);


			//fprintf(app_context.config->wholesalerTotal->out_stockBook,"inside BookKeeper->createstockBookTotal agent_id=%d stockBook.size()=%d \n",get_agent_id(),stockBook.size());
			ncon = 0;
			for (unsigned int i = 0; i < stockBook.size(); i++) {
				if (period == stockBook[i]->period) {
					//printf("i= %d stockBook[i]->production=%d", i,stockBook[i]->production);
					stockbookslip->production += stockBook[i]->production;
					stockbookslip->bought += stockBook[i]->bought;
					stockbookslip->period = stockBook[i]->period;
					//if (stockBook[i]->production > 0 || stockBook[i]->bought > 0) 20240310修正{
					if (stockBook[i]->price > 0) {
					stockbookslip->price = stockbookslip->price+stockBook[i]->price;
						ncon++;
					}
					stockbookslip->item_type_id = stockBook[i]->item_type_id;
					/*
					if (stockBook[i]->production > 0 || stockBook[i]->bought > 0) stockbookslip->price = stockBook[i]->price;
					else {
						if(stockBookTotal.size()==0) stockbookslip->price=0;
							else stockbookslip->price = stockBookTotal[stockBookTotal.size() - 1]->price;
					}
					*/
					//ncon++;

				}
			}
			//printf("after for ncon=%d \n",ncon);
			if (ncon == 0) {
				//periodの在庫帳記載がない場合
				stockbookslip->period = period;
			}
			else {
				stockbookslip->price = long long(double(stockbookslip->price) / ncon);
			}
		}
		stockbookslip->balance = previous_balance + stockbookslip->production - stockbookslip->bought;
		stockBookTotal.push_back(stockbookslip);
	}
	//----------------
	void creatematerialBookTotal(int period) {
		//期毎に各出荷と売却を集計した在庫帳
		//fprintf(app_context.config->wholesalerTotal->out_materialBook, "inside BookKeeper->creatematerialBookTotal, \n");
		//printf("inside creatematerialBookTotal  size=%d,period =%d,\n", materialBook.size(),period);
		long long previous_balance = 0;
		int size = materialBookTotal.size();
		if (materialBook.size() == 0) {
			materialbookslip = new materialBookSlip;
			materialbookslip->production = 0;
			materialbookslip->bought = 0;
			materialbookslip->period = period;
			materialbookslip->balance = 0;
			materialbookslip->previouos_balance = 0;
			previous_balance = 0;
			materialbookslip->price = 0;
		}
		else {
			materialbookslip = new materialBookSlip;
			materialbookslip->production = 0;
			materialbookslip->bought = 0;
			materialbookslip->price = 0;
			//printf("period=%d, materialBook.size=%d \n", period, materialBook.size());
			int period_max = materialBook[materialBook.size() - 1]->period;
			int ncon = 0;
			//vector <int> previous_balance;
			//previous_balance[0] = 0;


			//previous_balanceの定義を変更20220922
			if (period == 0) previous_balance = 0;
			else previous_balance = get_materialBalance(period - 1);


			//fprintf(app_context.config->wholesalerTotal->out_materialBook,"inside BookKeeper->creatematerialBookTotal agent_id=%d materialBook.size()=%d \n",get_agent_id(),materialBook.size());
			ncon = 0;
			for (unsigned int i = 0; i < materialBook.size(); i++) {
				if (period == materialBook[i]->period) {
					//printf("i= %d materialBook[i]->production=%d", i,materialBook[i]->production);
					materialbookslip->production += materialBook[i]->production;
					materialbookslip->bought += materialBook[i]->bought;
					materialbookslip->period = materialBook[i]->period;
					if (materialBook[i]->production > 0 || materialBook[i]->bought > 0) {
						materialbookslip->price = materialbookslip->price + materialBook[i]->price;
						ncon++;
					}
					/*
					if (materialBook[i]->production > 0 || materialBook[i]->bought > 0) materialbookslip->price = materialBook[i]->price;
					else {
						if (materialBookTotal.size() == 0) materialbookslip->price = 0;
						else materialbookslip->price = materialBookTotal[materialBookTotal.size() - 1]->price;
					}
					*/
					//ncon++;

				}
			
			}
			//printf("after for ncon=%d \n",ncon);
			if (ncon == 0) {
				//periodの在庫帳記載がない場合
				materialbookslip->period = period;
			}
			else {
				materialbookslip->price = long long(double (materialbookslip->price) / ncon);
			}
		}
		materialbookslip->balance = previous_balance + materialbookslip->production - materialbookslip->bought;
		materialBookTotal.push_back(materialbookslip);
	}
	//------------------------



	void createstockpriceBookTotal(int period) {
		//期毎に各出荷と売却を集計した在庫帳
		//fprintf(app_context.config->wholesalerTotal->out_stockBook, "inside BookKeeper->createstockBookTotal, \n");
		long long previous_balance = 0;
		int size
			= stockpriceBookTotal.size();
		//printf("before if\n");
		if (stockpriceBook.size() == 0) {
			stockpricebookslip = new stockpriceBookSlip;
			stockpricebookslip->production_price = 0;
			stockpricebookslip->bought_price = 0;
			stockpricebookslip->period = period;
			stockpricebookslip->balance_price = 0;
			stockpricebookslip->previous_balance = 0;
			previous_balance = 0;
		}
		else {
			//printf("before stockpricebookTotal\n");
		stockpricebookslip = new stockpriceBookSlip;
		stockpricebookslip->production_price = 0;
		stockpricebookslip->bought_price = 0;
		//printf("before int period_max\n");
		int period_max = stockpriceBook[stockpriceBook.size() - 1]->period;
		//printf("after int period_max\n");
		int ncon = 0;
		//vector <int> previous_balance;
		//previous_balance[0] = 0;
		if (period == 0) previous_balance = 0;
		else previous_balance = get_stockpriceBalance(period - 1);
		
		
		//fprintf(app_context.config->wholesalerTotal->out_stockBook,"inside BookKeeper->createstockBookTotal agent_id=%d stockBook.size()=%d \n",get_agent_id(),stockBook.size());
		ncon = 0;
		for (unsigned int i = 0; i < stockpriceBook.size(); i++) {
			if (period == stockpriceBook[i]->period) {
				//printf("i= %d stockBook[i]->production=%d", i,stockBook[i]->production);
				stockpricebookslip->production_price += stockpriceBook[i]->production_price;
				stockpricebookslip->bought_price += stockpriceBook[i]->bought_price;
				stockpricebookslip->period = stockpriceBook[i]->period;
				ncon++;

			}
		}
		//printf("after for \n");
		if (ncon == 0) {
			//periodの在庫帳記載がない場合
			stockpricebookslip->period = period;
		}
	}
		stockpricebookslip->balance_price = previous_balance + stockpricebookslip->production_price - stockpricebookslip->bought_price;
		stockpriceBookTotal.push_back(stockpricebookslip);
	}
	//------------
	void creatematerialpriceBookTotal(int period) {
		//期毎に各出荷と売却を集計した在庫帳
		//fprintf(app_context.config->wholesalerTotal->out_materialBook, "inside BookKeeper->creatematerialBookTotal, \n");
		long long previous_balance = 0;
		int size = materialpriceBookTotal.size();
		//printf("before if\n");
		if (materialpriceBook.size() == 0) {
			materialpricebookslip = new materialpriceBookSlip;
			materialpricebookslip->production_price = 0;
			materialpricebookslip->bought_price = 0;
			materialpricebookslip->period = period;
			materialpricebookslip->balance_price = 0;
			materialpricebookslip->previous_balance = 0;
			previous_balance = 0;
		}
		else {
			//printf("before materialpricebookTotal\n");
			materialpricebookslip = new materialpriceBookSlip;
			materialpricebookslip->production_price = 0;
			materialpricebookslip->bought_price = 0;
			//printf("before int period_max\n");
			int period_max = materialpriceBook[materialpriceBook.size() - 1]->period;
			//printf("after int period_max\n");
			int ncon = 0;
			//vector <int> previous_balance;
			//previous_balance[0] = 0;
			if (period == 0) previous_balance = 0;
			else previous_balance = get_materialpriceBalance(period - 1);


			//fprintf(app_context.config->wholesalerTotal->out_materialBook,"inside BookKeeper->creatematerialBookTotal agent_id=%d materialBook.size()=%d \n",get_agent_id(),materialBook.size());
			ncon = 0;
			for (unsigned int i = 0; i < materialpriceBook.size(); i++) {
				if (period == materialpriceBook[i]->period) {
					//printf("i= %d materialBook[i]->production=%d", i,materialBook[i]->production);
					materialpricebookslip->production_price += materialpriceBook[i]->production_price;
					materialpricebookslip->bought_price += materialpriceBook[i]->bought_price;
					materialpricebookslip->period = materialpriceBook[i]->period;
					ncon++;

				}
			
			}
			//printf("after for \n");
			if (ncon == 0) {
				//periodの在庫帳記載がない場合
				materialpricebookslip->period = period;
			
			}
		}
		materialpricebookslip->balance_price = previous_balance + materialpricebookslip->production_price - materialpricebookslip->bought_price;
		materialpriceBookTotal.push_back(materialpricebookslip);
		if (period == 1 || period == 2)
			fprintf(app_context.config->out_test7, "period=,%d,agent_id=,%d,production_price=,%I64d, bought_price=,%I64d,balance_price=,%I64d,\n", period, get_agent_id(), get_materialpriceBookSlipTotal(period)->production_price,get_materialpriceBookSlipTotal(period)->bought_price,get_materialpriceBookSlipTotal(period)->balance_price);

	}
	//--------------------
	struct cashBookSlip* get_cashbookslip() {
		int size = 0;
		size = cashBook.size();
		if (size > 0) return cashBook[size - 1];
		else return 0;
	}
	struct bankBookSlip* get_bankbookslip() {
		int size = 0;
		size = bankBook.size();
		if (size > 0) return bankBook[size - 1];
		else return 0;
	}
	struct stockBookSlip* get_stockbookslip() {
		int size = 0;
		size = stockBook.size();
		if (size > 0) return stockBook[size - 1];
		else return 0;
	}
	struct cashBookSlip* get_cashbookslip(int i) {
		int size = 0;
		size = cashBook.size();
		if (size > 0) return cashBook[i];
		else return 0;
	}
	struct bankBookSlip* get_bankbookslip(int i) {
		int size = 0;
		size = bankBook.size();
		if (size > 0) return bankBook[i];
		else return 0;
	}
	struct stockBookSlip* get_stockbookslip(int i) {
		int size = 0;
		size = stockBook.size();
		if (size > 0) return stockBook[i];
		else return 0;
	}

	struct stockpriceBookSlip* get_stockpricebookslip(int i) {
		int size = 0;
		size = stockpriceBook.size();
		if (size > 0) {
			/*if (agent_id == 545) {
				for (unsigned int j = 0; j < size; j++) fprintf(app_context.config->out_test3, "size=,%d,i=,%d,j=,%d,app_context.period=,%d,stockpriceBook[j]->period=,%d,	production_price,=%I64d,bought_price=,%I64d,balance_price=,%I64d,\n", size,i,j,app_context.period_count,stockpriceBook[j]->period,stockpriceBook[j]->production_price,stockpriceBook[j]->bought_price,stockpriceBook[j]->balance_price);
			}
			*/
		//	if(agent_id==545) fprintf(app_context.config->out_test3, "return_item,period=,%d,i=,%d,stockpriceBook[i]->perid=,%d,production_price=,%I64d,bought_orice=,%I64d,balance_price=,%I64d=,\n", app_context.period_count,i, stockpriceBook[i]->period, stockpriceBook[i]->production_price, stockpriceBook[i]->bought_price,stockpriceBook[i]->balance_price);
			return stockpriceBook[i];
		}
		else {
			//if (agent_id == 545) fprintf(app_context.config->out_test3, "size=,%d,i=,%d,period=,%d,\n", size, i, app_context.period_count);
			return 0;
		}
	}
	//-----------------------
	struct materialBookSlip* get_materialbookslip(int i) {
		int size = 0;
		size = materialBook.size();
		if (size > 0) return materialBook[i];
		else return 0;
	}
	struct genkaBookSlip* get_genkabookslip(int i) {
		int size = 0;
		size = genkaBook.size();
		if (size > 0) return genkaBook[i];
		else return 0;
	}

		struct materialpriceBookSlip* get_materialpricebookslip(int i) {
			int size = 0;
			size = materialpriceBook.size();
			if (size > 0) {
				return materialpriceBook[i];
			}
			else {
				//if (agent_id == 545) fprintf(app_context.config->out_test3, "size=,%d,i=,%d,period=,%d,\n", size, i, app_context.period_count);
				return 0;
			}
		}


		/*
		struct materialpriceBookSlip* get_materialpricebookslip(int period) {
			int size=0;
			int ijk=0;
			size = materialpriceBook.size();
			if (size > 0) {
				for (int i = 0; i < size; i++) {
					if (materialpriceBook[i]->period == period) {
						ijk = i;
						break;;
					}
				}
				//	if(agent_id==545) fprintf(app_context.config->out_test3, "return_item,period=,%d,i=,%d,materialpriceBook[i]->perid=,%d,production_price=,%I64d,bought_orice=,%I64d,balance_price=,%I64d=,\n", app_context.period_count,i, materialpriceBook[i]->period, materialpriceBook[i]->production_price, materialpriceBook[i]->bought_price,materialpriceBook[i]->balance_price);
				return materialpriceBook[ijk];
			}
			else {
				//if (agent_id == 545) fprintf(app_context.config->out_test3, "size=,%d,i=,%d,period=,%d,\n", size, i, app_context.period_count);
				return 0;
			}
		}
		*/
		//-----------------------------



		/*int get_bankbookSlip_balance(int period) {
			//period の最後の預金残高をgetする。
			int size = bankBook.size();
			int balance = 0;
			if (size == 0) balance = 0;
			else {
				for (int i = 0; i < size; i++) {
					if (bankBook[i]->period == period) {
						if (i == size - 1) balance = bankBook[i]->balance;//その行が預金通帳の最後の行ならその残高を返す
						else if (bankBook[i + 1]->period == period + 1) balance = bankBook[i]->balance;//その行が当該期の最後の行ならその残高を返す
						else balance = 99999;
					}
					else balance = 99999;
				}
			}
			return balance;

		}*/
		long long get_cashbookSlip_balance_kishu() {
			int size = cashBook.size();
			if (size < 1) return 0;
			else {
				int i = cashBook.size();
				return cashBook[i - 1]->balance;
			}
		}
		long long get_bankbookSlip_balance_kishu() {
			//直前の残高をgetする。
			int size = bankBook.size();
			if (size < 1) return 0;
			else {
				int i = bankBook.size();
				return bankBook[i - 1]->balance;
			}
		}
		long long get_cashbookSlip_balance() {
			//直前の残高をgetする。
			int size = cashBook.size();
			if (size <= 1) return 0;
			else {
				int i = cashBook.size() - 1;
				return cashBook[i - 1]->balance;
			}
		}
		long long get_bankbookSlip_balance() {
			//直前の残高をgetする。
			int size = bankBook.size();
			if (size <= 1) return 0;
			else {
				int i = bankBook.size() - 1;
				return bankBook[i - 1]->balance;
			}
		}

		long long get_cashkbookSlip_balance(int period) {
			//period の最後の現金残高をgetする。
			int size = cashBook.size();
			long long balance = 0;
			if (size == 0) balance = 0;
			else {
				for (int i = 0; i < size; i++) {
					if (cashBook[i]->period == period) {
						if (i == size - 1) balance = cashBook[i]->balance;//その行が預金通帳の最後の行ならその残高を返す
						else if (cashBook[i + 1]->period == period + 1) balance = cashBook[i]->balance;//その行が当該期の最後の行ならその残高を返す
						else balance = 99999;
					}
					else balance = 99999;
				}
			}
			return balance;

		}
		void print_cashbook(int period) {

			AccountData* accountData = NULL;
			if (agent_type == 31 || agent_type == 310 || agent_type == 311 || agent_type == 312 || agent_type == 3100 || agent_type == 3110 || agent_type == 3120) accountData = app_context.config->consumerData;
			else if (agent_type == 2131) accountData = app_context.config->retailerData;
			else if (agent_type == 2132) accountData = app_context.config->wholesalerData;
			else if (agent_type == 2130) accountData = app_context.config->soleproprietorData;
		//	else if (agent_type == 1) accountData = app_context.config->governmentData;	
			else {}
			/*if (agent_type == 31 || agent_type == 311 || agent_type == 312) {
				fprintf(accountData->out_title_list, "%d,%d,%d,%d,", app_context.period_count, agent_id, agent_type, size);
				for (int i = 0; i < size; i++) 	fprintf(accountData->out_cashbook, "%s,", title_list[i]->title.c_str());

				fprintf(accountData->out_title_list, "\n");
			*/

			for (unsigned int i = 0; i < cashBook.size(); i++) {
				if (cashBook[i]->period == period)
					fprintf(accountData->out_cashbook, "period=,%d,agent_id=,%d,agent_type,=%d,i=,%d,income=,%I64d,spending=,%I64d, balance=,%I64d,\n", period, get_agent_id(), get_agent_type(), i, get_cashbookslip(i)->income, get_cashbookslip(i)->spending, get_cashbookslip(i)->balance);
			}
		}
		void print_bankbook(int period) {
			AccountData* accountData = NULL;
			if (agent_type == 31 || agent_type == 310 || agent_type == 311 || agent_type == 312 || agent_type == 3100 || agent_type == 3110 || agent_type == 3120) accountData = app_context.config->consumerData;
			else if (agent_type == 2131) accountData = app_context.config->retailerData;
			else if (agent_type == 2132) accountData = app_context.config->wholesalerData;
			else if (agent_type == 2130) accountData = app_context.config->soleproprietorData;
		//	else if (agent_type == 1) accountData = app_context.config->governmentData;	
			else {}
			/*if (agent_type == 31 || agent_type == 311 || agent_type == 312) {
				fprintf(accountData->out_title_list, "%d,%d,%d,%d,", app_context.period_count, agent_id, agent_type, size);
				for (int i = 0; i < size; i++) 	fprintf(accountData->out_cashbook, "%s,", title_list[i]->title.c_str());

				fprintf(accountData->out_title_list, "\n");
			*/
			//printf("inside print_bankbook before for \n");
			for (unsigned int i = 0; i < bankBook.size(); i++) {
				//printf("before if\n");
				if (bankBook[i]->period == period)
					fprintf(accountData->out_bankbook, "period=,%d,agent_id=,%d,agent_type,=%d,i=,%d,deposit=,%I64d,withdrawal=,%I64d, balance=,%I64d,\n", period, get_agent_id(), get_agent_type(), i, get_bankbookslip(i)->deposit, get_bankbookslip(i)->withdrawal, get_bankbookslip(i)->balance);
				//printf("after fprintf\n");
			}
		}
	/*
		int get_number_of_stocks(int item_type_id) {
			set<Item*, PredForItemBySeq>* stocks = get_item_set(item_type_id);
			return stocks->size();
			//市場に商品が存在しなければsize=0

		}
		*/
		int get_number_of_stocks(int period, int item_type_id) {
			long long balance=0;
			for (unsigned int i = 0; i < stockBookTotal.size(); i++) {
				if (stockBookTotal[i]->period == period && stockBookTotal[i]->item_type_id == item_type_id)
				   balance = stockBookTotal[i]->balance;
				//	fprintf(accountData->out_stockbookTotal, "period=,%d,agent_id=,%d,agent_type,=%d,i=,%d,production=,%I64d,bought=,%I64d, balance=,%I64d,price=,%I64d,ave_price_in_market=,%I64d,item_type_id=,%d,\n",
				//		period, get_agent_id(), get_agent_type(), i, stockBookTotal[i]->production, stockBookTotal[i]->bought, stockBookTotal[i]->balance, stockBookTotal[i]->price, get_ave_price_in_market(), get_item_type_id());
			}
			return int(balance);
		}
		void print_stockbook(int period) {
			AccountData* accountData = NULL;
			if (agent_type == 31 || agent_type == 310 || agent_type == 311 || agent_type == 312 || agent_type == 3100 || agent_type == 3110 || agent_type == 3120) accountData = app_context.config->consumerData;
			else if (agent_type == 2131) accountData = app_context.config->retailerData;
			else if (agent_type == 2132) accountData = app_context.config->wholesalerData;
			else if (agent_type == 2130)accountData = app_context.config->soleproprietorData;
		//	else if (agent_type == 1) accountData = app_context.config->governmentData;	
			else {}
			//fprintf(accountData->out_stockbook, "agent_type=,%d.stockBook.size=,%d,\n", agent_type,stockBook.size());
			for (unsigned int i = 0; i < stockBook.size(); i++) {
				//printf("before if\n");;
				if (stockBook[i]->period == period)
					fprintf(accountData->out_stockbook, "period=,%d,agent_id=,%d,agent_type,=%d,i=,%d,production=,%I64d,bought=,%I64d, balance=,%I64d,item_tpe_id=,%d, price=,%I64d,ave_price=,%I64d,\n",
						period, get_agent_id(), get_agent_type(), i, get_stockbookslip(i)->production, get_stockbookslip(i)->bought, get_stockbookslip(i)->balance,
						get_stockbookslip(i)->item_type_id, get_stockbookslip(i)->price,get_ave_price_in_market());
				//printf("after fprintf\n");
			}
		}
		void print_stockpricebook(int period) {
			//printf(" t


			//start of print_stockpricebook agent_type=%d\n",agent_type);
			AccountData* accountData = NULL;
			if (agent_type == 31 || agent_type == 310 || agent_type == 311 || agent_type == 312 || agent_type == 3100 || agent_type == 3110 || agent_type == 3120) accountData = app_context.config->consumerData;
			else if (agent_type == 2131) accountData = app_context.config->retailerData;
			else if (agent_type == 2132) accountData = app_context.config->wholesalerData;
			else if (agent_type == 2130) accountData = app_context.config->soleproprietorData;
		//	else if (agent_type == 1) accountData = app_context.config->governmentData;	
			else {}
			//fprintf(accountData->out_stockbook, "agent_type=,%d.stockBook.size=,%d,\n", agent_type,stockBook.size());
			for (unsigned int i = 0; i < stockpriceBook.size(); i++) {
				//printf("before if\n");
				if (stockpriceBook[i]->period == period)
					fprintf(accountData->out_stockpricebook, "period=,%d,agent_id=,%d,agent_type,=%d,i=,%d,production_price=,%I64d,bought_price=,%I64d, balance_price=,%I64d,item_tpe_id=,%d,\n",
						period, get_agent_id(), get_agent_type(), i, get_stockpricebookslip(i)->production_price, get_stockpricebookslip(i)->bought_price, get_stockpricebookslip(i)->balance_price,
						get_stockpricebookslip(i)->item_type_id);
				//printf("after fprintf\n");
			}
			//printf("end of print_stockpricebook agent_type=%d\n", agent_type);
		}
		//-------------------
		void print_materialbook(int period) {
			AccountData* accountData = NULL;
			if (agent_type == 31 || agent_type == 310 || agent_type == 311 || agent_type == 312 || agent_type == 3100 || agent_type == 3110 || agent_type == 3120) accountData = app_context.config->consumerData;
			else if (agent_type == 2131) accountData = app_context.config->retailerData;
			else if (agent_type == 2132) accountData = app_context.config->wholesalerData;
			else if (agent_type == 2130) accountData = app_context.config->soleproprietorData;
		//	else if (agent_type == 1) accountData = app_context.config->governmentData;	
			else {}
			//fprintf(accountData->out_materialbook, "agent_type=,%d.materialBook.size=,%d,\n", agent_type,materialBook.size());
			for (unsigned int i = 0; i < materialBook.size(); i++) {
				//printf("before if\n");
				if (materialBook[i]->period == period)
					fprintf(accountData->out_materialbook, "period=,%d,agent_id=,%d,agent_type,=%d,i=,%d,production=,%I64d,bought=,%I64d, balance=,%I64d,item_tpe_id=,%d, price=,%I64d,\n",
						period, get_agent_id(), get_agent_type(), i, get_materialbookslip(i)->production, get_materialbookslip(i)->bought, get_materialbookslip(i)->balance,
						get_materialbookslip(i)->item_type_id, get_materialbookslip(i)->price);
				//printf("after fprintf\n");
			}
		}
		void print_materialpricebook(int period) {
			//printf(" top of print_materialpricebook agent_type=%d\n",agent_type);
			AccountData* accountData = NULL;
			if (agent_type == 31 || agent_type == 310 || agent_type == 311 || agent_type == 312 || agent_type == 3100 || agent_type == 3110 || agent_type == 3120) accountData = app_context.config->consumerData;
			else if (agent_type == 2131) accountData = app_context.config->retailerData;
			else if (agent_type == 2132) accountData = app_context.config->wholesalerData;
			else if (agent_type == 2130) accountData = app_context.config->soleproprietorData;
		//	else if (agent_type == 1) accountData = app_context.config->governmentData;	
			else {}
			//fprintf(accountData->out_materialbook, "agent_type=,%d.materialBook.size=,%d,\n", agent_type,materialBook.size());
			for (unsigned int i = 0; i < materialpriceBook.size(); i++) {
				//printf("before if\n");
				if (materialpriceBook[i]->period == period)
			
					fprintf(accountData->out_materialpricebook, "period=,%d,agent_id=,%d,agent_type,=%d,i=,%d,production_price=,%I64d,bought_price=,%I64d, balance_price=,%I64d,item_tpe_id=,%d,\n",
						period, get_agent_id(), get_agent_type(), i, get_materialpricebookslip(i)->production_price, get_materialpricebookslip(i)->bought_price, get_materialpricebookslip(i)->balance_price,
						get_materialpricebookslip(i)->item_type_id);

				//printf("after fprintf\n");
			}
			//printf("end of print_materialpricebook agent_type=%d\n", agent_type);
		}
	//-------------------
//-----------------------

	void print_genkabook(int period) {
		//printf("inside print_genkaook \n");
		AccountData* accountData = NULL;
	//	if (agent_type == 31 || agent_type==310 || agent_type == 311 || agent_type == 312) accountData = app_context.config->consumerData;
		if (agent_type == 2131) accountData = app_context.config->retailerData;
		else if (agent_type == 2132) accountData = app_context.config->wholesalerData;
		else if (agent_type == 2130) accountData = app_context.config->soleproprietorData;
	//	else if (agent_type == 1) accountData = app_context.config->governmentData;	
		else {}
		if(agent_type==2130 || agent_type == 2131 || agent_type == 2132){
		for (unsigned int i = 0; i < genkaBook.size(); i++) {
			if (genkaBook[i]->period == period)
				fprintf(accountData->out_genkabook, "period=,%d,agent_id=,%d, agent_type=,%d, size_shiped=,%d, direct_labor_cost=, %I64d, direct_material_fee_per_item=, %I64d, genka=, %I64d,item_type_id=,%d,\n ",
					period, get_agent_id(), get_agent_type(), get_genkabookslip(i)->size_shiped, get_genkabookslip(i)->direct_labor_cost, get_genkabookslip(i)->direct_material_cost, get_genkabookslip(i)->genka, get_genkabookslip(i)->item_type_id);
			}
		}
		else {}
	}
	void print_genkabooktotal(int period) {
		//printf("inside print_genkaook \n");
		AccountData* accountData = NULL;
		//	if (agent_type == 31 || agent_type == 311 || agent_type == 312) accountData = app_context.config->consumerData;
		if (agent_type == 2131) accountData = app_context.config->retailerData;
		else if (agent_type == 2132) accountData = app_context.config->wholesalerData;
		else if (agent_type == 2130) accountData = app_context.config->soleproprietorData;
	//	else if (agent_type == 1) accountData = app_context.config->governmentData;
		else {}
		if (agent_type==2130 || agent_type == 2131 || agent_type == 2132) {
			for (unsigned int i = 0; i < genkaBookTotal.size(); i++) {
				if (genkaBookTotal[i]->period == period)
					fprintf(accountData->out_genkabookTotal, "period=,%d,agent_id=,%d, agent_type=,%d, size_shiped=,%d, direct_labor_cost=, %I64d, direct_material_fee_per_item=, %I64d, genka=, %I64d,item_type_id=,%d,\n ",
						period, get_agent_id(), get_agent_type(), genkaBookTotal[i]->size_shiped, genkaBookTotal[i]->direct_labor_cost, genkaBookTotal[i]->direct_material_cost, genkaBookTotal[i]->genka, genkaBookTotal[i]->item_type_id);
			}
		}
		else {}
	}


	void print_cashbooktotal(int period) {
		AccountData* accountData = NULL;
		if (agent_type == 31 || agent_type == 310 || agent_type == 311 || agent_type == 312 || agent_type == 3100 || agent_type == 3110 || agent_type == 3120) accountData = app_context.config->consumerData;
		else if (agent_type == 2131) accountData = app_context.config->retailerData;
		else if (agent_type == 2132) accountData = app_context.config->wholesalerData;
		else if (agent_type == 2130) accountData = app_context.config->soleproprietorData;
		else if (agent_type == 1330) accountData = app_context.config->governmentData;	
	//	else if (agent_type == 1) accountData = app_context.config->governmentData;	
		else {}
		for (unsigned int i = 0; i < cashBookTotal.size(); i++) {
			if (cashBookTotal[i]->period == period)
				fprintf(accountData->out_cashbookTotal, "period=,%d,agent_id=,%d,agent_type,=%d,i=,%d,income=,%I64d,spending=,%I64d, balance=,%I64d,\n", period, get_agent_id(), get_agent_type(), i, cashBookTotal[i]->income, cashBookTotal[i]->spending, cashBookTotal[i]->balance);
		}
	}
	
	void print_bankbooktotal(int period) {
		AccountData* accountData = NULL;
		if (agent_type == 31 || agent_type == 310 || agent_type == 311 || agent_type == 312 || agent_type == 3100 || agent_type == 3110 || agent_type == 3120) accountData = app_context.config->consumerData;
		else if (agent_type == 2131) accountData = app_context.config->retailerData;
		else if (agent_type == 2132) accountData = app_context.config->wholesalerData;
		else if (agent_type == 2130) accountData = app_context.config->soleproprietorData;
	//	else if (agent_type == 1) accountData = app_context.config->governmentData;		
		else {}
		for (unsigned int i = 0; i < bankBookTotal.size(); i++) {
			if (bankBookTotal[i]->period == period)
				fprintf(accountData->out_bankbookTotal, "period=,%d,agent_id=,%d,agent_type,=%d,i=,%d,deposit=,%I64d,withdrawal=,%I64d, balance=,%I64d,\n", period, get_agent_id(), get_agent_type(), i, bankBookTotal[i]->deposit, bankBookTotal[i]->withdrawal, bankBookTotal[i]->balance);
		}
	}
	void print_stockbooktotal(int period) {
		//AgentAggregator->do_kimatus()で呼ばれる
		AccountData* accountData = NULL;
		if (agent_type == 31 || agent_type == 310 || agent_type == 311 || agent_type == 312 || agent_type == 3100 || agent_type == 3110 || agent_type == 3120) accountData = app_context.config->consumerData;
		else if (agent_type == 2131) accountData = app_context.config->retailerData;
		else if (agent_type == 2132) accountData = app_context.config->wholesalerData;
		else if (agent_type == 2130) accountData = app_context.config->soleproprietorData;
	//	else if (agent_type == 1) accountData = app_context.config->governmentData;	
		else {}
		//if (agent_type == 2131 || agent_type == 2132) {
		//printf("inside print_stockbooktotal \n");
		for (unsigned int i = 0; i < stockBookTotal.size(); i++) {
			if (stockBookTotal[i]->period == period) {
				fprintf(accountData->out_stockbookTotal, "period=,%d,agent_id=,%d,agent_type,=%d,i=,%d,production=,%I64d,bought=,%I64d, balance=,%I64d,price=,%I64d,ave_price_in_market=,%I64d,item_type_id=,%d,\n",
					period, get_agent_id(), get_agent_type(), i, stockBookTotal[i]->production, stockBookTotal[i]->bought, stockBookTotal[i]->balance, stockBookTotal[i]->price, get_ave_price_in_market(), get_item_type_id());
				if(app_context.period_count==21) fprintf(app_context.config->out_test12, "inside print_stockbooktotal,agent_id=,%d,\n", get_agent_id());
				if (agent_id == 509) fprintf(app_context.config->out509, "period=,%d,agent_id=,%d,production=,%d,inside print_stockbookTotal in BookKeeper do_kimatsu,\n", stockBookTotal[i]->period, get_agent_id(), stockBookTotal[i]->production);
				}
			}
		//}

	}
	void print_stockpricebooktotal(int period) {
		//printf("inside and top of print_stockpricebooktotal agent_type=%d\n", agent_type);
		AccountData* accountData = NULL;
		if (agent_type == 31 || agent_type == 310 || agent_type == 311 || agent_type == 312 || agent_type == 3100 || agent_type == 3110 || agent_type == 3120) accountData = app_context.config->consumerData;
		else if (agent_type == 2131) accountData = app_context.config->retailerData;
		else if (agent_type == 2132) accountData = app_context.config->wholesalerData;
		else if (agent_type == 2130) accountData = app_context.config->soleproprietorData;
	//	else if (agent_type == 1) accountData = app_context.config->governmentData;		
		else {}
		//if (agent_type == 2131 || agent_type == 2132) {
		//if(get_agent_id=545 || get_agent_id()==455){
		//printf("inside print_stockpricebooktotal befor for stockpriceBookTotal.size=%d\n", stockpriceBookTotal.size());
		for (unsigned int i = 0; i < stockpriceBookTotal.size(); i++) {
		//	if (stockBookTotal[i]->period == period) {
		    if (stockpriceBookTotal[i]->period == period) {
			//	printf("before fprint\n");
		//	printf("before fprintf agent_id=%d agent_type=%d stockpriceBookTotal[%d]->bought_price =%I64d\n", get_agent_id(),get_agent_type(),i,stockpriceBookTotal[i]->bought_price);
			fprintf(accountData->out_stockpricebookTotal, "period=,%d,agent_id=,%d,agent_type,=%d,production=,%I64d,bought=,%I64d, balance=,%I64d,\n",
				period, get_agent_id(), get_agent_type(), stockpriceBookTotal[i]->production_price, stockpriceBookTotal[i]->bought_price, stockpriceBookTotal[i]->balance_price);
		//	printf("after fprintf \n");
					}
			}
		//	printf("after for \n");
		//}
			//printf("end of print_stockpricebooktotal agent_type=%d", agent_type);
	}
	//-------------------
	void print_materialbooktotal(int period) {
		AccountData* accountData = NULL;
		if (agent_type == 31 || agent_type == 310 || agent_type == 311 || agent_type == 312 || agent_type == 3100 || agent_type == 3110 || agent_type == 3120) accountData = app_context.config->consumerData;
		else if (agent_type == 2131) accountData = app_context.config->retailerData;
		else if (agent_type == 2132) accountData = app_context.config->wholesalerData;
		else if (agent_type == 2130) accountData = app_context.config->soleproprietorData;
	//	else if (agent_type == 1) accountData = app_context.config->governmentData;
		else {}
		//if (agent_type == 2131 || agent_type == 2132) {
		//printf("inside print_materialbooktotal \n");
		for (unsigned int i = 0; i < materialBookTotal.size(); i++) {
			if (materialBookTotal[i]->period == period){
				
				fprintf(accountData->out_materialbookTotal, "period=,%d,agent_id=,%d,agent_type,=%d,i=,%d,production=,%I64d,bought=,%I64d, balance=,%I64d,price=,%I64d,\n",
					period, get_agent_id(), get_agent_type(), i, materialBookTotal[i]->production, materialBookTotal[i]->bought, materialBookTotal[i]->balance, materialBookTotal[i]->price);
				
			}
		}

	}
	void print_materialpricebooktotal(int period) {
		//printf("inside and top of print_materialpricebooktotal agent_type=%d\n", agent_type);
		AccountData* accountData = NULL;
		if (agent_type == 31 || agent_type == 310 || agent_type == 311 || agent_type == 312 || agent_type == 3100 || agent_type == 3110 || agent_type == 3120) accountData = app_context.config->consumerData;
		else if (agent_type == 2130) accountData = app_context.config->soleproprietorData;
		else if (agent_type == 2131) accountData = app_context.config->retailerData;
		else if (agent_type == 2132) accountData = app_context.config->wholesalerData;
	//	else if (agent_type == 1) accountData = app_context.config->governmentData;	
		else {}
		//if (agent_type == 2131 || agent_type == 2132) {
		//if(get_agent_id()==545 || get_agent_id()==455){
		//printf("inside print_materialpricebooktotal materialpriceBookTotal.size=%d\n", materialpriceBookTotal.size());
		for (unsigned int i = 0; i < materialpriceBookTotal.size(); i++) {
		//	if (materialBookTotal[i]->period == period) {
			if (materialpriceBookTotal[i]->period == period){

			//	printf("before fprintf \n")
				
				fprintf(accountData->out_materialpricebookTotal, "period=,%d,agent_id=,%d,agent_type,=%d,production=,%I64d,bought=,%I64d, balance=,%I64d,bought_get=,%I64d,bought_get2=,%I64d,\n",
					period, get_agent_id(), get_agent_type(),
					materialpriceBookTotal[i]->production_price, materialpriceBookTotal[i]->bought_price, materialpriceBookTotal[i]->balance_price
				,get_materialpriceBookSlipTotal(period)->bought_price, get_materialpriceBookSlipTotal(i)->bought_price);
				
					//printf("after fprintf \n");
			}
		}
		//printf("after for \n");
	//}
		//printf("end of print_materialpricebooktotal agent_type=%d", agent_type);
	}

	//---------------------------------------





	void print_cashbook() {

	}
	long long get_totalsales(int period) {
		//period期の総売り上げ高を返す
		int size = cashBook.size();
		if (size > 0) {
			long long sum = 0;
			for (int i = 0; i < size; i++) {
				if (cashBook[i]->period == period) sum += cashBook[i]->income;
			}
			return sum;
		}
		else return 0;

	}
	long long get_totalspending(int period) {
		int size = cashBook.size();
		if (size > 0) {
			long long sum = 0;
			for (int i = 0; i < size; i++) {
				if (cashBook[i]->period == period) sum += cashBook[i]->spending;
			}
			return sum;
		}
		else return 0;
	}
	long long get_totalbalance(int period) {
		/*int size=cashBook.size();
		if(size>0){
			int sum=0;
			for(int i=0;i<size;i++){
				if(cashBook[i]->period==period) sum+=cashBook[i]->balance;
			}
			return sum;
		}*/
		return get_cashbookslip()->balance;
	}
	void create_title_list() {
		//agentaggregator->do_kimatsu()の中で呼ばれるbookKeeperのdo_kimatsu()でagent毎に仕訳データに基づいて毎期末に呼ばれる
		int size = journal.size();
		int title_size = 0;
		int np = 0;
		for (int i = 0; i < size; i++) {
			if (title_list.size() == 0) {
				//始めはtitle_listが空なので、借方科目を無条件にtitle_list[0]にいれる
				account_title = new accountTitle;
				account_title->period = app_context.period_count;
				account_title->title = journal[i]->Dr_title;
				account_title->code = journal[i]->Dr_code;
				title_list.push_back(account_title);
				if (journal[i]->Cr_title != journal[i]->Dr_title) {//貸方科目が借方科目と異なれば、貸方科目もtitle_list[1]にいれる
					account_title = new accountTitle;
					account_title->title = journal[i]->Cr_title;
					account_title->code = journal[i]->Cr_code;
					title_list.push_back(account_title);
				}
			}
			else {//title_listが空ではない
				//journal[i]の借方科目がtitle_listの科目のいずれとも異なれば、新しい科目としてtitle_listに追加する。
				title_size = title_list.size();
				np = 0;
				for (int j = 0; j < title_size; j++) {
					if (journal[i]->Dr_title != title_list[j]->title) {
						np++;
						//if(app_context.period_count<5)	fprintf(app_context.config->out_kamoku, "%s,%s,\n",journal[i]->Dr_title.c_str(), title_list[j]->title.c_str());
					}
				}
				if (np == title_size) {
					account_title = new accountTitle;
					//account_title->period = app_context.period_count;
					account_title->title = journal[i]->Dr_title;
					account_title->code = journal[i]->Dr_code;
					title_list.push_back(account_title);
				}
				//journal[i]の貸方科目がtitle_listの科目のいずれとも異なれば、新しい科目としてtitle_listに追加する。
				title_size = title_list.size();
				np = 0;
				for (int j = 0; j < title_size; j++) {
					if (journal[i]->Cr_title != title_list[j]->title) np++;
				}
				if (np == title_size) {
					account_title = new accountTitle;
					//account_title->period = app_context.period_count;
					account_title->title = journal[i]->Cr_title;
					account_title->code = journal[i]->Cr_code;
					title_list.push_back(account_title);
				}

			}//elseの終わり
		}//for(int i=0...の終わり
	}
	void print_title_list() {
		int size = title_list.size();
		int item_type_id = 10;
		RetailMarketNavigator* market_navi = app_context.retail_market_navi_for_customer;
		//RetailMarket* market = market_navi->get_retail_market(agent_id, item_type_id);		//CustomerMarketNavigator所管　現時点ではAreamarketの参照　  20120524記　高島
		RetailMarket* market = market_navi->get_retail_market(item_type_id);		//CustomerMarketNavigator所管　現時点ではAreamarketの参照　  20120524記　高島
		AccountData* accountData = NULL;
		if (agent_type == 31 || agent_type == 310 || agent_type == 311 || agent_type == 312 || agent_type == 3100 || agent_type==3110 || agent_type==3120) accountData = app_context.config->consumerData;
		else if (agent_type == 2131) accountData = app_context.config->retailerData;
		else if (agent_type == 2132) accountData = app_context.config->wholesalerData;
		else if (agent_type == 2130) accountData = app_context.config->soleproprietorData;
	//	else if (agent_type == 1) accountData = app_context.config->governmentData;	
		else {}
		if (agent_type == 31 || agent_type==310 || agent_type == 311 || agent_type == 312
			//|| agent_type==1
			) {
			fprintf(accountData->out_title_list, "%d,%d,%d,%d,", app_context.period_count, agent_id, agent_type, size);
			for (int i = 0; i < size; i++) 	fprintf(accountData->out_title_list, "%s,", title_list[i]->title.c_str());
			for (int i = 0; i < size; i++) 	fprintf(accountData->out_title_list, "%d,", title_list[i]->code);
			fprintf(accountData->out_title_list, "\n");
		//fprintf(app_context.config->out_title_list_consumer, "%d,\n",market->get_ITEM_SET_size(item_type_id));
		}
		else if (agent_type==2130 || agent_type == 2131 || agent_type==2132){
			//fprintf(app_context.config->out_title_list_retailer, "%d,%d,%d,%d,%s,\n", app_context.period_count, agent_id, agent_type, size, title_list[i]->title.c_str());
			fprintf(accountData->out_title_list, "%d,%d,%d,%d,%d,", app_context.period_count, agent_id, agent_type, size,get_item_type_id());
			for (int i = 0; i < size; i++) 	fprintf(accountData->out_title_list, "%s,", title_list[i]->title.c_str());
			for (int i = 0; i < size; i++) 	fprintf(accountData->out_title_list, "%d,", title_list[i]->code);
			fprintf(accountData->out_title_list, "\n");
		}
	
		else {}
	}
	accountTitle* getAccountTitle(int period, string title) {
		int size = title_list.size();
		//struct <accountTitle*> account_title1;
		for (int i = 0; i < size; i++) {
			if (title_list[i]->period == period) {
				if (title_list[i]->title == title) return title_list[i];
			}
		}
		printf("title_listが見当たりませんでした。\n");
	}
	GLSlip* getGLslip() {
		
		int size = GLslip_list.size();
		//printf("inside getGLslip() period=%d size=%d\n", app_context.period_count, size);
		if (size == 0) {
			return NULL;
			//printf("inside getGLslip() period=%d size=%d\n", app_context.period_count, size);
		}
		else return GLslip_list[size - 1];//直前periodのGLslip_listを返す
		//vector<GLSlip*> GLslip_list;//各period毎のtitle_listのvectorを格納
	}
	void print_journal() {
		int size_journal = journal.size();//仕訳伝票の総数
		//printf("before for in print_journal size=%d \n",size_journal);
		for (int i = 0; i < size_journal; i++) {
			fprintf(app_context.config->out_journal, "%d,%d,%d,%s,%I64d,%s,%I64d,%s,",
				get_agent_id(), journal[i]->period,journal[i]->Dr_code,journal[i]->Dr_title.c_str(),journal[i]->dabit,
				journal[i]->description.c_str(),journal[i]->credit,journal[i]->Cr_title.c_str());
			
			/*fprintf(app_context.config->out_journal, "%d,%d,%d,%s,",
				get_agent_id(), journal[i]->period, journal[i]->Dr_code, journal[i]->Dr_title.c_str());
			*/
			fprintf(app_context.config->out_journal, "%d,\n", journal[i]->Cr_code);
			//(int period, int Dr_code, string Dr_title, int dabit, string description, int credit, string Cr_title, int Cr_code)
			//bookKeeper->createjournalSlip(app_context.period_count, 1, "売上原価", item->get_price(), "商品売上", item->get_price(), "製品", 0);

		}
		//printf("after for in print_jounal \n");
	}
	vector<trialBalanceSlip*>  get_TBtable(){
		return TB_table;
	}
	vector <cashBookSlip*> get_cashBookTotal() {
			return cashBookTotal;
	}
	struct cashBookSlip* get_cashBookSlipTotal(int period) {
		int size = cashBookTotal.size();
		int ijk=0;
		for (int i = 0; i < size; i++) {
			if (cashBookTotal[i]->period == period) ijk = i;
		}
		return cashBookTotal[ijk];
	}
	struct bankBookSlip* get_bankBookSlipTotal(int period) {
		int size = bankBookTotal.size();
		int ijk = 0;
		for (int i = 0; i < size; i++) {
			if (bankBookTotal[i]->period == period) ijk = i;
		}
		return bankBookTotal[ijk];
	}
	struct stockBookSlip* get_stockBookSlipTotal(int period) {
		int size = stockBookTotal.size();
	//	printf("inside get_stockBookSlipTotal size=%d \n", size);
		if (size > 0) {
			int ijk = 0;
			//printf("inside get_stockBookSlipTotal size=%d \n", size);
			for (int i = 0; i < size; i++) {
				if (stockBookTotal[i]->period == period) ijk = i;
			}
			//printf("after get_stockBookSlipTotal ijk=%d,\n", ijk);
			return stockBookTotal[ijk];
		}
		else return 0;
	
	}
	//---------------
	struct materialBookSlip* get_materialBookSlipTotal(int period) {
		int size = materialBookTotal.size();
		//	printf("inside get_materialBookSlipTotal size=%d \n", size);
		if (size > 0) {
			int ijk = 0;
			//printf("inside get_materialBookSlipTotal size=%d \n", size);
			for (int i = 0; i < size; i++) {
				if (materialBookTotal[i]->period == period) ijk = i;
			}
			//printf("after get_materialBookSlipTotal ijk=%d,\n", ijk);
			return materialBookTotal[ijk];
		}
		else return 0;

	}
	//-----------------
	struct genkaBookSlip* get_genkaBookSlipTotal(int period) {
		int size = genkaBookTotal.size();
		int ncount = 0;
		if (size > 0) {
			int ijk = 0;
			for (int i = 0; i < size; i++) {
				if (genkaBookTotal[i]->period == period) { ijk = i; ncount++; }
			}
			return genkaBookTotal[ijk];
			//if (ncount > 0) return genkaBookTotal[ijk];
			//else return 0;
		}
		else return 0;

	}
	//--------------------
	struct stockpriceBookSlip* get_stockpriceBookSlipTotal(int period) {
		//printf("inside get_stockriceBookSlipTotal \n");
		int size = stockpriceBookTotal.size();
		//printf("inside get_stockriceBookSlipTotal size=%d \n",size);
		if (size > 0) {
			int ijk = 0;
			for (int i = 0; i < size; i++) {
				if (stockpriceBookTotal[i]->period == period) ijk = i;
			}
			if(agent_id==545) fprintf(app_context.config->out_test3, "period=,%d,production_price=,%I64d,bought_price=,%I64d,balance_price=,%I64d,\n", period, stockpriceBookTotal[ijk]->production_price, stockpriceBookTotal[ijk]->bought_price, stockpriceBookTotal[ijk]->balance_price);
			return stockpriceBookTotal[ijk];
		}
		else return 0;

	}
	//---------------
	struct materialpriceBookSlip* get_materialpriceBookSlipTotal(int period) {
//	printf("inside get_materialriceBookSlipTotal \n");
		int size = materialpriceBookTotal.size();
	//	printf("inside get_materialriceBookSlipTotal size=%d \n",size);
		if (size > 0) {
			int ijk = 0;
			for (int i = 0; i < size; i++) {
				if (materialpriceBookTotal[i]->period == period) ijk = i;
			}
			if (agent_id == 545) fprintf(app_context.config->out_test3, "period=,%d,production_price=,%I64d,bought_price=,%I64d,balance_price=,%I64d,\n", period, materialpriceBookTotal[ijk]->production_price, materialpriceBookTotal[ijk]->bought_price, materialpriceBookTotal[ijk]->balance_price);
			return materialpriceBookTotal[ijk];
		}
		else return 0;

	}
	//----------------------
	

	void print_TB_table() {
		//agentaggregator->do_kimatsu()の中で特定のAgentについて呼ばれる
		/*int BStable_Dr_total = 0;
		int BStable_Cr_total = 0;
		int PLtable_Dr_total = 0;
		int PLtable_Cr_total = 0;
		int size = TB_table.size();
		*/
		 //printf("before AccountData* accountdata=NULL\n");
		AccountData* accountData=NULL;
		//printf("before consumerdata\n");
		if (get_agent_type() == 31 || get_agent_type() == 311 || get_agent_type() == 312 || get_agent_type() == 310 || get_agent_type() == 3100 || get_agent_type() == 3110 || get_agent_type() == 3120) accountData = app_context.config->consumerData;
		else if (get_agent_type() == 2130) accountData = app_context.config->soleproprietorData;
		else if (get_agent_type() == 2131) accountData = app_context.config->retailerData;
		else if (get_agent_type() == 2132) accountData = app_context.config->wholesalerData;
	//	else if (get_agent_type() == 1) accountData = app_context.config->governmentData;	
		else {}
		//printf("before for(i= agent_id=%d TB_table.size()=%d\n",get_agent_id(),size);
		//printf("before print_TB_table()\n");
		//-------------------------
		print_TB_table(accountData, TB_table);
		if(get_agent_id()==509) print_agent_TB_table(accountData, TB_table);
		//printf("after print_TB_table(accountData, TB_table)\n");
		//--------------------------------
	}
	void print_TB_table(AccountData* accountData, vector<trialBalanceSlip*> TB_table) {
		//bookKeeperの中で定義したprint_TB_table関数
		//print_TB_table()の中でagent毎に呼ばれる.
		long long BStable_Dr_total = 0;
		long long BStable_Cr_total = 0;
		long long PLtable_Dr_total = 0;
		long long PLtable_Cr_total = 0;
		int size = TB_table.size();
		for (int i = 0; i < size; i++) {
			//printf("inside for(i= %d agent_id=%d TB_table.size()=%d\n",i, get_agent_id(), size);
			if (i == 0) {
				fprintf(accountData->out_TBtable, "period=,%d,agent_id=%d agent_type=%d\n", TB_table[i]->period, get_agent_id(), get_agent_type());
				//printf("after fprintf i=%d\n",i);
				fprintf(accountData->out_TBtable, ",,,,,試算表,,,,,貸借対照表,,,,損益計算書,\n");
				fprintf(accountData->out_TBtable, ",,,科目コード,勘定科目,借方,貸方,,,期首残高,借方,貸方,期末残高,,借方,貸方,\n");
				//printf("after fprintf) inside for(i= %d agent_id=%d TB_table.size()=%d\n", i, get_agent_id(), size);

			}
			//試算表の各行プリントアウト
			//printf("%d,%s,%d,%d,", TB_table[i]->code, TB_table[i]->title.c_str(), TB_table[i]->Dr_total, TB_table[i]->Cr_total);
			fprintf(accountData->out_TBtable, ",,,%d,%s,%I64d,%I64d,", TB_table[i]->code, TB_table[i]->title.c_str(), TB_table[i]->Dr_total, TB_table[i]->Cr_total);

			if (TB_table[i]->code == 0) {
				//貸借対照表の資産科目各行プリントアウト
				fprintf(accountData->out_TBtable, ",,,,,%I64d,%I64d,,%I64d,,\n", TB_table[i]->opening_balance, TB_table[i]->Dr_total - TB_table[i]->Cr_total, TB_table[i]->closing_balance);
				BStable_Dr_total += TB_table[i]->Dr_total - TB_table[i]->Cr_total;
			}
			else if (TB_table[i]->code == 2) {
				//貸借対照表の負債科目各行プリントアウト
				fprintf(accountData->out_TBtable, ",,,,,%I64d,,%I64d,%I64d,,\n", TB_table[i]->opening_balance, TB_table[i]->Cr_total - TB_table[i]->Dr_total, TB_table[i]->closing_balance);
				BStable_Cr_total += TB_table[i]->Cr_total - TB_table[i]->Dr_total;
			}
			else if (TB_table[i]->code == 1) {
				//損益計算書の経費科目各行プリントアウト
				fprintf(accountData->out_TBtable, ",,,,,,,,,,%I64d,,,\n", TB_table[i]->Dr_total - TB_table[i]->Cr_total);
				PLtable_Dr_total += TB_table[i]->Dr_total - TB_table[i]->Cr_total;
			}
			else if (TB_table[i]->code == 3) {
				//損益計算書の売上科目各行プリントアウト
				fprintf(accountData->out_TBtable, ",,,,,,,,,,,%I64d,,\n", TB_table[i]->Cr_total - TB_table[i]->Dr_total);
				PLtable_Cr_total += TB_table[i]->Cr_total - TB_table[i]->Dr_total;
			}
		}
		//printf("after for agent_id=%d TB_table.size()=%d\n", get_agent_id(), size);
		//printf("当期利益,,,,,,%d,,%d,\n", BStable_Dr_total - BStable_Cr_total, PLtable_Cr_total - PLtable_Dr_total);
		fprintf(accountData->out_TBtable, ",,,,当期利益,,,,,,,%I64d,,,%I64d,\n", BStable_Dr_total - BStable_Cr_total, PLtable_Cr_total - PLtable_Dr_total);
		fprintf(accountData->out_TBtable, ",,,,合計,,,,,,%I64d,%I64d,,,%I64d,%I64d,\n", BStable_Dr_total, BStable_Dr_total, PLtable_Cr_total, PLtable_Cr_total);
		fprintf(accountData->out_TBtable, ",\n");
		set_profit(BStable_Dr_total - BStable_Cr_total);
		/*
		fprintf(accountData->out_profit, "period=,%d, agent_id=,%d, agent_type=,%d,profit=,%I64d,\n",
				app_context.period_count, get_agent_id(), get_agent_type(), get_profit());
       */
		print_profit(accountData, get_profit());
	
	//	printf("before print_profit\n");
		if (get_agent_type() ==2130 ||get_agent_type() == 2131 || get_agent_type() == 2132){
	//		printf("profit= %I64d \n", get_profit());
		//	AccountData* accountData=NULL;
		//		if (get_agent_type() == 2131) accountData = app_context.config->retailerData;
		//		else if (get_agent_type() == 2132) accountData = app_context.config->wholesalerData;
			//fprintf(accountData->out_profit, "agent_id=,%d, profit=,%I64d,\n)",get_agent_id(),get_profit());
				//print_profit(get_profit());
		}
		//printf("after print_profit \n");
	
	}
	void print_agent_TB_table(AccountData* accountData, vector<trialBalanceSlip*> TB_table) {
		//bookKeeperの中で定義したprint_TB_table関数
		//print_TB_table()の中で特定のagentについて呼ばれる.
		long long BStable_Dr_total = 0;
		long long BStable_Cr_total = 0;
		long long PLtable_Dr_total = 0;
		long long PLtable_Cr_total = 0;
		int size = TB_table.size();
		for (int i = 0; i < size; i++) {
			//printf("inside for(i= %d agent_id=%d TB_table.size()=%d\n",i, get_agent_id(), size);
			if (i == 0) {
				fprintf(accountData->out_agent_TBtable, "period=,%d,agent_id=%d agent_type=%d\n", TB_table[i]->period, get_agent_id(), get_agent_type());
				//printf("after fprintf i=%d\n",i);
				fprintf(accountData->out_agent_TBtable, ",,,,,試算表,,,,,貸借対照表,,,,損益計算書,\n");
				fprintf(accountData->out_agent_TBtable, ",,,科目コード,勘定科目,借方,貸方,,,期首残高,借方,貸方,期末残高,,借方,貸方,\n");
				//printf("after fprintf) inside for(i= %d agent_id=%d TB_table.size()=%d\n", i, get_agent_id(), size);

			}
			//試算表の各行プリントアウト
			//printf("%d,%s,%d,%d,", TB_table[i]->code, TB_table[i]->title.c_str(), TB_table[i]->Dr_total, TB_table[i]->Cr_total);
			fprintf(accountData->out_agent_TBtable, ",,,%d,%s,%I64d,%I64d,", TB_table[i]->code, TB_table[i]->title.c_str(), TB_table[i]->Dr_total, TB_table[i]->Cr_total);

			if (TB_table[i]->code == 0) {
				//貸借対照表の資産科目各行プリントアウト
				fprintf(accountData->out_agent_TBtable, ",,,,,%I64d,%I64d,,%I64d,,\n", TB_table[i]->opening_balance, TB_table[i]->Dr_total - TB_table[i]->Cr_total, TB_table[i]->closing_balance);
				BStable_Dr_total += TB_table[i]->Dr_total - TB_table[i]->Cr_total;
			}
			else if (TB_table[i]->code == 2) {
				//貸借対照表の負債科目各行プリントアウト
				fprintf(accountData->out_agent_TBtable, ",,,,,%I64d,,%I64d,%I64d,,\n", TB_table[i]->opening_balance, TB_table[i]->Cr_total - TB_table[i]->Dr_total, TB_table[i]->closing_balance);
				BStable_Cr_total += TB_table[i]->Cr_total - TB_table[i]->Dr_total;
			}
			else if (TB_table[i]->code == 1) {
				//損益計算書の経費科目各行プリントアウト
				fprintf(accountData->out_agent_TBtable, ",,,,,,,,,,%I64d,,,\n", TB_table[i]->Dr_total - TB_table[i]->Cr_total);
				PLtable_Dr_total += TB_table[i]->Dr_total - TB_table[i]->Cr_total;
			}
			else if (TB_table[i]->code == 3) {
				//損益計算書の売上科目各行プリントアウト
				fprintf(accountData->out_agent_TBtable, ",,,,,,,,,,,%I64d,,\n", TB_table[i]->Cr_total - TB_table[i]->Dr_total);
				PLtable_Cr_total += TB_table[i]->Cr_total - TB_table[i]->Dr_total;
			}
		}
		//printf("after for agent_id=%d TB_table.size()=%d\n", get_agent_id(), size);
		//printf("当期利益,,,,,,%d,,%d,\n", BStable_Dr_total - BStable_Cr_total, PLtable_Cr_total - PLtable_Dr_total);
		fprintf(accountData->out_agent_TBtable, ",,,,当期利益,,,,,,,%I64d,,,%I64d,\n", BStable_Dr_total - BStable_Cr_total, PLtable_Cr_total - PLtable_Dr_total);
		fprintf(accountData->out_agent_TBtable, ",,,,合計,,,,,,%I64d,%I64d,,,%I64d,%I64d,\n", BStable_Dr_total, BStable_Dr_total, PLtable_Cr_total, PLtable_Cr_total);
		fprintf(accountData->out_agent_TBtable, ",\n");
		set_profit(BStable_Dr_total - BStable_Cr_total);
		/*
		fprintf(accountData->out_profit, "period=,%d, agent_id=,%d, agent_type=,%d,profit=,%I64d,\n",
				app_context.period_count, get_agent_id(), get_agent_type(), get_profit());
	   */
		//print_profit(accountData, get_profit());

		//	printf("before print_profit\n");
		if (get_agent_type() ==2130 ||get_agent_type() == 2131 || get_agent_type() == 2132) {
			//		printf("profit= %I64d \n", get_profit());
				//	AccountData* accountData=NULL;
				//		if (get_agent_type() == 2131) accountData = app_context.config->retailerData;
				//		else if (get_agent_type() == 2132) accountData = app_context.config->wholesalerData;
					//fprintf(accountData->out_profit, "agent_id=,%d, profit=,%I64d,\n)",get_agent_id(),get_profit());
						//print_profit(get_profit());
		}
		//printf("after print_profit \n");

	}
	void print_profit(AccountData* accountData, long long profit) {
		//printf("before fprintf(accountData,profit agent_id=%d \n",get_agent_id());
		//fprintf(accountData->out_profit, "inside print_profit \n");
		fprintf(accountData->out_profit, "period=,%d, agent_id=,%d, agent_type=,%d,profit=,%I64d,\n",
			app_context.period_count, get_agent_id(), get_agent_type(), profit);
		//printf("after fprintf(accountData,profit\n");
	}
	void print_profit(long long profit) {
		AccountData* accountData = NULL;
		//printf("before consumerdata\n");
		if (get_agent_type() == 31 || get_agent_type() == 311 || get_agent_type() == 312 || get_agent_type() == 310 || get_agent_type() == 3100 || get_agent_type() == 3110 || get_agent_type() == 3120) accountData = app_context.config->consumerData;
		if (get_agent_type() == 2131) accountData = app_context.config->retailerData;
		else if (get_agent_type() == 2132) accountData = app_context.config->wholesalerData;
		else if (get_agent_type() == 2130) accountData = app_context.config->soleproprietorData;
	//	else if (get_agent_type() == 1) accountData = app_context.config->governmentData;	
		else {}
		//printf("before for(i= agent_id=%d TB_table.size()=%d\n",get_agent_id(),size);
		//printf("before print_profit(accountdata,profit) agent_type= %d\n",get_agent_type());
		//-------------------------
		if(get_agent_type() == 2130 ||get_agent_type() == 2131 || get_agent_type() == 2132) 
			print_profit(accountData, profit);
	}
	void createGL_Slip() {
		//AgentAggregatoのdo_kimatsu()で各Agent毎に呼ばれる
		// 毎期の仕訳帳に基づいて勘定科目毎に期中の各仕訳データの借方、貸方の合計値を計算する
		//TB_tableを作成する	
		//GLは作成しない、特定のAgentとperiodに対してGLをプリントアウトする
		int agent_id1 = 415;//wholesaler_worker
		int agent_id0 = 15;//retailer_worker
		int agent_id2 = 460;//soleproprietor_worker
		int period0 = 2;
		//GL(general ledger: 総勘定元帳)の各科目集計表作成
		int size = title_list.size();//勘定科目の総数
		int size_journal = journal.size();//仕訳伝票の総数
		//Agentのtype毎にプリントアウト先を指定する（consumer, retailer, wholesaler,soleproprietor)
		AccountData* accountData = NULL;
		if (get_agent_type() == 31 || get_agent_type() == 311 || get_agent_type() == 312 || get_agent_type() == 310 || get_agent_type() == 3100 || get_agent_type() == 3110 || get_agent_type() == 3120) accountData = app_context.config->consumerData;
		else if (get_agent_type() == 2131) accountData = app_context.config->retailerData;
		else if (get_agent_type() == 2132) accountData = app_context.config->wholesalerData;
		else if (get_agent_type() == 2130) accountData = app_context.config->soleproprietorData;
	//	else if ((get_agent_type() == 1)) accountData = app_context.config->governmentData;	
		else {}
		//printf("inside createGL_Slip() period = % d size=%d size_journal=%d agent_id= %d \n", app_context.period_count, size, size_journal, get_agent_id());
		if (app_context.period_count == 0) {
			if (agent_id == agent_id0 || agent_id == agent_id1 || agent_id == agent_id2)
				fprintf(accountData->out_GL, "\n, agent_id=,%d,agent_type=,%d,\n", agent_id, agent_type);
		}
		for (int i = 0; i < size; i++) { //勘定科目毎に借方、貸方を集計する、iは勘定科目、jは仕訳帳のfor分、残高は？
			title_list[i]->Dr_total = 0;
			title_list[i]->Cr_total = 0;
			//title_list[i]->balance = 0;
			if (title_list[i]->period > 0) title_list[i]->balance = get_balance(app_context.period_count - 1, title_list[i]->title);
					//一期前の期末残高を各科目仕訳データ集計前の残高とする
			else title_list[i]->balance = 0;
			//------残高計算、後で見直しする  20220821-------------
			/*
			if (title_list.size() == 0) {
				title_list[i]->opening_balance = 0;
				title_list[i]->balance = 0;
			}
			else {
				//printf("before getGLslip period=%d\n", app_context.period_count);
				//title_list_previous = getGLslip()->title_list;
				//printf("after getGLslip period=%d i=%d size of title_list\previous=%d\n", app_context.period_count,i,title_list_previous.size());
				//title_list[i]->opening_balance = title_list_previous[i]->balance;
				//printf("after title_list_previous[i]\n");
				//title_list[i]->opening_balance = getGLslip()->title_list[i]->balance;
				title_list[i]->opening_balance = 0;
				title_list[i]->balance = title_list[i]->opening_balance;
			}
			*/
			//-------残高計算終わり、後で見直しする 20220821-------------------
			//	printf("before total calculation agent_type=%d i= %d\n", get_agent_type(),i);
			//特定のAgentとperodについて勘定科目の見出しをプリントアウトする、GL用
			if (app_context.period_count < period0) {
				if (agent_id == agent_id0 || agent_id == agent_id1 || agent_id == agent_id2) fprintf(accountData->out_GL, "---------,----------,-----------,----------,-----------,---------,\n");
				if (agent_id == agent_id0 || agent_id == agent_id1 || agent_id == agent_id2) fprintf(accountData->out_GL, " ,%s,,借方,貸方,残高,\n", title_list[i]->title.c_str());
				if (agent_id == agent_id0 || agent_id == agent_id1 || agent_id == agent_id2) fprintf(accountData->out_GL, "---------,----------,-----------,----------,-----------,---------,\n");
			}
			//借方トータル、借方トータルの計算（科目ごと、仕訳データの各行ごとに残高を計算する、最後の残高の値が期末残高となっている）	
			for (int j = 0; j < size_journal; j++) {
				//借方の集計
				if (journal[j]->Dr_title == title_list[i]->title) {
					//借方合計を計算し、同時に残高を計算する
					//title_list[i]->code = journal[j]->Dr_code;
					title_list[i]->Dr_total += journal[j]->dabit;
					if (title_list[i]->code == 0 || title_list[i]->code == 1) title_list[i]->balance += journal[j]->dabit;
					if (title_list[i]->code == 2 || title_list[i]->code == 3) title_list[i]->balance -= journal[j]->dabit;
					//総勘定元帳のプリントアウト、仕訳データの各行毎に残高をプリントアウトする、経費科目の場合は残高＝借方トータル
					title_list[i]->period = journal[j]->period;
					if (agent_id == agent_id0 && title_list[i]->period < period0)fprintf(accountData->out_GL, " period=,%d,%s,%I64d,,%I64d,\n", title_list[i]->period, journal[j]->Cr_title.c_str(), journal[j]->dabit, title_list[i]->balance);
					if (agent_id == agent_id1 && title_list[i]->period < period0)fprintf(accountData->out_GL, " period=,%d,%s,%I64d,,%I64d,\n", title_list[i]->period, journal[j]->Cr_title.c_str(), journal[j]->dabit, title_list[i]->balance);
					if (agent_id == agent_id2 && title_list[i]->period < period0)fprintf(accountData->out_GL, " period=,%d,%s,%I64d,,%I64d,\n", title_list[i]->period, journal[j]->Cr_title.c_str(), journal[j]->dabit, title_list[i]->balance);

				}
				//	printf("inside j2 = % d\n", j);
					//貸方の集計
				else if (journal[j]->Cr_title == title_list[i]->title) {
					title_list[i]->Cr_total += journal[j]->credit;
					if (title_list[i]->code == 0 || title_list[i]->code == 1) title_list[i]->balance -= journal[j]->credit;
					if (title_list[i]->code == 2 || title_list[i]->code == 3) title_list[i]->balance += journal[j]->credit;
					//総勘定元帳のプリントアウト、仕訳データの各行毎に残高をプリントアウトする、売上科目の場合は残高＝貸方トータル
					title_list[i]->period = journal[j]->period;
					if (agent_id == agent_id0 && title_list[i]->period < period0)fprintf(accountData->out_GL, " period=,%d,%s,,%I64d,%I64d,\n", title_list[i]->period, journal[j]->Dr_title.c_str(), journal[j]->credit, title_list[i]->balance);
					if (agent_id == agent_id1 && title_list[i]->period < period0)fprintf(accountData->out_GL, " period=,%d,%s,,%I64d,%I64d,\n", title_list[i]->period, journal[j]->Dr_title.c_str(), journal[j]->credit, title_list[i]->balance);
					if (agent_id == agent_id2 && title_list[i]->period < period0)fprintf(accountData->out_GL, " period=,%d,%s,,%I64d,%I64d,\n", title_list[i]->period, journal[j]->Dr_title.c_str(), journal[j]->credit, title_list[i]->balance);

				}
				else {}
			}//for(j=0;...の終わり
			//残高の値を格納する
			//create_PR_balance(title_list[i]->period,title_list[i]->title, title_list[i]->balance);

			if (get_agent_type() == 2132 && get_agent_id() == 545 && app_context.period_count < 5) {
				//if(app_context.period_count==0)
					//fprintf(app_context.config->wholesalerData->out_balance, "i=,科目=,%s,%d,balnace=,%d,\n", i,title_list[i]->title, 0);
				//else
				/*
				if(app_context.period_count>0 && title_list[i]->title=="現金")
					fprintf(app_context.config->wholesalerData->out_balance, "period=,%d,i=,%d,科目=,%s,balnace=,%I64d,\n",title_list[i]->period, i, title_list[i]->title.c_str(),get_balance(app_context.period_count - 1, title_list[i]->title));
				*/
			}
			//特定agentとperiodについて、勘定科目の借方トータル、貸方トータルをプリントアウトする
			if (agent_id == agent_id0 && title_list[i]->period < period0)fprintf(accountData->out_GL, " period=,%d,total,%I64d,%I64d,\n", title_list[i]->period, title_list[i]->Dr_total, title_list[i]->Cr_total);
			if (agent_id == agent_id1 && title_list[i]->period < period0)fprintf(accountData->out_GL, " period=,%d,total,%I64d,%I64d,\n", title_list[i]->period, title_list[i]->Dr_total, title_list[i]->Cr_total);
			if (agent_id == agent_id2 && title_list[i]->period < period0)fprintf(accountData->out_GL, " period=,%d,total,%I64d,%I64d,\n", title_list[i]->period, title_list[i]->Dr_total, title_list[i]->Cr_total);
			//試算表を生成する（借方トータル、貸方トータルの値を格納する、両者の差から残高を計算し格納する
			//if (app_context.period_count == 0)
			//	createTBSlip(title_list[i]->period, title_list[i]->title, title_list[i]->Dr_total, title_list[i]->Cr_total, title_list[i]->code, 0);
			//else
		//	if (get_agent_type() == 2130 || get_agent_type() == 310) printf("before creatTBSlip agent_id=%d agent_type=%d title_list=%s\n", get_agent_id(), get_agent_type(),title_list[i]->title.c_str());
				createTBSlip(app_context.period_count, title_list[i]->title, title_list[i]->Dr_total, title_list[i]->Cr_total, title_list[i]->code, get_balance(app_context.period_count - 1, title_list[i]->title));
			//	if (get_agent_type() == 2130 || get_agent_type() == 310) printf("after creatTBSlip agent_id=%d agent_type=%d \n", get_agent_id(), get_agent_type());
				// createTBSlip(title_list[i]->period, title_list[i]->title, title_list[i]->Dr_total, title_list[i]->Cr_total, title_list[i]->code, get_balance(app_context.period_count - 1, title_list[i]->title));
			//title_list_previous.push_back(title_list[i]);
				//printf("after for(j=0,,,,,\n");
		}//for(i=0;....の終わり　各勘定科目について試算表の作成終わり
		if (app_context.period_count == app_context.config->run_period_count) {
			if (get_agent_id() == 545 || get_agent_id()==455) print_PR_balance();
		}
		//printf("before print_Books()\n");
		print_Books(TB_table);
		//printf("after print_Books()\n");
			//PR_balance.clear();
		//GLslip = new GLSlip;
		//GLslip_list.push_back(GLslip);
	}
	void print_Books(vector<trialBalanceSlip*> TB_table) {
		//試算表の各勘定科目について試算表の値をプリントアウトする。
		//create_GLSlip()の中の最後にで呼ばれる。
		//printf("start of print_Books period=%d agent_id=%d\n", app_context.period_count, get_agent_id());
		int size = TB_table.size();
		AccountData* accountData = NULL;
		if (get_agent_type() == 31 || get_agent_type() == 311 || get_agent_type() == 312 || get_agent_type() == 310 || get_agent_type() == 3100 || get_agent_type() == 3110 || get_agent_type() == 3120) accountData = app_context.config->consumerData;
		else if (get_agent_type() == 2131) accountData = app_context.config->retailerData;
		else if (get_agent_type() == 2132) accountData = app_context.config->wholesalerData;
		else if (get_agent_type() == 2130) accountData = app_context.config->soleproprietorData;
	//	else if ((get_agent_type() == 1)) accountData = app_context.config->governmentData;	
		else {}	
		//fprintf(accountData->out_TBtableCashBook, "period,agent_id,title, opening_balance, Dr_total, Cr_Total,closing_balance, \n");
		//printf("before for in print_Books() agent_id=%d agent_type=%d TBtable_size=%d, title_list[0]=%s\n",get_agent_id(),get_agent_type(),size,title_list[0]->title.c_str());
		
		for (int i = 0; i < size; i++) {
		//	printf("inside for in printBooks() i=%d agent_id=%d  title_list[%d]=%s size=%d\n",i,get_agent_id(),i,title_list[i]->title.c_str(),size);
		//	if (get_agent_type()==2131 && title_list[i]->title == "現金") {
			 if ( title_list[i]->title == "現金") {
		//		 printf("inside if before fprintf for gennkin\n");
				fprintf(accountData->out_TBtableCashBook, "%d,%d,%s,%I64d,%I64d,%I64d,%I64d,\n ",TB_table[i]->period,get_agent_id(),TB_table[i]->title.c_str(),TB_table[i]->opening_balance,TB_table[i]->Dr_total, TB_table[i]->Cr_total,TB_table[i]->closing_balance );
			 }
			 else if (title_list[i]->title == "預金") {
				 fprintf(accountData->out_TBtableBankBook, "%d,%d,%s,%I64d,%I64d,%I64d,%I64d,\n ", TB_table[i]->period, get_agent_id(), TB_table[i]->title.c_str(), TB_table[i]->opening_balance, TB_table[i]->Dr_total, TB_table[i]->Cr_total, TB_table[i]->closing_balance);
			 }
			 else if (title_list[i]->title == "製品在庫") {
				 fprintf(accountData->out_TBtableInventoryBook, "%d,%d,%s,%I64d,%I64d,%I64d,%I64d,\n ", TB_table[i]->period, get_agent_id(), TB_table[i]->title.c_str(), TB_table[i]->opening_balance, TB_table[i]->Dr_total, TB_table[i]->Cr_total, TB_table[i]->closing_balance);
			 }
			 else if (title_list[i]->title == "売上") {
				 fprintf(accountData->out_TBtableSaleBook, "%d,%d,%s,%I64d,%I64d,%I64d,%I64d,\n ", TB_table[i]->period, get_agent_id(), TB_table[i]->title.c_str(), TB_table[i]->opening_balance, TB_table[i]->Dr_total, TB_table[i]->Cr_total, TB_table[i]->closing_balance);
			 }
			 else if (title_list[i]->title == "直接労務費") {
		//		 printf("inside if before 直接労務費, i=%d\n", i);
				// printf("inside if before 直接労務費,%s i=%d\n",i,title_list[i]->title.c_str() );
		//		 printf("TBtable[%d]->period==%d,\n", i, app_context.period_count);
		//		 printf("TBtable[%d]->period==%d,agent_id=%d title=%s 借方トータル= %I64d\n", i, TB_table[i]->period,get_agent_id(),TB_table[i]->title.c_str(),TB_table[i]->Dr_total);
		//		 printf("period=%d,agent_id=%d,title=%s,期首残高＝%I64d,借方トータル＝%I64d,貸方トータル＝%I64d,期末残高＝%I64d,\n ", TB_table[i]->period, get_agent_id(), TB_table[i]->title.c_str(), TB_table[i]->opening_balance, TB_table[i]->Dr_total, TB_table[i]->Cr_total, TB_table[i]->closing_balance);
				 fprintf(accountData->out_TBtableDirectlaborcostBook, "%d,%d,%s,%I64d,%I64d,%I64d,%I64d,\n ", TB_table[i]->period, get_agent_id(), TB_table[i]->title.c_str(), TB_table[i]->opening_balance, TB_table[i]->Dr_total, TB_table[i]->Cr_total, TB_table[i]->closing_balance);
		//		 printf("inside if after fprintf直接労務費\n");
			 }
			 else if (title_list[i]->title == "直接材料費") {
		//		 printf("inside if before fprintf i=%d\n",i);			
				 fprintf(accountData->out_TBtableDirectmaterialfeeBook, "%d,%d,%s,%I64d,%I64d,%I64d,%I64d,\n ", TB_table[i]->period, get_agent_id(), TB_table[i]->title.c_str(), TB_table[i]->opening_balance, TB_table[i]->Dr_total, TB_table[i]->Cr_total, TB_table[i]->closing_balance);
		//		 printf("inside if after fprintf\n");
			 }
			// else {}
			 else if (TB_table[i]->title == "当期利益") {
			 fprintf(accountData->out_TBtableProfitBook, "%d,%d,%s,%I64d,%I64d,\n ", TB_table[i]->period, get_agent_id(), TB_table[i]->title.c_str(), TB_table[i]->Dr_total-TB_table[i]->Cr_total);
			 }
		//	 printf("after if in BookKeeper line 2040 i=%d agent_id=%d\n", i, get_agent_id());
		}//for(i=0....の終わり
	//	printf("end of print_Books period=%d agent_id=%d\n", app_context.period_count, get_agent_id());
	}
	void withdraw_bankdeposit(long long amount_withdrawal)	{
		//printf("start of withdraw_bankdepoist\n");
		//預金引き出し処理（仕訳処理、現金出納帳記入、預金帳記入）
		createjournalSlip(app_context.period_count, 0, "現金", amount_withdrawal, "預金引き出し", amount_withdrawal, "預金", 0);//20230611修正
		createcashBookSlip(app_context.period_count, amount_withdrawal, 0);
		createbankBookSlip(app_context.period_count, 0, amount_withdrawal);
		//printf("end of bank_deposit before fprint\n");
		AccountData* accountData = NULL;
		if (get_agent_type() == 31 || get_agent_type() == 311 || get_agent_type() == 312 || get_agent_type() == 310 || get_agent_type()==3100 || get_agent_type()==3110 || get_agent_type()==3120) accountData = app_context.config->consumerData;
		else if (get_agent_type() == 2131) accountData = app_context.config->retailerData;
		else if (get_agent_type() == 2132) accountData = app_context.config->wholesalerData;
		else if (get_agent_type() == 2130) accountData = app_context.config->soleproprietorData;
	//	else if ((get_agent_type() == 1)) accountData = app_context.config->governmentData;	
		else{}
		//printf("inside withdraw_bankdepoist agent_type=%d\n",get_agent_type());
		fprintf(accountData->out_deposit_withdrawalBook, "period=,%d, agent_id=, %d,amount_withdrawal=,%I64d,cashBook_balance=,%I64d,\n ", app_context.period_count, get_agent_id(), amount_withdrawal,get_cashbookslip()->balance);
		//printf("after fprint in withdraw_bankdeposit\n");
	}
	void deposit_bankaccount(long long amount_deposit) {
		//預金預け入れ処理（仕訳処理、現金出納帳記入、預金帳記入）
		createjournalSlip(app_context.period_count, 0, "預金", amount_deposit, "現金預け入れ", amount_deposit, "現金", 0);
		createcashBookSlip(app_context.period_count, 0, amount_deposit);
	    createbankBookSlip(app_context.period_count, amount_deposit, 0);
		AccountData* accountData = NULL;
		if (get_agent_type() == 31 || get_agent_type() == 311 || get_agent_type() == 312 || get_agent_type() == 310 || get_agent_type() == 3100 || get_agent_type() == 3110 || get_agent_type() == 3120) accountData = app_context.config->consumerData;
		else if (get_agent_type() == 2131) accountData = app_context.config->retailerData;
		else if (get_agent_type() == 2132) accountData = app_context.config->wholesalerData;
		else if (get_agent_type() == 2130) accountData = app_context.config->soleproprietorData;
	//	else if ((get_agent_type() == 1)) accountData = app_context.config->governmentData;	
		else {}
		fprintf(accountData->out_deposit_withdrawalBook, "period=,%d, agent_id=, %d,amount_deposit=,%I64d,bankBook_balance=,%I64d,\n ", app_context.period_count, get_agent_id(), amount_deposit, get_bankbookslip()->balance);
	}
	void clear_journal() {
		//accountant->do_kimatsu()で呼ばれる
		journal.clear();
	}
	void clear_title_list() {
		//accountant->do_kimatsu()で呼ばれる
		title_list.clear();
	}
	void clear_TB_table() {
		//Accountantのdo_kimatsu()で呼ばれる
		TB_table.clear();
	}
	void clear_PR_balance() {
		PR_balance.clear();
	}
};