#pragma once

// 双方向参照のため、ダミーのクラス定義
class Item;

/** 購入者 */
class Buyer {
public:
    virtual ~Buyer(){}
	virtual int Get_agent_id()=0;
    /**
     *  支払
     *
     *  @param item 商品
     */
    virtual void pay(Item* item) = 0;
    virtual int Get_AgentType() = 0;

};

