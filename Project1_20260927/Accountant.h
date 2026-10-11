#pragma once
#include  "BookKeeper.h"
#include <algorithm>
#include <vector> 
/*struct trialBalanceSlip {//試算表
	int period = 0;
	string title;//勘定科目
	int Dr_total = 0;//debit　各勘定科目毎の借方トータル(各期、各勘定科目毎のトータル）
	int Cr_total = 0;//credit 各勘定科目毎の貸方トータル(各期、各勘定科目毎のトータル）
	int code = 0;		//科目コード
};*/
class Accountant
{
	//vector<trialBalanceSlip*> TB_table;//temporaryなTB_table(TB_tableの内容集計用)bookKeeperlist[i]->get_TBtable()の代わり

	vector<trialBalanceSlip*> TB_table_consumer; //各会計科目毎に同一agent_type内の借方合計と貸方合計、各期毎
	vector<trialBalanceSlip*> TB_table_retailer;
	vector<trialBalanceSlip*> TB_table_wholesaler;
	vector<trialBalanceSlip*> TB_table_soleproprietor;
	vector<trialBalanceSlip*> TB_table_government;
	vector<string> title_list_consumer;
	vector<string> title_list_retailer;
	vector<string> title_list_wholesaler;
	vector<string> title_list_soleproprietor;
	vector<string> title_list_government;
	struct trialBalanceSlip* trialBalanceslip;//試算表
		/*	int period;
		string title;//勘定科目
		int Dr_total = 0;//debit　各勘定科目毎の借方トータル(各期、各勘定科目毎のトータル）
		int Cr_total = 0;//credit 各勘定科目毎の貸方トータル(各期、各勘定科目毎のトータル）
		int code = 0;		//科目コード
		*/
	struct cashBookSlip* cashbookslip;
	vector <cashBookSlip*> cashBook_consumer;//各期が一行のカテゴリータイプごとに集計されたcashbook
	vector <cashBookSlip*> cashBook_retailer;
	vector <cashBookSlip*> cashBook_wholesaler;
	vector<cashBookSlip*> cashBook_soleproprietor;
	vector<cashBookSlip*> cashBook_government;
	struct bankBookSlip* bankbookslip;
	vector <bankBookSlip*> bankBook_consumer;//各期が一行のカテゴリータイプごとに集計されたbankbook
	vector <bankBookSlip*> bankBook_retailer;
	vector <bankBookSlip*> bankBook_wholesaler;
	vector <bankBookSlip*> bankBook_soleproprietor;
	vector <bankBookSlip*> bankBook_government;	
	struct stockBookSlip* stockbookslip;
	vector <stockBookSlip*> stockBook_consumer;
	vector <stockBookSlip*> stockBook_retailer;
	vector <stockBookSlip*> stockBook_wholesaler;
	vector <stockBookSlip*> stockBook_soleproprietor;
	vector <stockBookSlip*> stockBook_government;	
	struct stockpriceBookSlip* stockpricebookslip;
	vector <stockpriceBookSlip*> stockpriceBook_consumer;
	vector <stockpriceBookSlip*> stockpriceBook_retailer;
	vector <stockpriceBookSlip*> stockpriceBook_wholesaler;
	vector <stockpriceBookSlip*> stockpriceBook_soleproprietor;
	vector <stockpriceBookSlip*> stockpriceBook_government;	
	struct materialBookSlip* materialbookslip;
	vector <materialBookSlip*> materialBook_consumer;
	vector <materialBookSlip*> materialBook_retailer;
	vector <materialBookSlip*> materialBook_wholesaler;
	vector <materialBookSlip*> materialBook_soleproprietor;
	vector <materialBookSlip*> materialBook_government;	
	struct materialpriceBookSlip* materialpricebookslip;
	vector <materialpriceBookSlip*> materialpriceBook_consumer;
	vector <materialpriceBookSlip*> materialpriceBook_retailer;
	vector <materialpriceBookSlip*> materialpriceBook_wholesaler;
	vector <materialpriceBookSlip*> materialpriceBook_soleproprietor;
	vector <materialpriceBookSlip*> materialpriceBook_government;
	struct genkaBookSlip* genkabookslip;
	vector <genkaBookSlip*> genkaBook_retailer;
	vector <genkaBookSlip*> genkaBook_wholesaler;
	vector <genkaBookSlip*> genkaBook_soleproprietor;
	vector <long long > price;//要素はitem_type_idに対応するagent平均価格
public:
	Accountant() {
	//	bankbookslip = new bankBookSlip; bankbookslip->balance = 0; bankbookslip->deposit = 0; bankbookslip->period = 0; bankbookslip->previous_balance = 0; bankbookslip->withdrawal = 0;
	//	cashbookslip = new cashBookSlip; cashbookslip->balance = 0; cashbookslip->income = 0; cashbookslip->period = 0; cashbookslip->previous_balance = 0; cashbookslip->spending = 0;
	//	genkabookslip = new genkaBookSlip; genkabookslip->direct_labor_cost = 0; genkabookslip->direct_material_cost = 0; genkabookslip->genka = 0; genkabookslip->item_type_id = 0;
	//	genkabookslip->period = 0; genkabookslip->size_shiped = 0;
	}
	vector <BookKeeper*> bookKeeperlist;
	void setup(){
		unsigned int size = 0;
		size=bookKeeperlist.size();
		
		for(unsigned int i=0;i<size;i++){
			fprintf(app_context.config->out3,"inside setup of accountant bookKeeperlist_size=%d bookKeeper_address=%x agent_id=%d agent_type=%d\n"
				,size,unsigned (bookKeeperlist[i]),bookKeeperlist[i]->get_agent_id(),bookKeeperlist[i]->get_agent_type());
			
		}
		
	}
	void set_bookKeeper(BookKeeper* bookKeeper){    //agent[i]->createBookKeeper()で呼ばれる
		bookKeeperlist.push_back(bookKeeper);
    }
	void initialize_TBtable(vector<trialBalanceSlip*> TB_table) {
		//この関数は現在は使わない、define_initial_TB_table(int category_type)を使う　2022年5月24日　荻林
		for (unsigned int i = 0; i < TB_table.size(); i++){
			TB_table[i]->Cr_total = 0;
			TB_table[i]->Dr_total = 0;
			TB_table[i]->opening_balance = 0;
			TB_table[i]->closing_balance = 0;
		}
	}
	void add_DrCrValues(int i,vector<trialBalanceSlip*> TB_table) {
		//SimulationAppのprocess_period()の中のapp_context.accountant->do_kimatsu();で呼ばれる
		//agent iの勘定科目がTB_tableと同じものを抽出し借方と貸方をそれぞれTB_tableに足し合わせる
		//引数のTB_tableは借方、貸方をエージェント単位で合計した試算表
		for (unsigned int j = 0; j < TB_table.size(); j++) {
			for (unsigned int k = 0; k < bookKeeperlist[i]->get_TBtable().size(); k++) {
				//printf("j=%d k=%d title=%s title=%s\n", j, k, TB_table_consumer[j]->title.c_str(),bookKeeperlist[i]->get_TBtable()[k]->title.c_str());
				if (bookKeeperlist[i]->get_TBtable()[k]->title == TB_table[j]->title) {
					if(bookKeeperlist[i]->get_agent_type()==2132)
						fprintf(app_context.config->out4, "period=,%d, agent_id=,%d, title=,%s,title=,%s,Dr_total=,%I64d,Cr_total=,%I64d,\n",
							bookKeeperlist[i]->get_TBtable()[k]->period,bookKeeperlist[i]->get_agent_id(),bookKeeperlist[i]->get_TBtable()[k]->title.c_str(),TB_table[j]->title.c_str(),
							bookKeeperlist[i]->get_TBtable()[k]->Dr_total,bookKeeperlist[i]->get_TBtable()[k]->Cr_total );
					TB_table[j]->Cr_total += bookKeeperlist[i]->get_TBtable()[k]->Cr_total;
					TB_table[j]->Dr_total += bookKeeperlist[i]->get_TBtable()[k]->Dr_total;
					TB_table[j]->opening_balance += bookKeeperlist[i]->get_TBtable()[k]->opening_balance;
					TB_table[j]->closing_balance += bookKeeperlist[i]->get_TBtable()[k]->closing_balance;
				}
			}//for(int k=0....の終わり
		}//for(int j=0.......の終わり
	}
	void add_IncomeSpendingValues(int i, int period,vector <cashBookSlip*> cashBook) {
		//カテゴリタイプ毎に、cashbookのincomeとspendingについて値を合計したものをcashBook_consumer, cashBook_retailer,cashBook_wholesalerとする。
	}
	void create_cashBookTotal(int category_type,int period) {
		//Accountant::do_kimatsu()で各periodの期末で呼ばれる 各エージェントカテゴリーごとにcashBookのincomeとspendingを集計する
		cashbookslip = new cashBookSlip;
		cashbookslip->income = 0;
		cashbookslip->spending = 0;
		cashbookslip->balance = 0;
		if (category_type == 31) {
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				if (bookKeeperlist[i]->get_agent_type() == 31 || bookKeeperlist[i]->get_agent_type() == 310 ||bookKeeperlist[i]->get_agent_type() == 311 || bookKeeperlist[i]->get_agent_type() == 312 || bookKeeperlist[i]->get_agent_type()==3100 || bookKeeperlist[i]->get_agent_type()==3110 || bookKeeperlist[i]->get_agent_type()==3120) {
					cashbookslip->income += bookKeeperlist[i]->get_cashBookSlipTotal(period)->income;
					cashbookslip->spending += bookKeeperlist[i]->get_cashBookSlipTotal(period)->spending;
					cashbookslip->balance += bookKeeperlist[i]->get_cashBookSlipTotal(period)->balance;
					cashbookslip->period = period;
				}	
			}//agent毎の合計終わり
			cashBook_consumer.push_back(cashbookslip);
		}
		/*
		else if (category_type == 1) {
			printf("inside print_cashBooktotal for government, bookKeeperlist.size()=%d ", bookKeeperlist.size());
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				if (bookKeeperlist[i]->get_agent_type() == 1) {
					printf("inside for in print_cashBooktotal i=%d get_agent_type() = %d agent_id=%d cashBooktotal.size=%d \n ",i,bookKeeperlist[i]->get_agent_type(),bookKeeperlist[i]->get_agent_id());
					cashbookslip->income += bookKeeperlist[i]->get_cashBookSlipTotal(period)->income;
					cashbookslip->spending += bookKeeperlist[i]->get_cashBookSlipTotal(period)->spending;
					cashbookslip->balance += bookKeeperlist[i]->get_cashBookSlipTotal(period)->balance;
					cashbookslip->period = period;
				}
			}//agent毎の合計終わり
			printf("inside print_cashBooktotal for government, income=%I64d, spending=%I64d, balance=%I64d\n", cashbookslip->income, cashbookslip->spending, cashbookslip->balance);
			cashBook_soleproprietor.push_back(cashbookslip);
		}
		*/
		else if (category_type == 2130) {
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				if (bookKeeperlist[i]->get_agent_type() == 2130) {
					cashbookslip->income += bookKeeperlist[i]->get_cashBookSlipTotal(period)->income;
					cashbookslip->spending += bookKeeperlist[i]->get_cashBookSlipTotal(period)->spending;
					cashbookslip->balance += bookKeeperlist[i]->get_cashBookSlipTotal(period)->balance;
					cashbookslip->period = period;
				}
			}//agent毎の合計終わり
			cashBook_soleproprietor.push_back(cashbookslip);
		}
		else if (category_type == 2131) {
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				if (bookKeeperlist[i]->get_agent_type() == 2131) {
					cashbookslip->income += bookKeeperlist[i]->get_cashBookSlipTotal(period)->income;
					cashbookslip->spending += bookKeeperlist[i]->get_cashBookSlipTotal(period)->spending;
					cashbookslip->balance += bookKeeperlist[i]->get_cashBookSlipTotal(period)->balance;
					cashbookslip->period = period;
				}
			}//agent毎の合計終わり
			cashBook_retailer.push_back(cashbookslip);
		}
		else if (category_type == 2132) {
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				if (bookKeeperlist[i]->get_agent_type() == 2132) {
					cashbookslip->income += bookKeeperlist[i]->get_cashBookSlipTotal(period)->income;
					cashbookslip->spending += bookKeeperlist[i]->get_cashBookSlipTotal(period)->spending;
					cashbookslip->balance += bookKeeperlist[i]->get_cashBookSlipTotal(period)->balance;
					cashbookslip->period = period;
				}
			}//agent毎の合計終わり
			cashBook_wholesaler.push_back(cashbookslip);
		}

	}
	void create_bankBookTotal(int category_type, int period) {
		bankbookslip = new bankBookSlip;
		bankbookslip->deposit= 0;
		bankbookslip->withdrawal = 0;
		bankbookslip->balance = 0;
		if (category_type == 31) {
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				if (bookKeeperlist[i]->get_agent_type() == 31 || bookKeeperlist[i]->get_agent_type() == 310 ||bookKeeperlist[i]->get_agent_type() == 311 || bookKeeperlist[i]->get_agent_type() == 312) {
					bankbookslip->deposit += bookKeeperlist[i]->get_bankBookSlipTotal(period)->deposit;
					bankbookslip->withdrawal += bookKeeperlist[i]->get_bankBookSlipTotal(period)->withdrawal;
					bankbookslip->balance += bookKeeperlist[i]->get_bankBookSlipTotal(period)->balance;
					bankbookslip->period = period;
				}
			}//agent毎の合計終わり
			bankBook_consumer.push_back(bankbookslip);
		}
		/*
		else if (category_type == 1) {
			bankbookslip->deposit = 0;
			bankbookslip->withdrawal = 0;
			bankbookslip->balance = 0;
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				if (bookKeeperlist[i]->get_agent_type() == 1) {
					bankbookslip->deposit += bookKeeperlist[i]->get_bankBookSlipTotal(period)->deposit;
					bankbookslip->withdrawal += bookKeeperlist[i]->get_bankBookSlipTotal(period)->withdrawal;
					bankbookslip->balance += bookKeeperlist[i]->get_bankBookSlipTotal(period)->balance;
					bankbookslip->period = period;
				}
			}//agent毎の合計終わり
			bankBook_soleproprietor.push_back(bankbookslip);
		}
		*/
		else if (category_type == 2130) {
			bankbookslip->deposit = 0;
			bankbookslip->withdrawal = 0;
			bankbookslip->balance = 0;
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				if (bookKeeperlist[i]->get_agent_type() == 2130) {
					bankbookslip->deposit += bookKeeperlist[i]->get_bankBookSlipTotal(period)->deposit;
					bankbookslip->withdrawal += bookKeeperlist[i]->get_bankBookSlipTotal(period)->withdrawal;
					bankbookslip->balance += bookKeeperlist[i]->get_bankBookSlipTotal(period)->balance;
					bankbookslip->period = period;
				}
			}//agent毎の合計終わり
			bankBook_soleproprietor.push_back(bankbookslip);
		}
		else if (category_type == 2131) {
			bankbookslip->deposit = 0;
			bankbookslip->withdrawal = 0;
			bankbookslip->balance = 0;
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				if (bookKeeperlist[i]->get_agent_type() == 2131) {
					bankbookslip->deposit += bookKeeperlist[i]->get_bankBookSlipTotal(period)->deposit;
					bankbookslip->withdrawal += bookKeeperlist[i]->get_bankBookSlipTotal(period)->withdrawal;
					bankbookslip->balance += bookKeeperlist[i]->get_bankBookSlipTotal(period)->balance;
					bankbookslip->period = period;
				}
			}//agent毎の合計終わり
			bankBook_retailer.push_back(bankbookslip);
		}
		else if (category_type == 2132) {
			bankbookslip->deposit = 0;
			bankbookslip->withdrawal = 0;
			bankbookslip->balance = 0;


			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				if (bookKeeperlist[i]->get_agent_type() == 2132) {
					bankbookslip->deposit += bookKeeperlist[i]->get_bankBookSlipTotal(period)->deposit;
					bankbookslip->withdrawal += bookKeeperlist[i]->get_bankBookSlipTotal(period)->withdrawal;
					bankbookslip->balance += bookKeeperlist[i]->get_bankBookSlipTotal(period)->balance;
					bankbookslip->period = period;
				}
			}//agent毎の合計終わり
			bankBook_wholesaler.push_back(bankbookslip);
		}

	}
	void initialize_price_vector(int category_type, int period,vector<long long >ave_price1, vector<int> nk1,vector <int> item_type_id) {
			vector<long long > ave_price;
			vector<int> nk;
			ave_price = ave_price1;
			nk = nk1;
			if (price.size() == 0) {
				for (int k = 0; k < item_type_id.size(); k++)
				price.push_back(0);
				//printf("price.size()= %d", price.size());
			}
			if (ave_price1.size() == 0) {
				for (int k = 0; k < item_type_id.size(); k++)
					ave_price1.push_back(0);
			}
			if (nk1.size() == 0) {
				for (int k = 0; k < item_type_id.size(); k++)
					nk1.push_back(0);
			}
			ave_price1 = ave_price;
			nk1 = nk;
			
		}
	
	void create_stockBookTotal(int category_type, int period) {
		stockbookslip = new stockBookSlip;
		stockbookslip->production = 0;
		stockbookslip->bought = 0;
		stockbookslip->balance = 0;
		stockbookslip->price = 0;
		int n = 0;//平均価格計算用
		vector <long long> ave_price;
		vector<int> nk;//item_type_id毎の平均価格計算用
	
		if (category_type == 31) {
			//initialize_price_vector(category_type, period, ave_price, nk);
		//	printf("before initialize vector period=%d idk.size=%d price.size=%d category_type=%d\n", app_context.period_count,app_context.config->item_type_idk.size(),price.size(),category_type);
			//if (bookKeeperlist[i]->get_agent_type() == 31 || bookKeeperlist[i]->get_agent_type() == 311 || bookKeeperlist[i]->get_agent_type() == 312) {
			//initialize_price_vector(category_type, period, ave_price, nk, app_context.config->item_type_idk);
			
			if (price.size() == 0) {
				for (int k = 0; k < app_context.config->item_type_idk.size(); k++)
					price.push_back(0);
			}
			if (ave_price.size() == 0) {
				for (int k = 0; k < app_context.config->item_type_idk.size(); k++)
					ave_price.push_back(0);
			}
			if(nk.size() == 0) {
				for (int k = 0; k < app_context.config->item_type_idk.size(); k++)
					nk.push_back(0);
			}
			
		//	printf("after initialize vector period=%dave_price.size=%d nk.size=%d \n", app_context.period_count,price.size(),nk.size());
			stockbookslip->production = 0;
			stockbookslip->bought = 0;
			stockbookslip->balance = 0;
			//printf("before for  bookKeeperlist.size=%d \n",bookKeeperlist.size());
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2131 && bookKeeperlist[i]->get_stockBookSlipTotal(period)!=0) {
				//if (bookKeeperlist[i]->get_agent_type() == 31) {
				if (bookKeeperlist[i]->get_agent_type() == 31 || bookKeeperlist[i]->get_agent_type() == 310 || bookKeeperlist[i]->get_agent_type() == 311 || bookKeeperlist[i]->get_agent_type() == 312) {
					//printf("inside if stockbookslip->production=%d\n",stockbookslip->production);
					//printf("size= in 20230124 %d\n", bookKeeperlist[i]->get_stockBookSlipTotal(period));
					stockbookslip->production += bookKeeperlist[i]->get_stockBookSlipTotal(period)->production;
					//printf("after get_stockBookSlipTotal in create_stockBookTotal \n");
					stockbookslip->bought += bookKeeperlist[i]->get_stockBookSlipTotal(period)->bought;
					//printf("before get_stockBookSlipTotal in create_stockBookTotal\n");
					stockbookslip->balance += bookKeeperlist[i]->get_stockBookSlipTotal(period)->balance;

					stockbookslip->price += bookKeeperlist[i]->get_stockBookSlipTotal(period)->price;
					//if(stockbookslip->price!=0) n++;
					if (bookKeeperlist[i]->get_stockBookSlipTotal(period)->price != 0) n++;
				
					for (int k = 0; k < app_context.config->item_type_idk.size(); k++) {
						if (bookKeeperlist[i]->get_stockBookSlipTotal(period)->item_type_id == app_context.config->item_type_idk[k]) {
							if (bookKeeperlist[i]->get_stockBookSlipTotal(period)->price != 0) {
								price[k] += bookKeeperlist[i]->get_stockBookSlipTotal(period)->price;
								nk[k]++;
							}
						}
					}

					//printf("after calculate price, nk price.size()= %d, nk.size()=%d \n",price.size(), nk.size());
				}//if(bookKeeperlist[i]-.....の終わり
			}//agent毎の合計終わり
		//	printf("after for in create_stockBookTotal in Accountant.h\n");
			stockbookslip->period = period;
			//stockbookslip->price =long long (double ( stockbookslip->price) / bookKeeperlist.size());
			stockbookslip->price = long long(double(stockbookslip->price) /n);
			stockBook_consumer.push_back(stockbookslip);
			fprintf(app_context.config->consumerTotal->out_priceBook, "period=,%d,", app_context.period_count);
			for (int k = 0; k < app_context.config->item_type_idk.size(); k++) {
				if (nk[k] != 0) ave_price[k] = long long(double(price[k]) / nk[k]);
				else ave_price[k] = 0;
				fprintf(app_context.config->consumerTotal->out_priceBook, "item_type_id=,%d,ave_price=,%I64d,", app_context.config->item_type_idk[k], ave_price[k]);
			}
			fprintf(app_context.config->consumerTotal->out_priceBook, "\n");
			//printf("before vector clear\n");
			ave_price.clear(); ave_price.shrink_to_fit();
			price.clear(); price.shrink_to_fit();
			nk.clear(); nk.shrink_to_fit();
			//printf("after vector clear\n");
		}//if(category_type==...の終わり
		/*
		else if (category_type == 1) {
			//initialize_price_vector(category_type, period, ave_price, nk, app_context.config->item_type_idks);

			if (price.size() == 0) {
				for (int k = 0; k < app_context.config->item_type_idks.size(); k++)
					price.push_back(0);
			}
			if (ave_price.size() == 0) {
				for (int k = 0; k < app_context.config->item_type_idks.size(); k++)
					ave_price.push_back(0);
			}
			if (nk.size() == 0) {
				for (int k = 0; k < app_context.config->item_type_idks.size(); k++)
					nk.push_back(0);
			}
			stockbookslip->production = 0;
			stockbookslip->bought = 0;
			stockbookslip->balance = 0;
			//printf("before for  bookKeeperlist.size=%d \n",bookKeeperlist.size());
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2131 && bookKeeperlist[i]->get_stockBookSlipTotal(period)!=0) {
				if (bookKeeperlist[i]->get_agent_type() == 1) {
					//printf("inside if stockbookslip->production=%d\n",stockbookslip->production);
					stockbookslip->production += bookKeeperlist[i]->get_stockBookSlipTotal(period)->production;
					//printf("after get_stockBookSlipTotal in create_stockBookTotal \n");
					stockbookslip->bought += bookKeeperlist[i]->get_stockBookSlipTotal(period)->bought;
					//printf("before get_stockBookSlipTotal in create_stockBookTotal\n");
					stockbookslip->balance += bookKeeperlist[i]->get_stockBookSlipTotal(period)->balance;
					stockbookslip->price += bookKeeperlist[i]->get_stockBookSlipTotal(period)->price;
					//if (stockbookslip->price != 0) n++;
					if (bookKeeperlist[i]->get_stockBookSlipTotal(period)->price != 0) n++;

					for (int k = 0; k < app_context.config->item_type_idks.size(); k++) {
						if (bookKeeperlist[i]->get_stockBookSlipTotal(period)->item_type_id == app_context.config->item_type_idks[k]) {
							if (bookKeeperlist[i]->get_stockBookSlipTotal(period)->price != 0) {
								price[k] += bookKeeperlist[i]->get_stockBookSlipTotal(period)->price;
								nk[k]++;
							}
						}
					}

				}
			}//agent毎の合計終わり
			//printf("after for \n");
			stockbookslip->period = period;
			//stockbookslip->price = long long(double(stockbookslip->price) / bookKeeperlist.size());
			stockbookslip->price = long long(double(stockbookslip->price) / n);
			stockBook_soleproprietor.push_back(stockbookslip);
			fprintf(app_context.config->soleproprietorTotal->out_priceBook, "period=,%d,", app_context.period_count);
			for (int k = 0; k < app_context.config->item_type_idks.size(); k++) {
				if (nk[k] != 0) ave_price[k] = long long(double(price[k]) / nk[k]);
				else ave_price[k] = 0;
				fprintf(app_context.config->soleproprietorTotal->out_priceBook, "item_type_id=,%d,ave_price=,%I64d,", app_context.config->item_type_idks[k], ave_price[k]);
			}
			fprintf(app_context.config->soleproprietorTotal->out_priceBook, "\n");

			ave_price.clear(); ave_price.shrink_to_fit();
			price.clear(); price.shrink_to_fit();
			nk.clear(); nk.shrink_to_fit();

		}//if(category_type==...の終わり
		*/
		else if (category_type == 2130) {
			//initialize_price_vector(category_type, period, ave_price, nk, app_context.config->item_type_idks);

			if (price.size() == 0) {
				for (int k = 0; k < app_context.config->item_type_idks.size(); k++)
					price.push_back(0);
			}
			if (ave_price.size() == 0) {
				for (int k = 0; k < app_context.config->item_type_idks.size(); k++)
					ave_price.push_back(0);
			}
			if (nk.size() == 0) {
				for (int k = 0; k < app_context.config->item_type_idks.size(); k++)
					nk.push_back(0);
			}
			stockbookslip->production = 0;
			stockbookslip->bought = 0;
			stockbookslip->balance = 0;
			//printf("before for  bookKeeperlist.size=%d \n",bookKeeperlist.size());
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2131 && bookKeeperlist[i]->get_stockBookSlipTotal(period)!=0) {
				if (bookKeeperlist[i]->get_agent_type() == 2130) {
					//printf("inside if stockbookslip->production=%d\n",stockbookslip->production);
					stockbookslip->production += bookKeeperlist[i]->get_stockBookSlipTotal(period)->production;
					//printf("after get_stockBookSlipTotal in create_stockBookTotal \n");
					stockbookslip->bought += bookKeeperlist[i]->get_stockBookSlipTotal(period)->bought;
					//printf("before get_stockBookSlipTotal in create_stockBookTotal\n");
					stockbookslip->balance += bookKeeperlist[i]->get_stockBookSlipTotal(period)->balance;
					stockbookslip->price += bookKeeperlist[i]->get_stockBookSlipTotal(period)->price;
					//if (stockbookslip->price != 0) n++;
					if (bookKeeperlist[i]->get_stockBookSlipTotal(period)->price != 0) n++;
					
					for (int k = 0; k < app_context.config->item_type_idks.size(); k++) {
						if (bookKeeperlist[i]->get_stockBookSlipTotal(period)->item_type_id == app_context.config->item_type_idks[k]) {
							if (bookKeeperlist[i]->get_stockBookSlipTotal(period)->price != 0) {
								price[k] += bookKeeperlist[i]->get_stockBookSlipTotal(period)->price;
								nk[k]++;
							}
						}
					}
					
				}
			}//agent毎の合計終わり
			//printf("after for \n");
			stockbookslip->period = period;
			//stockbookslip->price = long long(double(stockbookslip->price) / bookKeeperlist.size());
			stockbookslip->price = long long(double(stockbookslip->price) / n);
			stockBook_soleproprietor.push_back(stockbookslip);
			fprintf(app_context.config->soleproprietorTotal->out_priceBook, "period=,%d,", app_context.period_count);
			for (int k = 0; k < app_context.config->item_type_idks.size(); k++) {
				if (nk[k] != 0) ave_price[k] = long long(double(price[k]) / nk[k]);
				else ave_price[k] = 0;
				fprintf(app_context.config->soleproprietorTotal->out_priceBook, "item_type_id=,%d,ave_price=,%I64d,", app_context.config->item_type_idks[k], ave_price[k]);
			}
			fprintf(app_context.config->soleproprietorTotal->out_priceBook, "\n");

			ave_price.clear(); ave_price.shrink_to_fit();
			price.clear(); price.shrink_to_fit();
			nk.clear(); nk.shrink_to_fit();
			
		}
		else if (category_type == 2131) {
			stockbookslip->production = 0;
			stockbookslip->bought = 0;
			stockbookslip->balance = 0;
			//printf("before for  bookKeeperlist.size=%d \n",bookKeeperlist.size());
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2131 && bookKeeperlist[i]->get_stockBookSlipTotal(period)!=0) {
				if(bookKeeperlist[i]->get_agent_type() == 2131 ) {
					//printf("inside if stockbookslip->production=%d\n",stockbookslip->production);
					stockbookslip->production += bookKeeperlist[i]->get_stockBookSlipTotal(period)->production;
					//printf("after get_stockBookSlipTotal in create_stockBookTotal \n");
					stockbookslip->bought += bookKeeperlist[i]->get_stockBookSlipTotal(period)->bought;
					//printf("before get_stockBookSlipTotal in create_stockBookTotal\n");
					stockbookslip->balance += bookKeeperlist[i]->get_stockBookSlipTotal(period)->balance;
					stockbookslip->price += bookKeeperlist[i]->get_stockBookSlipTotal(period)->price;
					//if(stockbookslip->price != 0) n++;
					if (bookKeeperlist[i]->get_stockBookSlipTotal(period)->price != 0) n++;
				}
			}//agent毎の合計終わり
			//printf("after for \n");
			stockbookslip->period = period;
			//stockbookslip->price = long long(double(stockbookslip->price) / bookKeeperlist.size());
			stockbookslip->price = long long(double(stockbookslip->price) / n);
			stockBook_retailer.push_back(stockbookslip);
		}
		else if (category_type == 2132) {
			stockbookslip->production = 0;
			stockbookslip->bought = 0;
			stockbookslip->balance = 0;
			//printf("inside create_stockBookTotal");

			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2132 && bookKeeperlist[i]->get_stockBookSlipTotal(period) != 0) {
				if (bookKeeperlist[i]->get_agent_type() == 2132) {
					stockbookslip->production += bookKeeperlist[i]->get_stockBookSlipTotal(period)->production;
					stockbookslip->bought += bookKeeperlist[i]->get_stockBookSlipTotal(period)->bought;
					stockbookslip->balance += bookKeeperlist[i]->get_stockBookSlipTotal(period)->balance;
					stockbookslip->price += bookKeeperlist[i]->get_stockBookSlipTotal(period)->price;
					//if(stockbookslip->price != 0) n++;
					if (bookKeeperlist[i]->get_stockBookSlipTotal(period)->price != 0) n++;
				}
			}//agent毎の合計終わり
			stockbookslip->period = period;
			//stockbookslip->price = long long(double(stockbookslip->price) / bookKeeperlist.size());
			stockbookslip->price = long long(double(stockbookslip->price) / n);
			//fprintf(app_context.config->wholesalerTotal->out_stockBook, "period=,%d, production=,%d,bought=,%d, balance=,%d,\n",period, stockbookslip->production,stockbookslip->bought,stockbookslip->balance);
			stockBook_wholesaler.push_back(stockbookslip);
			//fprintf(app_context.config->wholesalerTotal->out_stockBook, "period=,%d,stockBook__wholesaler[size-1]->period=,%d,production=,%d,stockBook_wholesaler.size()=,%d,\n",
			//	period, stockBook_wholesaler[stockBook_wholesaler.size() - 1]->period, stockBook_wholesaler[stockBook_wholesaler.size() - 1]->production,stockBook_wholesaler.size());
		}
	//	printf("end of create_stockBookTotal \n");
	}

	void create_materialBookTotal(int category_type, int period) {
		materialbookslip = new materialBookSlip;
		materialbookslip->production = 0;
		materialbookslip->bought = 0;
		materialbookslip->balance = 0;
		materialbookslip->price = 0;
		long long ave_price = 0;
		//printf("before if \n category_type=%d \n",category_type);
		int n = 0;
		
		if (category_type == 31) {
			/*
			//if (bookKeeperlist[i]->get_agent_type() == 31 || bookKeeperlist[i]->get_agent_type() == 311 || bookKeeperlist[i]->get_agent_type() == 312) {
			materialbookslip->production = 0;
			materialbookslip->bought = 0;
			materialbookslip->balance = 0;
			//printf("before for  bookKeeperlist.size=%d \n",bookKeeperlist.size());
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2131 && bookKeeperlist[i]->get_materialBookSlipTotal(period)!=0) {
				//if (bookKeeperlist[i]->get_agent_type() == 31) {
				if (bookKeeperlist[i]->get_agent_type() == 31 || bookKeeperlist[i]->get_agent_type() == 311 || bookKeeperlist[i]->get_agent_type() == 312) {
					//printf("inside if materialbookslip->production=%d\n",materialbookslip->production);
					//printf("size= in 20230124 %d\n", bookKeeperlist[i]->get_materialBookSlipTotal(period));
					materialbookslip->production += bookKeeperlist[i]->get_materialBookSlipTotal(period)->production;
					//printf("after get_materialBookSlipTotal in create_materialBookTotal \n");
					materialbookslip->bought += bookKeeperlist[i]->get_materialBookSlipTotal(period)->bought;
					//printf("before get_materialBookSlipTotal in create_materialBookTotal\n");
					materialbookslip->balance += bookKeeperlist[i]->get_materialBookSlipTotal(period)->balance;
					materialbookslip->price += bookKeeperlist[i]->get_materialBookSlipTotal(period)->price;
					n++;

				}
			}//agent毎の合計終わり
			//printf("after for \n");
			materialbookslip->period = period;
			//materialbookslip->price =long long (double ( materialbookslip->price) / bookKeeperlist.size());
			materialbookslip->price = long long(double(materialbookslip->price) / n);
			materialBook_consumer.push_back(materialbookslip);
			*/
		}
		else if (category_type == 1) {
		}
		else if (category_type == 2130) {
			/*
			materialbookslip->production = 0;
			materialbookslip->bought = 0;
			materialbookslip->balance = 0;
			//printf("before for  bookKeeperlist.size=%d \n",bookKeeperlist.size());
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2131 && bookKeeperlist[i]->get_materialBookSlipTotal(period)!=0) {
				if (bookKeeperlist[i]->get_agent_type() == 2130) {
					//printf("inside if materialbookslip->production=%d\n",materialbookslip->production);
					materialbookslip->production += bookKeeperlist[i]->get_materialBookSlipTotal(period)->production;
					//printf("after get_materialBookSlipTotal in create_materialBookTotal \n");
					materialbookslip->bought += bookKeeperlist[i]->get_materialBookSlipTotal(period)->bought;
					//printf("before get_materialBookSlipTotal in create_materialBookTotal\n");
					materialbookslip->balance += bookKeeperlist[i]->get_materialBookSlipTotal(period)->balance;
					materialbookslip->price += bookKeeperlist[i]->get_materialBookSlipTotal(period)->price;
					if (materialbookslip->price != 0) n++;
				}
			}//agent毎の合計終わり
			//printf("after for \n");
			materialbookslip->period = period;
			//materialbookslip->price = long long(double(materialbookslip->price) / bookKeeperlist.size());
			materialbookslip->price = long long(double(materialbookslip->price) / n);
			materialBook_soleproprietor.push_back(materialbookslip);
		*/	
		}

		else if (category_type == 2131) {
			materialbookslip->production = 0;
			materialbookslip->bought = 0;
			materialbookslip->balance = 0;
			//printf("before for  bookKeeperlist.size=%d \n",bookKeeperlist.size());
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2131 && bookKeeperlist[i]->get_materialBookSlipTotal(period)!=0) {
				if (bookKeeperlist[i]->get_agent_type() == 2131) {
					//printf("inside if materialbookslip->production=%d\n",materialbookslip->production);
					materialbookslip->production += bookKeeperlist[i]->get_materialBookSlipTotal(period)->production;
					//printf("after get_materialBookSlipTotal in create_materialBookTotal \n");
					materialbookslip->bought += bookKeeperlist[i]->get_materialBookSlipTotal(period)->bought;
					//printf("before get_materialBookSlipTotal in create_materialBookTotal\n");
					materialbookslip->balance += bookKeeperlist[i]->get_materialBookSlipTotal(period)->balance;
					materialbookslip->price += bookKeeperlist[i]->get_materialBookSlipTotal(period)->price;
					if(materialbookslip->price != 0) n++;
				}
			}//agent毎の合計終わり
			//printf("after for \n");
			materialbookslip->period = period;
			//materialbookslip->price = long long(double(materialbookslip->price) / bookKeeperlist.size());
			materialbookslip->price = long long(double(materialbookslip->price) / n);
			materialBook_retailer.push_back(materialbookslip);
		}
		else if (category_type == 2132) {
		/*
			materialbookslip->production = 0;
			materialbookslip->bought = 0;
			materialbookslip->balance = 0;
			//printf("inside create_materialBookTotal");

			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2132 && bookKeeperlist[i]->get_materialBookSlipTotal(period) != 0) {
				if (bookKeeperlist[i]->get_agent_type() == 2132) {
					materialbookslip->production += bookKeeperlist[i]->get_materialBookSlipTotal(period)->production;
					materialbookslip->bought += bookKeeperlist[i]->get_materialBookSlipTotal(period)->bought;
					materialbookslip->balance += bookKeeperlist[i]->get_materialBookSlipTotal(period)->balance;
					materialbookslip->price += bookKeeperlist[i]->get_materialBookSlipTotal(period)->price;
					n++;
				}
			}//agent毎の合計終わり
			materialbookslip->period = period;
			//materialbookslip->price = long long(double(materialbookslip->price) / bookKeeperlist.size());
			materialbookslip->price = long long(double(materialbookslip->price) / n);
			//fprintf(app_context.config->wholesalerTotal->out_materialBook, "period=,%d, production=,%d,bought=,%d, balance=,%d,\n",period, materialbookslip->production,materialbookslip->bought,materialbookslip->balance);
			materialBook_wholesaler.push_back(materialbookslip);
			//fprintf(app_context.config->wholesalerTotal->out_materialBook, "period=,%d,materialBook__wholesaler[size-1]->period=,%d,production=,%d,materialBook_wholesaler.size()=,%d,\n",
			//	period, materialBook_wholesaler[materialBook_wholesaler.size() - 1]->period, materialBook_wholesaler[materialBook_wholesaler.size() - 1]->production,materialBook_wholesaler.size());
			*/
			}
		//printf("end of create_materialBookTotal \n");
	}



	void create_stockpriceBookTotal(int category_type, int period) {
		//printf("inside create_stockpriceBookTotal \n");
		stockpricebookslip = new stockpriceBookSlip;
		stockpricebookslip->production_price = 0;
		stockpricebookslip->bought_price = 0;
		stockpricebookslip->balance_price = 0;
		long long production_price = 0;
		long long bought_price = 0;
		long long balance_price = 0;
		//printf("before if \n category_type=%d \n",category_type);
		if (category_type == 31) {
			stockpricebookslip->production_price = 0;
			stockpricebookslip->bought_price = 0;
			stockpricebookslip->balance_price = 0;
			//printf("before for  bookKeeperlist.size=%d \n",bookKeeperlist.size());
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2131 && bookKeeperlist[i]->get_stockBookSlipTotal(period)!=0) {
				
				if (bookKeeperlist[i]->get_agent_type() == 31 || bookKeeperlist[i]->get_agent_type() == 310 ||bookKeeperlist[i]->get_agent_type() == 311 || bookKeeperlist[i]->get_agent_type() == 312) {
					//printf("inside if stockbookslip->production=%d\n",stockbookslip->production);
					stockpricebookslip->production_price += bookKeeperlist[i]->get_stockpriceBookSlipTotal(period)->production_price;
					//printf("after get_stockBookSlipTotal in create_stockBookTotal \n");
					stockpricebookslip->bought_price += bookKeeperlist[i]->get_stockpriceBookSlipTotal(period)->bought_price;
					//printf("before get_stockBookSlipTotal in create_stockBookTotal\n");
					stockpricebookslip->balance_price += bookKeeperlist[i]->get_stockpriceBookSlipTotal(period)->balance_price;

				}
			}//agent毎の合計終わり
			//printf("after for \n");
			stockpricebookslip->period = period;
			stockpriceBook_consumer.push_back(stockpricebookslip);
		}
		/*
		else if (category_type == 1) {
			stockpricebookslip->production_price = 0;
			stockpricebookslip->bought_price = 0;
			stockpricebookslip->balance_price = 0;
			//printf("before for  bookKeeperlist.size=%d \n",bookKeeperlist.size());
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2131 && bookKeeperlist[i]->get_stockBookSlipTotal(period)!=0) {

				if (bookKeeperlist[i]->get_agent_type() == 1) {
					//printf("inside if stockbookslip->production=%d\n",stockbookslip->production);
					stockpricebookslip->production_price += bookKeeperlist[i]->get_stockpriceBookSlipTotal(period)->production_price;
					//printf("after get_stockBookSlipTotal in create_stockBookTotal \n");
					stockpricebookslip->bought_price += bookKeeperlist[i]->get_stockpriceBookSlipTotal(period)->bought_price;
					//printf("before get_stockBookSlipTotal in create_stockBookTotal\n");
					stockpricebookslip->balance_price += bookKeeperlist[i]->get_stockpriceBookSlipTotal(period)->balance_price;

				}
			}//agent毎の合計終わり
			//printf("after for \n");
			stockpricebookslip->period = period;
			stockpriceBook_consumer.push_back(stockpricebookslip);
		}
		*/
		else if (category_type == 2130) {
			stockpricebookslip->production_price = 0;
			stockpricebookslip->bought_price = 0;
			stockpricebookslip->balance_price = 0;
			//printf("before for  bookKeeperlist.size=%d \n",bookKeeperlist.size());
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2131 && bookKeeperlist[i]->get_stockBookSlipTotal(period)!=0) {
				if (bookKeeperlist[i]->get_agent_type() == 2130) {
					//printf("inside if stockbookslip->production=%d\n",stockbookslip->production);
					stockpricebookslip->production_price += bookKeeperlist[i]->get_stockpriceBookSlipTotal(period)->production_price;
					//printf("after get_stockBookSlipTotal in create_stockBookTotal \n");
					stockpricebookslip->bought_price += bookKeeperlist[i]->get_stockpriceBookSlipTotal(period)->bought_price;
					//printf("before get_stockBookSlipTotal in create_stockBookTotal\n");
					stockpricebookslip->balance_price += bookKeeperlist[i]->get_stockpriceBookSlipTotal(period)->balance_price;

				}
			}//agent毎の合計終わり
			//printf("after for \n");
			stockpricebookslip->period = period;
			stockpriceBook_soleproprietor.push_back(stockpricebookslip);
		}
		else if (category_type == 2131) {
			stockpricebookslip->production_price = 0;
			stockpricebookslip->bought_price = 0;
			stockpricebookslip->balance_price = 0;
			//printf("before for  bookKeeperlist.size=%d \n",bookKeeperlist.size());
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2131 && bookKeeperlist[i]->get_stockBookSlipTotal(period)!=0) {
				if (bookKeeperlist[i]->get_agent_type() == 2131) {
					//printf("inside if stockbookslip->production=%d\n",stockbookslip->production);
					stockpricebookslip->production_price += bookKeeperlist[i]->get_stockpriceBookSlipTotal(period)->production_price;
					//printf("after get_stockBookSlipTotal in create_stockBookTotal \n");
					stockpricebookslip->bought_price += bookKeeperlist[i]->get_stockpriceBookSlipTotal(period)->bought_price;
					//printf("before get_stockBookSlipTotal in create_stockBookTotal\n");
					stockpricebookslip->balance_price += bookKeeperlist[i]->get_stockpriceBookSlipTotal(period)->balance_price;

				}
			}//agent毎の合計終わり
			//printf("after for \n");
			stockpricebookslip->period = period;
			stockpriceBook_retailer.push_back(stockpricebookslip);
		}
		else if (category_type == 2132) {
			stockpricebookslip->production_price = 0;
			stockpricebookslip->bought_price = 0;
			stockpricebookslip->balance_price = 0;
			//printf("inside create_stockBookTotal category_type-2132\n");

			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2132 && bookKeeperlist[i]->get_stockBookSlipTotal(period) != 0) {
				if (bookKeeperlist[i]->get_agent_type() == 2132) {
					stockpricebookslip->production_price +=production_price= bookKeeperlist[i]->get_stockpriceBookSlipTotal(period)->production_price;
					stockpricebookslip->bought_price += bought_price=bookKeeperlist[i]->get_stockpriceBookSlipTotal(period)->bought_price;
					stockpricebookslip->balance_price += balance_price=bookKeeperlist[i]->get_stockpriceBookSlipTotal(period)->balance_price;
					fprintf(app_context.config->out_test2, "period=< %d, agent_id=,%d,production_price_total=,%I64d,boubht_price_total=,%I64d,balance_price_total=,%I64d,production_price=, %I64d, boubht_price=, %I64d, balance_price=, %I64d,period=,%d\n"
						, stockpricebookslip->period, bookKeeperlist[i]->get_agent_id(), stockpricebookslip->production_price, stockpricebookslip->bought_price, stockpricebookslip->balance_price
						, production_price, bought_price, balance_price, bookKeeperlist[i]->get_stockpriceBookSlipTotal(period)->period);
					

				}
			}//agent毎の合計終わり
			//printf("agent毎の合計終わり\n");
			stockpricebookslip->period = period;
			//fprintf(app_context.config->wholesalerTotal->out_stockBook, "period=,%d, production=,%d,bought=,%d, balance=,%d,\n",period, stockbookslip->production,stockbookslip->bought,stockbookslip->balance);
			stockpriceBook_wholesaler.push_back(stockpricebookslip);
			fprintf(app_context.config->out_test2, "period= %d, production_price_total=,%I64d,boubht_price_total=,%I64d,balance_price_total=,%I64d,\n",
				stockpricebookslip->period, stockpricebookslip->production_price, stockpricebookslip->bought_price, stockpricebookslip->balance_price);
			//fprintf(app_context.config->wholesalerTotal->out_stockBook, "period=,%d,stockBook__wholesaler[size-1]->period=,%d,production=,%d,stockBook_wholesaler.size()=,%d,\n",
			//	period, stockBook_wholesaler[stockBook_wholesaler.size() - 1]->period, stockBook_wholesaler[stockBook_wholesaler.size() - 1]->production,stockBook_wholesaler.size());
		}
		//printf("end of create_stockpriceBookTotal \n");
	}
	//----------------
	void create_genkaBookTotal(int category_type, int period) {
		genkabookslip = new genkaBookSlip;
		genkabookslip->direct_labor_cost = 0;
		genkabookslip->direct_material_cost = 0;
		genkabookslip->genka = 0;
		genkabookslip->size_shiped = 0;
		long long production_price = 0;
		long long bought_price = 0;
		long long balance_price = 0;
		//printf("inside crete_genkaBookTotal before if \n category_type=%d \n",category_type);
		if (category_type == 31) {
			
		}
		else if (category_type == 2130) {
			//printf("inside if category_type=%d\n", category_type);
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {

				if (bookKeeperlist[i]->get_agent_type() == 2130) {
					//if (bookKeeperlist[i]->get_agent_type() == 2131 && bookKeeperlist[i]->get_genkaBookSlipTotal(period) != 0) {
					genkabookslip->direct_labor_cost += bookKeeperlist[i]->get_genkaBookSlipTotal(period)->direct_labor_cost;
					//genkabookslip->direct_material_cost += bookKeeperlist[i]->get_genkaBookSlipTotal(period)->direct_material_cost;
					genkabookslip->genka += bookKeeperlist[i]->get_genkaBookSlipTotal(period)->genka;
					genkabookslip->size_shiped += bookKeeperlist[i]->get_genkaBookSlipTotal(period)->size_shiped;
				}
			}

			//agent毎の合計終わり
			//printf("after for \n");
			genkabookslip->period = period;
			genkaBook_soleproprietor.push_back(genkabookslip);
		}
		else if (category_type == 2131) {
			//printf("inside if category_type=%d\n", category_type);
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//printf("inside for bookKeeperlist.size=%d i=%d agent_type=%d\n", bookKeeperlist.size(), i,bookKeeperlist[i]->get_agent_type());
				if (bookKeeperlist[i]->get_agent_type() == 2131 ) {
					//if (bookKeeperlist[i]->get_agent_type() == 2131 && bookKeeperlist[i]->get_genkaBookSlipTotal(period) != 0) {
					//printf("inside agent_type=2131 before direct_labor_cost agent_id=%d agent_type=%d\n",bookKeeperlist[i]->get_agent_id(),bookKeeperlist[i]->get_agent_type());
					//printf(" sahen= %d period=%d in Accountant.h line 602\n  ", genkabookslip->direct_labor_cost,period);
					//printf("address of bookKeeperlist[i]->get_genkaBookSlipTotal(period)=%x \n", bookKeeperlist[i]->get_genkaBookSlipTotal(period));
					//printf("sizes =%d\n", bookKeeperlist[i]->get_genkaBookSlipTotal(period)->item_type_id);
					genkabookslip->direct_labor_cost += bookKeeperlist[i]->get_genkaBookSlipTotal(period)->direct_labor_cost;
					//printf("inside agent_type=2131 before direct_material_cost\n");
					genkabookslip->direct_material_cost += bookKeeperlist[i]->get_genkaBookSlipTotal(period)->direct_material_cost;
					genkabookslip->genka += bookKeeperlist[i]->get_genkaBookSlipTotal(period)->genka;
					genkabookslip->size_shiped += bookKeeperlist[i]->get_genkaBookSlipTotal(period)->size_shiped;
				}
			}
			
			//agent毎の合計終わり
			//printf("after for in case of category_type=2131\n");
			genkabookslip->period = period;
			genkaBook_retailer.push_back(genkabookslip);
		}
		else if (category_type == 2132) {
			//printf("inside if category_type=%d\n", category_type);
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2132 && bookKeeperlist[i]->get_materialBookSlipTotal(period) != 0) {
				if (bookKeeperlist[i]->get_agent_type() == 2132) {
					genkabookslip->direct_labor_cost += bookKeeperlist[i]->get_genkaBookSlipTotal(period)->direct_labor_cost;
					genkabookslip->direct_material_cost += bookKeeperlist[i]->get_genkaBookSlipTotal(period)->direct_material_cost;
					genkabookslip->genka += bookKeeperlist[i]->get_genkaBookSlipTotal(period)->genka;
					genkabookslip->size_shiped += bookKeeperlist[i]->get_genkaBookSlipTotal(period)->size_shiped;

				}
			}//agent毎の合計終わり
			//printf("agent毎の合計終わり\n");
			genkabookslip->period = period;
			//fprintf(app_context.config->wholesalerTotal->out_materialBook, "period=,%d, production=,%d,bought=,%d, balance=,%d,\n",period, materialbookslip->production,materialbookslip->bought,materialbookslip->balance);
			genkaBook_wholesaler.push_back(genkabookslip);
		
			}
		//printf("end of create_materialpriceBookTotal \n");
	}
	//--------------------------
	void create_materialpriceBookTotal(int category_type, int period) {
		//printf("inside create_materialpriceBookTotal \n");
		materialpricebookslip = new materialpriceBookSlip;
		materialpricebookslip->production_price = 0;
		materialpricebookslip->bought_price = 0;
		materialpricebookslip->balance_price = 0;
		long long production_price = 0;
		long long bought_price = 0;
		long long balance_price = 0;
		//printf("before if \n category_type=%d \n",category_type);
		if (category_type == 31) {
			/*
			materialpricebookslip->production_price = 0;
			materialpricebookslip->bought_price = 0;
			materialpricebookslip->balance_price = 0;
			//printf("before for  bookKeeperlist.size=%d \n",bookKeeperlist.size());
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2131 && bookKeeperlist[i]->get_materialBookSlipTotal(period)!=0) {

				if (bookKeeperlist[i]->get_agent_type() == 31 || bookKeeperlist[i]->get_agent_type() == 311 || bookKeeperlist[i]->get_agent_type() == 312) {
					//printf("inside if materialbookslip->production=%d\n",materialbookslip->production);
					materialpricebookslip->production_price += bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->production_price;
					//printf("after get_materialBookSlipTotal in create_materialBookTotal \n");
					materialpricebookslip->bought_price += bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->bought_price;
					//printf("before get_materialBookSlipTotal in create_materialBookTotal\n");
					materialpricebookslip->balance_price += bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->balance_price;

				}
			}//agent毎の合計終わり
			//printf("after for \n");
			materialpricebookslip->period = period;
			materialpriceBook_consumer.push_back(materialpricebookslip);
			*/
		}
		else if (category_type == 1) {
		}
		else if (category_type == 2130) {
			/*
			materialpricebookslip->production_price = 0;
			materialpricebookslip->bought_price = 0;
			materialpricebookslip->balance_price = 0;
			//printf("before for  bookKeeperlist.size=%d \n",bookKeeperlist.size());
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2131 && bookKeeperlist[i]->get_materialBookSlipTotal(period)!=0) {
				if (bookKeeperlist[i]->get_agent_type() == 2130) {
					//printf("inside if materialbookslip->production=%d\n",materialbookslip->production);
					materialpricebookslip->production_price += bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->production_price;
					//printf("after get_materialBookSlipTotal in create_materialBookTotal \n");
					materialpricebookslip->bought_price += bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->bought_price;
					//printf("before get_materialBookSlipTotal in create_materialBookTotal\n");
					materialpricebookslip->balance_price += bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->balance_price;
					if (period == 1 || period == 2)
						fprintf(app_context.config->out_test6, "period=,%d,agent_id=,%d,production_price=,%I64d, bought_price=,%I64d,balance_price=,%I64d,\n"
							, period, bookKeeperlist[i]->get_agent_id(), bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->production_price
							, bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->bought_price, bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->balance_price);
				}
			}

			//agent毎の合計終わり
			//printf("after for \n");
			materialpricebookslip->period = period;
			materialpriceBook_soleproprietor.push_back(materialpricebookslip);
			*/
		}
		else if (category_type == 2131) {
			materialpricebookslip->production_price = 0;
			materialpricebookslip->bought_price = 0;
			materialpricebookslip->balance_price = 0;
			//printf("before for  bookKeeperlist.size=%d \n",bookKeeperlist.size());
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2131 && bookKeeperlist[i]->get_materialBookSlipTotal(period)!=0) {
				if (bookKeeperlist[i]->get_agent_type() == 2131) {
					//printf("inside if materialbookslip->production=%d\n",materialbookslip->production);
					materialpricebookslip->production_price += bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->production_price;
					//printf("after get_materialBookSlipTotal in create_materialBookTotal \n");
					materialpricebookslip->bought_price += bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->bought_price;
					//printf("before get_materialBookSlipTotal in create_materialBookTotal\n");
					materialpricebookslip->balance_price += bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->balance_price;
					if (period == 1 || period == 2)
						fprintf(app_context.config->out_test6, "period=,%d,agent_id=,%d,production_price=,%I64d, bought_price=,%I64d,balance_price=,%I64d,\n"
							, period, bookKeeperlist[i]->get_agent_id(), bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->production_price
							, bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->bought_price, bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->balance_price);
				}
			}

			//agent毎の合計終わり
			//printf("after for \n");
			materialpricebookslip->period = period;
			materialpriceBook_retailer.push_back(materialpricebookslip);
		}
		else if (category_type == 2132) {
			/*
			materialpricebookslip->production_price = 0;
			materialpricebookslip->bought_price = 0;
			materialpricebookslip->balance_price = 0;
			//printf("inside create_materialBookTotal category_type-2132\n");

			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				//if (bookKeeperlist[i]->get_agent_type() == 2132 && bookKeeperlist[i]->get_materialBookSlipTotal(period) != 0) {
				if (bookKeeperlist[i]->get_agent_type() == 2132) {
					materialpricebookslip->production_price += production_price = bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->production_price;
					materialpricebookslip->bought_price += bought_price = bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->bought_price;
					materialpricebookslip->balance_price += balance_price = bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->balance_price;
					fprintf(app_context.config->out_test2, "period=< %d, agent_id=,%d,production_price_total=,%I64d,boubht_price_total=,%I64d,balance_price_total=,%I64d,production_price=, %I64d, boubht_price=, %I64d, balance_price=, %I64d,period=,%d\n"
						, materialpricebookslip->period, bookKeeperlist[i]->get_agent_id(), materialpricebookslip->production_price, materialpricebookslip->bought_price, materialpricebookslip->balance_price
						, production_price, bought_price, balance_price, bookKeeperlist[i]->get_materialpriceBookSlipTotal(period)->period);


				}
			}//agent毎の合計終わり
			//printf("agent毎の合計終わり\n");
			materialpricebookslip->period = period;
			//fprintf(app_context.config->wholesalerTotal->out_materialBook, "period=,%d, production=,%d,bought=,%d, balance=,%d,\n",period, materialbookslip->production,materialbookslip->bought,materialbookslip->balance);
			materialpriceBook_wholesaler.push_back(materialpricebookslip);
			fprintf(app_context.config->out_test2, "period= %d, production_price_total=,%I64d,boubht_price_total=,%I64d,balance_price_total=,%I64d,\n",
				materialpricebookslip->period, materialpricebookslip->production_price, materialpricebookslip->bought_price, materialpricebookslip->balance_price);
			//fprintf(app_context.config->wholesalerTotal->out_materialBook, "period=,%d,materialBook__wholesaler[size-1]->period=,%d,production=,%d,materialBook_wholesaler.size()=,%d,\n",
			//	period, materialBook_wholesaler[materialBook_wholesaler.size() - 1]->period, materialBook_wholesaler[materialBook_wholesaler.size() - 1]->production,materialBook_wholesaler.size());
			*/
		}
		//printf("end of create_materialpriceBookTotal \n");
	}
	//-----------------
	//----------------------
	void define_initial_TB_table(int category_type) {
		unsigned int i_sizemax = 0;		unsigned int maxsize = 0;		int n = 0;
		if (category_type == 31) {
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				if (bookKeeperlist[i]->get_agent_type() == 31 || bookKeeperlist[i]->get_agent_type() == 310 ||bookKeeperlist[i]->get_agent_type() == 311 || bookKeeperlist[i]->get_agent_type() == 312 || bookKeeperlist[i]->get_agent_type()==3100 || bookKeeperlist[i]->get_agent_type()==3110 || bookKeeperlist[i]->get_agent_type()==3120) {
					if (n == 0) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
					else if (bookKeeperlist[i]->get_TBtable().size() > maxsize) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
					n++;
				}
			}
			TB_table_consumer = get_TBtable(bookKeeperlist[i_sizemax]->get_TBtable());
		}
		else if (category_type == 1) {
			/*
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				if (bookKeeperlist[i]->get_agent_type() == 1) {
					if (n == 0) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
					else if (bookKeeperlist[i]->get_TBtable().size() > maxsize) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
					n++;
				}
			}
			TB_table_government = get_TBtable(bookKeeperlist[i_sizemax]->get_TBtable());
			*/
		}
		else if (category_type == 2130) {
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				if (bookKeeperlist[i]->get_agent_type() == 2130) {
					if (n == 0) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
					else if (bookKeeperlist[i]->get_TBtable().size() > maxsize) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
					n++;
				}
			}
			TB_table_soleproprietor = get_TBtable(bookKeeperlist[i_sizemax]->get_TBtable());
		}
		else if (category_type == 2131) {
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				if (bookKeeperlist[i]->get_agent_type() == 2131) {
					if (n == 0) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
					else if (bookKeeperlist[i]->get_TBtable().size() > maxsize) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
					n++;
				}
			}
			TB_table_retailer = get_TBtable(bookKeeperlist[i_sizemax]->get_TBtable());
		}
		else if (category_type == 2132) {
			for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
				if (bookKeeperlist[i]->get_agent_type() == 2132) {
					if (n == 0) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
					else if (bookKeeperlist[i]->get_TBtable().size() > maxsize) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
					n++;
				}
			}
			TB_table_wholesaler = get_TBtable(bookKeeperlist[i_sizemax]->get_TBtable());
		}
		else {
			printf("定義されていないcategoryがあります\n");
			return;
		}
		
		

	}
	void define_initial_TB_table_consumer() {
		//この関数は現在は使わない、define_initial_TB_table(int category_type)を使う　2022年5月24日　荻林
		unsigned int i_sizemax = 0;		unsigned int maxsize = 0;		int n= 0;
		for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {			
			//if (bookKeeperlist[i]->get_agent_type() ==2131) {
			if (bookKeeperlist[i]->get_agent_type() == 31 || bookKeeperlist[i]->get_agent_type() == 310 ||bookKeeperlist[i]->get_agent_type() == 311 || bookKeeperlist[i]->get_agent_type() == 312){
				if (n == 0) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
				else if (bookKeeperlist[i]->get_TBtable().size() > maxsize) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
				n++;
			}
		}
		TB_table_consumer = get_TBtable(bookKeeperlist[i_sizemax]->get_TBtable());
	
	}
	void define_initial_TB_table_government() {
	/*
		//この関数は現在は使わない、define_initial_TB_table(int category_type)を使う　2025年12月19日　荻林
		unsigned int i_sizemax = 0;		unsigned int maxsize = 0;		int n = 0;
		for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
			if (bookKeeperlist[i]->get_agent_type() == 1) {
				if (n == 0) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
				else if (bookKeeperlist[i]->get_TBtable().size() > maxsize) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
				n++;
			}
		}
		TB_table_government = get_TBtable(bookKeeperlist[i_sizemax]->get_TBtable());
		*/
	}

	void define_initial_TB_table_soleproprietor() {
		//この関数は現在は使わない、define_initial_TB_table(int category_type)を使う　2022年5月24日　荻林
		unsigned int i_sizemax = 0;		unsigned int maxsize = 0;		int n = 0;
		for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
			if (bookKeeperlist[i]->get_agent_type() == 2130) {
				if (n == 0) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
				else if (bookKeeperlist[i]->get_TBtable().size() > maxsize) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
				n++;
			}
		}
		TB_table_soleproprietor = get_TBtable(bookKeeperlist[i_sizemax]->get_TBtable());
	}
	void define_initial_TB_table_retailer() {
		//この関数は現在は使わない、define_initial_TB_table(int category_type)を使う　2022年5月24日　荻林
		unsigned int i_sizemax = 0;		unsigned int maxsize = 0;		int n = 0;
		for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
			if (bookKeeperlist[i]->get_agent_type() == 2131) {
				if (n == 0) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
				else if (bookKeeperlist[i]->get_TBtable().size() > maxsize) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
				n++;
			}
		}
		TB_table_retailer = get_TBtable(bookKeeperlist[i_sizemax]->get_TBtable());
	}
	void define_initial_TB_table_wholesaler() {
		//この関数は現在は使わない、define_initial_TB_table(int category_type)を使う　2022年5月24日　荻林
		unsigned int i_sizemax = 0;		unsigned int maxsize = 0;		int n = 0;
		for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
			if (bookKeeperlist[i]->get_agent_type() == 2132) {
				if (n == 0) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
				else if (bookKeeperlist[i]->get_TBtable().size() > maxsize) { i_sizemax = i; maxsize = bookKeeperlist[i]->get_TBtable().size(); }
				n++;
			}
		}
		TB_table_wholesaler = get_TBtable(bookKeeperlist[i_sizemax]->get_TBtable());
	}
	vector<trialBalanceSlip*> get_TBtable(vector<trialBalanceSlip*> TB_table1) {
		//勘定科目のみ引数のTB_tableの勘定科目を取り出し、借方、貸方の数字はゼロの試算表をgetする。
		vector<trialBalanceSlip*> TB_table2;
		for (unsigned int i = 0; i < TB_table1.size(); i++) {
			trialBalanceslip = new trialBalanceSlip;
			trialBalanceslip->period = TB_table1[i]->period;
			trialBalanceslip->title = TB_table1[i]->title;
			trialBalanceslip->Dr_total = 0;// TB_table1[i]->Cr_total;
			trialBalanceslip->Cr_total = 0; //TB_table1[i]->Dr_total;
			trialBalanceslip->opening_balance = 0;
			trialBalanceslip->closing_balance = 0;
			trialBalanceslip->code = TB_table1[i]->code;
			TB_table2.push_back(trialBalanceslip);
		}
		return TB_table2;
	}


	void do_kimatsu() {
		//app_context.accountant->do_kimatsu();はSimulalterAppのprocess_period()の中の最後に毎期呼ばれる
		int size_max = 0;
		int size_min = 0;
		int n=0;
		int ncon = 0;
		//printf("start of do_kimatsu in accountant\n");
		//printf_s( "inside accountant period=,%d size of bookkeeperlist=,%d,\n", app_context.period_count, bookKeeperlist.size());
		fprintf_s(app_context.config->out_temporary, "inside accountant period=,%d size of bookkeeperlist=,%d,\n", app_context.period_count, bookKeeperlist.size());
		//TBtable.size()が最大のTB_tableを各カテゴリタイプのinitial TB_tableとして定義する
		//printf("before define_initial_TB_table\n");
		define_initial_TB_table(31);// printf("TB_table_consumer.size()=%d\n", TB_table_consumer.size());
	//	define_initial_TB_table(1);
		define_initial_TB_table(2130);
		define_initial_TB_table(2131);//printf("TB_table_retailer.size()=%d\n", TB_table_retailer.size());
		define_initial_TB_table(2132);//printf("TB_table_wholesaler.size()=%d\n", TB_table_wholesaler.size());
		//printf("after define_initial_TB_table in Accountant->do_kimatsu()\n");
		/*define_initial_TB_table_consumer();
		define_initial_TB_table_retailer();
		define_initial_TB_table_wholesaler();
		*/
		/*for (int i = 0; i < bookKeeperlist.size(); i++) {
		if(bookKeeperlist[i]->get_agent_type()==2132) bookKeeperlist[i]->print_TB_table();
		}*/
		for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
			if (bookKeeperlist[i]->get_agent_type() == 31 || bookKeeperlist[i]->get_agent_type() == 310 ||bookKeeperlist[i]->get_agent_type() == 311 || bookKeeperlist[i]->get_agent_type() == 312 || bookKeeperlist[i]->get_agent_type()==3100 || bookKeeperlist[i]->get_agent_type()==3110 || bookKeeperlist[i]->get_agent_type()==3120)
			add_DrCrValues(i, TB_table_consumer);
		//	else if (bookKeeperlist[i]->get_agent_type() == 1)		add_DrCrValues(i, TB_table_government);	
			else if (bookKeeperlist[i]->get_agent_type() == 2130)		add_DrCrValues(i, TB_table_soleproprietor);
			else if (bookKeeperlist[i]->get_agent_type() == 2131)		add_DrCrValues(i, TB_table_retailer);
			else if (bookKeeperlist[i]->get_agent_type() == 2132)         add_DrCrValues(i, TB_table_wholesaler);
		}//for(int i=0,....の終わり
		//printf("before create_cashBookTotal for consumer\n");
		create_cashBookTotal(31, app_context.period_count);
		//printf("after create_cashBookTotal for consumer\n");

		//create_cashBookTotal(1, app_context.period_count);
		//printf("after create_cashBookTotal for government\n");
		create_cashBookTotal(310, app_context.period_count);
		create_cashBookTotal(2130, app_context.period_count);
		create_cashBookTotal(2131, app_context.period_count);
		create_cashBookTotal(2132, app_context.period_count);
		//printf("after create_cashBookTotal for consumer\n");
		//create_bankBookTotal(1, app_context.period_count);	
		create_bankBookTotal(31, app_context.period_count);
		create_bankBookTotal(2130, app_context.period_count);
		create_bankBookTotal(2131, app_context.period_count);
		create_bankBookTotal(2132, app_context.period_count);
	//	printf(" before create_stockBookTotal \n");
	//	create_stockBookTotal(31, app_context.period_count);
		create_stockBookTotal(31, app_context.period_count);
		create_stockBookTotal(2130, app_context.period_count);
		create_stockBookTotal(2131, app_context.period_count);
		create_stockBookTotal(2132, app_context.period_count);
		//create_stockBookTotal(1, app_context.period_count);
