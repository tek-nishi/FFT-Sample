//
// アプリの実行環境
//

#include "appEnv.hpp"
#include <iostream>


// width, height 生成時のサイズ
// full_screen   true: フルスクリーン
// dynamic_size  true: ウインドウサイズにあわせて画面を変更
AppEnv::AppEnv(const int width, const int height,
               const bool full_screen, const bool dynamic_size) :
  dynamic_window_size_(dynamic_size),
  window_(width, height, false, full_screen),
  window_size_(width, height),
  current_window_size_(window_size_),
  viewport_ofs_(0, 0),
  viewport_size_(width, height),
  bg_color_(0, 0, 0, 0),
  mouse_current_pos_(0, 0),
  is_focus_(false)
{
  DOUT << "AppEnv()" << std::endl;

  // Windowを画面の中央へ移動
  const auto* video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  glfwSetWindowPos(window_(), (video_mode->width - width) / 2, (video_mode->height - height) / 2);

  // GLFWのハンドルに自分自身を登録
  glfwSetWindowUserPointer(window_(), this);

  // ウインドウのサイズ変更
  glfwSetWindowSizeCallback(window_(), changeWindowSize);
    
  // キーが押された時に呼ばれる関数を登録
  glfwSetCharCallback(window_(), createCharaInfo);
  glfwSetKeyCallback(window_(), createKeyInfo);

  // マウスイベント
  glfwSetMouseButtonCallback(window_(), mouseButtonCallback);
  glfwSetCursorPosCallback(window_(), mouseMoveCallback);

  // フォーカス
  glfwSetWindowFocusCallback(window_(), focusCallback);
  
  // GamePad
  gamepads_ = initGamePad();
  
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);

  // Windowの表示開始
  glfwShowWindow(window_());

  if (full_screen) {
    // フルスクリーンはモニタによって解像度がまちまちなので、viewportで補正
    int width, height;
    glfwGetFramebufferSize(window_(), &width, &height);
    DOUT << "framebuffer size:" << width << "," << height << std::endl;

    dynamicViewport(width, height);
  }
}


// アプリウインドウが開いてるならtrueを返す
bool AppEnv::isOpen() {
  return !glfwWindowShouldClose(window_());
}

// GLFWのハンドルを返却
// TIPS:const版も定義
// TIPS:*の右側のconstはポインタ値をconstにする
GLFWwindow* const AppEnv::getGlfwHandle() { return window_(); }
const GLFWwindow* const AppEnv::getGlfwHandle() const {return window_(); }


// アプリ更新処理開始
void AppEnv::begin() {
  glViewport(viewport_ofs_.x(), viewport_ofs_.y(),
             viewport_size_.x(), viewport_size_.y());
    
  // ウインドウの内容を指定色で消去
  glClearColor(bg_color_.r(), bg_color_.g(), bg_color_.b(), bg_color_.a());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // 半透明描画指示
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // 裏面は描画しない
  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
  
  // TIPS:2D描画だけなら裏面も描画してしまって問題ない
  //      表・裏の判定を行う必要がない
  glDisable(GL_CULL_FACE);

  // 深度テスト禁止
  glDisable(GL_DEPTH_TEST);

  // ライティング禁止
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);

  // 透視変換行列を作成
  auto matrix = camera_2d_(current_window_size_);

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(matrix.first.data());

  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(matrix.second.data());

  updateGamePad(gamepads_);
}

// アプリ更新処理終了
// 1. OpenGLの描画内容をウインドウに表示
// 2. キーやマウスイベントのポーリング
void AppEnv::end() {
  // GLFWへ描画指示
  glfwSwapBuffers(window_());

  // 入力(キー＆ボタン)の再初期化
  switchInputBuffer();

  if (!is_focus_) {
    // TIPS:OSXはWindow全面が覆い隠されると、全速力で更新が行われてしまう
    //      それに対処するためイベント待ちをおこなっている
    glfwWaitEvents();
  }
  glfwPollEvents();
}
  
// 入力(キー＆ボタン)の再初期化
void AppEnv::flushInput() {
  switchInputBuffer();
}
  
