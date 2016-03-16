//
// Game Pad
// FIXME:アプリ起動後の抜き差しには対応していません
//

#include "gamePad.hpp"
#include <iostream>
#include <cassert>


GamePad::GamePad(const int id) :
  id_(id),
  axis_button_(false),
  press_axis_button_(AXIS_BUTTON_NUM),
  push_axis_button_(AXIS_BUTTON_NUM),
  pull_axis_button_(AXIS_BUTTON_NUM)
{
  glfwGetJoystickButtons(id_, &button_num_);
  glfwGetJoystickAxes(id_, &axis_num_);

  DOUT << "GamePad: id:" << id_ << " button:" << button_num_ << " axis:" << axis_num_ << std::endl;

  // ボタンの数に応じた変数の初期化
  press_button_.resize(button_num_);
  std::fill(std::begin(press_button_), std::end(press_button_), 0);
  
  push_button_.resize(button_num_);
  std::fill(std::begin(push_button_), std::end(push_button_), 0);
  
  pull_button_.resize(button_num_);
  std::fill(std::begin(pull_button_), std::end(pull_button_), 0);

  axis_value_.resize(axis_num_);
  std::fill(std::begin(axis_value_), std::end(axis_value_), 0.0f);
}

// ボタン数を返す
int GamePad::buttons() const { return button_num_; }

// 軸数を返す
int GamePad::axes() const { return axis_num_; }

// 指定ボタンがPress状態か返す
// index = ボタン番号
bool GamePad::isPressButton(const int index) const {
  return press_button_[index];
}

// 指定ボタンがPush状態か返す
// index = ボタン番号
bool GamePad::isPushButton(const int index) const {
  return push_button_[index];
}

// 指定ボタンがPull状態か返す
// index = ボタン番号
bool GamePad::isPullButton(const int index) const {
  return pull_button_[index];
}

// 指定軸の倒れ具合を返す
// index = 軸番号
float GamePad::axis(const int index) const {
  return axis_value_[index];
}


// 2軸を簡易ボタンとして登録
// x_index        左右ボタンとみなす軸番号
// y_index        上下ボタンとみなす軸番号
// axis_threshold どこまで倒したら押下とみなすかの値[0.0, 1.0]
// 戻り値         true なら設定成功
bool GamePad::registAxisButton(const int x_index, const int y_index, const float axis_threshold) {
  // 軸番号が範囲外の場合は設定しない
  if ((x_index >= axis_num_) || (y_index >= axis_num_)) return false;
  
  axis_button_ = true;
  axis_threshold_ = axis_threshold;

  axis_x_index_ = x_index;
  axis_y_index_ = y_index;

  std::fill(std::begin(press_axis_button_), std::end(press_axis_button_), 0);
  std::fill(std::begin(push_axis_button_), std::end(push_axis_button_), 0);
  std::fill(std::begin(pull_axis_button_), std::end(pull_axis_button_), 0);

  return true;
}

// 簡易ボタン判定中止
void GamePad::disableAxisButton() {
  axis_button_ = false;
}

// 簡易ボタンが有効ならtrueを返す
bool GamePad::isEnableAxisButton() const {
  return axis_button_;
}

// 簡易ボタンがPress状態か返す
// index = ボタン番号
bool GamePad::isPressAxisButton(const int index) const {
  return press_axis_button_[index];
}

// 簡易ボタンがPush状態か返す
// index = ボタン番号
bool GamePad::isPushAxisButton(const int index) const {
  return push_axis_button_[index];
}

// 簡易ボタンがPull状態か返す
// index = ボタン番号
bool GamePad::isPullAxisButton(const int index) const {
  return pull_axis_button_[index];
}

// GamePadが有効ならtrueを返す
bool GamePad::isPresent() const {
  return (glfwJoystickPresent(id_) == GL_TRUE) ? true : false;
}
  // 入力をクリア
void GamePad::flush() {
  std::fill(std::begin(press_button_), std::end(press_button_), 0);
  std::fill(std::begin(push_button_), std::end(push_button_), 0);
  std::fill(std::begin(pull_button_), std::end(pull_button_), 0);
  std::fill(std::begin(axis_value_), std::end(axis_value_), 0.0f);

  std::fill(std::begin(press_axis_button_), std::end(press_axis_button_), 0);
  std::fill(std::begin(push_axis_button_), std::end(push_axis_button_), 0);
  std::fill(std::begin(pull_axis_button_), std::end(pull_axis_button_), 0);
}


// 内部状態の更新
void GamePad::update() {
  // ボタンのpress状況を取得
  int button_num;
  const auto* buttons = glfwGetJoystickButtons(id_, &button_num);

  if (button_num > 0) {
    for (int i = 0; i < button_num; ++i) {
      // ボタンの Press / Push / Pull 情報を生成
      // Pushは押した瞬間、Pullは離した瞬間だけtrueになる
      push_button_[i] = !press_button_[i] && buttons[i];
      pull_button_[i] = press_button_[i] && !buttons[i];
      
      press_button_[i] = buttons[i];
    }
  }

  // 軸の状況を取得
  int axis_num;
  const auto* axes = glfwGetJoystickAxes(id_, &axis_num);
  if (axis_num > 0) {
    for (int i = 0; i < axis_num; ++i) {
      axis_value_[i] = axes[i];
    }

    if (axis_button_) {
      // 簡易ボタン向けの処理
      u_char buttons[AXIS_BUTTON_NUM];
      std::fill(std::begin(buttons), std::end(buttons), 0);

      // 簡易ボタンのpress状態を作成
      // それぞれ軸の値が閾値を超えたらPressとみなす
      // TIPS:右側の比較式の結果(true / false)をそのまま利用
      buttons[AXIS_RIGHT] = axis_value_[axis_x_index_] > axis_threshold_;
      buttons[AXIS_LEFT]  = axis_value_[axis_x_index_] < -axis_threshold_;
      buttons[AXIS_DOWN]  = axis_value_[axis_y_index_] > axis_threshold_;
      buttons[AXIS_UP]    = axis_value_[axis_y_index_] < -axis_threshold_;
      
      for (int i = 0; i < AXIS_BUTTON_NUM; ++i) {
        // ボタンの Press / Push / Pull 情報を生成
        // Pushは押した瞬間、Pullは離した瞬間だけtrueになる
        push_axis_button_[i] = !press_axis_button_[i] && buttons[i];
        pull_axis_button_[i] = press_axis_button_[i] && !buttons[i];
        
        press_axis_button_[i] = buttons[i];
      }
    }
  }
}


// PCに繋がれているGamePad情報の収集
std::vector<GamePad> initGamePad() {
  std::vector<GamePad> gamepads;

  // PCに接続されているGamePadを検出してコンテナに格納
  for (int id = GLFW_JOYSTICK_1; id <= GLFW_JOYSTICK_LAST; ++id) {
    if (glfwJoystickPresent(id) == GL_TRUE) {
      // emplace_backでコンテナに直接インスタンスを生成
      gamepads.emplace_back(id);
    }
  }

  return gamepads;
}

// コンテナのGamePadの状態をまとめて更新
void updateGamePad(std::vector<GamePad>& gamepads) {
  for (auto& pad : gamepads) {
    if (pad.isPresent()) {
      pad.update();
    }
  }
}

// コンテナのGamePadの状態をまとめてクリア
void flushGamePad(std::vector<GamePad>& gamepads) {
  for (auto& pad : gamepads) {
    if (pad.isPresent()) {
      pad.flush();
    }
  }
}

