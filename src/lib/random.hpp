#pragma once

//
// 乱数
//

#include "defines.hpp"
#include <random>


class Random {
  std::mt19937 engine_;
  std::uniform_real_distribution<float> dist_zero_to_one_;

  
public:
  Random();


  void setSeed(const u_int new_seed);
  
  // [0, last) を返す
  // lastは0より大きい値であること
  int operator()(const int last);

  // [first, last] を返す
  // first < last であること
  int operator()(const int first, const int last);
  
  // [0.0f, 1.0f] を返す
  float operator()();

  // [first, last] を返す
  // first < last であること
  float operator()(const float first, const float last);
};
