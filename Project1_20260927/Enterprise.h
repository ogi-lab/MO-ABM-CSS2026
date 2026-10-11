#pragma once
#include "Agent.h"
#include <stdio.h>
#include <stdarg.h>
#include "SimulatorConfig.h"
#include "SimulatorContext.h"
#include "DataAnalyzer.h"
#include "DataAnalyzerNavigator.h"
#include "ConsumerAggregatorNavigator.h"
#include "ConsumerAggregator.h"
#include "GovernmentAggregatorNavigator.h"	
extern SimulatorContext app_context;
// safe fprintf helper: FILE* が nullptr のときは何もしない
static inline void safe_fprintf(FILE* f, const char* fmt, ...) {
	if (!f) return;
	va_list args;
	va_start(args, fmt);
	vfprintf(f, fmt, args);
	va_end(args);
}
/** 企業 */
class Enterprise : public Agent {
protected:
	long long total_sales = 0;
	vector <Agent*> employee_list;
	vector <Agent* > executive_list;
	vector <long long> employee_wage_list;
	vector <long long> executive_wage_list;
	vector <long long> withholdings_employee;
	vector <long long> withholdings_executive;
	//long long withholdings0;
	double tax_rate_income = app_context.config->governmentParameter->tax_rate_income;
	double tax_rate_corporate = app_context.config->governmentParameter->tax_rate_corporate;
	
	virtual void define_ave_price_in_market() {}
	virtual void define_size_of_stock_in_market() {}
	/** 給与支払い */
	//virtual void pay_wages()=0;
    /** 納税 */
	
	void pay_tax_withholdings() {
		fprintf(app_context.config->out_tax_revenue_received, "period=,%d,enterprise_id=,%d,inside pay_tax_withholdings(),\n",
			app_context.period_count, get_agent_id());	
		if (app_context.config->government_count == 0) {
			tax_rate_income = 0; tax_rate_corporate = 0;	
		}
		vector<int> employee_list;
		employee_list = hr->get_employee_list();
		//従業員の源泉徴収税の支払い	
		long long tax_amount_employee=0;
		Agent* ag;	
		ag = app_context.governmentAggregatorNavigator->get_agent_aggregator()->get_agent_list()[0];//政府エージェント
	//	fprintf(app_context.config->outEB_test, "\n,inside  pay_tax_withholdings, bdfore for period=,%d,enterprise_id=,%d,withholdin_employeeg.size()=%d employee_list.size=%d\n",
//			app_context.period_count, get_agent_id(),withholdings_employee.size(), employee_list.size());	
		for (unsigned int i = 0; i < withholdings_employee.size(); i++) {
		//	fprintf(app_context.config->outEB_test, "\n,inside for in  pay_tax_withholdings, period=,%d,enterprise_id=,%d,employee_id=,%d,i=%d withholding_employee=,%I64d \n",
		//		app_context.period_count, get_agent_id(),employee_list[i],i, withholdings_employee[i]);	
			tax_amount_employee += withholdings_employee[i];
		}
			//	if (tax_amount > 0) {
	
		/*従業員の源泉徴収税を政府に支払う*/
			//ag->receive_tax(tax_amount_employee);
		ag->receive_tax(tax_amount_employee,get_agent_id(), get_agent_type());
		//	ag->set_income(tax_amount_employee);//政府の所得に源泉徴収税を加える
			//仕訳
		//	fprintf(app_context.config->outEB_test, "before shiwake, period=,%d,enterprise_id=,%d,income_tax_withheld_paid=,%I64d,\n",
		//		app_context.period_count, get_agent_id(), tax_amount_employee);
		//bookKeeper->createjournalSlip(app_context.period_count, 2, "預り金", long long(double(total_wage) * (tax_rate_income)), "源泉所得税納税", long long(double(total_wage) * (tax_rate_income)), "現金", 0);
		bookKeeper->createjournalSlip(app_context.period_count, 2, "預り金", tax_amount_employee, "源泉所得税納税", tax_amount_employee, "現金", 0);
		//fprintf(app_context.config->outEB_test, "after shiwake, period=,%d,enterprise_id=,%d,income_tax_withheld_paid=,%I64d,\n", app_context.period_count, get_agent_id(), tax_amount_employee);
		// 
	
		//経営者の源泉徴収税の支払い
		//if (get_agent_type() != 2130) {//retailerかwholesalerの場合のみ
			long long tax_amount_executive = 0;
			vector< int > executive_list;
			executive_list = hr->get_executive_list();

			for (unsigned int i = 0; i < withholdings_executive.size(); i++) {

				tax_amount_executive += withholdings_executive[i];
			}
		//	fprintf(app_context.config->out_trace_tax_income, "period=,%d,tax_amount_executive()=,%lld,inside pay_tax_withholdings,\n",app_context.period_count,tax_amount_executive);
			//経営者の源泉徴収税を政府に支払う
			//app_context.governmentAggregatorNavigator->get_agent_aggregator()->get_agent_list()[0]->receive_tax(tax_amount_executive);
			//ag->receive_tax(tax_amount_executive);
			ag->receive_tax(tax_amount_executive, get_agent_id(), get_agent_type());//経営者の源泉徴収税支払い
			//	ag->set_income(tax_amount_executive);//政府の所得に源泉徴収税を加える	
			//仕訳
			//		bookKeeper->createjournalSlip(app_context.period_count, 2, "預り金", long long(double(total_wage) * (tax_rate_income)), "源泉所得税納税", long long(double(total_wage) * (tax_rate_income)), "現金", 0);
			if(get_agent_type()!=2130) bookKeeper->createjournalSlip(app_context.period_count, 2, "預り金", tax_amount_executive, "源泉所得税納税", tax_amount_executive, "現金", 0);
		//}

	} 
	void pay_tax_proprietor() {
		if(app_context.config->government_count==0){
			tax_rate_income=0; tax_rate_corporate=0;		
		}	
		//期末に個人事業主が所得税を支払う
	
		long long tax_amount_proprietor = 0;
		Agent* ag;
		ag = app_context.governmentAggregatorNavigator->get_agent_aggregator()->get_agent_list()[0];//政府エージェント

		long long income = bookKeeper->get_profit();//個人事業主の所得は企業の利益と同じ,個人事業主経営者は一人 
		if (income < 0) income = 0;	
		//各ｴｰｼﾞｪﾝﾄの前期末のprofitの値がincomeとして入っていることを確認した20260122。
		//if (income > 0) {
			//個人事業主の場合は利益に税率をかけて所得税を支払う、利益が正でなければ支払わない		
			tax_amount_proprietor = long long(double(income) * tax_rate_income);
			if (tax_amount_proprietor > 0) {
				//ag->receive_tax(tax_amount_proprietor);//この関数の中にincomeの設定が含まれている
				ag->receive_tax(tax_amount_proprietor, get_agent_id(), get_agent_type());//個人事業主の所得税支払い	
			//	ag->set_income(tax_amount_proprietor);//政府の所得に個人事業主の所得税を加える	

			//	fprintf(app_context.config->out_trace_tax_income, "period=,%d,agent_id=,%d,agent_type()=,%d からagent_type(),=%d, への納税, tax_amount_proprietor=,%lld,income=,%lld,inside pay_tax_proprietor in enterprise.h \n",
			//		app_context.period_count,get_agent_id(),get_agent_type(), ag->get_agent_type(),tax_amount_proprietor,income);
//					app_context.governmentAggregatorNavigator->get_agent_aggregator()->get_agent_list()[0]->receive_tax(tax_amount_proprietor);
				//	fprintf(app_context.config->out_tax, "period=,%d,enterprise_id=,%d,income_tax_paid=,%I64d,income=,%I64d,\n", app_context.period_count, get_agent_id(), tax_amount, income);
			}
		//}
		//else
		//	fprintf(app_context.config->out_trace_tax_income, "income=,%lld,agent_type()=,%d からagent_type(),=%d, への納税, tax_amount_proprietor=,%lld, insidepay_tax_prorprietor else \n", income,get_agent_type(), ag->get_agent_type(), tax_amount_proprietor);
//		ag->receive_tax(tax_amount_proprietor);

		//仕訳は不要、個人事業主の会計帳簿に記録されるべきものだから

		
	/*----------------
		//個人事業主の場合は利益に税率をかけて所得税を支払う
		long long profit = 0;
		Agent* ag;
		vector<int> executive_list = hr->get_executive_list();//agent_idのlist
		for (unsigned int i = 0; i < executive_list.size(); i++) {
		ag = app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(executive_list[i]);
		profit = ag->get_bookKeeper()->get_profit();
		if (profit < 0) profit = 0;
		//printf("inside other_kishu_actions() before set_income and pay_tax for executive agent_id=%d \n", executive_list[i]);
		ag->set_income(profit);//役員給与は利益に応じて決定されるため、ここで所得を設定する
		//printf("inside other_kishu_actions() after set_income before pay_tax for executive agent_id=%d executive_list[%d]=%d agent_type=%d profit=%I64d \n", ag->get_agent_id(),i,executive_list[i],ag->get_agent_type(),profit);
	//	ag->pay_tax(profit);//役員給与に対する所得税の支払い,前期利益を渡す、税金額は役員給与に対して計算される
		//printf("end of pay_tax for executive in other_kishu_action()in Enterprise.h agent_type=%d executive_id=%d\n", get_agent_type(), executive_list[i]);

			}

		}
			//int previous_balance = bookKeeper->get_cashkbookSlip_balance(app_context.period_count);
		bookKeeper->createcashBookSlip(app_context.period_count, 0, total_wage);
	*/

	}