// ウィンドウサイズを返す
const Vec2f& AppEnv::viewSize() const { return current_window_size_; }

// ウインドウの位置を変更
// pos 新しい位置
void AppEnv::windowPosition(const Vec2i& pos) {
  glfwSetWindowPos(window_(), pos.x(), pos.y());
}

// ウインドウの位置を取得
// FIXME:glfwGetWindowPos の引数がconstでないので、constにできない
Vec2i AppEnv::windowPosition() {
  int x_pos;
  int y_pos;
  
  glfwGetWindowPos(window_(), &x_pos, &y_pos);
    
  return Vec2i(x_pos, y_pos);
}


// 背景色の変更
// color 色
void AppEnv::bgColor(const Color& color) { bg_color_ = color; }

  
// 押されたキーを取得
// 'A'とか'a'とか(押されてないときは0)
// キーリピート付き
u_int AppEnv::getPushedKey() const { return pushed_key_; }

  
// 当該キーが押されているならtrueを返す
// key 'A'とか'7'とか
// SOURCE:include/GLFW/glfw3.h 271〜396
bool AppEnv::isPressKey(const int key) const {
  return press_keys_.count(key);
}
  
// 当該キーが押された瞬間trueを返す
// key 'A'とか'7'とか
// SOURCE:include/GLFW/glfw3.h 271〜396
bool AppEnv::isPushKey(const int key) const {
  return push_keys_.count(key);
}

// 当該キーが離された瞬間trueを返す
bool AppEnv::isPullKey(const int key) const {
  return pull_keys_.count(key);
}

  
// マウスのカーソル位置を返す
const Vec2f& AppEnv::mousePosition() const { return mouse_current_pos_; }

// マウスのカーソル位置を指定
void AppEnv::mousePosition(const Vec2f& pos) {
  mouse_current_pos_ = pos;

  Vec2f window_pos = windowPosition(Vec2f(pos.x(), -pos.y()),
                                    current_window_size_, Vec2f(viewport_size_.x(), viewport_size_.y()));

  Vec2f mouse_pos = Vec2f(window_pos.x() + viewport_ofs_.x(), window_pos.y() + viewport_ofs_.y());
  
  glfwSetCursorPos(window_(), mouse_pos.x(), mouse_pos.y());
}

// マウスカーソルのON/OFF
void AppEnv::mouseCursor(const bool disp) {
  glfwSetInputMode(window_(), GLFW_CURSOR, disp ? GLFW_CURSOR_NORMAL
                                                : GLFW_CURSOR_HIDDEN);
}

// 当該ボタンが押されているならtrueを返す
// button Mouse::LEFT
//        Mouse::Right
bool AppEnv::isPressButton(const Mouse button) const {
  return press_buttons_.count(static_cast<int>(button));
}
  
// 当該ボタンが押された瞬間trueを返す
// button Mouse::LEFT
//        Mouse::RIGHT
bool AppEnv::isPushButton(const Mouse button) const {
  return push_buttons_.count(static_cast<int>(button));
}

// 当該ボタンが離された瞬間trueを返す
// button Mouse::LEFT
//        Mouse::RIGHT
bool AppEnv::isPullButton(const Mouse button) const {
  return pull_buttons_.count(static_cast<int>(button));
}

// フォーカス状態
bool AppEnv::isFocus() const { return is_focus_; }

// GamePadの接続数
size_t AppEnv::numGamePad() const { return gamepads_.size(); }

// 指定番号のGamePadを取得
// TIPS:const版も定義
const GamePad& AppEnv::gamePad(const int index) const {
  return gamepads_[index];
}

GamePad& AppEnv::gamePad(const int index) {
  return gamepads_[index];
}

  
// キーが押された時に呼び出される
void AppEnv::createCharaInfo(GLFWwindow* window, const u_int chara) {
  auto* const obj = static_cast<AppEnv*>(glfwGetWindowUserPointer(window));
    
  obj->pushed_key_ = chara;
}

