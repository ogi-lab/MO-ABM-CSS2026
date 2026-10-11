#pragma once
#include <cstdio>
#include "MarketData.h"

class RetailMarketData : public MarketData{
public:
	RetailMarketData() {
		FILE_set();
	}
	void FILE_set() {
		fopen_s(&out_toukei_bought, "..\\FolderForIOFiles\\data\\retailMarketData\\out_toukei_bought.csv", "w");
		fopen_s(&out_toukei_supplied, "..\\FolderForIOFiles\\data\\retailMarketData\\out_toukei_supplied.csv", "w");
		fopen_s(&out_toukei_bought_ave, "..\\FolderForIOFiles\\data\\retailMarketData\\out_toukei_bought_ave.csv", "w");
		fopen_s(&out_toukei_supplied_ave, "..\\FolderForIOFiles\\data\\retailMarketData\\out_toukei_supplied_ave.csv", "w");
		fopen_s(&out_toukei_stock, "..\\FolderForIOFiles\\data\\retailMarketData\\out_toukei_stock.csv", "w");
		fopen_s(&out_toukei_stock_ave, "..\\FolderForIOFiles\\data\\retailMarketData\\out_toukei_stock_ave.csv", "w");
		fopen_s(&out_toukei_areamarket, "..\\FolderForIOFiles\\data\\retailMarketData\\out_toukei_areamarket.csv", "w");
		fopen_s(&out_toukei_areamarket_ave, "..\\FolderForIOFiles\\data\\retailMarketData\\out_toukei_areamarket_ave.csv", "w");
		fopen_s(&out2_toukei_bought, "..\\FolderForIOFiles\\data\\retailMarketData\\out2_toukei_bought.csv", "w");
		fopen_s(&out2_toukei_supplied, "..\\FolderForIOFiles\\data\\retailMarketData\\out2_toukei_supplied.csv", "w");
		  fopen_s(&out2_toukei_stock, "..\\FolderForIOFiles\\data\\retailMarketData\\out2_toukei_stock.csv", "w");
		  fopen_s(&out2_toukei_areamarket, "..\\FolderForIOFiles\\data\\retailMarketData\\out2_toukei_areamarket.csv", "w");
		fopen_s(&out_income_expenses, "..\\FolderForIOFiles\\data\\retailMarketData\\out_income_expenses.csv", "w");
		fopen_s(&out_income_expenses_ave, "..\\FolderForIOFiles\\data\\retailMarketData\\out_income_expenses_ave.csv", "w");
		fopen_s(&out_total_sales, "..\\FolderForIOFiles\\data\\retailMarketData\\out_total_sales.csv", "w");
		fopen_s(&out_total_sales_ave, "..\\FolderForIOFiles\\data\\retailMarketData\\out_total_sales_ave.csv", "w");
		fopen_s(&ITEM_SET_size_1, "..\\FolderForIOFiles\\data\\retailMarketData\\ITEM_SET_size_1.csv", "w");
		fopen_s(&ITEM_SET_size_2, "..\\FolderForIOFiles\\data\\retailMarketData\\ITEM_SET_size_2.csv", "w");
		fopen_s(&ITEM_SET_size_3, "..\\FolderForIOFiles\\data\\retailMarketData\\ITEM_SET_size_3.csv", "w");
		fopen_s(&item_type_id_for_production, "..\\FolderForIOFiles\\data\\retailMarketData\\out_item_type_id_for_production.csv", "w");
		fopen_s(&out_qmax, "..\\FolderForIOFiles\\data\\retailMarketData\\out_qmax.csv", "w");
		fopen_s(&areamarket1, "..\\FolderForIOFiles\\data\\retailMarketData\\areamarket1.csv", "w");
		fopen_s(&areamarket2, "..\\FolderForIOFiles\\data\\retailMarketData\\areamarket2.csv", "w");
		fopen_s(&areamarket3, "..\\FolderForIOFiles\\data\\retailMarketData\\areamarket3.csv", "w");
		fopen_s(&out_confirm_random_shaffle, "..\\FolderForIOFiles\\data\\retailMarketData\\out_confirm_random_shaffle.csv", "w");
		fopen_s(&out_ave_price_list, "..\\FolderForIOFiles\\data\\retailMarketData\\out_ave_price_list.csv", "w");
	}

};