	void pay_tax_corporate() {
		//期末に法人税を支払う
	
		long long profit = bookKeeper->get_profit();
		if (profit > 0) {
			long long tax_amount = 0;
			tax_amount = long long(double(profit) * tax_rate_corporate);
			if (tax_amount > 0) {
				//app_context.governmentAggregatorNavigator->get_agent_aggregator()->get_agent_list()[0]->receive_tax(tax_amount);
				app_context.governmentAggregatorNavigator->get_agent_aggregator()->get_agent_list()[0]->receive_tax(tax_amount, get_agent_id(), get_agent_type());
			//	fprintf(app_context.config->out_tax, "period=,%d,enterprise_id=,%d,corporate_tax_paid=,%I64d,profit=,%I64d,\n", app_context.period_count, get_agent_id(), tax_amount, profit);
			}
		}
	}
    /** 購買 */
    virtual void buy_retails() = 0;
    /** 決算 */
    virtual void kessan() = 0;
	//virtual void other_kishu_actions() = 0;//支払賃金の額の決定（前期の売上高に応じて決める）
	           //純粋仮想関数ではなく、enterpriseの中で実装する方式に変更
	virtual void other_actions() = 0;//給与支払い以外の期中行動,給与支払いは期末
	//virtual void transferstocks_to_StockRecord()=0;
	/**決算以外の期末アクション*/
	virtual void other_kimatsu_actions()=0;
	//virtual void print_boughtItems(int agent_id,int period)=0;
	/*void print_boughtItems(int agent_id,int period){
		get_dataAnalyzer()->print_boughtitems(agent_id,period);
	}*/

	/*DataAnalyzer* get_dataAnalyzer(){
		DataAnalyzerNavigator* data_navi=app_context.dataAnalyzer_navi_for_retailMarket;
		DataAnalyzer* dataAnalyzer=data_navi->get_dataAnalyzer();
		return dataAnalyzer;
	}*/
	long long get_profit(){
		long long profit = 0;
		if (app_context.period_count > 0) {
			return bookKeeper->get_profit();
			if (profit < 0) profit = 0;					
		}
		else profit = 0;
		return profit;
	}	