//		printf("before create_stockpriceBookTotal \n");
		create_stockpriceBookTotal(31, app_context.period_count);
		create_stockpriceBookTotal(2130, app_context.period_count);
		create_stockpriceBookTotal(2131, app_context.period_count);
		create_stockpriceBookTotal(2132, app_context.period_count);
		//create_stockpriceBookTotal(1, app_context.period_count);
	//	printf("before create_materialBookTotal \n");
		//create_materialBookTotal(31, app_context.period_count);
		//create_materialBookTotal(2130, app_context.period_count);
		create_materialBookTotal(2131, app_context.period_count);
		//create_materialBookTotal(2132, app_context.period_count);
	
	//	printf("before create_materialpriceBookTotal \n");
		//create_materialpriceBookTotal(31, app_context.period_count);
		//create_materialpriceBookTotal(2130, app_context.period_count);
		create_materialpriceBookTotal(2131, app_context.period_count);
		//create_materialpriceBookTotal(2132, app_context.period_count);
	//	printf("before create_genkaBookTotal 2130\n");
		create_genkaBookTotal(2130, app_context.period_count);
		//printf(" before create_genkaBookTotal 2131\n");
		create_genkaBookTotal(2131, app_context.period_count);
	//	printf("before create_genaBookTotal 2132 \n");
		create_genkaBookTotal(2132, app_context.period_count);
		//printf("after create_genaBookTotal 2132 \n");



		//printf("after create_stockBookTotal \n");
		printf("before consumerTotal=NULL\n");
		AccountantData* consumerTotal = NULL;
		consumerTotal = app_context.config->consumerTotal; {
		    print_TB_table(consumerTotal, TB_table_consumer); 
			print_cashBook(app_context.period_count,consumerTotal, cashBook_consumer);
			print_bankBook(app_context.period_count, consumerTotal, bankBook_consumer);
			print_stockBook(app_context.period_count, consumerTotal, stockBook_consumer);
		    print_stockpriceBook(app_context.period_count, consumerTotal, stockpriceBook_consumer);
		}
	//	printf("after consumerTotal=NULL\n");
		/*
		AccountantData* governmentTotal = NULL;
		governmentTotal = app_context.config->governmentTotal; {
			print_TB_table(governmentTotal, TB_table_government);
			print_cashBook(app_context.period_count, governmentTotal, cashBook_government);
			print_bankBook(app_context.period_count, governmentTotal, bankBook_government);
			print_stockBook(app_context.period_count, governmentTotal, stockBook_government);
			print_stockpriceBook(app_context.period_count, governmentTotal, stockpriceBook_government);
		}
		printf("before governmentTotal=NULL\n");
		*/
		//printf("after governmentTotal=NULL\n");
		AccountantData* soleproprietorTotal = NULL;
		soleproprietorTotal = app_context.config->soleproprietorTotal; {
	//printf("before print_TB_table for soleproprietorTotal\n");	
			print_TB_table(soleproprietorTotal, TB_table_soleproprietor);
		//	printf("after print_TB_table before cashBook\n");
			print_cashBook(app_context.period_count, soleproprietorTotal, cashBook_soleproprietor);
			print_bankBook(app_context.period_count, soleproprietorTotal, bankBook_soleproprietor);
			print_stockBook(app_context.period_count, soleproprietorTotal, stockBook_soleproprietor);
			print_stockpriceBook(app_context.period_count, soleproprietorTotal, stockpriceBook_soleproprietor);
			print_genkaBook(app_context.period_count, soleproprietorTotal, genkaBook_soleproprietor);
		}
	//	printf("after soleproprietorTotal=NULL\n");
		AccountantData* retailerTotal = NULL;	retailerTotal = app_context.config->retailerTotal; {
		//	printf("inside retailerTotal before print_TB_talble \n");
			print_TB_table(retailerTotal, TB_table_retailer);
		//	printf("inside retailerTotal after print_TB_table \n");
			print_cashBook(app_context.period_count,retailerTotal, cashBook_retailer);
			print_bankBook(app_context.period_count, retailerTotal, bankBook_retailer);
			print_stockBook(app_context.period_count, retailerTotal, stockBook_retailer);
			print_stockpriceBook(app_context.period_count, retailerTotal, stockpriceBook_retailer);
			//-------------------
			print_materialBook(app_context.period_count, retailerTotal, materialBook_retailer);
			print_materialpriceBook(app_context.period_count, retailerTotal, materialpriceBook_retailer);
			//--------------------
			print_genkaBook(app_context.period_count, retailerTotal, genkaBook_retailer);
		}
	//	printf("before wholesalerTotal=NULL\n");
		AccountantData* wholesalerTotal = NULL; wholesalerTotal = app_context.config->wholesalerTotal; {
			print_TB_table(wholesalerTotal, TB_table_wholesaler);
			print_cashBook(app_context.period_count,wholesalerTotal, cashBook_wholesaler);
			print_bankBook(app_context.period_count, wholesalerTotal, bankBook_wholesaler);
			print_stockBook(app_context.period_count, wholesalerTotal, stockBook_wholesaler);
			print_stockpriceBook(app_context.period_count, wholesalerTotal, stockpriceBook_wholesaler);
			print_genkaBook(app_context.period_count, wholesalerTotal, genkaBook_wholesaler);	
		}
	//	printf("after wholesalerTotal=NULL\n");