void AppEnv::createKeyInfo(GLFWwindow* window, const int key, const int scancode, const int action, const int mods) {
  auto* const obj = static_cast<AppEnv*>(glfwGetWindowUserPointer(window));

  // キーのpush,press,pull情報を生成
  switch (action) {
  case GLFW_PRESS:
    obj->push_keys_.insert(key);
    obj->press_keys_.insert(key);
    break;

  case GLFW_RELEASE:
    obj->pull_keys_.insert(key);
    obj->press_keys_.erase(key);
    break;
  }
}

void AppEnv::changeWindowSize(GLFWwindow* window, const int width, const int height) {
  // ウインドウサイズの変更をアプリに伝える
  auto* const obj = static_cast<AppEnv*>(glfwGetWindowUserPointer(window));
  if (obj->dynamic_window_size_) {
    // 描画サイズは固定(アスペクト比固定)
    obj->dynamicViewport(width, height);
  }
  else {
    // Windowのサイズと描画サイズは一致
    obj->current_window_size_ << width, height;
    obj->viewport_size_ << width, height;
  }
}


void AppEnv::mouseButtonCallback(GLFWwindow* window, const int button, const int action, const int mods) {
  auto* const obj = static_cast<AppEnv*>(glfwGetWindowUserPointer(window));

  // ボタン入力情報を生成
  switch (action) {
  case GLFW_PRESS:
    obj->push_buttons_.insert(button);
    obj->press_buttons_.insert(button);
    break;

  case GLFW_RELEASE:
    obj->pull_buttons_.insert(button);
    obj->press_buttons_.erase(button);
    break;
  }
}

void AppEnv::mouseMoveCallback(GLFWwindow* window, const double x_pos, const double y_pos) {
  auto* const obj = static_cast<AppEnv*>(glfwGetWindowUserPointer(window));
    
  Vec2f pos = screenPosition(Vec2f(x_pos - obj->viewport_ofs_.x(), y_pos - obj->viewport_ofs_.y()),
                             obj->current_window_size_,
                             Vec2f(obj->viewport_size_.x(), obj->viewport_size_.y()));
  // TIPS:Yは上下が逆
  obj->mouse_current_pos_ << pos.x(), -pos.y();
}

void AppEnv::focusCallback(GLFWwindow* window, int focus) {
  auto* const obj = static_cast<AppEnv*>(glfwGetWindowUserPointer(window));

  obj->is_focus_ = focus ? true : false;
}

// 画面中央が(0, 0)の座標を計算
Vec2f AppEnv::screenPosition(const Vec2f& pos, const Vec2f& window, const Vec2f& viewport) {
  // ウインドウサイズと描画サイズの違いも考慮する
  Vec2f screen_rate(window.array() / viewport.array());
  Vec2f view_pos(pos - viewport / 2.0f);

  return Vec2f(view_pos.array() * screen_rate.array());
}

// 画面中央が(0, 0)の座標→左上が(0, 0)の座標
Vec2f AppEnv::windowPosition(const Vec2f& pos, const Vec2f& window, const Vec2f& viewport) {
  Vec2f screen_rate = window.array() / viewport.array();
  
  return Vec2f(pos.array() / screen_rate.array()) + viewport / 2.0f;
}

// 入力バッファを切り替える
void AppEnv::switchInputBuffer() {
  pushed_key_ = 0;

  push_keys_.clear();
  pull_keys_.clear();

  push_buttons_.clear();
  pull_buttons_.clear();
}

// 動的Viewport(アスペクト比固定)
void AppEnv::dynamicViewport(const int width, const int height) {
  // 描画サイズは固定(アスペクト比固定)
  float view_width  = window_size_.x();
  float view_height = window_size_.y();

  // アスペクト比から縦に揃えるか横に揃えるか決める
  float view_aspect   = view_height / view_width;
  float window_aspect = float(height) / float(width);
  if (view_aspect < window_aspect) {
    int window_width  = width;
    int window_height = width * view_aspect;

    // 上下に余白が入る
    viewport_ofs_  << 0, (height - window_height) / 2;
    viewport_size_ << window_width, window_height;
  }
  else {
    int window_width  = height / view_aspect;
    int window_height = height;

    // 左右に余白が入る
    viewport_ofs_  << (width - window_width) / 2, 0;
    viewport_size_ << window_width, window_height;
  }
}
  
