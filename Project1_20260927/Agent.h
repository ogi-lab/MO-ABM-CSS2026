
#pragma once

#include "Item.h"
#include "BookKeeper.h"

#include "Accountant.h"
#include "HR.h"
#include <vector>
extern SimulatorContext app_context;
using namespace std;
/** エージェント */
class Agent {
	protected:	
	BookKeeper* bookKeeper;
	HR* hr;
	int agent_type = 0;    
	     /* = 1 GOVERNMENT
			= 2130  SOLEPROPRIETOR
		    = 2131 RETAILER
	        = 2132  WHOLESALER
			= 2133  EQUIPMENT MAKER
			= 2134  SME
			= 2141  BANK			
			= 31 CONSUMER
			= 310 Soleproprietor_worker
			= 3100 Soleproprietor_exective
			= 311 Retailer_worker
			= 3110 Retailer_exective
			= 312 Wholesaler_worker
			= 3120 Wholesaler_exective
			= 41 GOVERNMENT AGGREGATOR
			= 42　CONSUMER AGGREGATOR
			= 43  RETAILER AGGREGATOR
			= 44  WHOLESALER AGGREGATOR
			= 45  EQUIPMENT AGGREGATOR
			= 46 BANK AGGREGATOR
			= 48 SOLEPROPRIETOR AGGREGATOR
			= 47 ENTERPRISE MANAGER
			= 49 SME AGGREGATOR	
			*/
	std::vector <double> utility_weight;//added by ogibayashi 20210622
	std::vector <int> bought_items_number;
	vector<long long > ave_price_list0;//自分が関わる製品市場にある製品の期首平均製品価格リスト
			//retailer製品市場の場合はvector<int> item_type_idkと順番が対応している
			//wholesaler製品市場の場合はvector<int> item_type_idkwと順番が対応している
	
    int agent_id = 0; /**< エージェントID */
    long long cash = 0;     /**< 現金 */
	long long deposit = 0;  /**<預金*/
	long long initial_cash = 0;
	long long initial_deposit = 0;
	long long Eb = 0; /**Budget for consumption during the buying actions*/
	long long Eb0 = 0; /**Budget for consumption*/
	long long asset_begining = 0;  //期首資産残高
	long long asset_end = 0;       //期末資産残高
	long long income = 0;    /**期の収入*/
	long long previous_income = 0; /**前期の収入*/	
	long long expenses = 0;  /**期の支出*/
	double rwd = 0.0;   //預金引き出し率
	double rpc = 0.0;   //消費性向　propensity to consume
	long long fixed_wage = 0;
	long long wage = 0;
	long long paid_tax_amount = 0;	
	

public:
	//BookKeeper* bookKeeper;
	//Agent(int agent_id, int agent_type): agent_id(agent_id) {
	     Agent(int agent_id, int agent_type){
		this->agent_id=agent_id;this->agent_type=agent_type;
		//printf("agent_id=%d,before set_utility\n",agent_id);
	//	set_utility_weight();
		for (unsigned int i = 0; i < app_context.config->item_type_idk.size(); i++) 	bought_items_number.push_back(0);
	}
    virtual ~Agent(){}
	void set_agent_type(int type){  //Agentをnew する時点で必ず指定する:コンストラクタで指定するようにしたのでこの関数は不要
		this->agent_type=type;
	}
	int get_agent_type() {
		return agent_type;
	}
	virtual void print_min_price() {}
	void set_paid_tax_amount(long long paid_tax_amount) { this->paid_tax_amount = paid_tax_amount; }
	 long long get_paid_tax_amount() { return paid_tax_amount; }
	virtual int get_size_shiped() { return 0; }
	virtual void receive_tax(long long tax_amount) {}
	virtual void receive_tax(long long tax_amount, int agent_id,int agent_type) {}

	//virtual void set_withholdings(long long w) {}
	//virtual long long get_withholdings() { return 0; }

