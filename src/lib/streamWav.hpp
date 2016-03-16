#pragma once

//
// Wavのストリーミング再生
//

#include "defines.hpp"
#include <string>
#include <fstream>
#include <vector>
#include "wav.hpp"


class StreamWav {
public:
  explicit StreamWav(const std::string& file);
  
  bool isStereo() const;
  u_int sampleRate() const;
  
  void loop(const bool loop);

  // 再生位置を先頭に戻す
  void toTop();

  bool isEnd() const;
  
  // 再生バッファにデータを読み込む
  size_t read(std::vector<char>& buffer);

  
private:
  std::ifstream fstr_;

  Wav::Info info;
  size_t top_pos_;

  bool loop_;

  size_t last_size_;

  
  // ファイルからバッファへデータを読み込む
  size_t readData(std::vector<char>& buffer, const size_t offset, const size_t size);
  
};
