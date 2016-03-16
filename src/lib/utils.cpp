//
// 雑多な関数
//

#include "utils.hpp"


// 切り上げて一番近い２のべき乗値を求める
int int2pow(const int value) {
	int res = 1;

	while (res < (1 << 30)) {
		if (res >= value) break;
		res *= 2;
	}

	return res;
}