    void set_cash(long long cash) {this->cash = cash;}
	long long get_cash(){return cash;}
	void set_deposit(long long deposit) { this->deposit = deposit; }
	long long get_deposit() { return deposit; }
	void createBookKeeper(){  //Agentaggregatorのsetup()で呼ばれる
		bookKeeper=new BookKeeper(this->agent_id, this->agent_type);
	  fprintf(app_context.config->out3,"agent_id=%d agent_type=%d address=%x にbookKeeperが生成されました、bookKeeperのagent_id=%d agent_type=%d bookKeeperのアドレス=%x,\n"
		  ,agent_id,agent_type,unsigned(this),bookKeeper->get_agent_id(),bookKeeper->get_agent_type(), unsigned (this->bookKeeper));
	
	  
	  
	  app_context.accountant->set_bookKeeper(bookKeeper);//Accountantに各agentのbookKeeperの参照を持たせる
														//各Agentが会計士に決算の依頼をするのに相当する
														//これにより会計士は各Agentの会計データにアクセスできるようになる。
	}
	BookKeeper* get_bookKeeper() {
		return bookKeeper;
	}
	void createHR(){
		hr=new HR(this->agent_id, this->agent_type);
		fprintf(app_context.config->out3,"agent_id=%d agent_type=%d address=%x にHRが生成されました、hrのagent_id=%d agent_type=%d hrのアドレス=%x,\n"
			,agent_id,agent_type,unsigned (this),hr->get_agent_id(),hr->get_agent_type(), unsigned (hr));
		/*
		printf("inside createHR before hr->set_up(), agent_id=%d agent_type=%d address=%x にHRが生成されました、hrのagent_id=%d agent_type=%d hrのアドレス=%x,\n"
			, agent_id, agent_type, unsigned(this), hr->get_agent_id(), hr->get_agent_type(), unsigned(hr));
		*/
		hr->set_up();
		//printf("insid createHR after hr->set_up()\n");
	}
	HR* get_HR() { return hr; }
	void set_utility_weight_random() {
		//item_type_idk: retailer+ soleproprietorが製造するitm_type_idの集合
		//Agentのconstructorで呼ばれるが、consumerのset_upでも呼ばれる。Agentのconstructorで呼ばれるのをコメント化20210627
		double r = 0.0;
	//	printf("inside set_ultility item_type_idk.size()=%d", app_context.config->item_type_idk.size());
		for (unsigned int i = 0; i < app_context.config->item_type_idk.size(); i++) {
			r = RandomGenerator::random_double(0.2, 1.0);//added by ogibayashi 20210622
			utility_weight.push_back(r);
		}
	}
	void set_utility_weight_biased() {
		//item_type_idk: retailer+ soleproprietorが製造するitm_type_idの集合
		//Agentのconstructorで呼ばれるが、consumerのset_upでも呼ばれる。Agentのconstructorで呼ばれるのをコメント化20210627
		double r = 0.0;
		//	printf("inside set_ultility item_type_idk.size()=%d", app_context.config->item_type_idk.size());
		for (unsigned int i = 0; i < app_context.config->item_type_idk.size(); i++) {
			if (i < app_context.config->item_type_id_count_r)
				//r = RandomGenerator::random_double(0.5, 1.0);
				r = RandomGenerator::random_double(app_context.config->retailerParameter->min_utility_weight, 1.0);
			else
				r = RandomGenerator::random_double(0, 1.0);//added by ogibayashi 20210622
			utility_weight.push_back(r);
		}
	}
	void set_utility_weight_biased03() {
		//item_type_idk: retailer+ soleproprietorが製造するitm_type_idの集合
		//Agentのconstructorで呼ばれるが、consumerのset_upでも呼ばれる。Agentのconstructorで呼ばれるのをコメント化20210627
		double r = 0.0;
		//	printf("inside set_ultility item_type_idk.size()=%d", app_context.config->item_type_idk.size());
		for (unsigned int i = 0; i < app_context.config->item_type_idk.size(); i++) {
			if (i < app_context.config->item_type_id_count_r)
				r = RandomGenerator::random_double(app_context.config->retailerParameter->min_utility_weight, 1.0);
			else
				r = RandomGenerator::random_double(0, 1.0);//added by ogibayashi 20210622
			utility_weight.push_back(r);
		}
	}

	void renew_utility_weight() {
		//消費者タイプによって購入製品を区分けするケース、今は使っていない。
		double r = 0.0;
		//utilityの初期化
	/*
	for (unsigned int i = 0; i < utility_weight.size(); i++) {
			utility_weight.pop_back();
		}
	*/
		//utilityの再定義
		if (get_agent_type() == 310) {
			//soleproprietor_worker　retailer生産品種を購入する
			for (unsigned int i = 0; i < app_context.config->item_type_idk.size(); i++) {
				if (i < app_context.config->item_type_idkr.size()) r = RandomGenerator::random_double(0, 1.0);
					//item_type_idkr: retailerが製造するitem_type_idの集合
				else r = 0;
				utility_weight[i] = r;
			}	
		}
		else if (get_agent_type() == 311 || get_agent_type() == 312) {
			//retailer_worker or wholesaler_worker  soleproprietor生産品種を購入する
			for (unsigned int i = 0; i < app_context.config->item_type_idk.size(); i++) {
				if (i < app_context.config->item_type_idkr.size()) r = 0;
				else	r = RandomGenerator::random_double(0, 1.0);
				utility_weight[i] = r;
			}
		}
	}
	/*void reset_bought_items_number() {
		for (unsigned int i = 0; i < app_context.config->item_type_idk.size(); i++) {
			//bought_items_number.push_back(0);
			bought_items_number[i] = 0;
		}
	}*/
	void reset_bought_items_number() {
		bought_items_number.resize(app_context.config->item_type_idk.size(), 0); // サイズを合わせて0埋め
		std::fill(bought_items_number.begin(), bought_items_number.end(), 0);    // 全要素を0に
	}