	void other_kishu_actions() {
		//enterprise.hで純粋仮想関数宣言ではない。Enterprice.hで実装（この場所）
		//Enterprise.hのdo_kishu()で呼ばれる
		//profitは給与支払い額決定用（給与＝固定給＋利益に比例した増分）
		//fprintf(app_context.config->out_trace_tax_income, " period=,%d,agent_id=,%d,profit=,%lld,inside other_kisyu_action before define_wages,\n"
	//		, app_context.period_count, get_agent_id(),bookKeeper->get_profit());
		long long total_sales = 0;
		if (app_context.period_count > 0) total_sales = bookKeeper->get_totalsales(app_context.period_count - 1);
		else total_sales = 0;
		//printf("inside other_kishu_actions() before define employee_wage_list profit=%d \n", profit);
		define_employee_wage_list(get_profit());
		//printf("inside other_kishu_actions() before define executive_wage_list \n");
		define_executive_wage_list(get_profit());
		//printf("end of define executive_wage_list in other_kishu_action()in Enterprise.h agent_type=%d\n", get_agent_type());
		/*個人事業主の場合、profitをincomeとして設定し、所得税を支払う*/
			//普通agentの場合はEnterprise-do_kimatsu()のpay_wages()でincomeを設定する


		//この部分はEnterpiseの期末に持ってこなければいけない
	
		//if(get_agent_type()==2130) printf("end of other_kisyu_actions()in Enterprise.h  agent_type=%d\n", get_agent_type());
		 
///---------------
	}
	void define_employee_wage_list(long long previous_profit) {
		//税引き前の給与を定義する。税金支払は期末に行う
		// 
		//Enterprise-＞other_kishu_actions()で呼ばれる
		double tax_rate_income= app_context.config->governmentParameter->tax_rate_income;
		double tax_rate_corporate=app_context.config->governmentParameter->tax_rate_corporate;
		if(app_context.config->government_count==0){
			tax_rate_income=0; tax_rate_corporate=0;		
		}	
		//enterprise->kishu()->other_kishu_actions()の中で呼ばれる
		// 
		//if (employee_wage_list.size() > 0) employee_wage_list.clear();
		employee_wage_list.clear();
		withholdings_employee.clear();
		double rw_max = 0.07;
		double rw_min = 0.05;
		double rate = 0.0;
		
		long long wage = 0;
		long long fixed_wage = 0;

		vector<int> employee_list;
		employee_list = hr->get_employee_list();
		vector<int> executive_list;
		executive_list = hr->get_executive_list();//agent_idのlist
		//if (employee_list.size() == 0) printf("agent_type=%d agent_id=%d employee.list.size()=%d \n", get_agent_type(), get_agent_id(), employee_list.size());
		Agent* ag;

		for (unsigned int i = 0; i < employee_list.size(); i++) {
			rate = RandomGenerator::random_double(rw_min, rw_max);//	ボーナス率
			int employee_id = 0;
			ag = app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(employee_list[i]);
		//	fprintf(app_context.config->out3, "address of ag= %x, address of employee_list[%d]=%x,\n", unsigned(ag), i, unsigned(employee_list[i]));
			fixed_wage = ag->get_fixed_wage();
			//	if (app_context.period_count == 0) fixed_wage = long long(double (fixed_wage) / 4);
				//fixed_wage=app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(employee_list[i]->get_agent_id())->get_fixed_wage();
				//employee_id=app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(employee_list[i]->get_agent_id())->get_agent_id();
			employee_id = employee_list[i];
			//printf("inside define_employee_wage_list after fixed_wage=ag->get_fixed_wage() agent_type=%d employee_id=%d fixed_wage=%d \n", get_agent_type(), employee_list[i], fixed_wage);	

			//wageの決定（最低賃金は固定給とし、利益が出ていれば利益に比例して増加させる）	
			long long profit_per_capita = 0;
			profit_per_capita = long long(double(previous_profit) / double(employee_list.size() + executive_list.size()));
			if (app_context.config->ncase != 0) {
				if (app_context.period_count == 0)wage = fixed_wage;
				else {
					//if (profit_per_capita <= fixed_wage) wage = long long(double(profit_per_capita) * 0.8);
					wage = fixed_wage + long long(double(profit_per_capita - fixed_wage) * rate);
				}
			}
			else { //app_context.config->ncase == 0のときは、利益に関係なく固定給のみを支払う	
				wage = fixed_wage;
			}

		//	if (previous_profit < 0) wage = fixed_wage;
			//else wage = fixed_wage + long long (double(previous_profit)/double(employee_list.size()+executive_list.size()) * rate);
			
			//wageをvectorに格納しておく。	
			employee_wage_list.push_back(wage);

		//	printf("i=%d wage=%I64d,agent_id=%d agent_type=%d employee_wage_list.size()=%d\n", i, wage, ag->get_agent_id(), ag->get_agent_type(), employee_wage_list.size());
			bookKeeper->createjournalSlip(app_context.period_count, 1, "直接労務費", wage, "製品生産", wage, "未払給与", 2);
			
			if (app_context.config->government_count>0 && tax_rate_income > 0) {
				bookKeeper->createjournalSlip(app_context.period_count, 2, "未払給与", long long(double(wage) * tax_rate_income), "源泉所得税", long long(double(wage) * tax_rate_income), "預り金", 2);
					withholdings_employee.push_back(long long(double(wage) * tax_rate_income));
						//預り金をvectorに格納しておく。	
				//	fprintf(app_context.config->outEB_test, "before withholdings, period=,%d,enterprise_id=,%d,employee_id=,%d,withholdings_employe.size()=%d withholdings paid =%I64d, wege=,%I64d,tax_rate_income=%f \n",
				//			app_context.period_count, get_agent_id(), employee_id, withholdings_employee.size(),long long(double(wage) * tax_rate_income), wage, tax_rate_income);

						fprintf(app_context.config->out_wage_test1, "period=,%d,employee_no.=,%d, agent_id=,%d,agent_type=,%d,fixed_wage=,%I64d,wage=,%I64d,previous_profit=,%I64d,\n",
					app_context.period_count, i, get_agent_id(), get_agent_type(), fixed_wage, wage, previous_profit);
			}
			
		}
		hr->set_employee_wage_list(employee_wage_list);
	//	if(get_agent_type()!=2130)printf("inside enterprise define_employee_wage_list fter set->employee_wage_list , agent_id=%d agent_type=%d hr->get_employee_wage_list.size()=%d\n",get_agent_id(),get_agent_type(), (hr->get_employee_wage_list()).size());
	}
	void define_executive_wage_list(long long previous_profit) {

		//Enterprise-＞other_kishu_actions()で呼ばれる
		//printf("inside define_executive_wage_list before double tax_rate_income previous_profit=%d \n", previous_profit);
		double tax_rate_income = app_context.config->governmentParameter->tax_rate_income;
		//printf("inside define_executive_wage_list after double tax_rate_income \n");
		double tax_rate_corporate = app_context.config->governmentParameter->tax_rate_corporate;
		if (app_context.config->government_count ==0 ) 
		{
			tax_rate_income = 0; tax_rate_corporate = 0;	
		}
		//printf("inside define_executive_wage_list after double tax_rate_corporate \n");
		//enterprise->kishu()->other_kishu_actions()の中で呼ばれる	
		vector<int> executive_list;
		executive_list = hr->get_executive_list();//agent_idのlist
		vector<int> employee_list;
		employee_list = hr->get_employee_list();
		double rw_max = 0.2;
		double rw_min = 0.3;
		//double rate = RandomGenerator::random_double(rw_min, rw_max);
		double rate = 0.2;
		long long wage = 0;
		long long fixed_wage = 0;
		executive_wage_list.clear();
		withholdings_executive.clear();
	    //app_context.withholdings0 = 0;		
		/*  経営者報酬の決定　期首に実施、period=0ではwage=fixed=wage　*/
	//	if (executive_list.size() == 0) printf("agent_type=%d agent_id=%d executive_list.size()=%d \n", get_agent_type(), get_agent_id(), executive_list.size());
		Agent* ag;
		for (unsigned int i = 0; i < executive_list.size(); i++) {
			int executive_id = 0;
			ag = app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(executive_list[i]);
			fixed_wage = ag->get_fixed_wage();
			//printf("inside define_executive_wage_list after fixed_wage=ag->get_fixed_wage() agent_type=%d executive_id=%d fixed_wage=%d \n", get_agent_type(), executive_list[i], fixed_wage);
		//	if (app_context.period_count == 0) fixed_wage = long long(double(fixed_wage)/4.0);
			executive_id = executive_list[i];
			long long profit_per_capita = 0;
			long long wage1 = 0;
			profit_per_capita = long long(double(previous_profit) / double(employee_list.size() + executive_list.size()));
			//経営者報酬の額の決定と源泉所得税額の格納
			if (get_agent_type() != 2130) {
				if (app_context.config->ncase != 0) {
					if (app_context.period_count == 0) {
						wage = fixed_wage;
						app_context.withholdings0 += long long(double(wage) * tax_rate_income);
					}

					else wage = fixed_wage + long long(double(profit_per_capita - fixed_wage) * rate);
				}
				else wage = fixed_wage;	//app_context.config->ncase == 0のときは、利益に関係なく固定給のみを支払う	

				//	if (profit_per_capita - fixed_wage < 0) wage = fixed_wage;
			//	else wage =long long (double (profit_per_capita - fixed_wage) * rate) + fixed_wage;
			}
			else {
			//個人事業主の場合は所得税引き後の利益が収入、自分の所得は経費にならないので仕訳は行わない	
			//	double rate1 = app_context.config->consumerParameter->rpc;
				if (app_context.period_count == 0) {
					wage = fixed_wage;
					app_context.withholdings0 += long long(double(wage) * tax_rate_income);

					fprintf(app_context.config->out_test7, "inside define_executive_wage_list period=,%d,enterprise_id=,%d,executive_id=,%d, fixed_wage=,%lld,wage=,%lld,withholding0=,%lld, \n"
						, app_context.period_count, get_agent_id(), executive_id, fixed_wage, wage,app_context.withholdings0);
				}
				else if (previous_profit < 0) wage = fixed_wage;//利益がマイナスの場合でも固定給相当額は預金引き出しで賄う		
				//else wage = long long(double (profit_per_capita)*(1-tax_rate_income));
				else wage = long long(double(profit_per_capita));
			}
			
			wage1 = long long(double(wage) * (1 - tax_rate_income));
			ag->set_paid_tax_amount(wage-wage1);//役員給与に対する源泉徴収税額を設定する	
			//if(app_context.period_count==0) ag->set_income(wage1);
			//else ag->set_income(wage1);//役員給与を所得として設定する	
			//printf("inside define_executive_wage_list before push_back agent_type=%d executive_id=%d wage=%d \n", get_agent_type(), executive_id, wage);
			//wage経営者報酬をvectorに格納しておく。	
			executive_wage_list.push_back(wage);
			//預り金をvectorに格納しておく。	
			withholdings_executive.push_back(long long(double(wage) * tax_rate_income));
			//fprintf(app_context.config->out_Eb, "after executive_wage_list_executive.push_back, period=,%d,executive_id=,%d,enterprise_id=,%d,wage=,%lld,wage1=,%lld,income=%f \n"
			//	, app_context.period_count, executive_id,get_agent_id(), wage, wage1,income);
		//	printf("i=%d wage=%I64d,agent_id=%d agent_type=%d executive_wage_list.size()=%d\n", i, wage, ag->get_agent_id(), ag->get_agent_type(), executive_wage_list.size());

			//printf("inside define_executive_wage_list after push_back agent_type=%d executive_id=%d wage=%d \n", get_agent_type(), executive_id, wage);
			//直接労務費としての仕訳はしない		
			if (get_agent_type() != 2130) {
				bookKeeper->createjournalSlip(app_context.period_count, 1, "役員報酬", wage, "製品生産", wage, "未払給与", 2);
				fprintf(app_context.config->out_wage_test1, "period=,%d,employee_no.=,%d, agent_id=,%d,agent_type=,%d,fixed_wage=,%I64d,wage=,%I64d,previous_profit=,%I64d,\n",
					app_context.period_count, i, get_agent_id(), get_agent_type(), fixed_wage, wage, previous_profit);
				//	printf("inside define_executive_wage_list before createjournalSlip agent_type=%d executive_id=%d wage=%d \n", get_agent_type(), executive_id, wage);
				if(app_context.config->government_count>0 && tax_rate_income>0) {
					bookKeeper->createjournalSlip(app_context.period_count, 2, "未払給与", long long(double(wage) * tax_rate_income), "源泉所得税", long long(double(wage) * tax_rate_income), "預り金", 2);
					
				}	
				//預り金をvectorに格納しておく。
				//else withholdings_executive.push_back(long long(double(wage) * tax_rate_income));	
			//	fprintf(app_context.config->outEB_test, "after whitholdings_executive.push_back, period=,%d,enterprise_id=,%d,executive_id=,%d,withholdings_executive.size()=%d withholdings paid =%I64d, wege=,%I64d,tax_rate_income=%f \n",
			//		app_context.period_count, get_agent_id(), executive_id, withholdings_executive.size(), long long(double(wage) * tax_rate_income), wage, tax_rate_income);
			//	printf("inside define_executive_wage_list after createjournalSlip agent_type=%d executive_id=%d wage=%d \n", get_agent_type(), executive_id, wage);
			}
			else {
				//個人事業主の場合は利益が収入、自分の所得は経費にならないので仕訳は行わない	
				/*
				if (previous_profit - fixed_wage < 0) wage = fixed_wage;
				else wage = long long((previous_profit - fixed_wage) * rate) + fixed_wage;
				//printf("inside define_executive_wage_list before push_back agent_type=%d executive_id=%d wage=%d \n", get_agent_type(), executive_id, wage);
				//wage経営者報酬をvectorに格納しておく。	
				executive_wage_list.push_back(wage);
				*/
			}

		}//forの終わりr
		//printf("inside define_executive_wage_list before hr->set_executive_wage_list agent_type=%d \n", get_agent_type());
		hr->set_executive_wage_list(executive_wage_list);
	//	if(get_agent_type()!=2130)printf("inside define_executive_wage_list after hr->set_executive_wage_list agent_type=%d executive_wage_list.size()=%d \n", get_agent_type(),(hr->get_executive_wage_list()).size());	
		//print_executive_wage_list();
		//app_context.config->set_withholdings0(withholdings0);
		for (unsigned int ij = 0; ij < executive_wage_list.size(); ij++) {
			fprintf(app_context.config->out_executive_wage_list, "period=,%d, size=,%d,agent_id=,%d,agent_type=,%d,executive_id=,%d,executive_wage=,%I64d,profit=,%I64d,\n",
				app_context.period_count, executive_wage_list.size(),get_agent_id(), get_agent_type(),executive_list[ij], executive_wage_list[ij], previous_profit);
		}
		for(unsigned int ij = 0; ij < withholdings_executive.size(); ij++) {
			fprintf(app_context.config->out_withholding_tax_executive, "period=,%d, size=,%d,enterprise_id=,%d,agent_type=,%d,executive_id.=,%d,withholdings_executive=,%I64d,profit=,%I64d,\n",
				app_context.period_count, withholdings_executive.size(), get_agent_id(), get_agent_type(), executive_list[ij], withholdings_executive[ij], previous_profit);
		}
	}
	void print_executive_wage_list() {
		vector<long long> executive_wage_list;
		executive_wage_list = hr->get_executive_wage_list();
		for (unsigned int ij = 0; ij < executive_wage_list.size(); ij++) {
			fprintf(app_context.config->out_executive_wage_list, "period=,%d, agent_id=,%d,agent_type=,%d,executive_id=,%d,executive_wage=,%I64d,\n",
				app_context.period_count,get_agent_id(), get_agent_type(), executive_list[ij], executive_wage_list[ij]);
		}
	}
	/** 給与支払い */
	void pay_wages() {
		double tax_rate_income = app_context.config->governmentParameter->tax_rate_income;
		double tax_rate_corporate = app_context.config->governmentParameter->tax_rate_corporate;
		if (app_context.config->government_count == 0) { tax_rate_income = 0; tax_rate_corporate = 0; }
		//Enterpriseのdo_kimatsu()で呼ばれる
		//employee_list（Enterpriseで定義してある）記載の各自に給与を支払う　
		//total_sales(enterpriseで定義してある）の一定割合とする。
	//	printf("start of pay_wages agent_type=%d employee_wage_list.size=%d \n", get_agent_type(),(hr->get_employee_wage_list()).size());

		//employeeへの給与支払い
		vector <int> employee_list;
		employee_list = hr->get_employee_list();
		//employee_wage_list = hr->get_employee_wage_list();
	//	printf("before for in pay_wages agent_id=%d agent_type=%d employee_list.size()=%d, employee_wage_list.size()=%d hr->get_emloyee_wage_list.size()=%d \n", get_agent_id(),get_agent_type(),employee_list.size(), employee_wage_list.size(),(hr->get_employee_wage_list()).size());
		long long total_wage = 0;//全従業員への給与支払い合計額
		long long wage = 0, wage1 = 0;
		Agent* ag;
		if (employee_list.size() >0){
			for (unsigned int i = 0; i < employee_list.size(); i++) {
				wage = employee_wage_list[i];//enterpriseの属性変数、define_employee_wage_listで期首に定義済み、それを参照する。
				wage1 = long long(double(wage) * (1 - tax_rate_income));//源泉所得税の控除後の給与支払額
				if (tax_rate_income == 0) wage1 = wage;	
				int employee_id = employee_list[i];
				total_wage += wage1;
				ag = app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(employee_list[i]);
		//		fprintf(app_context.config->out3, "inside pay_wages in Enterprise.h  address of ag= %x, address of employee_list[%d]=%x,employer_id=%d, employer_type=%d\n", unsigned(ag), i, unsigned(employee_list[i]), get_agent_id(), get_agent_type());
				ag->receive_wages(wage1, this->agent_id);//各従業員への給与支払い this->agent_idはemployerのagent_id
				//仕訳はtotal_wageでまとめて行う	
				//ag->set_income(wage1);//従業員給与を所得として設定する
				//app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(employee_list[i]->get_agent_id())->receive_wages(wage);			
				//cash -= wage;

				//if(app_context.period_count>0) balance=bookKeeper->get_totalbalance(app_context.period_count-1);
				//balance = bookKeeper->get_cashbookslip()->balance;
				//bookKeeper->createjournalSlip(app_context.period_count, 1, "固定給与", wage, "固定給支払", wage, "現金", 0);

			}

		if (bookKeeper->get_cashbookslip()->balance > total_wage || bookKeeper->get_bankbookslip()->balance > total_wage) {
			//給与支払いは現金残高又は預金残高がtotal_wageより大きい場合に限る。
			//もし現金残高がtotal_wageより少ない場合は預金を引き出す
			if (bookKeeper->get_cashbookslip()->balance < total_wage) bookKeeper->withdraw_bankdeposit(total_wage);

			//bookKeeper->createjournalSlip(app_context.period_count, 2, "未払給与", total_wage, "固定給支払", total_wage, "現金", 0);
			fprintf(app_context.config->out_wage_test1, "period=,%d, agent_id=,%d,agent_type=,%d,,,,,total_,wage=,%I64d,\n",
				app_context.period_count, get_agent_id(), get_agent_type(), total_wage);
			if(app_context.config->government_count!=0 && tax_rate_income>0)
			bookKeeper->createjournalSlip(app_context.period_count, 2, "未払給与", long long(double(total_wage) * (1 - tax_rate_income)), "給与支払", long long(double(total_wage) * (1 - tax_rate_income)), "現金", 0);
			else 
				bookKeeper->createjournalSlip(app_context.period_count, 2, "未払給与", total_wage, "給与支払", total_wage, "現金", 0);

			//	bookKeeper->createjournalSlip(app_context.period_count, 2, "預り金", long long(double(total_wage) * (tax_rate_income)), "源泉所得税納税", long long(double(total_wage) * (tax_rate_income)), "現金", 0);
				//app_context.governmentAggregatorNavigator->get_agent_aggregator()->get_agent_list()[0]->receive_tax(long long(double(total_wage) * (tax_rate_income)));
				//app_context.government_aggregator->get_agent_list()[0]->receive_tax(long long(double(total_wage) * (tax_rate_income)));
				//int previous_balance = bookKeeper->get_cashkbookSlip_balance(app_context.period_count);
			bookKeeper->createcashBookSlip(app_context.period_count, 0, total_wage);
		}
	}//end of if (employee_list.size() != 0) 

	//	printf("inside pay_wage before executive\n");
		
		//executiveへの給与支払い
		// 個人事業主以外は、executive_wage_listに基づき決められた額を支払う、define_executive_wage_listで期首に定義済み、それを参照する。
		//個人事業主は決算後の利益に基づき
		vector <int> executive_list;
		executive_list = hr->get_executive_list();
		//executive_wage_list = hr->get_executive_wage_list();
	//	printf("executive_list.size()=%d\n", executive_list.size());
		total_wage = 0;//全役員への給与支払い合計額
		/*
		fprintf(app_context.config->out_trace_buy, "period=,%d,consumer_id=,%d,enterprise_id=,%d,executive_list.size()=,%d,inside pay_wages\n",
		             app_context.period_count,executive_list[0],get_agent_id(),executive_list.size());
		*/
					 /*
		if (get_agent_type() == 2130){
			//個人事業主の場合、利益に基づき決定された額を支払う
			long long profit = 0;

			for (unsigned int i = 0; i < executive_list.size(); i++) {
				ag = app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(executive_list[i]);
				profit =bookKeeper->get_profit();
				if (profit < 0) profit = 0;
				wage = profit;
				wage1 = long long(double(wage) * (1 - tax_rate_income));	
				//printf("inside pay_wages() before set_income and receive_wages for executive agent_id=%d \n", executive_list[i]);
				ag->receive_wages(wage1, this->agent_id);//個人事業主給与の支払い,税引き後利益を渡す
				//printf("end of receive_wages for executive in pay_wages()in Enterprise.h agent_type=%d executive_id=%d\n", get_agent_type(), executive_list[i]);
				ag->set_income(wage1);//役員給与は利益に応じて決定されるため、ここで所得を設定する
				//printf("inside pay_wages() after set_income before receive_wages for executive agent_id=%d executive_list[%d]=%d agent_type=%d profit=%I64d \n", ag->get_agent_id(),i,executive_list[i],ag->get_agent_type(),profit);
				//仕訳はしない。
			}

		}
		*/
		//else {
		//経営者報酬の支払い	個人事業かどうかに関係なく、以下のロジックで支払う
			for (unsigned int i = 0; i < executive_list.size(); i++) {
				//printf("inside for i=%d before wage=\n", i);
				wage = executive_wage_list[i];//vectorで宣言、define_executive_wage_listで定義
				wage1 = long long(double(wage) * (1 - tax_rate_income));
				fprintf(app_context.config->out_trace_government, " period=,%d,executive_id=,%d,enterprise_id=,%d, wage=,%I64d,wage1=,%lld,wage-wage1=,%lld,tax_rate_income=,%f,get_income()=,%lld,inside_pay_wage_executives, \n",
					app_context.period_count, executive_list[i], get_agent_id(), wage, wage1,wage-wage1,tax_rate_income, get_income());	
				//printf("before exective_id after wage=%d \n",wage);
				int executive_id = executive_list[i];
				//	printf("inside for  in paywage for executive i=%d executive_id=%d",i, executive_id);
				total_wage += wage1;
				ag = app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(executive_list[i]);
//				fprintf(app_context.config->out3, "inside pay_wages in Enterprise.h  address of ag= %x, address of executive_list[%d]=%x,executive_id=%d, executive_type=%d\n", unsigned(ag), i, unsigned(executive_list[i]), get_agent_id(), get_agent_type());
				/*
				fprintf(app_context.config->out_trace_buy, " period=,%d,executive_id=,%d,enterprise_id=,%d, wage1=,%I64d,tax_rate_income=%f,get_income()=,%lld,inside_pay_wages, \n",
					app_context.period_count, executive_list[i], get_agent_id(), wage1, tax_rate_income, get_income());
				*/
//	fprintf(app_context.config->out_Eb, " inside pay wage,period=,%d,executive_id=,%d,enterprise_id=,%d, wage1=,%I64d,tax_rate_income=%f,get_income()=,%lld,inside_pay_wages, \n",
//					app_context.period_count, executive_list[i], get_agent_id(), wage1, tax_rate_income, get_income());
				ag->receive_wages(wage1, this->agent_id);//各executiveへの給与支払い額 this->agent_idはemployerのagent_id
				//ag->set_income(wage1);//役員給与を所得として設定する
			//	fprintf(app_context.config->outEB_test, "after receive_wages for executive , period=,%d,enterprise_id=,%d,executive_id=,%d, wege=,%I64d,tax_rate_income=%f \n",
			//		app_context.period_count, get_agent_id(), executive_id, wage, tax_rate_income);
			//	if (ag->get_agent_id() == 10) fprintf(app_context.config->out_test0, "just after receive_wages for executive , period=,%d,enterprise_id=,%d,executive_id=,%d,agent_id=,%d, agent_type()=,%d,get_income()=,%I64d,wage=,%lld,fixed_wage=,%lld,tax_rate_income=%f \n",
				//	app_context.period_count, get_agent_id(), executive_id, ag->get_agent_id(), ag->get_agent_type(), ag->get_income(), wage, ag->get_fixed_wage(), tax_rate_income);
				//app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(employee_list[i]->get_agent_id())->receive_wages(wage);			
				//cash -= wage;

				//if(app_context.period_count>0) balance=bookKeeper->get_totalbalance(app_context.period_count-1);
				//balance = bookKeeper->get_cashbookslip()->balance;
				//bookKeeper->createjournalSlip(app_context.period_count, 1, "固定給与", wage, "固定給支払", wage, "現金", 0);

			}
		//}
	//	printf("after for inside pay_wage for executive\n");
		if (bookKeeper->get_cashbookslip()->balance > total_wage || bookKeeper->get_bankbookslip()->balance > total_wage) {
			//給与支払いは現金残高又は預金残高がtotal_wageより大きい場合に限る。
			//もし現金残高がtotal_wageより少ない場合は預金を引き出す
			if (bookKeeper->get_cashbookslip()->balance < total_wage) bookKeeper->withdraw_bankdeposit(total_wage);
			if (get_agent_type() != 2130) {
				if (app_context.config->government_count==0 || tax_rate_income == 0) {
					bookKeeper->createjournalSlip(app_context.period_count, 2, "未払給与", total_wage, "固定給支払", total_wage, "現金", 0);
					fprintf(app_context.config->out_wage_test1, "period=,%d, agent_id=,%d,agent_type=,%d,,,,,total_,wage=,%I64d,\n",
						app_context.period_count, get_agent_id(), get_agent_type(), total_wage);
				}
				else
					//源泉所得税控除がある場合　政府ありかつ所得税率>0の場合
				bookKeeper->createjournalSlip(app_context.period_count, 2, "未払給与", long long(double(total_wage) * (1 - tax_rate_income)), "役員報酬支払", long long(double(total_wage) * (1 - tax_rate_income)), "現金", 0);
				//個人事業主は所得に対する仕訳は行わない
		//		bookKeeper->createjournalSlip(app_context.period_count, 2, "預り金", long long(double(total_wage) * (tax_rate_income)), "源泉所得税納税", long long(double(total_wage) * (tax_rate_income)), "現金", 0);
				//app_context.government_aggregator->get_agent_list()[0]->receive_tax(long long(double(total_wage) * (tax_rate_income)));
				//所得税を支払う
				//app_context.governmentAggregatorNavigator->get_agent_aggregator()->get_agent_list()[0]->receive_tax(long long(double(total_wage) * (tax_rate_income)));
			}
			else {
				
				//個人事業主の場合は仕訳をしない
			//	long long profit;
				//	Agent* ag;
					vector<int> executive_list = hr->get_executive_list();//agent_idのlist
					for (unsigned int i = 0; i < executive_list.size(); i++) {
						
						ag = app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(executive_list[i]);
						//profit = ag->get_bookKeeper()->get_profit();
						//if (profit < 0) profit = 0;
						
						//printf("inside other_kishu_actions() before set_income and pay_tax for executive agent_id=%d \n", executive_list[i]);
					//	ag->set_income(profit);//役員給与は利益に応じて決定されるため、ここで所得を設定する
						//printf("inside other_kishu_actions() after set_income before pay_tax for executive agent_id=%d executive_list[%d]=%d agent_type=%d profit=%I64d \n", ag->get_agent_id(),i,executive_list[i],ag->get_agent_type(),profit);
					//	ag->pay_tax(profit);//役員給与に対する所得税の支払い,前期利益を渡す、税金額は役員給与に対して計算される
						//printf("end of pay_tax for executive in other_kishu_action()in Enterprise.h agent_type=%d executive_id=%d\n", get_agent_type(), executive_list[i]);	
					//	if(ag->get_agent_id()==10) fprintf(app_context.config->out_test0, "after receive_wages for executive , period=,%d,enterprise_id=,%d,executive_id=,%d,agent_id=,%d, agent_type()=,%d,get_income()=,%I64d,wage=,%lld,fixed_wage=,%lld,tax_rate_income=%f \n",
						//	app_context.period_count, get_agent_id(), executive_list[i], ag->get_agent_id(), ag->get_agent_type(), ag->get_income(), wage, ag->get_fixed_wage(), tax_rate_income);

					}
					
			}
			//int previous_balance = bookKeeper->get_cashkbookSlip_balance(app_context.period_count);
			bookKeeper->createcashBookSlip(app_context.period_count, 0, total_wage);

		}

	}
	
public:
     Enterprise(int agent_id,int agent_type) : Agent(agent_id,agent_type) {}
    virtual ~Enterprise() {};
	/*納税*/
	void pay_tax() {
		if (app_context.config->government_count != 0) {
	//		fprintf(app_context.config->out_trace_tax_income, "period=,%d,agent_id=,%d, agent_type=,%d,profit=,%lld,inside pay_tax at kimatsu() in enterprose.h, \n"
		//		, app_context.period_count, get_agent_id(), get_agent_type(), bookKeeper->get_profit());
			pay_tax_withholdings();
			//pay_tax_proprietor();
			// pay_tax_corporate();
		}
		else {}
	}

