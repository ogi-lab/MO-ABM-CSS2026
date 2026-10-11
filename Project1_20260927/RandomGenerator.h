#pragma once

#include <random>
#include <algorithm>
#include <utility>
#include <stdio.h>
#include "SimulatorContext.h"

/** 乱数生成クラス
 *
 * std::mt19937（メルセンヌ・ツイスタ）を単一の乱数エンジンとして
 * クラス全体で共有する。乱数の生成・分布への変換をすべて<random>の
 * 機能に委ねることで、旧rand()実装にあった
 *   ・生成器自体の統計的な偏り（周期の短さ、下位ビットの相関など）
 *   ・rand() % n による剰余バイアス
 *   ・処理系やコンパイラによってrand()の実装・RAND_MAXが異なる問題
 * を解消し、乱数生成過程を完全に自己完結・再現可能にしている。
 */
extern SimulatorContext app_context;

class RandomGenerator {
public:
    RandomGenerator() {}

    // 全ての呼び出しが共有する単一のmt19937エンジン（クラス静的）
    static std::mt19937& engine() {
        static std::mt19937 eng;
        return eng;
    }

    // 関数オブジェクト：[0, max) の範囲の一様乱数を返す
    unsigned int operator()(unsigned int max) {
        if (max == 0) return 0;
        std::uniform_int_distribution<unsigned int> dist(0, max - 1);
        return dist(engine());
    }

    // 定義はmain.cppに置く。SimulatorConfig.hの先頭でRandomGenerator.hが
    // includeされる関係上、このファイル単体ではSimulatorConfigがまだ
    // 前方宣言のみで完全型になっていないため、ここでapp_context.config->seedに
    // アクセスすることはできない。SimulatorConfig.hの読み込みが完了した後で
    // 定義する必要があるため、本体はmain.cppに置く。
    static void set_seed();

    /**
     * 一様乱数
     *
     * 最小値から最大値までの一様乱数を吐き出す(最小値、最大値を含む)
     *
     *  @param min 最小値
     *  @param max 最大値
     */
    static int random_int(int min, int max) {
        if (min == max) {
            return min;
        }
        else {
            std::uniform_int_distribution<int> dist(min, max);
            return dist(engine());
        }
    }
    static long long random_int(long long min, long long max) {
        if (min == max) return min;
        else {
            std::uniform_int_distribution<long long> dist(min, max);
            return dist(engine());
        }
    }
    static double random_double(double min, double max) {
        if (min == max) return min;
        else {
            std::uniform_real_distribution<double> dist(min, max);
            return dist(engine());
        }
    }

    /**
     * 一様乱数によるシャッフル（Fisher-Yates）
     * std::shuffle に、クラスで共有しているmt19937エンジンを渡して実行する。
     */
    template<typename RandomIt>
    static void shuffle(RandomIt first, RandomIt last) {
        std::shuffle(first, last, engine());
    }
};