	int get_agent_id(){return agent_id;}
	void define_agent_type(int type){
		agent_type=type;
	}
	/*Agent* get_agent(int agent_id) {
		Agent* ag;
		if (this->get_agent_id() == agent_id) ag = this;
		return ag;
	}*/
	void print_agent_type() {
		fprintf(app_context.config->out_agent_type, "period=,%d,agent_id=,%d,agent_type=,%d,fixed_wage=,%I64d,wage=,%I64d\n", app_context.period_count, get_agent_id(), get_agent_type(), get_fixed_wage(), get_wage());
	}
	void print_utility_weight() {
		static bool first_call = true;
		if (first_call) {
			// 最初の1回だけ実行したい処理
			std::cout << "This runs only once\n";
			fprintf(app_context.config->out_utility_weight, ",,,,");
						for (unsigned int j = 0; j < app_context.config->item_type_idk.size(); j++) {
							fprintf(app_context.config->out_utility_weight, "item[%d],",j);
						}
						fprintf(app_context.config->out_utility_weight, "\n");
			first_call = false;
		}


		fprintf(app_context.config->out_utility_weight, "agent_no=,%d,agent_type=,%d,", get_agent_id(),get_agent_type());
		for (unsigned int j = 0; j < app_context.config->item_type_idk.size(); j++) {
			//fprintf(app_context.config->out_utility_weight, "utility_weight[%d]=,%7.3f,", j, utility_weight[j]);
			fprintf(app_context.config->out_utility_weight, " % 7.3f, ", utility_weight[j]);
		}
		fprintf(app_context.config->out_utility_weight, "\n");
	}
	virtual void receive_wages(long long wage, int agent_id){};
	/*
	void set_ave_price_list(vector<long long> ave_price_list) {
		int size = ave_price_list.size();
		//vectorの初期化
		if(ave_price_list0.size()>0) for (int i = 0; i < ave_price_list0.size(); i++) 	ave_price_list0.pop_back();
		
		for (int i = 0; i < size; i++) {
			ave_price_list0.push_back(ave_price_list[i]);
			//ave_price_list0[i] = ave_price_list[i];
		}
	}
	*/
	/*
	long long get_ave_price(int item_type_id) {
		long long ave_price = 0;
		if (get_agent_type() == 2131) {
			for (int i = 0; i < app_context.config->item_type_idk.size(); i++) {
				if (item_type_id == app_context.config->item_type_idk[i]) {
					ave_price = ave_price_list0[i];
				}
			}
		}
		else if (get_agent_type() == 2132) {
			for (int i = 0; i < app_context.config->item_type_idkw.size(); i++) {
				if (item_type_id == app_context.config->item_type_idkw[i]) {
					ave_price = ave_price_list0[i];
				}
			}
		}
		return ave_price;
	}
	*/
    /** 初期設定 */
	long long  get_fixed_wage(){return fixed_wage;};
	long long get_wage(){return wage;}
	long long get_depoist(){return deposit;}
	virtual void pay_tax(long long income) {}
	virtual void pay_tax() {}
	
	void set_income(long long income) { this->income = income; }
	long long get_income(){return income;}
	long long get_expenses(){return expenses;}
	long long get_Eb(){return Eb;}
	long long get_Eb0(){return Eb0;}
	
	
	virtual void set_up() = 0;//各最下層クラスで実装している

    /** 期首処理 */
    virtual void do_kishu() = 0;//consumer.h及びenterprise.hで実装している,
	                            //Enterprise.hではother_kishu_action()で支払賃金を決めている。

    /** 行動 */
    virtual void action() = 0;
	   //enterpriseはEnterprise.hで実装（virtualでother_actions())
		//それ以外はConsumer.hで実装

    /** 期末処理 */
    virtual void do_kimatsu() = 0;

