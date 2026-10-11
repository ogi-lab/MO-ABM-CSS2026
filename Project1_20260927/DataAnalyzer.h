#pragma once
#include "BoughtRecord.h"
#include "SuppliedRecord.h"
#include "StockRecord.h"
#include "AreaMarketRecord.h"
// 双方向参照のため、ダミーのクラス定義
class Item;
//class BoughtRecord;

class DataAnalyzer{

//	boughtRecord=new BoughtRecord();
public:
	DataAnalyzer(){}
	~DataAnalyzer(){}

	virtual void add_item(Item* item)=0;
	virtual void print_boughtitems(int agent_id,int period)=0;
	virtual int number_of_bought_items(int agent_id,int period,int item_type_id)=0;
	virtual void set_boughtItem(int period, int item_type_id, int price, int seller_id, int buyer_id)=0;
	virtual void set_stockItem(int period, int item_type_id, int price, int seller_id)=0;
	virtual void set_suppliedItem(int period, int item_type_id, int price, int seller_id, int qmax)=0;
	virtual void set_areamarketItem(int period, int item_type_id, int price, int seller_id)=0;
	//virtual void print_supplieditems(int agent_id,int period)=0;
	//virtual int number_of_supplied_items(int agent_id,int period,int item_type_id)=0;
protected:
	/* MarketDataAnalyzerに移管20210815
	BoughtRecord boughtRecord;
	SuppliedRecord suppliedRecord;
	StockRecord stockRecord;
	AreaMarketRecord areamarketRecord;
	*/
	virtual void set_boughtItem(struct BoughtItem* boughtItem) = 0;
	virtual void set_suppliedItem(struct SuppliedItem* suppliedItem)=0;
	virtual void set_stockItem(struct StockItem* stockItem) = 0;
public:
	virtual int get_ave_price(int period,int item_type_id){return 0;}
	virtual void set_dataAnalyzer_type(int dataAnalyze_type) = 0;
	virtual int get_dataAnalyzer_type() { return 9; }
};

