#pragma once

// 
// libpngによる画像読み書き
//
// PNG_COLOR_TYPE_RGB		    RGB
// PNG_COLOR_TYPE_RGB_ALPHA	RGBA
//

#include "defines.hpp"
#include <string>
#include <vector>
#include <png.h>


class Png {
	enum {
    PNG_SIG_HEADER = 8,
    // PNGかどうかを判別するために読み込むデータ量

    PNG_COLOR_TYPE_NONE = -1
  };

	int type_;
	int width_;
	int height_;
	std::vector<u_char> image_;

  
public:
	Png(const std::string& path);

  
  // 画像タイプ
  // PNG_COLOR_TYPE_RGB
  // PNG_COLOR_TYPE_RGB_ALPHA
  int type() const;

  // サイズ
  int width() const;
  int height() const;

  // ピクセルデータ
  const u_char* image() const;


private:
  // png読み込み時のコールバック
  static void readFunc(png_struct* hdl, png_bytep buf, png_size_t size);
  
};


// RGB8で書き出し
void WritePng(const std::string& path, const u_int width, const u_int height, u_char* image);
