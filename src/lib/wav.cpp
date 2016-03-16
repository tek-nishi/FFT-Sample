
//
// wavデータを扱う
//

#include "wav.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>


Wav::Wav(const std::string& file) {
  std::ifstream fstr(file, std::ios::binary);
  if (!fstr) {
    DOUT << "Can't open:" << file << std::endl;
    throw;
  }
    
  // ファイル情報を解析
  if (!analyzeWavFile(info, fstr)) {
    throw "This file isn't WAV.";
  }

  if ((info.id != 1) || (info.bit != 16)) {
    // IDが１で量子化ビット数が16以外は扱わない
    DOUT << "Wav format error. " << info.id << " " << info.bit << std::endl;
    throw;
  }
    
  // 再生時間(秒)
  time_ = info.size / info.ch / 2.0f / info.sample_rate;

  // データ読み込み
  data_.resize(info.size);
  searchChunk(fstr, "data");
  fstr.seekg(4, fstr.cur);      
  fstr.read(&data_[0], info.size);
}

// チャンネル数を返す
u_int Wav::channel() const { return info.ch; }

// データがステレオならtrueを返す
bool Wav::isStereo() const { return info.ch == 2; }

// サンプリングレートを返す
u_int Wav::sampleRate() const { return info.sample_rate; }

// データサイズ(バイト数)を返す
u_int Wav::size() const { return info.size; }

// 再生時間(秒)を返す
float Wav::time() const { return time_; }

// 波形データを返す
const char* Wav::data() const { return &data_[0]; }


// 指定バイト数のメモリの内容をint値にする
u_int Wav::getValue(const char* ptr, const u_int num) {
  u_int value = 0;

  for (u_int i = 0; i < num; ++i, ++ptr) {
    // TIPS:int型より小さい型はint型に拡張されてから計算されるので8bit以上シフトしても問題ない
    value = value + (static_cast<u_char>(*ptr) << (i * 8));
  }
#ifdef __BIG_ENDIAN__
  value = (value << 24) | ((value << 8) & 0xff0000) | ((value >> 8) & 0xff00) | (value >> 24);
#endif
			
  return value;
}
  
// wavの指定チャンクを探す
bool Wav::searchChunk(std::ifstream& fstr, const char* chunk) {
  enum {
    // チャンクが始まる位置
    WAV_HEADER_SIZE = 12
  };
    
  fstr.seekg(WAV_HEADER_SIZE, fstr.beg);
  // チャンクの並びは不定なので、常にファイルの先頭から探す

  while (1) {
    char tag[4];
    fstr.read(tag, 4);
    if (!std::strncmp(tag, chunk, 4)) {
      return true;
    }

    // 次のチャンクへ
    char data[4];
    fstr.read(data, 4);
    u_int chunk_size = getValue(data, 4);
    fstr.seekg(chunk_size, fstr.cur);
    
    if (fstr.eof()) break; 
  }
  return false;
}

// チャンクのサイズを取得
u_int Wav::getChunkSize(std::ifstream& fstr) {
  char data[4];
  fstr.read(data, 4);
  return getValue(data, 4);
}

// wavの情報を取得
bool Wav::analyzeWavFile(Info& info, std::ifstream& fstr) {
  // ファイルがwav形式か判別
  enum {
    WAV_HEADER_SIZE = 12
  };
    
  char header[WAV_HEADER_SIZE];
  fstr.read(header, WAV_HEADER_SIZE);
  if (std::strncmp(&header[0], "RIFF", 4)) {
    DOUT << "This file isn't RIFF format." << std::endl;
    return false;
  }
  if (std::strncmp(&header[8], "WAVE", 4)) {
    DOUT << "This file isn't WAVE format." << std::endl;
    return false;
  }
      
  enum {
    // fmtチャンク内のデータ位置
    WAV_ID          = 0,
    WAV_CH          = WAV_ID + 2,
    WAV_SAMPLE_RATE = WAV_CH + 2,
    WAV_BPS         = WAV_SAMPLE_RATE + 4,
    WAV_BLOCK_SIZE  = WAV_BPS + 4,
    WAV_BIT         = WAV_BLOCK_SIZE + 2,
  };

  // fmtチャンクを探してデータ形式を取得
  if (!searchChunk(fstr, "fmt ")) {
    DOUT << "No chank 'fmt'." << std::endl;
    return false;
  }
  u_int chunk_size = getChunkSize(fstr);
  std::vector<char> chunk(chunk_size);
  fstr.read(&chunk[0], chunk_size);

  info.id = getValue(&chunk[WAV_ID], 2);
  info.ch = getValue(&chunk[WAV_CH], 2);
  info.sample_rate = getValue(&chunk[WAV_SAMPLE_RATE], 4);
  info.bit = getValue(&chunk[WAV_BIT], 2);

  // dataチャンクを探してデータ長を取得
  if (!searchChunk(fstr, "data")) {
    DOUT << "No chank 'data'." << std::endl;
    return false;
  }
  info.size = getChunkSize(fstr);

  return true;
}
  
