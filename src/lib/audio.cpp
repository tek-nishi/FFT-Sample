
//
// OpenAL管理
//

#include "audio.hpp"
#include <iostream>
#include <string>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include "wav.hpp"
#include "vector.hpp"


#if defined (_MSC_VER)
#pragma comment (lib, "openal32.lib")
#endif


Audio::Audio() {
  DOUT << "Audio()" << std::endl;
    
  // OpenALの初期化
  device_  = alcOpenDevice(nullptr);
  context_ = alcCreateContext(device_, nullptr);
  alcMakeContextCurrent(context_);
}

Audio::~Audio() {
  DOUT << "~Audio()" << std::endl;

  // OpenALの後始末
  alcMakeContextCurrent(nullptr);
  alcDestroyContext(context_);
  alcCloseDevice(device_);
}

  
// リスナーの位置を変更
// x, y, z →位置
void Audio::lisnerPosition(const float x, const float y, const float z) {
  ALfloat listener_pos[] = { x, y, z };
  alListenerfv(AL_POSITION, listener_pos);
}

// pos →位置
void Audio::lisnerPosition(const Vec3f& pos) {
  lisnerPosition(pos.x(), pos.y(), pos.z());
}

// リスナーの向きを変更
// at_x, at_y, at_z →向いている方向をあらわすベクトル
// up_x, up_y, up_z →上方向をあらわすベクトル
void Audio::lisnerOrientation(const float at_x, const float at_y, const float at_z,
                              const float up_x, const float up_y, const float up_z) {
  ALfloat listener_orientation[] = { at_x, at_y, at_z,
                                     up_x, up_y, up_z };
  alListenerfv(AL_ORIENTATION, listener_orientation);
}

// at →向いている方向をあらわすベクトル
// up →上方向をあらわすベクトル
void Audio::lisnerOrientation(const Vec3f& at, const Vec3f& up) {
  lisnerOrientation(at.x(), at.y(), at.z(),
                    up.x(), up.y(), up.z());
}

// リスナーの移動速度を変更
// x, y, z →移動速度
void Audio::lisnerVerocity(const float x, const float y, const float z) {
  ALfloat listener_velocity[] = { x, y, z };
  alListenerfv(AL_VELOCITY, listener_velocity);
}

// vec →移動速度
void Audio::lisnerVerocity(const Vec3f& vec) {
  lisnerVerocity(vec.x(), vec.y(), vec.z());
}


Buffer::Buffer(const std::string& path) {
  DOUT << "Buffer()" << std::endl;

  // バッファを１つ確保
  alGenBuffers(1, &id_);

  // WAVファイルの読み込み
  Wav wav_data(path);
  duration_sec_ = wav_data.time();
  
  // 波形データをバッファにコピー
  alBufferData(id_,
               wav_data.isStereo() ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
               wav_data.data(),
               wav_data.size(),
               wav_data.sampleRate());
}

Buffer::Buffer() :
  duration_sec_(0.0f)
{
  DOUT << "Buffer()" << std::endl;

  // バッファを１つ確保
  alGenBuffers(1, &id_);
}

Buffer::~Buffer() {
  DOUT << "~Buffer()" << std::endl;

  // バッファの後始末
  alDeleteBuffers(1, &id_);
}

  
// バッファの識別子
ALuint Buffer::id() const { return id_; }
// 再生時間
float Buffer::duration() const { return duration_sec_; }

void Buffer::bind(const bool stereo,
                  const void* data, const u_int size, const u_int rate) const {
  alBufferData(id_, stereo ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, data, size, rate);
}


// ソースの管理を代行
Source::Source() {
  DOUT << "Source()" << std::endl;

  // ソースを１つ確保
  alGenSources(1, &id_);
}

Source::~Source() {
  DOUT << "~Source()" << std::endl;

  // ソースの後始末
  unbindBuffer();
  alDeleteSources(1, &id_);
}

  
ALuint Source::name() const { return id_; }

  
// ソースにバッファを割り当てる
void Source::bindBuffer(const Buffer& buffer) const {
  alSourcei(id_, AL_BUFFER, buffer.id());
}

