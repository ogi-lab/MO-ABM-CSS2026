#pragma once
#include <cstdio>

class AccountData {
public:
	//------------------
	FILE* out_toukei_bought;
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
	FILE* out_title_list;
	FILE* out_GL;
	FILE* out_TBtable;
	FILE* out_agent_TBtable;
	FILE* out_cashbook;
	FILE* out_cashbookTotal;
	FILE* out_bankbook;
	FILE* out_bankbookTotal;
	FILE* out_stockbook;
	FILE* out_stockbookTotal;
	FILE* out_balance;
	FILE* out_stockpricebook;
	FILE* out_stockpricebookTotal;
	FILE* out_balance_price;

	FILE* out_materialbook;
	FILE* out_materialbookTotal;
	FILE* out_materialpricebook;
	FILE* out_materialpricebookTotal;
	FILE* out_genkabook;
	FILE* out_genkabookTotal;
	FILE* out_profit;//期末経常利益プリントアウト用
	FILE* out_TBtableCashBook;
	FILE* out_TBtableBankBook;
	FILE* out_TBtableInventoryBook;
	FILE* out_TBtableSaleBook;
	FILE* out_TBtableDirectlaborcostBook;
	FILE* out_TBtableDirectmaterialfeeBook;
	FILE* out_TBtableProfitBook;
	FILE* out_deposit_withdrawalBook;
	FILE* out_quantity_priceBook;
	FILE* out_income_taxRevenue;
	//FILE* out_confirm_rundom_shaffle;
	//FILE* out_price;

	AccountData() {
		FILE_set();
	}

	virtual void FILE_set() {}

};
