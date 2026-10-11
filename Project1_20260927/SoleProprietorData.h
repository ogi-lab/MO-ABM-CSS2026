#pragma once
#include <cstdio>
#include "AccountData.h"

class SoleProprietorData : public AccountData {
public:
	SoleProprietorData() {
		FILE_set();
	}
	void FILE_set() {
		//------------------------------------------------
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
		//---------------------------
		fopen_s(&out_title_list, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_title_list.csv", "w");
		fopen_s(&out_GL, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_GL.csv", "w");
		fopen_s(&out_TBtable, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_TBtable.csv", "w");
		fopen_s(&out_agent_TBtable, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_agent_TBtable.csv", "w");
		fopen_s(&out_cashbook, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_cashbook.csv", "w");
		fopen_s(&out_cashbookTotal, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_cashbookTotal.csv", "w");
		fopen_s(&out_bankbook, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_bankbook.csv", "w");
		fopen_s(&out_bankbookTotal, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_bankbookTotal.csv", "w");
		fopen_s(&out_stockbook, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_stockbook.csv", "w");
		fopen_s(&out_stockbookTotal, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_stockbookTotal.csv", "w");
		fopen_s(&out_stockpricebook, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_stockpricebook.csv", "w");
		fopen_s(&out_stockpricebookTotal, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_stockpricebookTotal.csv", "w");
		fopen_s(&out_balance, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_balance.csv", "w");

		fopen_s(&out_materialbook, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_materialbook.csv", "w");
		fopen_s(&out_materialbookTotal, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_materialbookTotal.csv", "w");
		fopen_s(&out_materialpricebook, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_materialpricebook.csv", "w");
		fopen_s(&out_materialpricebookTotal, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_materialpricebookTotal.csv", "w");

		fopen_s(&out_genkabook, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_genkabook.csv", "w");
		fopen_s(&out_genkabookTotal, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_genkabookTotal.csv", "w");
		fopen_s(&out_profit, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_profit.csv", "w");
		fopen_s(&out_TBtableCashBook, "..\\FolderForIOFiles\\data\\soleproprietorData\\TBtable\\out_TBtableCashBook.csv", "w");
		fopen_s(&out_TBtableBankBook, "..\\FolderForIOFiles\\data\\soleproprietorData\\TBtable\\out_TBtableBankBook.csv", "w");
		fopen_s(&out_TBtableInventoryBook, "..\\FolderForIOFiles\\data\\soleproprietorData\\TBtable\\out_TBtableInventoryBook.csv", "w");
		fopen_s(&out_TBtableSaleBook, "..\\FolderForIOFiles\\data\\soleproprietorData\\TBtable\\out_TBtableSaleBook.csv", "w");
		fopen_s(&out_TBtableDirectlaborcostBook, "..\\FolderForIOFiles\\data\\soleproprietorData\\TBtable\\out_TBtableDirectlaborcostBook.csv", "w");
		fopen_s(&out_TBtableDirectmaterialfeeBook, "..\\FolderForIOFiles\\data\\soleproprietorData\\TBtable\\out_TBtableDirectmaterialfeeBook.csv", "w");
		fopen_s(&out_TBtableProfitBook, "..\\FolderForIOFiles\\data\\soleproprietorData\\TBtable\\out_TBtableProfitBook.csv", "w");

		fopen_s(&out_deposit_withdrawalBook, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_deposit_withdrawalBook.csv", "w");
		fopen_s(&out_quantity_priceBook, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_quantity_priceBook.csv", "w");
		//fopen_s(&out_price, "..\\FolderForIOFiles\\data\\soleproprietorData\\out_price.csv", "w");
	}

};