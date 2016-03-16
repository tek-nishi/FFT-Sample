#pragma once

//
// Game Pad
// FIXME:アプリ起動後の抜き差しには対応していません
//

#include "defines.hpp"
#include <GLFW/glfw3.h>
#include <vector>


class GamePad {
  int id_;

  // ボタン数と軸数
  int button_num_;
  int axis_num_;

  // ボタンの Press / Push / Pull の状況
  std::vector<u_char> press_button_;
  std::vector<u_char> push_button_;
  std::vector<u_char> pull_button_;

  // 軸の状況
  std::vector<float> axis_value_;

  // 2軸を簡易ボタンとして扱うための変数
  bool  axis_button_;
  float axis_threshold_;
  int   axis_x_index_;
  int   axis_y_index_;

  // 簡易ボタンの Press / Push / Pull の状況
  std::vector<u_char> press_axis_button_;
  std::vector<u_char> push_axis_button_;
  std::vector<u_char> pull_axis_button_;
  
  
public:
  // 簡易ボタンの定義
  enum {
    AXIS_UP,
    AXIS_DOWN,
    AXIS_LEFT,
    AXIS_RIGHT,

    AXIS_BUTTON_NUM
  };

  
  explicit GamePad(const int id);

  // GamePadのボタン数と軸数
  int buttons() const;
  int axes() const;

  // ボタンの Press / Push / Pull の状況
  bool isPressButton(const int index) const;
  bool isPushButton(const int index) const;
  bool isPullButton(const int index) const;

  // 軸の状況
  float axis(const int index) const;

  // 2軸を簡易ボタンとして登録
  // x_index        左右ボタンとみなす軸
  // y_index        上下ボタンとみなす軸
  // axis_threshold どこまで倒したら押下とみなすかの値[0.0, 1.0]
  // 戻り値         true なら設定成功
  bool registAxisButton(const int x_index, const int y_index, const float axis_threshold);

  // 簡易ボタン判定中止
  void disableAxisButton();

  // 簡易ボタンが有効ならtrueを返す
  bool isEnableAxisButton() const;
  
  // 簡易ボタンの Press / Push / Pull の状況
  bool isPressAxisButton(const int index) const;
  bool isPushAxisButton(const int index) const;
  bool isPullAxisButton(const int index) const;
  
  // GamePadが有効ならtrueを返す
  bool isPresent() const;

  // 入力をクリア
  void flush();
  
  // GamePad 内部状態の更新
  void update();
};


// PCに繋がれたGamePadの情報を収集
std::vector<GamePad> initGamePad();

// GamePadの内部状態をまとめて更新
void updateGamePad(std::vector<GamePad>& gamepads);

// コンテナのGamePadの状態をまとめてクリア
void flushGamePad(std::vector<GamePad>& gamepads);
