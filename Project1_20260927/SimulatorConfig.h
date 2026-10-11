#pragma once
#include "RandomGenerator.h"
#include "MarketData.h"
#include "RetailMarketData.h"
#include "MaterialMarketData.h"
#include "ConsumerData.h"
#include "RetailerData.h"
#include "WholesalerData.h"
#include "SoleProprietorData.h"
#include "GovernmentData.h"	
#include "ConsumerTotal.h"
#include "RetailerTotal.h"
#include "WholesalerTotal.h"
#include "SoleProprietorTotal.h"
#include "GovernmentTotal.h"
#include "ConsumerParameter.h"
#include "RetailerParameter.h"
#include "WholesalerParameter.h"
#include "SoleproprietorParameter.h"
#include "GovernmentParameter.h"
#include "ProducerParameter.h"
#include "AreaMarketParameter.h"
#include <vector>
#include <direct.h>


/** 設定情報 */
class SimulatorConfig {
public:
	MarketData* retailMarketData;
	MarketData* materialMarketData;
	AccountData* consumerData;
	AccountData* retailerData;
	AccountData* wholesalerData;
	AccountData* governmentData;	
	AccountData* soleproprietorData;
	AccountantData* consumerTotal;
	AccountantData* retailerTotal;
	AccountantData* wholesalerTotal;
	AccountantData* soleproprietorTotal;
	AccountantData* governmentTotal;	
	ParameterData* consumerParameter;
	ParameterData* retailerParameter;
	ParameterData* wholesalerParameter;
	ParameterData* soleproprietorParameter;
	ParameterData* producerParameter;
	ParameterData* governmentParameter;
	ParameterData* areamarketParameter;

	//20120628 修正　変数ごとのカテゴリ分け、及びコード内の別場所設定のパラメータ（数値でコードに直打ち）をconfigに移設
	
	//～～～～～～～注意！！！変数の命名規則について～～～～～～～～～～～～
	/*
		命名は
		「対象エージェント名」_「変数名」_「複数種の場合の違い」
		で命名すること。

		例）　consumer _ fixed_wage _ min
		      consumer _ fixed_wage _ max

		理由：max_wageにした場合、変数が大量に増えてきた際に同様の名前が増えて変数名を特定することが困難になる。
		　　　max,minなどは上限下限設定系のパラメータで今後大量に増えることが予測されるため、app_config->で呼び出しを行う
		   　 際に検索の困難が予想される為。
	*/

	//シミュレーション基本パラメータ
	int seed=0;//乱数シード　parameterFile2で読み込む
	int ncase=1;//ncase=0 CR simulation, ncase=1 CRW simulation 
	int case_const_qcount=1;//=0 qcount=constant
    int run_period_count = 0;				//計算する最大期数
//	int critical_period_for_demand;     //需要の平均、分散を計算するための期間
	int consumer_count = 0;					//consumer数
	int retailer_count = 0;					//retailer数
	int wholesaler_count = 0;				//wholesaler数
	int equipmentmaker_count = 0;			//equipement_maker数
	int soleproprietor_count = 0;           //個人事業主数
	int bank_count = 0;						//bank数
	int government_count = 0;               //government数
	int product_class_count = 0;            //製品種の数
	//employee数
	int retailer_worker_count = 0;				//リテイラー一社当たりの労働者数
	int wholesaler_worker_count = 0;					//ホールセラー一社当たりの労働者数
	int soleproprietor_worker_count = 0;
	int n_equipment_worker = 0;				//設備製造一社当たりの労働者数
	int n_bank_worker = 0;					//銀行一社当たりの労働者数
	int n_government_worker = 0;			//行政の労働者数
	int n_max_employee = 0;					//一社当たりの雇用上限数
	//executive数
	int retailer_executive_count = 0;        //retailerの役員数
	int wholesaler_executive_count = 0;      //wholesalerの役員数
	int soleproprietor_executive_count = 0;  //soleproprietorの役員数（個人事業主なので常に一人だが可変の変数としておく）
	//long long withholdings0;
	//void set_withholdings0(long long w) { withholdings0 = w; }
	//long long get_withholdings0() { return withholdings0; }


	int item_type_id_count0 = 0;	//使っていない消去可
	int item_type_id_count = 0;				//アイテムの製品種数（CtoB市場にある製品種別の最大数）
	int item_type_id_count_r = 0;
	int item_type_id_count_s = 0;
	int item_type_id_count_w = 0;			//アイテムの製品種数 (BtoB市場にある製品種別の最大数）

	std::vector<int> item_type_idk0;
	std::vector <int> item_type_idk;//retailer+soleproprietorの製造する製品種の集合(retailMarket)
	std::vector<int> item_type_idkr;//retailer
	std::vector<int> item_type_idks;//soleproprietor
	std::vector<int> item_type_idkw;//wholesellerの製造する製品種の集合(materialMarket)