   // virtual void set_up() {};
/*	void hire_employee(int agent_id) {
		Agent* ag;
		ag=app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(agent_id);
		employee_list.push_back(ag);
		fprintf(app_context.config->out_agent_address,",retailer_id=,%d, employee_id=,%d,employee_address=,%x,\n",get_agent_id(),ag->get_agent_id(),ag);
		//employee_list.push_back(app_context.consumerAggregatorNavigator->get_agent_aggregator()->get_agent(agent_id));
	}
	*/
    void do_kishu() {
	//	if (agent_type == 2130) printf("starting inside kishu()\n");
		//printf("inside do_kishu() in Enterprise.h agent_type=%d agent_id=%d\n", agent_type, get_agent_id());
		//**********************************
		if (agent_type == 2131 || agent_type == 2132 || agent_type==2130) {
			if (app_context.period_count > 0) {
				if (app_context.period_count == 21) fprintf(app_context.config->out_test12, "before define_ave_price_in_market at kishu() , agent_id=,%d,agent_type=,%d,\n", get_agent_id(), get_agent_type());
				define_ave_price_in_market();//Enterprise.hでvirtualに宣言している.
											//実装はproducer.h
				if (app_context.period_count == 21) fprintf(app_context.config->out_test12, "after define_ave_price_in_market at kishu() , agent_id=,%d,agent_type=,%d,\n", get_agent_id(), get_agent_type());
				define_size_of_stock_in_market();
			}
		}
		// printf("inside kisyu() before accounting treatment at kisyu()agent_type=%d\n",agent_type);
		accounting_treatment_at_kishu();//agent.hで関数定義
	 //*************************************
       //printf("inside do_kishu() in Enterprise.h after accounting treatement _at _kishu() agent_id=%d period=%d do_kishu -- enterprise %d\n",get_agent_id(),app_context.period_count,agent_id);
	//	fprintf(app_context.config->out,"do_kishu -- enterprise %d\n",agent_id);
	//	if(app_context.period_count>0) print_boughtItems(this->agent_id,app_context.period_count-1);
		//pay_tax();//期末に移動する
		// if(get_agent_type() == 2130) printf("before other_kishu_actions() in Enterprise agent_type=%d\n",agent_type);
		other_kishu_actions();//支払賃金を決める
		//if (get_agent_type() == 2130) printf("inside do_kishu() in Enterprise.h\n");
		//fprintf(app_context.config->out_test0,"end of do_kishu() in Enterprise.h, period=,%d,agent_type=%d agent_id=%d\n", app_context.period_count,agent_type, get_agent_id());
	}
	
