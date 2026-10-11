#pragma once
#include "MarketBuyDealer.h"
#include "DirectBuyDealer.h"
class RollAggregator{
	MarketBuyDealer* marketBuyDealer;
	DirectBuyDealer* directBuyDealer;
public:
	RollAggregator(){
		marketBuyDealer= new MarketBuyDealer();//Buydealerを継承、今は何も関数変数が定義されていない
		directBuyDealer= new DirectBuyDealer();//Buydealerを継承、今は何も関数変数が定義されていない
	
	};
	MarketBuyDealer* getMarketBuyDealer(){return marketBuyDealer;}
};