	//Consumerパラメータ群
		//ファイル設定系
	
	int consumer_fixed_wage_min = 0;		//消費者所与給与の下限値	20120523記　高島 / 20120629min_wageから変名　高島
	int consumer_fixed_wage_max = 0;		//消費者所与給与の上限値	20120523記　高島 / 20120629max_wageから変名　高島
	//double consumer_rwd;                   //預金引き出し率
	//double  consumer_rpc;                   //限界消費性向
	
	//Retailerパラメータ群
		//ファイル設定系
	/*
	int retailer_init_cash_min = 0;			//リテイラー初期所持金の下限値 20120529コメント修正荻林	/ min_retailer_cashから変名　高島
	int retailer_init_cash_max = 0;			//リテイラー初期所持金の上限値 20120529コメント修正荻林 / max_retaielr_cashから変名　高島
	int retailer_qmax_min = 0;              //製品の生産上限初期設定の下限値  20120529記　荻林 20120629 修　高島 / min_qmax_retailerから変名　高島
	int retailer_qmax_max = 0;              //製品の生産下限初期設定の上限値　20120529記　荻林 20120629 修　高島 / max_qmax_retailerから変名　高島
	int retailer_price_min = 0;
	int retailer_price_max = 0;
	double retailer_price_delta = 0.0;         //一人の生産者の売り出し価格のプラスマイナス 20240212
	*/
	 //producerパラメター群
	int critical_flag_for_priceup = 0;
	int critical_flag_for_pricedown = 0;
	double producer_demand_delta_min = 0.0;	//需要基準量決定ルールにおけるsigmaの係数2012_6_3荻林
	double producer_demand_delta_max = 0.0;	//需要基準量決定ルールにおけるsigmaの係数2012_6_3荻林
	double producer_price_alpha = 0.0;		//価格改定時の価格上下降率2012_6_3荻林
	//Wholesalerパラメータ群
	//ファイル設定系
	/*
	int wholesaler_init_cash_min = 0;			//ホールセラー初期所持金の下限値 20121231荻林	/ min_retailer_cashから変名　高島
	int wholesaler_init_cash_max = 0;			//ホールセラー初期所持金の上限値 20121231荻林 / max_retaielr_cashから変名　高島
	int wholesaler_qmax_min = 0;              //製品の生産上限初期設定の下限値  20121231記　荻林 20120629 修　高島 / min_qmax_retailerから変名　高島
	int wholesaler_qmax_max = 0;              //製品の生産下限初期設定の上限値　20121231記　荻林 20120629 修　高島 / max_qmax_retailerから変名　高島
	int wholesaler_price_min = 0;
	int wholesaler_price_max = 0;
	double wholesaler_price_delta = 0.0;         //一人の生産者の売り出し価格のプラスマイナス20240212
	*/
	/*									   //SoleProprietorパラメータ群
	int soleproprietor_init_cash_min = 0;			//ホールセラー初期所持金の下限値 20121231荻林	/ min_retailer_cashから変名　高島
	int soleproprietor_init_cash_max = 0;			//ホールセラー初期所持金の上限値 20121231荻林 / max_retaielr_cashから変名　高島
	int soleproprietor_qmax_min = 0;              //製品の生産上限初期設定の下限値  20121231記　荻林 20120629 修　高島 / min_qmax_retailerから変名　高島
	int soleproprietor_qmax_max = 0;              //製品の生産下限初期設定の上限値　20121231記　荻林 20120629 修　高島 / max_qmax_retailerから変名　高島
	int soleproprietor_price_min = 0;
	int soleproprietor_price_max = 0;
	double soleproprietor_price_delta = 0.0;         //一人の生産者の売り出し価格のプラスマイナス20240212
	*/
		//内部計算系
	double retailer_demand_per = 0.0;			//確認事項20120628　parameter設定内部でのみ使っているためメンバ変数である必要はあるか
	int retaielr_fixedcost_min = 0;			//20120629min_fixedcost_retailerから変名　高島
	int retaielr_fixedcost_max = 0;			//20120629max_fixedcost_retaielrはら変名　高島
	int retailer_variablecost_min = 0;		//20120629min_variablecost_retailerから変名　高島
	int retailer_variablecost_max = 0;		//20120629max_variablecost_retailerから変名　高島



	//RandomGenerator *rg;