/*for (int i = 0; i < bookKeeperlist.size(); i++) {
	TB_table = bookKeeperlist[i]->get_TBtable();
	if (bookKeeperlist[i]->get_agent_id() == 15 || bookKeeperlist[i]->get_agent_id() == 460 || bookKeeperlist[i]->get_agent_id() == 540) {
		fprintf(app_context.config->out_temporary, "agent_id=%d agent_type=%d TB_table.size()=%d\n", bookKeeperlist[i]->get_agent_id(), bookKeeperlist[i]->get_agent_type(),TB_table.size());

		//int size = TB_table.size();
		for (int j = 0; j < TB_table.size(); j++) fprintf(app_context.config->out_temporary, " code=%d %s, Dr_total=%d Cr_total=%d,\n", TB_table[j]->code,TB_table[j]->title.c_str(), TB_table[j]->Dr_total, TB_table[j]->Cr_total);

	}
	if (bookKeeperlist[i]->get_agent_type()== 311 || bookKeeperlist[i]->get_agent_type() == 312) {
		if (n == 0) { size_max = bookKeeperlist[i]->get_TBtable().size(); size_min = bookKeeperlist[i]->get_TBtable().size(); }
		if (bookKeeperlist[i]->get_TBtable().size()> size_max) size_max = bookKeeperlist[i]->get_TBtable().size();
		if (bookKeeperlist[i]->get_TBtable().size() < size_min) size_min = bookKeeperlist[i]->get_TBtable().size();
		n++;
	}
	bookKeeperlist[i]->clear_TB_table();  //bookKeeper内のTB_tableを収去する
}
*/
	//	printf("end of do_kimatsu in accountant before print_IO_table\n");
		print_IO_table_government();
		// print_IO_table()は現在使わない、将来のために残す　2022年5月24日　荻林
		// 
	//	printf("end of do_kimatsu in accountant after print_IO_table\n");	
		fprintf(app_context.config->out_temporary, "size_max=%d size_min=%d\n", size_max, size_min);
		for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
			bookKeeperlist[i]->clear_TB_table();
			bookKeeperlist[i]->clear_journal();
			//bookKeeperlist[i]->clear_title_list();2022年10月25日削除科目リストの中の未登場科目だけをvectorに追加する
		}
		TB_table_consumer.clear();
		TB_table_soleproprietor.clear();
		TB_table_retailer.clear();
		TB_table_wholesaler.clear();
		//TB_table_government.clear();
	}
	void print_TB_table(AccountantData* accountantData, vector<trialBalanceSlip*> TB_table) {
		//Accountant.hの中で定義したprint_TB_talbe関数
		//Accountantのdo_kimatsu()の中で呼ばれる
		//app_context.accountant->do_kimatsu();はSimulalterAppのprocess_period()の中の最後に毎期呼ばれる
		long long BStable_Dr_total = 0;
		long long BStable_Cr_total = 0;
		long long PLtable_Dr_total = 0;
		long long PLtable_Cr_total = 0;
		int size = TB_table.size();
		//printf("inside print_TB_table before for category_type=%d TB_table.size()=%d address of TB_table=%x\n", accountantData->get_category_type(), size);
		for (int i = 0; i < size; i++) {
			if (i == 0) {
			//	printf("試算表のタイトル行出力前 period=,%d, size=,%d,category_type=,%d,\n", TB_table[i]->period, size, accountantData->get_category_type());			
			//	printf("before fprintf accountData->out_TBtable period=%d\n",TB_table[i]->period);
			//	printf("period=,%d, size=,%d,category_type=,%d,\n", TB_table[i]->period, size, accountantData->get_category_type());
				fprintf(accountantData->out_TBtable, "period=,%d, size=,%d,category_type=,%d,\n", TB_table[i]->period, size, accountantData->get_category_type());
			
			//	printf("after fprintf accountData->out_TBtable\n");		
				fprintf(accountantData->out_TBtable, ",,試算表,,,,,貸借対照表,,,,損益計算書,\n");
				fprintf(accountantData->out_TBtable, "科目コード,勘定科目,借方,貸方,,,期首残高,借方,貸方,期末残高,,借方,貸方,\n");
		//		printf("inside print_TB_table before if文 period=,%d, size=,%d,category_type=,%d,\n", TB_table[i]->period, size, accountantData->get_category_type());
				if (TB_table[i]->code == 0 || TB_table[i]->code == 2) fprintf(accountantData->out_TBtable_balance, "\n,period=,%d,", TB_table[i]->period);

			}
			//試算表の各行プリントアウト
			fprintf(accountantData->out_TBtable, "%d,%s,%I64d,%I64d,", TB_table[i]->code, TB_table[i]->title.c_str(), TB_table[i]->Dr_total, TB_table[i]->Cr_total);
		//	printf("inside print_TB_table before 資産科目 i=%d category_type=%d TB_table.size()=%d\n", i, accountantData->get_category_type(), size);	
			if (TB_table[i]->code == 0) {
				//貸借対照表の資産科目各行プリントアウト
				fprintf(accountantData->out_TBtable, ",,%I64d,%I64d,,%I64d,,\n", TB_table[i]->opening_balance,TB_table[i]->Dr_total - TB_table[i]->Cr_total,TB_table[i]->closing_balance);
				BStable_Dr_total += TB_table[i]->Dr_total - TB_table[i]->Cr_total;
			}
	
			else if (TB_table[i]->code == 2) {
			//	printf("inside print_TB_table before 負債科目 i=%d category_type=%d TB_table.size()=%d\n", i, accountantData->get_category_type(), size);
				//貸借対照表の負債科目各行プリントアウト
				fprintf(accountantData->out_TBtable, ",,%I64d,,%I64d,%I64d,,\n", TB_table[i]->opening_balance,TB_table[i]->Cr_total - TB_table[i]->Dr_total,TB_table[i]->closing_balance);
				BStable_Cr_total += TB_table[i]->Cr_total - TB_table[i]->Dr_total;
			}
			else if (TB_table[i]->code == 1) {
			//	printf("inside print_TB_table before 経費科目 i=%d category_type=%d TB_table.size()=%d\n", i, accountantData->get_category_type(), size);
				//損益計算書の経費科目各行プリントアウト
				fprintf(accountantData->out_TBtable, ",,,,,,,%I64d,,,\n", TB_table[i]->Dr_total - TB_table[i]->Cr_total);
				PLtable_Dr_total += TB_table[i]->Dr_total - TB_table[i]->Cr_total;
			}
			else if (TB_table[i]->code == 3) {
			//	printf("inside print_TB_table before 売上科目 i=%d category_type=%d TB_table.size()=%d\n", i, accountantData->get_category_type(), size);
				//損益計算書の売上科目各行プリントアウト
				fprintf(accountantData->out_TBtable, ",,,,,,,,%I64d,,\n", TB_table[i]->Cr_total - TB_table[i]->Dr_total);
				PLtable_Cr_total += TB_table[i]->Cr_total - TB_table[i]->Dr_total;
			}
			if (TB_table[i]->code == 0 || TB_table[i]->code == 2) {
				fprintf(accountantData->out_TBtable_balance, "%s,%I64d,", TB_table[i]->title.c_str(), TB_table[i]->closing_balance);	
			}
			
		}
		//printf("after for(i= category_type=%d TB_table.size()=%d\n", accountantData->get_category_type(), size);
		fprintf(accountantData->out_TBtable, ",当期利益,,,,,,,%I64d,,,%I64d,\n", BStable_Dr_total - BStable_Cr_total, PLtable_Cr_total - PLtable_Dr_total);
		fprintf(accountantData->out_TBtable, ",合計,,,,,,%I64d,%I64d,,,%I64d,%I64d,\n", BStable_Dr_total, BStable_Dr_total, PLtable_Cr_total, PLtable_Cr_total);
		fprintf(accountantData->out_TBtable, ",\n");
		fprintf(accountantData->out_profit, "period=,%d, category_type=,%d,当期利益=, %I64d,%I64d,\n",
			app_context.period_count,accountantData->get_category_type(), BStable_Dr_total - BStable_Cr_total, PLtable_Cr_total - PLtable_Dr_total);
		//printf("before printBooks category_type= %d\n", accountantData->get_category_type());
		print_Books(accountantData, TB_table);
		//printf("after print_Books\n");
	}
	void print_Books(AccountantData* accountantData, vector<trialBalanceSlip*> TB_table) {
	
		//試算表の各勘定科目について試算表の値をプリントアウトする。
		//create_GLSlip()の中の最後にで呼ばれる。
		int size = TB_table.size();
		//AccountantData* accountantData = NULL;
		if (accountantData->category_type == 31) accountantData = app_context.config->consumerTotal;
		//else if (accountantData->category_type == 1) accountantData = app_context.config->governmentTotal;	
		else if (accountantData->category_type == 2130) accountantData = app_context.config->soleproprietorTotal;
		else if (accountantData->category_type == 2131) accountantData = app_context.config->retailerTotal;
		else if (accountantData->category_type == 2132) accountantData = app_context.config->wholesalerTotal;
		//fprintf(accountData->out_TBtableCashBook, "period,agent_id,title, opening_balance, Dr_total, Cr_Total,closing_balance, \n");
		for (int i = 0; i < size; i++) {
			//printf("before if agent_type=%d \n",get_agent_type());
		//	if (get_agent_type()==2131 && title_list[i]->title == "現金") {
			if (TB_table[i]->title == "現金") {
				fprintf(accountantData->out_TBtableCashBook, "%d,%s,%I64d,%I64d,%I64d,%I64d,\n ", TB_table[i]->period, TB_table[i]->title.c_str(), TB_table[i]->opening_balance, TB_table[i]->Dr_total, TB_table[i]->Cr_total, TB_table[i]->closing_balance);
			}
			else if (TB_table[i]->title == "預金") {
				fprintf(accountantData->out_TBtableBankBook, "%d,%s,%I64d,%I64d,%I64d,%I64d,\n ", TB_table[i]->period, TB_table[i]->title.c_str(), TB_table[i]->opening_balance, TB_table[i]->Dr_total, TB_table[i]->Cr_total, TB_table[i]->closing_balance);
			}
			else if (TB_table[i]->title == "製品在庫") {
				fprintf(accountantData->out_TBtableInventoryBook, "%d,%s,%I64d,%I64d,%I64d,%I64d,\n ", TB_table[i]->period, TB_table[i]->title.c_str(), TB_table[i]->opening_balance, TB_table[i]->Dr_total, TB_table[i]->Cr_total, TB_table[i]->closing_balance);
			}
			else if (TB_table[i]->title == "売上") {
				fprintf(accountantData->out_TBtableSaleBook, "%d,%s,%I64d,%I64d,%I64d,%I64d,\n ", TB_table[i]->period, TB_table[i]->title.c_str(), TB_table[i]->opening_balance, TB_table[i]->Dr_total, TB_table[i]->Cr_total, TB_table[i]->closing_balance);
			}

			else if (TB_table[i]->title == "直接労務費") {
				//printf("inside 直接労務費 size=%d i=%d\n",size,i);
				fprintf(accountantData->out_TBtableDirectlaborcostBook, "%d,%s,%I64d,%I64d,%I64d,%I64d,\n ", TB_table[i]->period, TB_table[i]->title.c_str(), TB_table[i]->opening_balance, TB_table[i]->Dr_total, TB_table[i]->Cr_total, TB_table[i]->closing_balance);
			}
			
			else if (TB_table[i]->title == "直接材料費") {
				//printf("inside 直接材料費\n");
				fprintf(accountantData->out_TBtableDirectmaterialfeeBook, "%d,%s,%I64d,%I64d,%I64d,%I64d,\n ", TB_table[i]->period, TB_table[i]->title.c_str(), TB_table[i]->opening_balance, TB_table[i]->Dr_total, TB_table[i]->Cr_total, TB_table[i]->closing_balance);
			}
			//else if (TB_table[i]->title == "当期利益") {


			
			
			//printf("after if\n");
		}//for(i=0....の終わり
	}
	void print_cashBook(int period, AccountantData* accountantData, vector < cashBookSlip*> cashBookTotal) {
		if (period == 0) {
			fprintf(accountantData->out_cashBook, "category_type=,%d,\n", accountantData->category_type);
			fprintf(accountantData->out_cashBook, " period, income,spending, balance,\n");
		}
			for (unsigned int i = 0; i < cashBookTotal.size(); i++) {
				if(cashBookTotal[i]->period==period)
				fprintf(accountantData->out_cashBook, "%d,%I64d,%I64d,%I64d,\n", cashBookTotal[i]->period, cashBookTotal[i]->income, cashBookTotal[i]->spending, cashBookTotal[i]->balance);
			}
	}
	void print_bankBook(int period, AccountantData* accountantData, vector < bankBookSlip*> bankBookTotal) {
		//consumerTotal,retailerTota;,wholelsalerTotalのout_bankBook.csvへのプリントアウト
		if (period == 0) {
			fprintf(accountantData->out_bankBook, "category_type=,%d,\n", accountantData->category_type);
			fprintf(accountantData->out_bankBook, " period, deposit,withdrawal, balance,\n");
		}
		for (unsigned int i = 0; i < bankBookTotal.size(); i++) {
			if (bankBookTotal[i]->period == period)
				fprintf(accountantData->out_bankBook, "%d,%I64d,%I64d,%I64d,\n", bankBookTotal[i]->period, bankBookTotal[i]->deposit, bankBookTotal[i]->withdrawal, bankBookTotal[i]->balance);
		}
	}
	void print_stockBook(int period, AccountantData* accountantData, vector < stockBookSlip*> stockBookTotal) {
		//printf("inside print_stockBook in Accountant period=%d\n",period);
		//consumerTotal,retailerTota;,wholelsalerTotalのout_bankBook.csvへのプリントアウト
		long long ave_price=0;
		if (period == 0) {
			fprintf(accountantData->out_stockBook, "category_type=,%d,\n", accountantData->category_type);
			fprintf(accountantData->out_stockBook, " period, production,bought, balance,ave_price,\n");
		}
		//printf("inside print_stockBook before if\n");
		//fprintf(accountantData->out_stockBook, "period=,%d,stockBookTotal[0]->period=,%d,stockBookTotal.size=,%d,\n", period,stockBookTotal[0]->period,stockBookTotal.size());
		if (stockBookTotal.size() > 0) {
			for (unsigned int i = 0; i < stockBookTotal.size(); i++) {
				if (stockBookTotal[i]->period == period)	{
					//ave_price =long long(double ( stockBookTotal[i]->price) / stockBookTotal.size());
					ave_price = long long(double(stockBookTotal[i]->price));
				//	fprintf(accountantData->out_stockBook, "%d,%I64d,%I64d,%I64d,\n", stockBookTotal[i]->period, stockBookTotal[i]->production, stockBookTotal[i]->bought, stockBookTotal[i]->balance );
					fprintf(accountantData->out_stockBook, "%d,%I64d,%I64d,%I64d,%I64d,\n", stockBookTotal[i]->period, stockBookTotal[i]->production, stockBookTotal[i]->bought, stockBookTotal[i]->balance,ave_price);
				}
			}
		}
	}
	void print_stockpriceBook(int period, AccountantData* accountantData, vector < stockpriceBookSlip*> stockpriceBookTotal) {
		//consumerTotal,retailerTota;,wholelsalerTotalのout_bankBook.csvへのプリントアウト
		if (period == 0) {
			fprintf(accountantData->out_stockpriceBook, "category_type=,%d,\n", accountantData->category_type);
			fprintf(accountantData->out_stockpriceBook, " period, production_price,bought_price, balance_price,\n");
		}
		//fprintf(accountantData->out_stockBook, "period=,%d,stockBookTotal[0]->period=,%d,stockBookTotal.size=,%d,\n", period,stockBookTotal[0]->period,stockBookTotal.size());
		if (stockpriceBookTotal.size() > 0) {
			for (unsigned int i = 0; i < stockpriceBookTotal.size(); i++) {
				if (stockpriceBookTotal[i]->period == period)
					fprintf(accountantData->out_stockpriceBook, "%d,%I64d,%I64d,%I64d,\n", stockpriceBookTotal[i]->period, stockpriceBookTotal[i]->production_price, stockpriceBookTotal[i]->bought_price, stockpriceBookTotal[i]->balance_price);
			}
		}
	}
	void print_cashBook_retailer(int period, int agent_type){
		for(unsigned int i=0;i<bookKeeperlist.size();i++){
			if(bookKeeperlist[i]->get_agent_type()==2131)bookKeeperlist[i]->print_cashbook(period);
		}
	}
	void print_cashBook_soleproprietor(int period, int agent_type) {
		for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
			if (bookKeeperlist[i]->get_agent_type() == 2130)bookKeeperlist[i]->print_cashbook(period);
		}
	}
	//----------------------
	void print_materialBook(int period, AccountantData* accountantData, vector < materialBookSlip*> materialBookTotal) {
		//printf("inside print_materialBook in Accountant period=%d\n",period);
		//consumerTotal,retailerTota;,wholelsalerTotalのout_bankBook.csvへのプリントアウト
		long long ave_price = 0;
		if (period == 0) {
			fprintf(accountantData->out_materialBook, "category_type=,%d,\n", accountantData->category_type);
			fprintf(accountantData->out_materialBook, " period, production,bought, balance,ave_price,\n");
		}
	//	printf("inside print_materialBook before if materialBookTotl.size()= %d\n", materialBookTotal.size());
		//fprintf(accountantData->out_materialBook, "period=,%d,materialBookTotal[0]->period=,%d,materialBookTotal.size=,%d,\n", period,materialBookTotal[0]->period,materialBookTotal.size());
		if (materialBookTotal.size() > 0) {
			for (unsigned int i = 0; i < materialBookTotal.size(); i++) {
				if (materialBookTotal[i]->period == period) {
					//ave_price =long long(double ( materialBookTotal[i]->price) / materialBookTotal.size());
					ave_price = long long(double(materialBookTotal[i]->price));
					//	fprintf(accountantData->out_materialBook, "%d,%I64d,%I64d,%I64d,\n", materialBookTotal[i]->period, materialBookTotal[i]->production, materialBookTotal[i]->bought, materialBookTotal[i]->balance );
					fprintf(accountantData->out_materialBook, "%d,%I64d,%I64d,%I64d,%I64d,\n", materialBookTotal[i]->period, materialBookTotal[i]->production, materialBookTotal[i]->bought, materialBookTotal[i]->balance, ave_price);
				}
			}
		}
	}
	void print_materialpriceBook(int period, AccountantData* accountantData, vector < materialpriceBookSlip*> materialpriceBookTotal) {
		//consumerTotal,retailerTota;,wholelsalerTotalのout_bankBook.csvへのプリントアウト
		if (period == 0) {
			fprintf(accountantData->out_materialpriceBook, "category_type=,%d,\n", accountantData->category_type);
			fprintf(accountantData->out_materialpriceBook, " period, production_price,bought_price, balance_price,\n");
		}
		//fprintf(accountantData->out_materialBook, "period=,%d,materialBookTotal[0]->period=,%d,materialBookTotal.size=,%d,\n", period,materialBookTotal[0]->period,materialBookTotal.size());
		if (materialpriceBookTotal.size() > 0) {
			for (unsigned int i = 0; i < materialpriceBookTotal.size(); i++) {
				if (materialpriceBookTotal[i]->period == period)
					fprintf(accountantData->out_materialpriceBook, "%d,%I64d,%I64d,%I64d,\n", materialpriceBookTotal[i]->period, materialpriceBookTotal[i]->production_price, materialpriceBookTotal[i]->bought_price, materialpriceBookTotal[i]->balance_price);
			}
		}
	}
	
	//-----------------------
	void print_genkaBook(int period, AccountantData* accountantData, vector < genkaBookSlip*> genkaBookTotal) {
		//consumerTotal,retailerTota;,wholelsalerTotalのout_bankBook.csvへのプリントアウト
		if (period == 0) {
			fprintf(accountantData->out_genkaBook, "category_type=,%d,\n", accountantData->category_type);
			fprintf(accountantData->out_genkaBook, " period, size_shiped,direct_labor_cost, direct_material_cost,genka,\n");
		}
		//fprintf(accountantData->out_materialBook, "period=,%d,materialBookTotal[0]->period=,%d,materialBookTotal.size=,%d,\n", period,materialBookTotal[0]->period,materialBookTotal.size());
		if (genkaBookTotal.size() > 0) {
			for (unsigned int i = 0; i < genkaBookTotal.size(); i++) {
				if (genkaBookTotal[i]->period == period)
					fprintf(accountantData->out_genkaBook, "%d,%d,%I64d,%I64d,%I64d,\n", genkaBookTotal[i]->period, genkaBookTotal[i]->size_shiped, genkaBookTotal[i]->direct_labor_cost, genkaBookTotal[i]->direct_material_cost,genkaBookTotal[i]->genka);
			}
		}
	}


	void print_cashBook_wholesaler(int period, int agent_type) {
		for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
			if (bookKeeperlist[i]->get_agent_type() == 2132)bookKeeperlist[i]->print_cashbook(period);
		}
	}
	void print_stockBook_soleproprietor(int period, int agent_type) {
		for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
			if (bookKeeperlist[i]->get_agent_type() == 2130)bookKeeperlist[i]->print_stockbook(period);
		}
	}
	void print_stockpriceBook_soleproprietor(int period, int agent_type) {
		for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
			if (bookKeeperlist[i]->get_agent_type() == 2130)bookKeeperlist[i]->print_stockpricebook(period);
		}
	}

	void print_stockBook_retailer(int period, int agent_type) {
		for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
			if (bookKeeperlist[i]->get_agent_type() == 2131)bookKeeperlist[i]->print_stockbook(period);
		}
	}
	void print_stockpriceBook_retailer(int period, int agent_type) {
		for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
			if (bookKeeperlist[i]->get_agent_type() == 2131)bookKeeperlist[i]->print_stockpricebook(period);
		}
	}
	void print_stockBook_wholesaler(int period, int agent_type) {
		for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
			if (bookKeeperlist[i]->get_agent_type() == 2132)bookKeeperlist[i]->print_stockbook(period);
		}
	}
	void print_stockpriceBook_wholesaler(int period, int agent_type) {
		for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {
			if (bookKeeperlist[i]->get_agent_type() == 2132)bookKeeperlist[i]->print_stockpricebook(period);
		}
	}
	void print_totalsalesave_soleproprietor() {
		long long sum = 0; long long sum_e = 0; long long sum_b = 0;
		fprintf(app_context.config->retailMarketData->out_total_sales, "start of print_totalsalesave()\n");
		int n = 0;
		long long totalsales = 0; long long total_spending = 0; long long total_balance = 0;
		//printf("before for in printtotalsalesave\n");
		//retailer分の計算
		for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {

			if (bookKeeperlist[i]->get_agent_type() == 2130) {
				//printf("inside for i=%d\n",i);
				sum += bookKeeperlist[i]->get_totalsales(app_context.period_count);//period期の総売り上げ高を返す
				sum_e += bookKeeperlist[i]->get_totalspending(app_context.period_count); // e ... expendeture
				sum_b += bookKeeperlist[i]->get_totalbalance(app_context.period_count); // b ... balance
				n++;
				fprintf(app_context.config->materialMarketData->out_total_sales, "size=%d, i=,%d, n=,%d,period=,%d,agent_id=,%d,total_sales=,%I64d,total_spending,=%I64d, total_balance=,%I64d,initial_cash=,%I64d,\n"
					, bookKeeperlist.size(), i, n, app_context.period_count, bookKeeperlist[i]->get_agent_id(), bookKeeperlist[i]->get_totalsales(app_context.period_count), bookKeeperlist[i]->get_totalspending(app_context.period_count), bookKeeperlist[i]->get_totalbalance(app_context.period_count), bookKeeperlist[i]->get_initial_cash());
			}

			//printf("end of for i=%d\n",i);
		}
		fprintf(app_context.config->materialMarketData->out_total_sales_ave, "period=,%d, total_sales_ave=,%8.3f,total_spending_ave=,%8.3f,total_balance_ave=,%8.3f,initial_cash=,\n",
			app_context.period_count, double(sum) / n, double(sum_e) / n, double(sum_b) / n);
	}

	void print_totalsalesave_retailer() {
		long long sum = 0; long long sum_e = 0; long long sum_b = 0;
		fprintf(app_context.config->retailMarketData->out_total_sales, "start of print_totalsalesave()\n");
		int n = 0;
		long long totalsales = 0; long long total_spending = 0; long long total_balance = 0;
		//printf("before for in printtotalsalesave\n");
		//retailer分の計算
		for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {

			if (bookKeeperlist[i]->get_agent_type() == 2131) {
				//printf("inside for i=%d\n",i);
				sum += bookKeeperlist[i]->get_totalsales(app_context.period_count);//period期の総売り上げ高を返す
				sum_e += bookKeeperlist[i]->get_totalspending(app_context.period_count); // e ... expendeture
				sum_b += bookKeeperlist[i]->get_totalbalance(app_context.period_count); // b ... balance
				n++;
				fprintf(app_context.config->retailMarketData->out_total_sales, "size=%d, i=,%d, n=,%d,period=,%d,agent_id=,%d,total_sales=,%I64d,total_spending,=%I64d, total_balance=,%I64d,initial_cash=,%I64d,\n"
					, bookKeeperlist.size(), i, n, app_context.period_count, bookKeeperlist[i]->get_agent_id(), bookKeeperlist[i]->get_totalsales(app_context.period_count), bookKeeperlist[i]->get_totalspending(app_context.period_count), bookKeeperlist[i]->get_totalbalance(app_context.period_count), bookKeeperlist[i]->get_initial_cash());
			}

			//printf("end of for i=%d\n",i);
		}
		fprintf(app_context.config->retailMarketData->out_total_sales_ave, "period=,%d, total_sales_ave=,%8.3f,total_spending_ave=,%8.3f,total_balance_ave=,%8.3f,initial_cash=,\n",
			app_context.period_count, double(sum) / n, double(sum_e) / n, double(sum_b) / n);
	}
	void print_totalsalesave_wholesaler(){
		long long sum = 0; long long sum_e = 0; long long sum_b = 0;
		fprintf(app_context.config->retailMarketData->out_total_sales, "start of print_totalsalesave()\n");
		int n = 0;
		long long totalsales = 0; long long total_spending = 0; long long total_balance = 0;
		//printf("before for in printtotalsalesave\n");
		//retailer分の計算
		for (unsigned int i = 0; i < bookKeeperlist.size(); i++) {

			if (bookKeeperlist[i]->get_agent_type() == 2132) {
				//printf("inside for i=%d\n",i);
				sum += bookKeeperlist[i]->get_totalsales(app_context.period_count);//period期の総売り上げ高を返す
				sum_e += bookKeeperlist[i]->get_totalspending(app_context.period_count); // e ... expendeture
				sum_b += bookKeeperlist[i]->get_totalbalance(app_context.period_count); // b ... balance
				n++;
				fprintf(app_context.config->materialMarketData->out_total_sales, "size=%d, i=,%d, n=,%d,period=,%d,agent_id=,%d,total_sales=,%I64d,total_spending,=%I64d, total_balance=,%I64d,initial_cash=,%I64d,\n"
					, bookKeeperlist.size(), i, n, app_context.period_count, bookKeeperlist[i]->get_agent_id(), bookKeeperlist[i]->get_totalsales(app_context.period_count), bookKeeperlist[i]->get_totalspending(app_context.period_count), bookKeeperlist[i]->get_totalbalance(app_context.period_count), bookKeeperlist[i]->get_initial_cash());
			}

			//printf("end of for i=%d\n",i);
		}
		fprintf(app_context.config->materialMarketData->out_total_sales_ave, "period=,%d, total_sales_ave=,%8.3f,total_spending_ave=,%8.3f,total_balance_ave=,%8.3f,initial_cash=,\n",
			app_context.period_count, double(sum) / n, double(sum_e) / n, double(sum_b) / n);
	}
	void print_IO_table_government() {
		fprintf(app_context.config->out_trace_government, "period=,%d,inside print_IO_table_government()\n",app_context.period_count);
		//IO_tableの各象限の要素の値を求める
		long long a11 = 0, a12 = 0, a13 = 0, a21 = 0, a22 = 0, a23 = 0, a31 = 0, a32 = 0, a33 = 0;
		long long b11 = 0, b12 = 0, b13 = 0, b21 = 0, b22 = 0, b23 = 0;
		long long c11 = 0, c12 = 0, c13 = 0, c21 = 0, c22 = 0, c23 = 0, c31 = 0, c32 = 0, c33 = 0;
		//printf("before difine a11\n");
		a11 = a12 = a13 = a21 = a22 = a23 = a31 = a32 = a33 = 0;
		b11 = b12 = b13 = b21 = b22 = b23 = 0;
		c11 = c12 = c13 = c21 = c22 =c23= c31 = c32 = c33 =0;
		long long genka_retailer = 0;
		long long genka_wholesaler = 0;
		long long genka_soleproprietor = 0;
		long long uriage_retailer = 0;
		long long uriage_wholesaler = 0;
		long long uriage_soleproprietor = 0;
		long long roumuhi_sagaku_retailer = 0;
		long long roumuhi_sagaku_wholesaler = 0;
		long long roumuhi_sagaku_soleproprietor = 0;
		long long yakuin_houshu_retailer = 0;
		long long yakuin_houshu_wholesaler = 0;
		long long yakuin_houshu_soleproprietor = 0;
		//printf("before for in print_IO_table\n");
		for (unsigned int i = 0; i < TB_table_retailer.size(); i++) {
			if (TB_table_retailer[i]->title == "未払給与") b11 = TB_table_retailer[i]->Cr_total;
			if (TB_table_retailer[i]->title == "直接材料費") a21 = TB_table_retailer[i]->Cr_total;
			if (TB_table_retailer[i]->title == "売上") uriage_retailer = TB_table_retailer[i]->Cr_total;
			if (TB_table_retailer[i]->title == "製品在庫") c13 = TB_table_retailer[i]->Dr_total - TB_table_retailer[i]->Cr_total;
			if (TB_table_retailer[i]->title == "売上原価")  genka_retailer = TB_table_retailer[i]->Dr_total;
			if (TB_table_retailer[i]->title == "直接労務費") roumuhi_sagaku_retailer = TB_table_retailer[i]->Dr_total - TB_table_retailer[i]->Cr_total;
			if (TB_table_retailer[i]->title == "役員報酬") yakuin_houshu_retailer = TB_table_retailer[i]->Dr_total;
		}
		//b11 = b11 + yakuin_houshu_retailer;
		c11 = uriage_retailer;
		b21 = c11 - genka_retailer - roumuhi_sagaku_retailer;
		b21 = b21 - yakuin_houshu_retailer;
		for (unsigned int i = 0; i < TB_table_wholesaler.size(); i++) {
			if (TB_table_wholesaler[i]->title == "未払給与") {
				b12 = TB_table_wholesaler[i]->Cr_total;// printf("b12=%d\n", b12);
			}
			//if (TB_table_wholesaler[i]->title == "直接材料費") a21 = TB_table_wholesaler[i]->Cr_total;
			if (TB_table_wholesaler[i]->title == "売上") uriage_wholesaler = TB_table_wholesaler[i]->Cr_total;
			if (TB_table_wholesaler[i]->title == "製品在庫") {
				c23 = TB_table_wholesaler[i]->Dr_total - TB_table_wholesaler[i]->Cr_total;
			}
			if (TB_table_wholesaler[i]->title == "売上原価")  genka_wholesaler = TB_table_wholesaler[i]->Dr_total;
			if (TB_table_wholesaler[i]->title == "直接労務費") roumuhi_sagaku_wholesaler = TB_table_wholesaler[i]->Dr_total - TB_table_wholesaler[i]->Cr_total;
			if (TB_table_wholesaler[i]->title == "役員報酬") yakuin_houshu_wholesaler = TB_table_wholesaler[i]->Dr_total;
		}
		//b12 = b12 + yakuin_houshu_wholesaler;
		b22 = uriage_wholesaler - genka_wholesaler - roumuhi_sagaku_wholesaler;
		b22 = b22 - yakuin_houshu_wholesaler;
		for (unsigned int i = 0; i < TB_table_soleproprietor.size(); i++) {
			if (TB_table_soleproprietor[i]->title == "未払給与") b13 = TB_table_soleproprietor[i]->Cr_total;
			//	if (TB_table_retailer[i]->title == "直接材料費") a21 = TB_table_retailer[i]->Cr_total;
			if (TB_table_soleproprietor[i]->title == "売上") uriage_soleproprietor = TB_table_soleproprietor[i]->Cr_total;
			if (TB_table_soleproprietor[i]->title == "政府売上") c32 = TB_table_soleproprietor[i]->Cr_total;
			if (TB_table_soleproprietor[i]->title == "製品在庫") c33 = TB_table_soleproprietor[i]->Dr_total - TB_table_soleproprietor[i]->Cr_total;
			if (TB_table_soleproprietor[i]->title == "売上原価")  genka_soleproprietor = TB_table_soleproprietor[i]->Dr_total;
			if (TB_table_soleproprietor[i]->title == "直接労務費  ") roumuhi_sagaku_soleproprietor = TB_table_soleproprietor[i]->Dr_total - TB_table_soleproprietor[i]->Cr_total;
			//if (TB_table_soleproprietor[i]->title == "役員報酬") yakuin_houshu_soleproprietor = TB_table_soleproprietor[i]->Dr_total;
		}
		//b13 = b13 + yakuin_houshu_soleproprietor;
		c31 = uriage_soleproprietor;
		b23 = (c31+c32) - genka_soleproprietor - roumuhi_sagaku_soleproprietor;
		//b23 == b23 - yakuin_houshu_soleproprietor;
		printf("b21= %I64d b22=%I64d b23=%I64d \n", b21, b22, b23);
		printf(" uriage_retailer=%I64d gennka_retailer=%I64d keijyourieki=%I64d\n", uriage_retailer, genka_retailer, b21);
		printf(" uriage_wholesaler=%I64d gennka_wholesaler=%I64d keijyourieki=%I64d\n", uriage_wholesaler, genka_wholesaler, b22);
		printf(" uriage_soleproprietor=%I64d gennka_soleproprietor=%I64d keijyourieki=%I64d\n", uriage_soleproprietor, genka_wholesaler, b23);
		print_table_government(a11, a12, a13, a21, a22, a23, a31, a32, a33, b11, b12, b13, b21, b22, b23, c11, c12,c13, c21, c22, c23,c31, c32,c33);
		if (app_context.period_count == 0) {
			//ヘッダー行の出力
			fprintf(app_context.config->out_GDP, "period,GDP1,GDP2,GDP1-GDP2,b11,b12,b13,b21,b22,b23,c11,c12,c13,c21,c22,c23,c31,c32,c33,\n");
			fprintf(app_context.config->out_GDP2, "period,GDP1,GDP2,GDP1-GDP2,b11,b12,b13,b21,b22,b23,c11,c12,c13,c21,c22,c23,c31,c32,c33,\n");
		}
		long long GDP1 = b11 + b12 + b13 + b21 + b22 + b23;//分配側からみたGDP
		long long GDP2 = c11 + c12 + c13+ c21 + c22 + c23+ c31 + c32+c33;//需要側からみたGDP
		fprintf(app_context.config->out_GDP, "%d,%I64d,%I64d,%I64d,\n", app_context.period_count, GDP1, GDP2, GDP1 - GDP2);
		fprintf(app_context.config->out_GDP2, "%d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d\n"
			, app_context.period_count, GDP1, GDP2, GDP1 - GDP2, b11, b12, b13, b21, b22, b23, c11, c12,c13, c21, c22,c23, c31, c32,c33);
	}
	void print_table_government(long long a11, long long a12, long long a13, long long a21, long long a22, long long a23, long long a31, long long a32, long long a33, long long b11, long long b12, long long b13, long long b21, long long b22, long long b23, long long c11, long long c12, long long c13,long long c21, long long c22, long long c23,long long c31, long long c32,long long c33) {
		printf(" start of print_table()\n");
		long long atotal = a11 + a12 + a13 + a21 + a22 + a23 + a31 + a32 + a33;
		long long btotal = b11 + b12 + b13 + b21 + b22 + b23;
		long long ctotal = c11 + c12 + c13 + c21 + c22 + c23 + c31 + c32 + c33;
		fprintf(app_context.config->out_IO_table, "period=,%d,\n", app_context.period_count);
		fprintf(app_context.config->out_IO_table, ",Retailer,Wholesaler,Soleproprietor,中間セクター合計,最終消費,政府消費,在庫増分,最終需要合計,\n");
		fprintf(app_context.config->out_IO_table, "Retailer,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%lld\n", a11, a12, a13, a11 + a12 + a13, c11, c12, c13 ,c11 + c12+c13);
		fprintf(app_context.config->out_IO_table, "Wholesaler,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%lld\n", a21, a22, a23, a21 + a22 + a23, c21, c22,c23, c21 + c22+c23);
		fprintf(app_context.config->out_IO_table, "Soleproprietor,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%lld\n", a31, a32, a33, a31 + a32 + a33, c31, c32,c33, c31 + c32+c33);
		fprintf(app_context.config->out_IO_table, "中間セクター合計,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%lld\n", a11 + a21 + a31, a12 + a22 + a32, a13 + a23 + a33, atotal, c11 + c21 + c31, c12 + c22 + c32, c13+c23+c33,ctotal);
		fprintf(app_context.config->out_IO_table, "支払給与,%I64d,%I64d,%I64d,%I64d,\n", b11, b12, b13, b11 + b12 + b13);
		fprintf(app_context.config->out_IO_table, "当期利益,%I64d,%I64d,%I64d,%I64d,\n", b21, b22, b23, b21 + b22 + b23);
		fprintf(app_context.config->out_IO_table, "付加価値合計,%I64d,%I64d,%I64d,%I64d,\n", b11 + b21, b12 + b22, b13 + b23, btotal);
		fprintf(app_context.config->out_IO_table, "\n");
		printf("end of print_table()\n");
	}


	void print_IO_table() {
		//IO_tableの各象限の要素の値を求める
		long long a11 = 0, a12 = 0, a13 = 0, a21 = 0, a22 = 0, a23 = 0, a31 = 0, a32 = 0, a33 = 0;
		long long b11 = 0, b12 = 0, b13 = 0, b21 = 0, b22 = 0, b23 = 0;
		long long c11 = 0, c12 = 0, c21 = 0, c22 = 0, c31 = 0, c32 = 0;
		//printf("before difine a11\n");
		a11 = a12 = a13= a21 = a22 = a23 = a31 = a32 = a33 = 0;
		b11 = b12 = b13 = b21 = b22 = b23 = 0;
		c11 = c12 = c21 = c22 = c31= c32 = 0;
		long long genka_retailer=0;
		long long genka_wholesaler=0;
		long long genka_soleproprietor = 0;
		long long uriage_retailer = 0;
		long long uriage_wholesaler = 0;
		long long uriage_soleproprietor = 0;
		long long roumuhi_sagaku_retailer = 0;
		long long roumuhi_sagaku_wholesaler = 0;
		long long roumuhi_sagaku_soleproprietor = 0;
		long long yakuin_houshu_retailer = 0;
		long long yakuin_houshu_wholesaler = 0;
		long long yakuin_houshu_soleproprietor = 0;
		//printf("before for in print_IO_table\n");
		for (unsigned int i = 0; i < TB_table_retailer.size(); i++) {
			if (TB_table_retailer[i]->title == "未払給与") b11= TB_table_retailer[i]->Cr_total;
			if (TB_table_retailer[i]->title == "直接材料費") a21 = TB_table_retailer[i]->Cr_total;
			if (TB_table_retailer[i]->title == "売上") uriage_retailer = TB_table_retailer[i]->Cr_total;
			if (TB_table_retailer[i]->title == "製品在庫") c12 = TB_table_retailer[i]->Dr_total-TB_table_retailer[i]->Cr_total;
			if (TB_table_retailer[i]->title == "売上原価")  genka_retailer= TB_table_retailer[i]->Dr_total;
			if (TB_table_retailer[i]->title == "直接労務費") roumuhi_sagaku_retailer = TB_table_retailer[i]->Dr_total - TB_table_retailer[i]->Cr_total;
			if (TB_table_retailer[i]->title == "役員報酬") yakuin_houshu_retailer= TB_table_retailer[i]->Dr_total;
		}
		//b11 = b11 + yakuin_houshu_retailer;
		c11 = uriage_retailer;
		b21 = c11 - genka_retailer-roumuhi_sagaku_retailer;
		b21 = b21 - yakuin_houshu_retailer;
		for (unsigned int i = 0; i < TB_table_wholesaler.size(); i++) {
			if (TB_table_wholesaler[i]->title == "未払給与") {
				b12 = TB_table_wholesaler[i]->Cr_total;// printf("b12=%d\n", b12);
			}
			//if (TB_table_wholesaler[i]->title == "直接材料費") a21 = TB_table_wholesaler[i]->Cr_total;
			if (TB_table_wholesaler[i]->title == "売上") uriage_wholesaler = TB_table_wholesaler[i]->Cr_total;
			if (TB_table_wholesaler[i]->title == "製品在庫") {
				c22 = TB_table_wholesaler[i]->Dr_total - TB_table_wholesaler[i]->Cr_total;
			}
			if (TB_table_wholesaler[i]->title == "売上原価")  genka_wholesaler = TB_table_wholesaler[i]->Dr_total;
			if (TB_table_wholesaler[i]->title == "直接労務費") roumuhi_sagaku_wholesaler = TB_table_wholesaler[i]->Dr_total - TB_table_wholesaler[i]->Cr_total;
			if (TB_table_wholesaler[i]->title == "役員報酬") yakuin_houshu_wholesaler = TB_table_wholesaler[i]->Dr_total;
		}
		//b12 = b12 + yakuin_houshu_wholesaler;
		b22 = uriage_wholesaler - genka_wholesaler-roumuhi_sagaku_wholesaler;
		b22 = b22 - yakuin_houshu_wholesaler;
		for (unsigned int i = 0; i < TB_table_soleproprietor.size(); i++) {
			if (TB_table_soleproprietor[i]->title == "未払給与") b13 = TB_table_soleproprietor[i]->Cr_total;
		//	if (TB_table_retailer[i]->title == "直接材料費") a21 = TB_table_retailer[i]->Cr_total;
			if (TB_table_soleproprietor[i]->title == "売上") uriage_soleproprietor = TB_table_soleproprietor[i]->Cr_total;
			if (TB_table_soleproprietor[i]->title == "製品在庫") c32 = TB_table_soleproprietor[i]->Dr_total - TB_table_soleproprietor[i]->Cr_total;
			if (TB_table_soleproprietor[i]->title == "売上原価")  genka_soleproprietor = TB_table_soleproprietor[i]->Dr_total;
			if (TB_table_soleproprietor[i]->title == "直接労務費  ") roumuhi_sagaku_soleproprietor = TB_table_soleproprietor[i]->Dr_total - TB_table_soleproprietor[i]->Cr_total;
			//if (TB_table_soleproprietor[i]->title == "役員報酬") yakuin_houshu_soleproprietor = TB_table_soleproprietor[i]->Dr_total;
		}
		//b13 = b13 + yakuin_houshu_soleproprietor;
		c31 = uriage_soleproprietor;
		b23 = c31 - genka_soleproprietor - roumuhi_sagaku_soleproprietor;
		//b23 == b23 - yakuin_houshu_soleproprietor;
		printf("b21= %I64d b22=%I64d b23=%I64d \n", b21, b22, b23);
		printf(" uriage_retailer=%I64d gennka_retailer=%I64d keijyourieki=%I64d\n",uriage_retailer,genka_retailer,b21);
		printf(" uriage_wholesaler=%I64d gennka_wholesaler=%I64d keijyourieki=%I64d\n", uriage_wholesaler, genka_wholesaler, b22);
		printf(" uriage_soleproprietor=%I64d gennka_soleproprietor=%I64d keijyourieki=%I64d\n", uriage_soleproprietor, genka_wholesaler, b23);
		print_table(a11,a12,a13,a21,a22,a23,a31,a32,a33,b11,b12,b13,b21,b22,b23,c11,c12,c21,c22,c31,c32);
		if (app_context.period_count == 0) {
			//ヘッダー行の出力
			fprintf(app_context.config->out_GDP, "period,GDP1,GDP2,GDP1-GDP2,b11,b12,b13,b21,b22,b23,c11,c12,c21,c22,c31,c32\n");
			fprintf(app_context.config->out_GDP2, "period,GDP1,GDP2,GDP1-GDP2,b11,b12,b13,b21,b22,b23,c11,c12,c21,c22,c31,c32\n");
		}
		long long GDP1 = b11 + b12 + b13+  b21 + b22 + b23;//分配側からみたGDP
		long long GDP2 = c11 + c12 + c21 + c22+ c31 + c32;//需要側からみたGDP
		fprintf(app_context.config->out_GDP, "%d,%I64d,%I64d,%I64d,\n", app_context.period_count, GDP1, GDP2,GDP1-GDP2);
		fprintf(app_context.config->out_GDP2, "%d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,\n", app_context.period_count, GDP1, GDP2, GDP1 - GDP2, b11, b12, b13,b21, b22,b23, c11, c12, c21, c22,c31,c32);
	}
	void print_table(long long a11, long long a12, long long a13,long long a21, long long a22, long long a23, long long a31,long long a32,long long a33,long long b11, long long b12, long long b13,long long b21, long long b22, long long b23,long long c11, long long c12, long long c21, long long c22,long long c31, long long c32) {
		printf(" start of print_table()\n");	
		fprintf(app_context.config->out_IO_table, "period=,%d,\n", app_context.period_count);
		fprintf(app_context.config->out_IO_table, ",Retailer,Wholesaler,Soleproprietor,中間セクター合計,最終消費,在庫増分,最終需要合計,\n");
		fprintf(app_context.config->out_IO_table, "Retailer,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,\n", a11, a12, a13,a11 + a12+a13, c11, c12, c11 + c12);
		fprintf(app_context.config->out_IO_table, "Wholesaler,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,\n", a21, a22,a23, a21 + a22+a23, c21, c22, c21 + c22);
		fprintf(app_context.config->out_IO_table, "Soleproprietor,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,\n", a31, a32, a33, a31 + a32 + a33, c31, c32, c31 + c32);
		fprintf(app_context.config->out_IO_table, "中間セクター合計,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,%I64d,\n", a11+a21+a31, a12+a22+a32, a13 + a23+a33,a11+a12+a13+a21+a22+a23+a31+a32+a33, c11+c21+c31, c12+c22+c32, c11 + c12+c21+c22+c31+c32);
		fprintf(app_context.config->out_IO_table, "支払給与,%I64d,%I64d,%I64d,%I64d,\n",b11,b12,b13,b11+b12+b13 );
		fprintf(app_context.config->out_IO_table, "当期利益,%I64d,%I64d,%I64d,%I64d,\n",b21,b22,b23,b21+b22+b23);
		fprintf(app_context.config->out_IO_table, "付加価値合計,%I64d,%I64d,%I64d,%I64d,\n",b11+b21, b12+b22,b13+b23, b11 + b12+b13+b21+b22+b23);
		fprintf(app_context.config->out_IO_table, "\n");
		printf("end of print_table()\n");		
	}
	
};