	void action(){
		//if (get_agent_type() == 2130)printf("inside action in Enterprze before other_actions() agent_id=%d\n",get_agent_id());
		other_actions();
		//if(get_agent_type()==2130) printf("inside action in Enterprze after other_actions() agent_id=%d\n",get_agent_id());
	};


   void do_kimatsu() {
		//print_boughtItems(this->agent_id);
     //   if(get_agent_type()==2132) printf("do_kimatsu pay_wages -- enterprise\n");
	  // fprintf(app_context.config->outEB_test,"inside do_kimatsu in enterprise, before pay_wage agent_id=%d employee_list.size()=%d employee_wage_list.size()=%d\n",get_agent_id(),(hr->get_employee_list()).size(),(hr->get_employee_wage_list()).size());
	   kessan();
	 /*賃金支払*/
	   pay_wages();
	 //  printf("inside do_kimatsu in enterprise, after pay_wages,before kessan agent_id=%d \n", get_agent_id());
	  // if (get_agent_type() == 2132) printf("after pay_wages\n");
	  // fprintf(app_context.config->outEB_test, "inside do_kimatsu in enterprise, ,before pay_tax agent_type=%d withholdings_employee.size()=%d\n",
		 //  get_agent_type(),withholdings_employee.size());
	 //  fprintf(app_context.config->out_test0, "inside do_kimatsu in enterprise, ,after pay\wages agent_type=%d withholdings_employee.size()=%d\n",
	//	   get_agent_type(), withholdings_employee.size());

      /*納税*/
	   if(app_context.config->government_count!=0) pay_tax();
	  // fprintf(app_context.config->outEB_test, "inside do_kimatsu in enterprise, after pay_tax,before kessan agent_id=%d agent_type=%d\n", get_agent_id(),get_agent_type());
	  // printf("after pay_tax \n");
    
	//	pay_tax();//納税を期末に移動
	//	fprintf(app_context.config->outEB_test, "inside do_kimatsu in enterprise, after kessan,before other_kimatsu_actions agent_id=%d agent_type=%d\n", get_agent_id(), get_agent_type());
		other_kimatsu_actions();
		//printf("after other_kimatsu_actions()\n");
		//if (get_agent_type() == 2132) printf("end of do_kimatsu\n");
		//transferstocks_to_StockRecord();//決算以外の処理 other_actions()に変更するproducerのみがこの関数を使う
    }
};

