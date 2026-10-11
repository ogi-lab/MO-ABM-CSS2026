//CData.h
#ifndef INC_CDATA
#define INC_CDATA
#include <stdio.h>
#include <vector>
//#include "CAccount.h"
using namespace std;

class CData
{
	vector <int > statistics_kyuyo;	//もらった給与をその場で記憶してプリントアウト（いらなければ捨てる）
	vector <int > kyuyo_no;			//statistics_kyuyoのエージェント番号保存
	vector <int > sort_static_kyuyo;//給与のソートしたエージェントナンバー
	vector <int > statistics_income;	//給与の総計
	vector <int > new_inmoney;
	vector <CProduct_class *> mem_class_statics;//市場の消費者の購入製品別統計データの集合
	vector <CProduct_class *> mem_class_statics_w;//ホールセラーの購入製品別統計データ集合
	vector <CProduct_class *>::iterator del_pc;
	vector <CProduct_class *> mem_class_stock_statics;//市場のリテイラー在庫製品別統計データの集合
	vector <CProduct_class *> mem_class_stock_statics_w;//市場のホールセラー在庫製品別統計データ集合


public:
	void 


};

#endif