	//書き込み用FILE
	//基本書き込み用FILE
	FILE *parameter1;		//読み込みパラメータの記述、設定したパラメータがあっているかの確認用、Set_parameter以外で絶対に使わないこと　20120628　創　高島
	FILE* parameter2;
	FILE* parameter3;
	FILE *out;
	FILE *out2;
	FILE *out3;
	FILE* out4;
	FILE* out509;
	//retailMarket用書き込みFILE
	/*FILE* out_toukei_bought;
	FILE *out_toukei_supplied;
	FILE *out_toukei_stock;
	FILE *out_toukei_areamarket;
	FILE *out_toukei_bought_ave;
	FILE *out_toukei_supplied_ave;
	FILE *out_toukei_stock_ave;
	FILE *out_toukei_areamarket_ave;
	FILE *out2_toukei_bought;
	FILE *out2_toukei_supplied;
	FILE *out2_toukei_stock;
	*/
	FILE *out_q_price;
	FILE *SBS_history;
	FILE *out_ave_price;
	FILE *out_agent_address;
	FILE *out_Eb;
	FILE *out_income_expenses;
	FILE *out_income_expenses_ave;
	FILE* out_test0;
	FILE *out_test;
	FILE* out_test2;
	FILE* out_test3;
	FILE* out_test4;
	FILE* out_test5;
	FILE* out_test6;
	FILE* out_test7;
	FILE* out_test8;
	FILE* out_test9;
	FILE* out_test10;
	FILE* out_test11;
	FILE* out_wage_test1;
	FILE* out_test12;
	FILE* out_test13;
	FILE* out_test14;
	FILE* out_genka_test15;
	FILE* out_size_test16;
	FILE* out_size;
	FILE* out_renewprice_test17;
	FILE* out_min_price;
	//FILE *out_total_sales;
	//FILE *out_total_sales_ave;
	//FILE *out_cashbook;
	FILE *out_producer_consumer_income_expenses;
	FILE* out_utility_weight;
	
	FILE* out_kamoku;
	
	FILE* out_agent_type;
	FILE* out_journal;
	
