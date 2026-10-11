#pragma once
#include <cstdio>
class ParameterData {
public:
	int init_cash_min = 0;
	int init_cash_max = 0;
	int qmax_min = 0;
	int qmax_max = 0;
	int qmin_min = 0;
	int qmin_max = 0;
	int price_min = 0;//初期価格min
	int price_max = 0;//初期価格max /
	int min_price_min = 0;
	int min_price_max = 0;
	double initial_quantity_ratio = 0.0;//初期生産量の最大生産量比
	double price_delta = 0.0;
	int fixed_wage_min = 0;		//消費者所与給与の下限値	20120523記　高島 / 20120629min_wageから変名　高島
	int fixed_wage_max = 0;		//消費者所与給与の上限値	20120523記　高島 / 20120629max_wageから変名　高島
	double rwd = 0.0;                   //預金引き出し率
	double rpc = 0.0;                   //限界消費性向
	long long min_consumption = 0;//最低消費額	
	double critical_stock_ratio_price = 0.0;//価格変更判断の限界在庫率
	double critical_stock_ratio_quantity = 0.0;//生産量判断の限界在庫率
	int critical_period_for_demand = 0;
	double price_range_ratio = 0.0; // シャッフル時の価格幅係数
	double tax_rate_income = 0.0;
	double tax_rate_corporate = 0.0;
	double market_purchasing_ratio = 0.0;
	double firm_subsidy_ratio = 0.0;
	double beta = 0.0;
	double min_utility_weight = 0.0;
	int case_monopolistic;//競争市場なら=0　独占市場なら=１　AreaMarketParameterで指定する。
	int case_wihtout_expense_initial_deposit_and_cash=1;  //初期現金と預金を消費しない場合は=1　初期現金と預金を消費する場合は=0
	//読み込み用ファイル
	FILE* Parameter_FILE_3;//producer's parameter(生産者共通パラメータ）
	FILE* Parameter_FILE_4;//consumer's parameter
	FILE* Parameter_FILE_5;//retailer's parameter
	FILE* Parameter_FILE_6;//wholesaler's parameter
	FILE* Parameter_FILE_7;//soleproprietor's parameter
	FILE* Parameter_FILE_8;//areamarket parameter
	FILE* Parameter_FILE_9;//government parameter
	//書き込み用ファイル
	FILE* parameter3;//procuer's parameter
	FILE* parameter4;//consumer's paprameter
	FILE* parameter5;//retailer's parameter
	FILE* parameter6;//wholesaler's parameter
	FILE* parameter7;//soleproprietor's parameter
	FILE* parameter8;
	FILE* parameter9;
	ParameterData() {
		Set_parameter();
	}
	virtual void Set_parameter() {};
};