	void accounting_treatment_at_kishu(){
		//printf("start of accounting treatment at kishu() agent_type=%d\n", get_agent_type());
		
		//consuemr->do_kishu(), enterprise_do_kisyu()で期首処理の始めに呼ばれる
		bookKeeper->createcashBookSlip(app_context.period_count, 0, 0);//前期繰越用 //income, spending
		bookKeeper->createbankBookSlip(app_context.period_count, 0, 0);//前期繰越用 //depoist, withdrawal
		bookKeeper->createstockBookSlip(app_context.period_count, 0, 0, 0, 0);//前期繰越用//production, bought, item_type_id, price
		bookKeeper->createstockpriceBookSlip(app_context.period_count, 0, 0, 0);//前期繰越用//production_price, bought_price, tiem_type_id
		//printf("inside do_kishu() after create book slips agent_type=%d\n", get_agent_type());
		long long balance = 0;
	//	if (app_context.period_count > 0 ) printf("inside do_kishu()in accounting treatment at kishu()  period=%d agent_id=%d agent_type=%d cash_balance=%I64d\n", app_context.period_count, get_agent_id(),get_agent_type(), bookKeeper->get_cashBookSlipTotal(app_context.period_count - 1)->balance);
	//	if (app_context.period_count > 0 ) printf("inside do_kishu() period=%d agent_id=%d bank_balance=%I64d\n", app_context.period_count, get_agent_id(), bookKeeper->get_bankBookSlipTotal(app_context.period_count - 1)->balance);
		//	if (bookKeeper->get_cashbookslip()->balance < 0) {//cash不足なら預金を引き出す
		//if(app_context.period_count>0) printf("inside accounting treatment at kishu() balance in cash=%I64d\n", bookKeeper->get_cashBookSlipTotal(app_context.period_count - 1)->balance);
		//if(app_context.period_count>0) printf("inside accounting treatment at kishu() balance in bank=%I64d\n", bookKeeper->get_bankBookSlipTotal(app_context.period_count - 1)->balance);
		//printf("inside accounting treatment at kishu() before if statements get_cashBookSlipTotal agent_type=%d\n", get_agent_type());
		if (app_context.period_count > 0 && bookKeeper->get_cashBookSlipTotal(app_context.period_count - 1)->balance < 0) {//現金残高がマイナスの場合
			balance = bookKeeper->get_cashBookSlipTotal(app_context.period_count - 1)->balance;
			//if(app_context.period_count>0 && bookKeeper->get_cashBalance(app_context.period_count-1)<0){
			//	balance = bookKeeper->get_cashBalance(app_context.period_count - 1);
		//		balance = bookKeeper->get_cashbookslip()->balance;
				//while (bookKeeper->get_cashbookslip()->balance-item->get_price() < 0) {
			//printf("inside accounting treatment at kishu() before withdraw_bankdeposit balance=%I64d\n", balance);
			bookKeeper->withdraw_bankdeposit(-balance);
			/*
			bookKeeper->createjournalSlip(app_context.period_count, 0, "現金", -balance, "預金引き出し", -balance, "預金", 0);
			bookKeeper->createcashBookSlip(app_context.period_count, -balance, 0);
			bookKeeper->createbankBookSlip(app_context.period_count, 0, -balance);
			*/
			//}
		}
		//else if (bookKeeper->get_bankbookslip()->balance < 0){//預金不足なら現金を預け入れる
		else if (app_context.period_count > 0 && bookKeeper->get_bankBookSlipTotal(app_context.period_count - 1)->balance < 0) {//預金残高がマイナスの場合
			balance = bookKeeper->get_bankBookSlipTotal(app_context.period_count - 1)->balance;
			//else if (app_context.period_count>0 && bookKeeper->get_bankBalance(app_context.period_count-1) < 0) {
				//balance = bookKeeper->get_bankBalance(app_context.period_count - 1);
				//balance = bookKeeper->get_bankbookslip()->balance;
				//while (bookKeeper->get_cashbookslip()->balance-item->get_price() < 0) {
			bookKeeper->deposit_bankaccount(-balance);
			/*
			bookKeeper->createjournalSlip(app_context.period_count, 0, "預金", -balance, "現金預け入れ", -balance, "現金", 0);
			bookKeeper->createcashBookSlip(app_context.period_count, 0, -balance);
			bookKeeper->createbankBookSlip(app_context.period_count, -balance, 0);
			*/
		}
		//printf("end of accounting treatment at kishu() agent_type=%d\n", get_agent_type());
	}


};