	//FILE* areamarket1;//生産開始前期首在庫リスト   
	//FILE* areamarket2; //生産後購買開始前の在庫リスト
	//FILE* areamarket3; //購買後期末在庫リスト
	FILE* out_temporary;
	FILE* out_consumer_type;
	FILE* out_item_type_id;
	FILE* out_IO_table;//産業連関表のプリント用
	FILE* out_GDP;
	FILE* out_GDP2;//c11,c12,c22,c21,b12,b11,b21,b22を記載したversion
	//読み込み用FILE
	FILE *Parameter_FILE_1;
	FILE *Parameter_FILE_2;
	//FILE *Parameter_FILE_3;
	//FILE *Parameter_FILE_4;
	//FILE *Parameter_FILE_5;
	//FILE *Parameter_FILE_6;
	//FILE *Parameter_FILE_7;
	FILE *Parameter_FILE_8;
	FILE *Parameter_FILE_9;
	FILE* error_message;
	FILE* out_qmax;
	FILE* out_qcount_decision;
	FILE* outEB;
	FILE* outEB_test;
	FILE* out_executive_wage_list;
	FILE* out_exexutive_list;
	FILE* out_trace_buy;
	FILE* out_trace_supply_sell;
	FILE* out_trace_tax_income;
	FILE* out_trace_government;
	FILE* out_withholding_tax_executive;
	FILE* out_tax_revenue_received;
	FILE* out_consumer_income_Eb_tax;
	FILE* out_Eb_retailes;
	FILE* out_Eb_retailes_government;
	FILE* out_economy_debug;
    SimulatorConfig()		//コンストラクタ
	{
		_mkdir("..\\FolderForIOFiles\\data");
		_mkdir("..\\FolderForIOFiles\\data\\materialMarketData");
		_mkdir("..\\FolderForIOFiles\\data\\retailMarketData");
		_mkdir("..\\FolderForIOFiles\\data\\consumerData");
		_mkdir("..\\FolderForIOFiles\\data\\retailerData");
		_mkdir("..\\FolderForIOFiles\\data\\wholesalerData");
		_mkdir("..\\FolderForIOFiles\\data\\soleproprietorData");
		_mkdir("..\\FolderForIOFiles\\data\\governmentData");	
		_mkdir("..\\FolderForIOFiles\\data\\consumerTotal");
		_mkdir("..\\FolderForIOFiles\\data\\retailerTotal");
		_mkdir("..\\FolderForIOFiles\\data\\wholesalerTotal");
		_mkdir("..\\FolderForIOFiles\\data\\soleproprietorTotal");
		_mkdir("..\\FolderForIOFiles\\data\\governmentTotal");
		_mkdir("..\\FolderForIOFiles\\data\\consumerTotal\\TBtable");
		_mkdir("..\\FolderForIOFiles\\data\\retailerTotal\\TBtable");
		_mkdir("..\\FolderForIOFiles\\data\\wholesalerTotal\\TBtable");
		_mkdir("..\\FolderForIOFiles\\data\\soleproprietorTotal\\TBtable");
		_mkdir("..\\FolderForIOFiles\\data\\governmentTotal\\TBtable");	
		_mkdir("..\\FolderForIOFiles\\data\\consumerData\\TBtable");
		_mkdir("..\\FolderForIOFiles\\data\\retailerData\\TBtable");
		_mkdir("..\\FolderForIOFiles\\data\\wholesalerData\\TBtable");
		_mkdir("..\\FolderForIOFiles\\data\\soleproprietorData\\TBtable");
		_mkdir("..\\FolderForIOFiles\\data\\governmentData\\TBtable");	
		

        // TODO Read form file
		//errno_t err;
		retailMarketData = new RetailMarketData();
		materialMarketData = new MaterialMarketData();
		consumerData = new ConsumerData();
		retailerData = new RetailerData();
		wholesalerData = new WholesalerData();
		soleproprietorData = new SoleProprietorData();
		consumerTotal = new ConsumerTotal();
		retailerTotal = new RetailerTotal();
		wholesalerTotal = new WholesalerTotal();
		soleproprietorTotal = new SoleProprietorTotal();
		governmentTotal = new GovernmentTotal();
		printf("before consumerParameter\n");
		consumerParameter = new ConsumerParameter();//constructerでSet_parameter()が呼び出されパラメータが読み込まれる
		printf("beforenew retailerParameter\n");
		retailerParameter = new RetailerParameter();
		wholesalerParameter = new WholesalerParameter();
		soleproprietorParameter = new SoleproprietorParameter();
		governmentParameter = new GovernmentParameter();
		printf("before new producerParameter()\n");
		producerParameter = new ProducerParameter();
		printf("before new areamarketParameter\n");
		areamarketParameter = new AreaMarketParameter();
		printf("before set_parameter\n");
		Set_parameter();	//パラメータ設定の外部化、ファイルからの読み込み以外のパラメータ設定はここではやらないこと　創 20120628 高島
		printf("ater set_pramaeter\n");
		//2012_6_3荻林追加-----------固定費、変動費考慮
		retailer_demand_per=(double(consumerParameter->fixed_wage_min)+double(consumerParameter->fixed_wage_max))/2*double(consumer_count)/double(retailer_count);
		retaielr_fixedcost_min=int(retailer_demand_per/double(item_type_id_count)*0.5*0.1);
		retaielr_fixedcost_max=int(retailer_demand_per/double(item_type_id_count)*1.0*0.1);;
		retailer_variablecost_min=int(retailer_demand_per/double(item_type_id_count)/retailerParameter->init_cash_max*0.25*0.5);		//最少変動費＝コスト/最大生産量
		retailer_variablecost_max=int(retailer_demand_per/double(item_type_id_count)/retailerParameter->init_cash_min*0.5*0.5);		//最大変動費＝コスト/最少生産量

		FILE_set();		//ファイルパスセットの関数化　20120604 変　高島

		define_item_type_id();
		//rg=new RandomGenerator();
		
    }

