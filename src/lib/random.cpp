//
// 乱数
//

#include "random.hpp"
#include <cassert>


Random::Random() :
  engine_(std::mt19937::default_seed),
  dist_zero_to_one_(0.0f, 1.0f)
{}


void Random::setSeed(const u_int new_seed) {
  engine_.seed(new_seed);
}
  
// [0, last) を返す
int Random::operator()(const int last) {
  assert((last > 0) && "The value has to be bigger than 0.");
  return engine_() % last;
}

// [first, last] を返す
int Random::operator()(const int first, const int last) {
  assert((first <= last) && "The last value has to be same or bigger than first value.");
  return first + operator()(last - first + 1);
}

  
// [0.0f, 1.0f] を返す
float Random::operator()() {
  return dist_zero_to_one_(engine_);
}

// [first, last] を返す
float Random::operator()(const float first, const float last) {
  assert((first <= last) && "The last value has to be same or bigger than first value.");
  return first + (last - first) * operator()();
}
