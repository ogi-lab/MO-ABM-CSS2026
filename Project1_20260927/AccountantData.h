#pragma once
#include <cstdio>

class AccountantData {
	//子クラスはConsumerTotal,RetailerTotal,WholesalerTotal, 
public:
	//------------------------------------------
	//FILE* out_toukei_bought;
	FILE* out_toukei_supplied;
	FILE* out_toukei_stock;
	FILE* out_toukei_areamarket;
	FILE* out_toukei_bought_ave;
	FILE* out_toukei_supplied_ave;
	FILE* out_toukei_stock_ave;
	FILE* out_toukei_areamarket_ave;
	FILE* out2_toukei_bought;
	FILE* out2_toukei_supplied;
	FILE* out2_toukei_stock;
	FILE* out2_toukei_areamarket;
	FILE* out_income_expenses;
	FILE* out_income_expenses_ave;
	FILE* out_total_sales;
	FILE* out_total_sales_ave;
	FILE* ITEM_SET_size_1;
	FILE* ITEM_SET_size_2;
	FILE* ITEM_SET_size_3;
	FILE* item_type_id_for_production;
	FILE* out_qmax;
	FILE* areamarket1;//生産開始前期首在庫リスト
	FILE* areamarket2; //生産後購買開始前の在庫リスト
	FILE* areamarket3; //購買後期末在庫リスト
	//--------------------------------------
	//FILE* out_title_list;
	//FILE* out_GL;
	FILE* out_TBtable;
	FILE* out_cashBook;
	FILE* out_bankBook;
	FILE* out_stockBook;
	FILE* out_TBtable_balance;
	FILE* out_balance;
	FILE* out_stockpriceBook;
	FILE* out_balance_price;

	FILE* out_materialBook;
	FILE* out_materialpriceBook;
	FILE* out_genkaBook;
	FILE* out_profit;
	FILE* out_TBtableCashBook;
	FILE* out_TBtableBankBook;
	FILE* out_TBtableInventoryBook;
	FILE* out_TBtableSaleBook;
	FILE* out_TBtableDirectlaborcostBook;
	FILE* out_TBtableDirectmaterialfeeBook;
	FILE* out_TBtableProfitBook;
	int category_type = 0;
	FILE* out_priceBook;//item_type_id毎のAgent平均価格プリントアウト用


	AccountantData() {
		FILE_set();
	}
	virtual int get_category_type() = 0;
	virtual void FILE_set() {}

};