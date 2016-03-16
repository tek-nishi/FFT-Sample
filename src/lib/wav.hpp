
#pragma once

//
// wavデータを扱う
//

#include "defines.hpp"
#include <string>
#include <vector>
#include <fstream>


class Wav {
public:
  struct Info {
    u_int id;
    u_int ch;
    u_int sample_rate;
    u_int bit;
    u_int size;
  };


  explicit Wav(const std::string& file);

  // チャンネル数を返す
	u_int channel() const;

  // データがステレオならtrueを返す
  bool isStereo() const;

  // サンプリングレートを返す
  u_int sampleRate() const;

  // データサイズ(バイト数)を返す
	u_int size() const;

  // 再生時間(秒)を返す
  float time() const;

  // 波形データを返す
	const char* data() const;

  
  // wavの情報を取得
  static bool analyzeWavFile(Info& info, std::ifstream& fstr);


private:
  Info info;
  float time_;
  std::vector<char> data_;

  
  // 指定バイト数のメモリの内容をint値にする
  static u_int getValue(const char* ptr, const u_int num);
  
  // wavの指定チャンクを探す
  static bool searchChunk(std::ifstream& fstr, const char* chunk);

  // チャンクのサイズを取得
  static u_int getChunkSize(std::ifstream& fstr);
  
};

