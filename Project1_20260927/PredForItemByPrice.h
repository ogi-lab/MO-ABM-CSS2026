#pragma once
#include "Item.h"
using namespace std;

/** 価格による比較のためのプリディケート */
class PredForItemByPrice { //: public binary_function<Item*, Item*, bool> {
public:
    bool operator()(Item* left, Item* right ) const {
        return left->get_price() < right->get_price();
    }
};
