#pragma once

//
// 雑多な関数
//

#include "defines.hpp"
#include <cmath>
#include <algorithm>


// radian → degree
template <typename RealValue>
RealValue radToDeg(const RealValue rad) {
	return rad * 180 / M_PI;
}

// degree → radian
template <typename RealValue>
RealValue degToRad(const RealValue deg) {
	return deg * M_PI / 180;
}


// 配列の要素数を取得
template <typename T>
std::size_t elemsof(const T& value) {
  return std::end(value) - std::begin(value);
}

// 切り上げて一番近い２のべき乗値を求める
int int2pow(const int value);
