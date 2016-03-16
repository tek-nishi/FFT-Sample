
#pragma once

//
// OpenAL管理
//

#include "defines.hpp"
#include <string>
#include <memory>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include "vector.hpp"


// OpenALの初期化と後始末を代行
class Audio {
  ALCdevice*  device_;
  ALCcontext* context_;

  
public:
  Audio();
  ~Audio();

  // このクラスはコピー禁止
  Audio(const Audio&) = delete;
  Audio& operator=(const Audio&) = delete;

  
  // リスナーの位置を変更
  // x, y, z →位置
  static void lisnerPosition(const float x, const float y, const float z);

  // pos →位置
  static void lisnerPosition(const Vec3f& pos);

  // リスナーの向きを変更
  // at_x, at_y, at_z →向いている方向をあらわすベクトル
  // up_x, up_y, up_z →上方向をあらわすベクトル
  static void lisnerOrientation(const float at_x, const float at_y, const float at_z,
                                const float up_x, const float up_y, const float up_z);

  // at →向いている方向をあらわすベクトル
  // up →上方向をあらわすベクトル
  static void lisnerOrientation(const Vec3f& at, const Vec3f& up);

  // リスナーの移動速度を変更
  // x, y, z →移動速度
  static void lisnerVerocity(const float x, const float y, const float z);

  // vec →移動速度
  static void lisnerVerocity(const Vec3f& vec);

};


// バッファの管理を代行
class Buffer {
  ALuint id_;
  float duration_sec_;


public:
  Buffer();
  explicit Buffer(const std::string& path);
  ~Buffer();

  // このクラスはコピー禁止
  Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;

  
  // バッファの識別子
  ALuint id() const;

  // 再生時間(秒)
  float duration() const;

  // 波形データを割り当て
  // TIPS:割り当て後、dataの内容は破棄できる
  void bind(const bool stereo, const void* data, const u_int size, const u_int rate) const;
};


// ソースの管理を代行
class Source {
  ALuint id_;


public:
  Source();
  ~Source();

  // このクラスはコピー禁止
  Source(const Source&) = delete;
  Source& operator=(const Source&) = delete;

  
  ALuint name() const;

  
  // ソースにバッファを割り当てる
  void bindBuffer(const Buffer& buffer) const;

  // ソースに割り当てられたバッファを解除
  void unbindBuffer() const;


  // 再生開始
  void play() const;

  // 再生停止
  void stop() const;

  // 一時停止(この後、再生すると続きからになる)
  void pause() const;

  // 音量変更(value: 0.0f~)
  void gain(const float value) const;

  // 再生ピッチ変更(value: 0.0f~)
  void pitch(const float value) const;

  // ループのON/OFF
  // value: trueでループON、falseでループOFF
  void looping(const bool value) const;

  // ソース位置の変更
  // x, y, z →位置
  void position(const float x, const float y, const float z) const;

  // pos →位置
  void position(const Vec3f& pos) const;

  // ソースの移動速度を変更
  // x, y, z →移動速度
  void verocity(const float x, const float y, const float z) const;

  // pos →移動速度
  void verocity(const Vec3f& vec) const;

  // 再生中??
  bool isPlaying() const;

  // 再生位置(秒)
  float currentTime() const;

  // Bufferの再生キューイング
  void queueBuffer(const Buffer& buffer) const;
  // 再生完了Bufferのid取得
  ALuint unqueueBuffer() const;

  // キューイングされたBufferの再生完了数を取得
  int processed() const;
  
};


class Media {
  std::shared_ptr<Buffer> buffer_;
  std::shared_ptr<Source> source_;
  

public:
  Media();

  explicit Media(const std::string& path);
  ~Media();

  // 再生開始
  void play() const;

  // 再生停止
  void stop() const;

  // 一時停止(この後、再生すると続きからになる)
  void pause() const;

  // 音量変更(value: 0.0f~)
  void gain(const float value) const;

  // 再生ピッチ変更(value: 0.0f~)
  void pitch(const float value) const;

  // ループのON/OFF
  // value: trueでループON、falseでループOFF
  void looping(const bool value) const;

  // ソース位置の変更
  // x, y, z →位置
  void position(const float x, const float y, const float z) const;

  // pos →位置
  void position(const Vec3f& pos) const;

  // ソースの移動速度を変更
  // x, y, z →移動速度
  void verocity(const float x, const float y, const float z) const;

  // pos →移動速度
  void verocity(const Vec3f& vec) const;

  // 再生中??
  bool isPlaying() const;

  // 再生位置(秒)
  float currentTime() const;

  // 再生時間(秒)
  float duration() const;

};
