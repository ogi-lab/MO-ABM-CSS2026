#pragma once
#include <cstdio>

class MarketData {
public:
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
	FILE* out_ave_price_list;
	FILE* ITEM_SET_size_1;
	FILE* ITEM_SET_size_2;
	FILE* ITEM_SET_size_3;
	FILE* item_type_id_for_production;
	FILE* out_qmax;
	FILE* areamarket1;//生産開始前期首在庫リスト   
	FILE* areamarket2; //生産後購買開始前の在庫リスト
	FILE* areamarket3; //購買後期末在庫
	FILE* out_confirm_random_shaffle;

	MarketData() {
		FILE_set();
	}

	virtual void FILE_set() {}

};
