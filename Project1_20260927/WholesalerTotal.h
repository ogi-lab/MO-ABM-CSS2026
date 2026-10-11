#pragma once
#include <cstdio>
#include "AccountantData.h"

class WholesalerTotal : public AccountantData {
public:
	//	FILE* out_TBtable;
	WholesalerTotal() {
		FILE_set();
	}
	int get_category_type() {
		category_type = 2132;
		return category_type;
	}
	void FILE_set() {
		/*
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
		*/
		//fopen_s(&out_title_list, "..\\FolderForIOFiles\\data\\consumerData\\out_title_list.csv", "w");
		//fopen_s(&out_GL, "..\\FolderForIOFiles\\data\\consumerData\\out_GL.csv", "w");
		fopen_s(&out_TBtable, "..\\FolderForIOFiles\\data\\wholesalerTotal\\out_TBtable.csv", "w");
		fopen_s(&out_cashBook, "..\\FolderForIOFiles\\data\\wholesalerTotal\\out_cashBook.csv", "w");
		fopen_s(&out_bankBook, "..\\FolderForIOFiles\\data\\wholesalerTotal\\out_bankBook.csv", "w");
		fopen_s(&out_stockBook, "..\\FolderForIOFiles\\data\\wholesalerTotal\\out_stockBook.csv", "w");
		fopen_s(&out_stockpriceBook, "..\\FolderForIOFiles\\data\\wholesalerTotal\\out_stockpriceBook.csv", "w");
		fopen_s(&out_TBtable_balance, "..\\FolderForIOFiles\\data\\wholesalerTotal\\out_TBtable_balance.csv", "w");
		fopen_s(&out_genkaBook, "..\\FolderForIOFiles\\data\\wholesalerTotal\\out_genkaBook.csv", "w");
		fopen_s(&out_profit, "..\\FolderForIOFiles\\data\\wholesalerTotal\\out_profit.csv", "w");
		fopen_s(&out_TBtableCashBook, "..\\FolderForIOFiles\\data\\wholesalerTotal\\TBtable\\out_TBtableCashBook.csv", "w");
		fopen_s(&out_TBtableBankBook, "..\\FolderForIOFiles\\data\\wholesalerTotal\\TBtable\\out_TBtableBankBook.csv", "w");
		fopen_s(&out_TBtableInventoryBook, "..\\FolderForIOFiles\\data\\wholesalerTotal\\TBtable\\out_TBtableInventoryBook.csv", "w");
		fopen_s(&out_TBtableSaleBook, "..\\FolderForIOFiles\\data\\wholesalerTotal\\TBtable\\out_TBtableSaleBook.csv", "w");
		fopen_s(&out_TBtableDirectlaborcostBook, "..\\FolderForIOFiles\\data\\wholesalerTotal\\TBtable\\out_TBtableDirectlaborcostBook.csv", "w");
		fopen_s(&out_TBtableDirectmaterialfeeBook, "..\\FolderForIOFiles\\data\\wholesalerTotal\\TBtable\\out_TBtableDirectmaterialfeeBook.csv", "w");
		fopen_s(&out_TBtableProfitBook, "..\\FolderForIOFiles\\data\\wholesalerTotal\\TBtable\\out_TBtableProfitBook.csv", "w");
		fopen_s(&out_priceBook, "..\\FolderForIOFiles\\data\\wholesalerTotal\\out_priceBook.csv", "w");
	}
};
