#pragma once
#include "Item.h"
using namespace std;

/** 商品の生成順（item_seq）による比較のためのプレディケート
 *
 *  Producer(Retailer/Wholesaler/SoleProprietor)の自身の在庫集合(stock_map)を
 *  set<Item*>にデフォルトのポインタ比較(operator<)で持たせると、集合の走査順が
 *  Itemオブジェクトのヒープアドレスに依存してしまう。ヒープアドレスは同一の
 *  ソース・同一シードでも実行のたび（プロセス起動ごと）に異なりうるため、
 *  在庫を走査しながら価格をランダムに揺らす処理（renew_price_in_market()等、
 *  price_delta>0の生産者でループ内でRandomGeneratorを呼び出す実装）と組み合わさると、
 *  ランダムな呼び出し結果の割り当て先が実行ごとに異なり、同一シード・同一コードでも
 *  シミュレーション結果が再現しない（同じ乱数の呼び出し回数は一致するのに、
 *  結果が食い違う）という不具合の原因になる。
 *
 *  このプレディケートは生成順（item_seq、Itemの生成時に単調増加のカウンタから
 *  割り当てる連番）で比較することで、ヒープアドレスに関係なく、常に生成された順に
 *  決定的に走査されるようにする。
 */
class PredForItemBySeq {
public:
    bool operator()(Item* left, Item* right) const {
        return left->get_item_seq() < right->get_item_seq();
    }
};
