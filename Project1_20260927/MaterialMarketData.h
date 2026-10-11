#pragma once
#include <cstdio>
#include "MarketData.h"

class MaterialMarketData : public MarketData {
public:
	MaterialMarketData() {
		FILE_set();
	}
	void FILE_set() {
		
		fopen_s(&out_toukei_bought, "..\\FolderForIOFiles\\data\\materialMarketData\\out_toukei_bought.csv", "w");
		fopen_s(&out_toukei_supplied, "..\\FolderForIOFiles\\data\\materialMarketData\\out_toukei_supplied.csv", "w");
		fopen_s(&out_toukei_bought_ave, "..\\FolderForIOFiles\\data\\materialMarketData\\out_toukei_bought_ave.csv", "w");
		fopen_s(&out_toukei_supplied_ave, "..\\FolderForIOFiles\\data\\materialMarketData\\out_toukei_supplied_ave.csv", "w");
		fopen_s(&out_toukei_stock, "..\\FolderForIOFiles\\data\\materialMarketData\\out_toukei_stock.csv", "w");
		fopen_s(&out_toukei_stock_ave, "..\\FolderForIOFiles\\data\\materialMarketData\\out_toukei_stock_ave.csv", "w");
		fopen_s(&out_toukei_areamarket, "..\\FolderForIOFiles\\data\\materialMarketData\\out_toukei_areamarket.csv", "w");
		fopen_s(&out_toukei_areamarket_ave, "..\\FolderForIOFiles\\data\\materialMarketData\\out_toukei_areamarket_ave.csv", "w");
		fopen_s(&out2_toukei_bought, "..\\FolderForIOFiles\\data\\materialMarketData\\out2_toukei_bought.csv", "w");
		fopen_s(&out2_toukei_supplied, "..\\FolderForIOFiles\\data\\materialMarketData\\out2_toukei_supplied.csv", "w");
		fopen_s(&out2_toukei_stock, "..\\FolderForIOFiles\\data\\materialMarketData\\out2_toukei_stock.csv", "w");
		fopen_s(&out2_toukei_areamarket, "..\\FolderForIOFiles\\data\\materialMarketData\\out2_toukei_areamarket.csv", "w");
		fopen_s(&out_income_expenses, "..\\FolderForIOFiles\\data\\materialMarketData\\out_income_expenses.csv", "w");
		fopen_s(&out_income_expenses_ave, "..\\FolderForIOFiles\\data\\materialMarketData\\out_income_expenses_ave.csv", "w");
		fopen_s(&out_total_sales, "..\\FolderForIOFiles\\data\\materialMarketData\\out_total_sales.csv", "w");
		fopen_s(&out_total_sales_ave, "..\\FolderForIOFiles\\data\\materialMarketData\\out_total_sales_ave.csv", "w");
		fopen_s(&ITEM_SET_size_1, "..\\FolderForIOFiles\\data\\materialMarketData\\ITEM_SET_size_1.csv", "w");
		fopen_s(&ITEM_SET_size_2, "..\\FolderForIOFiles\\data\\materialMarketData\\ITEM_SET_size_2.csv", "w");
		fopen_s(&ITEM_SET_size_3, "..\\FolderForIOFiles\\data\\materialMarketData\\ITEM_SET_size_3.csv", "w");
		fopen_s(&item_type_id_for_production, "..\\FolderForIOFiles\\data\\materialMarketData\\out_item_type_id_for_production.csv", "w");
		fopen_s(&out_qmax, "..\\FolderForIOFiles\\data\\materialMarketData\\out_qmax.csv", "w");
		fopen_s(&areamarket1, "..\\FolderForIOFiles\\data\\materialMarketData\\areamarket1.csv", "w");
		fopen_s(&areamarket2, "..\\FolderForIOFiles\\data\\materialMarketData\\areamarket2.csv", "w");
		fopen_s(&areamarket3, "..\\FolderForIOFiles\\data\\materialMarketData\\areamarket3.csv", "w");
		fopen_s(&out_confirm_random_shaffle, "..\\FolderForIOFiles\\data\\materialMarketData\\out_confirm_random_shaffle.csv", "w");
		fopen_s(&out_ave_price_list, "..\\FolderForIOFiles\\data\\materialMarketData\\out_ave_price_list.csv", "w");
	}

};