// ソースに割り当てられたバッファを解除
void Source::unbindBuffer() const {
  alSourcei(id_, AL_BUFFER, 0);
}


// 再生開始
void Source::play() const {
  alSourcePlay(id_);
}

// 再生停止
void Source::stop() const {
  alSourceStop(id_);
}

// 一時停止(この後、再生すると続きからになる)
void Source::pause() const {
  alSourcePause(id_);
}

// 音量変更(value: 0.0f~)
void Source::gain(const float value) const {
  alSourcef(id_, AL_GAIN, value);
}

// 再生ピッチ変更(value: 0.0f~)
void Source::pitch(const float value) const {
  alSourcef(id_, AL_PITCH, value);
}

// ループのON/OFF
// value: trueでループON、falseでループOFF
void Source::looping(const bool value) const {
  alSourcei(id_, AL_LOOPING, value ? AL_TRUE : AL_FALSE);
}

// ソース位置の変更
// x, y, z →位置
void Source::position(const float x, const float y, const float z) const {
  ALfloat source_pos[] = { x, y, z };
  alSourcefv(id_, AL_POSITION, source_pos);
}

// pos →位置
void Source::position(const Vec3f& pos) const {
  position(pos.x(), pos.y(), pos.z());
}

// ソースの移動速度を変更
// x, y, z →移動速度
void Source::verocity(const float x, const float y, const float z) const {
  ALfloat source_velocity[] = { x, y, z };
  alSourcefv(id_, AL_VELOCITY, source_velocity);
}

// pos →移動速度
void Source::verocity(const Vec3f& vec) const {
  verocity(vec.x(), vec.y(), vec.z());
}

// 再生中??
bool Source::isPlaying() const {
  ALint state;
  alGetSourcei(id_, AL_SOURCE_STATE, &state);
  return state == AL_PLAYING;
}

// 再生位置(秒)
float Source::currentTime() const {
  ALfloat current_time_sec;
  alGetSourcef(id_, AL_SEC_OFFSET, &current_time_sec);
  return current_time_sec;
}


void Source::queueBuffer(const Buffer& buffer) const {
  ALuint buffers = buffer.id();
  alSourceQueueBuffers(id_, 1, &buffers);
}

ALuint Source::unqueueBuffer() const {
  ALuint buffer;
  alSourceUnqueueBuffers(id_, 1, &buffer);

  return buffer;
}

int Source::processed() const {
  int processed;
  alGetSourcei(id_, AL_BUFFERS_PROCESSED, &processed);
      
  return processed;
}


Media::Media() {}

Media::Media(const std::string& path) :
  buffer_(std::make_shared<Buffer>(path)),
  source_(std::make_shared<Source>())
{
  DOUT << "Media()" << std::endl;

  source_->bindBuffer(*buffer_);
}
  
Media::~Media() {
  DOUT << "~Media()" << std::endl;
}

  
// 再生開始
void Media::play() const {
  source_->play();
}

// 再生停止
void Media::stop() const {
  source_->stop();
}

// 一時停止(この後、再生すると続きからになる)
void Media::pause() const {
  source_->pause();
}

// 音量変更(value: 0.0f~)
void Media::gain(const float value) const {
  source_->gain(value);
}

// 再生ピッチ変更(value: 0.0f~)
void Media::pitch(const float value) const {
  source_->pitch(value);
}

// ループのON/OFF
// value: trueでループON、falseでループOFF
void Media::looping(const bool value) const {
  source_->looping(value);
}

// ソース位置の変更
// x, y, z →位置
void Media::position(const float x, const float y, const float z) const {
  source_->position(x, y, z);
}

// pos →位置
void Media::position(const Vec3f& pos) const {
  source_->position(pos);
}

// ソースの移動速度を変更
// x, y, z →移動速度
void Media::verocity(const float x, const float y, const float z) const {
  source_->verocity(x, y, z);
}

// pos →移動速度
void Media::verocity(const Vec3f& vec) const {
  source_->verocity(vec);
}

// 再生中??
bool Media::isPlaying() const {
  return source_->isPlaying();
}

// 再生位置(秒)
float Media::currentTime() const {
  return source_->currentTime();
}

// 再生時間(秒)
float Media::duration() const {
  return buffer_->duration();
}
