#pragma once

//
// ストリーミングによる再生
// FIXME:インスタンス生成時に自動的に再生を始める
//

#include "defines.hpp"
#include "audio.hpp"
#include "streamWav.hpp"
#include <string>
#include <vector>
#include <memory>
#include <mutex>


class Streaming {
  enum {
    BUFFER_NUM    = 2,
    SLEEP_TIME_MS = 250
  };

  // TIPS:再生スレッドよりStreamingのインスタンスが先に破棄されることがあるので
  //      shared_ptrを使う
  std::shared_ptr<Source> source_;
  bool pause_;

  // 再生スレッドとの連絡用
  struct Param {
    std::mutex mutex;
    bool stopped;

    Param() :
      stopped(false)
    {}
  };
  std::shared_ptr<Param> param_;

  
public:
  Streaming(const std::string& path, const bool loop = false);


  // gain [0.0, 1.0]
  void gain(const float gain);
  void pause(const bool pause);

  // 再生を止めて、スレッドを破棄する
  void stop();

  bool isPlaying();

  
private:
  // FIXME:読み込みバッファを引数で渡している
  static void queueStream(StreamWav& stream, Source& source, Buffer& buffer,
                          std::vector<char>& sound_buffer);
  
  // std::threadによる再生処理
  static void streamProc(const std::string path, const bool loop,
                         std::shared_ptr<Source> source, std::shared_ptr<Param> param);
};
