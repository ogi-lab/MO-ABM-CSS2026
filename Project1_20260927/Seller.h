#pragma once
#include <string>
/** 販売者  */
/*struct journalSlip { // 仕訳帳の一行分
    int period = 0;
    long long dabit = 0;//借方金額
    long long credit = 0;//貸方金額
    string Dr_title;//借方科目
    string Cr_title;//貸方科目
    int Dr_code = 0;//借方科目区分
    int Cr_code = 0;//貸方科目区分
    string description; //摘要

};*/
using namespace std;
class Seller {
public:
    virtual ~Seller() {};
	virtual int Get_agent_id()=0;
    virtual int Get_AgentType() = 0;
    virtual void createjournalSlip(int period, int Dr_code, string Dr_title, long long dabit, string description, long long credit, string Cr_title, int Cr_code) {}
    /**
	 *  支払金額受領
     *
     *  @param item 商品
     */
    virtual void receive_payments(Item* item) {};

    /**
     *  在庫追加
     *
     *  @param item 商品
     */
    virtual void add_stock(Item* item) = 0;

    /**
     *  在庫削除
     *
     *  @param item 商品
     */
    virtual void remove_stock(Item* item) = 0;
	virtual int get_qmax(int item_type_id)=0;
	/**
	 *売れた製品情報の格納
	 */
	virtual void set_soldItem(int period, int item_type_id, long long price)=0;//Defined in producer.h
    virtual void add_soldItem(Item* item) = 0;
};