	void FILE_set()
	{
	    //constructorで呼ばれる
		fopen_s(&out, "..\\FolderForIOFiles\\data\\out.txt", "w");
		fopen_s(&out2, "..\\FolderForIOFiles\\data\\out2.txt", "w");
		fopen_s(&out3, "..\\FolderForIOFiles\\data\\out3.txt", "w");
		fopen_s(&out4, "..\\FolderForIOFiles\\data\\out4.csv", "w");
		fopen_s(&out509, "..\\FolderForIOFiles\\data\\out509.csv", "w");
		//-----------------
		/*
		fopen_s(&out_toukei_bought, "..\\FolderForIOFiles\\data\\out_toukei_bought.csv", "w");
		fopen_s(&out_toukei_supplied, "..\\FolderForIOFiles\\data\\out_toukei_supplied.csv", "w");
		fopen_s(&out_toukei_bought_ave, "..\\FolderForIOFiles\\data\\out_toukei_bought_ave.csv", "w");
		fopen_s(&out_toukei_supplied_ave, "..\\FolderForIOFiles\\data\\out_toukei_supplied_ave.csv", "w");
		fopen_s(&out_toukei_stock, "..\\FolderForIOFiles\\data\\out_toukei_stock.csv", "w");
		fopen_s(&out_toukei_stock_ave, "..\\FolderForIOFiles\\data\\out_toukei_stock_ave.csv", "w");
		fopen_s(&out_toukei_areamarket, "..\\FolderForIOFiles\\data\\out_toukei_areamarket.csv", "w");
		fopen_s(&out_toukei_areamarket_ave, "..\\FolderForIOFiles\\data\\out_toukei_areamarket_ave.csv", "w");
		fopen_s(&out2_toukei_bought, "..\\FolderForIOFiles\\data\\out2_toukei_bought.csv", "w");
		fopen_s(&out2_toukei_supplied, "..\\FolderForIOFiles\\data\\out2_toukei_supplied.csv", "w");
		fopen_s(&out2_toukei_stock, "..\\FolderForIOFiles\\data\\out2_toukei_stock.csv", "w");
		*/
		fopen_s(&out_q_price, "..\\FolderForIOFiles\\data\\out_q_price.csv", "w");
		fopen_s(&SBS_history, "..\\FolderForIOFiles\\data\\out_SBS_history.csv", "w");
		fopen_s(&out_ave_price, "..\\FolderForIOFiles\\data\\out_ave_price.csv", "w");
		fopen_s(&out_agent_address, "..\\FolderForIOFiles\\data\\out_agent_address.csv", "w");
		fopen_s(&out_Eb, "..\\FolderForIOFiles\\data\\out_Eb.csv", "w");
		fopen_s(&out_income_expenses, "..\\FolderForIOFiles\\data\\out_income_expenses.csv", "w");
		fopen_s(&out_income_expenses_ave, "..\\FolderForIOFiles\\data\\out_income_expenses_ave.csv", "w");
		fopen_s(&out_test0, "..\\FolderForIOFiles\\data\\out_test0.csv", "w");	
		fopen_s(&out_test, "..\\FolderForIOFiles\\data\\out_test.csv", "w");
		fopen_s(&out_test2, "..\\FolderForIOFiles\\data\\out_test2.csv", "w");
		fopen_s(&out_test3, "..\\FolderForIOFiles\\data\\out_test3.csv", "w");
		fopen_s(&out_test4, "..\\FolderForIOFiles\\data\\out_test4.csv", "w");
		fopen_s(&out_test5, "..\\FolderForIOFiles\\data\\out_test5.csv", "w");
		fopen_s(&out_test6, "..\\FolderForIOFiles\\data\\out_test6.csv", "w");
		fopen_s(&out_test7, "..\\FolderForIOFiles\\data\\out_test7.csv", "w");
		fopen_s(&out_test8, "..\\FolderForIOFiles\\data\\out_test8.csv", "w");
		fopen_s(&out_test9, "..\\FolderForIOFiles\\data\\out_test9.csv", "w");
		fopen_s(&out_test10, "..\\FolderForIOFiles\\data\\out_test10.txt", "w");
		fopen_s(&out_test11, "..\\FolderForIOFiles\\data\\out_test11.txt", "w");
		fopen_s(&out_test12, "..\\FolderForIOFiles\\data\\out_test12.csv", "w");
		fopen_s(&out_test13, "..\\FolderForIOFiles\\data\\out_test13.csv", "w");
		fopen_s(&out_test14, "..\\FolderForIOFiles\\data\\out_test14.csv", "w");
		fopen_s(&out_genka_test15, "..\\FolderForIOFiles\\data\\out_genka_test15.csv", "w");
		fopen_s(&out_size_test16, "..\\FolderForIOFiles\\data\\out_size_test16.csv", "w");
		fopen_s(&out_size, "..\\FolderForIOFiles\\data\\out_size.csv", "w");
		fopen_s(&out_renewprice_test17, "..\\FolderForIOFiles\\data\\out_renewprice_test17.csv", "w");
		fopen_s(&out_wage_test1, "..\\FolderForIOFiles\\data\\out_wage_test1.csv", "w");
		fopen_s(&out_trace_buy, "..\\FolderForIOFiles\\data\\out_trace_buy.csv", "w");	
		//fopen_s(&out_total_sales, "..\\FolderForIOFiles\\data\\out_total_sales.csv", "w");
		//fopen_s(&out_total_sales_ave, "..\\FolderForIOFiles\\data\\out_total_sales_ave.csv", "w");
		//fopen_s(&out_cashbook, "..\\FolderForIOFiles\\data\\out_cashbook.csv", "w");
		fopen_s(&out_producer_consumer_income_expenses, "..\\FolderForIOFiles\\data\\out_producer_consumer_income_expenses.csv", "w");
		fopen_s(&out_utility_weight, "..\\FolderForIOFiles\\data\\out_utility_weight.csv", "w");
		//fopen_s(&areamarket1, "..\\FolderForIOFiles\\data\\areamarket1.csv", "w");
		//fopen_s(&areamarket2, "..\\FolderForIOFiles\\data\\areamarket2.csv", "w");
		//fopen_s(&areamarket3, "..\\FolderForIOFiles\\data\\areamarket3.csv", "w");
		fopen_s(&out_temporary, "..\\FolderForIOFiles\\data\\out_temporary.txt", "w");
		fopen_s(&out_consumer_type, "..\\FolderForIOFiles\\data\\out_consumer_type.csv", "w");
		//fopen_s(&out_title_list_consumer, "..\\FolderForIOFiles\\data\\out_title_list_consumer.csv", "w");
		//fopen_s(&out_title_list_retailer, "..\\FolderForIOFiles\\data\\out_title_list_retailer.csv", "w");
		//fopen_s(&out_title_list_wholesaler, "..\\FolderForIOFiles\\data\\out_title_list_wholesaler.csv", "w");
		fopen_s(&out_kamoku, "..\\FolderForIOFiles\\data\\out_kamoku.csv", "w");
		//fopen_s(&out_GL_consumer,"..\\FolderForIOFiles\\data\\out_GL_consumer.csv", "w"); 
		//fopen_s(&out_GL_retailer,"..\\FolderForIOFiles\\data\\out_GL_retailer.csv", "w");
		//fopen_s(&out_GL_wholesaler,"..\\FolderForIOFiles\\data\\out_GL_wholesaler.csv", "w");
		fopen_s(&out_agent_type, "..\\FolderForIOFiles\\data\\out_agent_type.csv", "w");
		fopen_s(&out_journal, "..\\FolderForIOFiles\\data\\out_journal.csv", "w");
		//fopen_s(&out_TBtable_consumer, "..\\FolderForIOFiles\\data\\out_TBtable_consumer.csv", "w");
		//fopen_s(&out_TBtable_wholesaler, "..\\FolderForIOFiles\\data\\out_TBtable_wholesaler.csv", "w");
		//fopen_s(&out_TBtable_retailer, "..\\FolderForIOFiles\\data\\out_TBtable_retailer.csv", "w");
		fopen_s(&out_IO_table, "..\\FolderForIOFiles\\data\\out_IO_table.csv", "w");
		fopen_s(&out_GDP, "..\\FolderForIOFiles\\data\\out_GDP.csv", "w");
		fopen_s(&out_GDP2, "..\\FolderForIOFiles\\data\\out_GDP2.csv", "w");
		fopen_s(&out_item_type_id, "..\\FolderForIOFiles\\data\\out_item_type_id.csv", "w");
		fopen_s(&error_message, "..\\FolderForIOFiles\\data\\error_message.csv", "w");
		fopen_s(&out_qmax, "..\\FolderForIOFiles\\data\\out_qmax.csv", "w");
		fopen_s(&out_qcount_decision, "..\\FolderForIOFiles\\data\\out_qcount_decision.csv", "w");
		fopen_s(&outEB, "..\\FolderForIOFiles\\data\\outEB.txt", "w");
		fopen_s(&outEB_test, "..\\FolderForIOFiles\\data\\outEB_test.txt", "w");	
		fopen_s(&out_executive_wage_list, "..\\FolderForIOFiles\\data\\out_executive_wage_list.csv", "w");
		fopen_s(&out_trace_supply_sell, "..\\FolderForIOFiles\\data\\out_trace_supply_sell.csv", "w");
		fopen_s(&out_exexutive_list, "..\\FolderForIOFiles\\data\\out_executive_list.csv", "w");
		fopen_s(&out_trace_tax_income, "..\\FolderForIOFiles\\data\\out_trace_tax_income.csv","w");
		fopen_s(&out_trace_government, "..\\FolderForIOFiles\\data\\out_trace_government.csv", "w");
		fopen_s(&out_withholding_tax_executive, "..\\FolderForIOFiles\\data\\out_withholding_tax_executive.csv", "w");
		fopen_s(&out_tax_revenue_received, "..\\FolderForIOFiles\\data\\out_tax_revenue_received.csv", "w");
		fopen_s(&out_consumer_income_Eb_tax, "..\\FolderForIOFiles\\data\\out_consumer_income_Eb_tax.csv", "w");
		fopen_s(&out_Eb_retailes, "..\\FolderForIOFiles\\data\\out_Eb_retailes.csv", "w");
		fopen_s(&out_Eb_retailes_government, "..\\FolderForIOFiles\\data\\out_Eb_retailes_government.csv", "w");
		fopen_s(&out_economy_debug, "..\\FolderForIOFiles\\data\\out_economy_debug.txt", "w");
		fopen_s(&out_min_price, "..\\FolderForIOFiles\\data\\out_min_price.csv", "w");
	}
	void define_item_type_id(){
		//contructorで呼ばれる
		//item_type_idkr: retailer生産品種 
		//item_type_idks: soleproprietor生産品種
		//item_type_idk: retailer及びsoleproprietor生産品種
		//item_type_idkw: wholesaler生産品種
		item_type_id_count = item_type_id_count_r + item_type_id_count_s;
		for (int i = 0; i < item_type_id_count_r; i++) {
			//i=0?item_type_id_count_r-1　ならretailer生産品種	
			item_type_idkr.push_back((i + 1) * 10);
			item_type_idk.push_back(item_type_idkr[i]);
		}
		for (int i = 0; i < item_type_id_count_s; i++) {
			//i=item_type_id_count_r?item_type_id_coun_r+item_type_id_count_s-1ならsoleproprietor生産品種
			item_type_idks.push_back((item_type_id_count_r + i + 1) * 10);
			item_type_idk.push_back(item_type_idks[i]);
		}
		for (int i = 0; i < item_type_id_count_w; i++) {
			item_type_idkw.push_back((item_type_id_count+i + 1) * 10);
		}
		print_item_type_id();
	}
	void print_item_type_id() {
		fprintf(out_item_type_id, "item_type_idk=,");
		for (int i = 0; i < item_type_id_count; i++) {
			fprintf(out_item_type_id, "%d,", item_type_idk[i]);
		}
		fprintf(out_item_type_id, "\n");
		fprintf(out_item_type_id, "item_type_idkw=,");
		for (int i = 0; i < item_type_id_count_w; i++) {
			fprintf(out_item_type_id, "%d,", item_type_idkw[i]);
		}
		fprintf(out_item_type_id, "\n");
	}
	void Set_parameter()
	{
		//変数の初期化　基本的にはいらないはず
		run_period_count =0;
		//critical_period_for_demand=0;
        retailer_count = 0;	
        consumer_count = 0;	
		wholesaler_count = 0;				//wholesaler数
		equipmentmaker_count=0;			//equipement_maker数
		soleproprietor_count=0;           //個人事業主数
		bank_count=0;						//bank数
		government_count=0;               //government数
		
		consumer_fixed_wage_min=0;			
		consumer_fixed_wage_max=0;
		/*
		retailer_init_cash_min=0;
		retailer_init_cash_max=0;
		retailer_qmax_min=0;
		retailer_qmax_max=0;
		*/
		item_type_id_count=0;
		item_type_id_count_w=0;
		critical_flag_for_priceup=0;
		critical_flag_for_pricedown=0;
		producer_demand_delta_min=0;
		producer_demand_delta_max=0;
		producer_price_alpha=0;
		soleproprietor_count=20;

		printf("before fopen\n");
		//パラメータファイルのオープン
		fopen_s(&Parameter_FILE_1,"..\\FolderForIOFiles\\parameter\\Set_parameter_1.csv","r");
		fopen_s(&Parameter_FILE_2, "..\\FolderForIOFiles\\parameter\\Set_parameter_2.csv", "r");
		//fopen_s(&Parameter_FILE_3, "..\\FolderForIOFiles\\parameter\\Set_parameter_3.csv", "r");

		//パラメータファイルからの読み込み
		//printf("before fscanf\n");
		fscanf_s(Parameter_FILE_1, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &run_period_count, &consumer_count, &retailer_count,&wholesaler_count,&soleproprietor_count,&equipmentmaker_count,&bank_count,&government_count,&retailer_worker_count,&wholesaler_worker_count,&soleproprietor_worker_count,&retailer_executive_count,&wholesaler_executive_count,&soleproprietor_executive_count);
		//fscanf_s(Parameter_FILE_2,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%lf,%lf,%lf,\n",&critical_period_for_demand,&consumer_fixed_wage_min,&consumer_fixed_wage_max,&retailer_init_cash_min,&wholesaler_init_cash_min,&retailer_init_cash_max,&wholesaler_init_cash_max,&retailer_qmax_min,&wholesaler_qmax_min,&soleproprietor_qmax_min,&retailer_qmax_max,&wholesaler_qmax_max,&soleproprietor_qmax_max,&item_type_id_count_r,&item_type_id_count_s,&item_type_id_count_w,&critical_flag_for_priceup,&critical_flag_for_pricedown,&producer_demand_delta_min,&producer_demand_delta_max,&producer_price_alpha);
		//printf("after fscanf_s(Parameter_FILE_1\n");
		if (retailer_count == 0) { retailer_worker_count = 0; retailer_executive_count = 0; }
		if (wholesaler_count == 0) { wholesaler_worker_count = 0; wholesaler_executive_count = 0; }	
		fscanf_s(Parameter_FILE_2, "%d,%d,%d,%d,%d,%d,%lf,%lf,%lf,\n",&seed, &item_type_id_count_r, &item_type_id_count_s, &item_type_id_count_w,
			&critical_flag_for_priceup, &critical_flag_for_pricedown, &producer_demand_delta_min, &producer_demand_delta_max, &producer_price_alpha);
		if(retailer_count==0) item_type_id_count_r=0;
		if (wholesaler_count == 0) item_type_id_count_w = 0;
		//printf("after fscanf\n");
		//パラメータファイルのクローズ
		fclose(Parameter_FILE_1);	
		//wholesaler_count=10;
		//equipmentmaker_count=2;
		//bank_count=1;

		//ファイル読み込みパラメータの出力
		fopen_s(&parameter1,"..\\FolderForIOFiles\\data\\parameter1.csv","w");
		fopen_s(&parameter2, "..\\FolderForIOFiles\\data\\parameter2.csv", "w");
		//fopen_s(&parameter3, "..\\FolderForIOFiles\\data\\parameter3.csv", "w");
		fprintf(parameter1,"parameter_FILE_1\n");
		//fprintf(parameter,"run_period_count(d)	,critical_period_for_demand(d)	,retailer_count(d)	,consumer_count(d)	,consumer_fixed_wage_min(d)	,consumer_fixed_wage_max(d)	,retailer_init_cash_min(d)	,retailer_init_cash_max(d)	,retailer_qmax_min(d)	,retailer_qmax_max(d)	,item_type_id_count(d)	,item_type_id_count_w(d)	,critical_flag_for_priceup(d)	,critical_flag_for_pricedown(d)	,producer_demand_delta_min(lf)	,producer_demand_delta_max(lf)	,producer_price_alpha(lf)\n");
		//fprintf(parameter,"%d					,%d								,%d					,%d					,%d							,%d							,%d							,%d							,%d						,%d						,%d						,%d							,%d								,%d								,%lf							,%lf							,%lf\n"
		fprintf(parameter1, "run_period_count, consumer_count, retailer_count, wholesaler_count, soleproprietor_count,equipmentmaker_count, bank_count, government_count, retailer_worker_count, wholesaler_worker_count,soleproprietor_worker_count,retailer_executive_count,wholesaler_executive_count,soleproprietor_executive_count);\n");
		fprintf(parameter1, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", run_period_count, consumer_count, retailer_count, wholesaler_count, soleproprietor_count,equipmentmaker_count, bank_count, government_count, retailer_worker_count, wholesaler_worker_count,soleproprietor_worker_count, retailer_executive_count, wholesaler_executive_count, soleproprietor_executive_count);

		fprintf(parameter2, "parameter_FILE_2\n");
		fprintf(parameter2, "seed,item_type_id_count_r, item_type_id_count_s,item_type_id_count_w, critical_flag_for_priceup, critical_flag_for_pricedown, producer_demand_delta_min, producer_demand_delta_max, producer_price_alpha,\n");
		fprintf(parameter2, "%d,%d,%d,%d,%d,%d,%lf,%lf,%lf,\n", seed,item_type_id_count_r,item_type_id_count_s,item_type_id_count_w, critical_flag_for_priceup, critical_flag_for_pricedown, producer_demand_delta_min, producer_demand_delta_max, producer_price_alpha);
		//fprintf(parameter2, "critical_period_for_demand, consumer_fixed_wage_min, consumer_fixed_wage_max,
		//  retailer_init_cash_min, wholesaler_init_cash_min,retailer_init_cash_max,wholesaler_init_cash_max,
		//  retailer_qmax_min,wholesaler_qmax_min, soleproprietor_qmax_min,retailer_qmax_max, wholesaler_qmax_max,soleproprietor_qmax_max,
		// item_type_id_count_r, item_type_id_count_s,item_type_id_count_w, 
		// critical_flag_for_priceup, critical_flag_for_pricedown, producer_demand_delta_min, producer_demand_delta_max, producer_price_alpha,\n");
		//fprintf(parameter2, "%d,%d,%d,
		//%d,%d,%d,%d,
		//%d,%d,%d,%d,%d,%d,%d,%d,%lf,%lf,%lf,\n", critical_period_for_demand, consumer_fixed_wage_min, consumer_fixed_wage_max, retailer_init_cash_min, wholesaler_init_cash_min, retailer_init_cash_max, wholesaler_init_cash_max, retailer_qmax_min, wholesaler_qmax_min, soleproprietor_qmax_min, retailer_qmax_max, wholesaler_qmax_max, soleproprietor_qmax_max, item_type_id_count_r, item_type_id_count_s, item_type_id_count_w, critical_flag_for_priceup, critical_flag_for_pricedown, producer_demand_delta_min, producer_demand_delta_max, producer_price_alpha);

		//fprintf(parameter3, "parameter_FILE_3\n");
		
	}
};
