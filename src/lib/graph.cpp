//
// グラフィック関連
//

#include "graph.hpp"
#include <vector>
#include <GLFW/glfw3.h>
#include "matrix.hpp"


Color::Color() :
  red_(0.0f),
  green_(0.0f),
  blue_(0.0f),
  alpha_(1.0f)
{}

// 値を0.0〜1.0で指定
Color::Color(const float red, const float green, const float blue,
             const float alpha) :
  red_(red),
  green_(green),
  blue_(blue),
  alpha_(alpha)
{}

// 色の取得
float Color::r() const { return red_; }
float Color::g() const { return green_; }
float Color::b() const { return blue_; }
float Color::a() const { return alpha_; }
  
// 色の変更
void Color::r(const float value) { red_ = value; }
void Color::g(const float value) { green_ = value; }
void Color::b(const float value) { blue_ = value; }
void Color::a(const float value) { alpha_ = value; }

// 参照でのアクセス
float& Color::r() { return red_; }
float& Color::g() { return green_; }
float& Color::b() { return blue_; }
float& Color::a() { return alpha_; }

// 簡単な演算機能
// FIXME:値が適正かどうか調べたほうがいいのか??
Color Color::operator +(const Color& rhs) const {
  Color added_color(red_   + rhs.red_,
                    green_ + rhs.green_,
                    blue_  + rhs.blue_,
                    alpha_ + rhs.alpha_);

  return added_color;
}

Color& Color::operator +=(const Color& rhs) {
  red_   += rhs.red_;
  green_ += rhs.green_;
  blue_  += rhs.blue_;
  alpha_ += rhs.alpha_;

  return *this;
}

Color Color::operator -(const Color& rhs) const {
  Color added_color(red_   - rhs.red_,
                    green_ - rhs.green_,
                    blue_  - rhs.blue_,
                    alpha_ - rhs.alpha_);

  return added_color;
}

Color& Color::operator -=(const Color& rhs) {
  red_   -= rhs.red_;
  green_ -= rhs.green_;
  blue_  -= rhs.blue_;
  alpha_ -= rhs.alpha_;

  return *this;
}

Color Color::operator *(const float rhs) const {
  Color mul_color(red_   * rhs,
                  green_ * rhs,
                  blue_  * rhs,
                  alpha_ * rhs);

  return mul_color;
}

Color& Color::operator *=(const float rhs) {
  red_   *= rhs;
  green_ *= rhs;
  blue_  *= rhs;
  alpha_ *= rhs;

  return *this;
}

Color Color::operator /(const float rhs) const {
  assert(rhs != 0.0f);
  
  Color dev_color(red_   / rhs,
                  green_ / rhs,
                  blue_  / rhs,
                  alpha_ / rhs);

  return dev_color;
}

Color& Color::operator /=(const float rhs) {
  assert(rhs != 0.0f);

  red_   /= rhs;
  green_ /= rhs;
  blue_  /= rhs;
  alpha_ /= rhs;

  return *this;
}

// OpenGLへ描画色を指定
void Color::setToGl() const {
  glColor4f(red_, green_, blue_, alpha_);
}


// 定型色
const Color Color::black   = Color(0.0f, 0.0f, 0.0f);
const Color Color::maroon  = Color(0.5f, 0.0f, 0.0f);
const Color Color::red     = Color(1.0f, 0.0f, 0.0f);
const Color Color::green   = Color(0.0f, 0.5f, 0.0f);
const Color Color::lime    = Color(0.0f, 1.0f, 0.0f);
const Color Color::olive   = Color(0.5f, 0.5f, 0.0f);
const Color Color::yellow  = Color(1.0f, 1.0f, 0.0f);
const Color Color::navy    = Color(0.0f, 0.0f, 0.5f);
const Color Color::blue    = Color(0.0f, 0.0f, 1.0f);
const Color Color::purple  = Color(0.5f, 0.0f, 0.5f);
const Color Color::magenta = Color(1.0f, 0.0f, 1.0f);
const Color Color::teal    = Color(0.0f, 0.5f, 0.5f);
const Color Color::cyan    = Color(0.0f, 1.0f, 1.0f);
const Color Color::gray    = Color(0.5f, 0.5f, 0.5f);
const Color Color::white   = Color(1.0f, 1.0f, 1.0f);


// 色を0~255で指定する
Color color256(const int red, const int green, const int blue, const int alpha) {
  return Color(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
}

// 色をHSBで指定
// hue, saturation, brightness [0, 1.0]
Color colorHSB(const float hue, const float saturation, const float brightness) {
  float r = brightness;
  float g = brightness;
  float b = brightness;
  
  if (saturation > 0.0) {
    float h = hue * 6.0;
    int   i = h;
    float f = h - i;
    switch (i) {
    default:
    case 0:
      g *= 1.0 - saturation * (1.0 - f);
      b *= 1.0 - saturation;
      break;

    case 1:
      r *= 1.0 - saturation * f;
      b *= 1.0 - saturation;
      break;

    case 2:
      r *= 1.0 - saturation;
      b *= 1.0 - saturation * (1.0 - f);
      break;
      
    case 3:
      r *= 1.0 - saturation;
      g *= 1.0 - saturation * f;
      break;
      
    case 4:
      r *= 1.0 - saturation * (1.0 - f);
      g *= 1.0 - saturation;
      break;
      
    case 5:
      g *= 1.0 - saturation;
      b *= 1.0 - saturation * f;
      break;      
    }
  }
  
  return Color(r, g, b);
}


// 点を描画
// x, y   位置
// radius 半径
// color  色
void drawPoint(const float x, const float y,
               const float radius,
               const Color& color) {

  GLfloat vtx[] = {
    x, y
  };

  glVertexPointer(2, GL_FLOAT, 0, vtx);

  // 点の大きさ
  glPointSize(radius);
  
  // 色を設定
  color.setToGl();

  glEnableClientState(GL_VERTEX_ARRAY);

  // OpenGLに点の描画を指示
  glDrawArrays(GL_POINTS, 0, 1);

  glDisableClientState(GL_VERTEX_ARRAY);
}

// 点を描画(回転、拡大縮小つき)
// x, y      位置
// radius    半径
// color     色
// angle_rad 回転角度(ラジアン)
// scaling   横、縦の拡大縮小率
// origin    矩形の原点位置
void drawPoint(const float x, const float y,
               const float radius,
               const Color& color,
               const float angle_rad,
               const Vec2f& scaling,
               const Vec2f& origin) {

  // 回転、拡大縮小の行列を生成
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(x, y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawPoint(-origin.x(), -origin.y(),
            radius,
            color);

  // 行列を元に戻す
  glPopMatrix();
}


// 線を描画
// start_x, start_y 始点
// end_x, end_y     終点
// line_width       線幅
// color            色
void drawLine(const float start_x, const float start_y,
              const float end_x, const float end_y,
              const float line_width,
              const Color& color) {

  GLfloat vtx[] = {
    start_x, start_y,
    end_x,   end_y
  };

  glVertexPointer(2, GL_FLOAT, 0, vtx);

  // 線分の太さを指示
  glLineWidth(line_width);

  // 色を設定
  color.setToGl();

  glEnableClientState(GL_VERTEX_ARRAY);

  // OpenGLに線分の描画を指示
  glDrawArrays(GL_LINES, 0, 2);

  glDisableClientState(GL_VERTEX_ARRAY);
}

// 線を描画(回転、拡大縮小つき)
// start_x, start_y 始点
// end_x, end_y     終点
// line_width       線幅
// color            色
// angle_rad        回転角度(ラジアン)
// scaling          横、縦の拡大縮小率
// origin           矩形の原点位置
void drawLine(const float start_x, const float start_y,
              const float end_x, const float end_y,
              const float line_width,
              const Color& color,
              const float angle_rad,
              const Vec2f& scaling,
              const Vec2f& origin) {
  
  // 回転、拡大縮小の行列を生成
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(start_x, start_y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawLine(-origin.x(), -origin.y(),
           (end_x - start_x) - origin.x(), (end_y - start_y) - origin.y(), 
           line_width,
           color);

  // 行列を元に戻す
  glPopMatrix();
}


// 三角形を描画
// x1, y1 ~ x3, y3 頂点
// line_width      線幅
// color           色
void drawTriangle(const float x1, const float y1,
                  const float x2, const float y2,
                  const float x3, const float y3,
                  const float line_width,
                  const Color& color) {

  GLfloat vtx[] = {
    x1, y1,
    x2, y2,
    x3, y3,
  };

  glVertexPointer(2, GL_FLOAT, 0, vtx);

  // 線分の太さを指示
  glLineWidth(line_width);

  // 色を設定
  color.setToGl();

  glEnableClientState(GL_VERTEX_ARRAY);

  // OpenGLに線分の描画を指示
  glDrawArrays(GL_LINE_LOOP, 0, 3);

  glDisableClientState(GL_VERTEX_ARRAY);
}

// 三角形を描画(回転、拡大縮小つき)
// x1, y1 ~ x3, y3  頂点
// line_width       線幅
// color            色
// angle_rad        回転角度(ラジアン)
// scaling          横、縦の拡大縮小率
// origin           矩形の原点位置
void drawTriangle(const float x1, const float y1,
                  const float x2, const float y2,
                  const float x3, const float y3,
                  const float line_width,
                  const Color& color,
                  const float angle_rad,
                  const Vec2f& scaling,
                  const Vec2f& origin) {
  
  // 回転、拡大縮小の行列を生成
  // 一番小さい座標値を原点とする
  float min_x = std::min(x1, std::min(x2, x3));
  float min_y = std::min(y1, std::min(y2, y3));
  
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(min_x, min_y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawTriangle((x1 - min_x) - origin.x(), (y1 - min_y) - origin.y(),
               (x2 - min_x) - origin.x(), (y2 - min_y) - origin.y(),
               (x3 - min_x) - origin.x(), (y3 - min_y) - origin.y(),
               line_width,
               color);

  // 行列を元に戻す
  glPopMatrix();
}

// 塗りつぶし三角形を描画
// x1, y1 ~ x3, y3 頂点
// color           色
void drawFillTriangle(const float x1, const float y1,
                      const float x2, const float y2,
                      const float x3, const float y3,
                      const Color& color) {

  GLfloat vtx[] = {
    x1, y1,
    x2, y2,
    x3, y3,
  };

  glVertexPointer(2, GL_FLOAT, 0, vtx);

  // 色を設定
  color.setToGl();

  glEnableClientState(GL_VERTEX_ARRAY);

  // OpenGLに三角ポリゴンの描画を指示
  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableClientState(GL_VERTEX_ARRAY);
}

// 塗りつぶし三角形を描画(回転、拡大縮小つき)
// x1, y1 ~ x3, y3  頂点
// color            色
// angle_rad        回転角度(ラジアン)
// scaling          横、縦の拡大縮小率
// origin           矩形の原点位置
void drawFillTriangle(const float x1, const float y1,
                      const float x2, const float y2,
                      const float x3, const float y3,
                      const Color& color,
                      const float angle_rad,
                      const Vec2f& scaling,
                      const Vec2f& origin) {

  // 回転、拡大縮小の行列を生成
  // 一番小さい座標値を原点とする
  float min_x = std::min(x1, std::min(x2, x3));
  float min_y = std::min(y1, std::min(y2, y3));
  
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(min_x, min_y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawFillTriangle((x1 - min_x) - origin.x(), (y1 - min_y) - origin.y(),
                   (x2 - min_x) - origin.x(), (y2 - min_y) - origin.y(),
                   (x3 - min_x) - origin.x(), (y3 - min_y) - origin.y(),
                   color);

  // 行列を元に戻す
  glPopMatrix();
}


// 円を描画
// center_x, center_y 円の中心位置
// radius_x, radius_y 半径(横と縦)
// division           円の分割数(数値が大きいと滑らかな円になる)
// line_width         線幅
// color              色
void drawCircle(const float center_x, const float center_y,
                const float radius_x, const float radius_y,
                const int division,
                const float line_width,
                const Color& color) {
  // 線分の太さを指示
  glLineWidth(line_width);

  // 色を設定
  color.setToGl();

  // 頂点データを生成
  std::vector<GLfloat> vtx;
  vtx.reserve(division * 2);
  for (int i = 0; i < division; ++i) {
    float r = (M_PI * 2.0 * i) / division;

    vtx.push_back(radius_x * std::sin(r) + center_x);
    vtx.push_back(radius_y * std::cos(r) + center_y);
  }

  glVertexPointer(2, GL_FLOAT, 0, &vtx[0]);
  
  glEnableClientState(GL_VERTEX_ARRAY);
  
  glDrawArrays(GL_LINE_LOOP, 0, GLsizei(vtx.size() / 2));
  
  glDisableClientState(GL_VERTEX_ARRAY);
}

// 円を描画(回転、拡大縮小つき)
// center_x, center_y 円の中心位置
// radius_x, radius_y 半径(横と縦)
// division           円の分割数(数値が大きいと滑らかな円になる)
// line_width         線幅
// color              色
// angle_rad          回転角度(ラジアン)
// scaling            横、縦の拡大縮小率
// origin             矩形の原点位置
void drawCircle(const float center_x, const float center_y,
                const float radius_x, const float radius_y,
                const int division,
                const float line_width,
                const Color& color,
                const float angle_rad,
                const Vec2f& scaling,
                const Vec2f& origin) {
  
  // 回転、拡大縮小の行列を生成
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(center_x, center_y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawCircle(-origin.x(), -origin.y(),
             radius_x, radius_y,
             division,
             line_width,
             color);

  // 行列を元に戻す
  glPopMatrix();
}


// 塗り潰し円
// center_x, center_y 円の中心位置
// radius_x, radius_y 半径(横と縦)
// division           円の分割数(数値が大きいと滑らかな円になる)
// color              色
void drawFillCircle(const float center_x, const float center_y,
                    const float radius_x, const float radius_y,
                    const int division,
                    const Color& color) {
  // 色を設定
  color.setToGl();

  // 頂点データを生成
  std::vector<GLfloat> vtx;
  vtx.reserve(division * 2);
  vtx.push_back(center_x);
  vtx.push_back(center_y);
  for (int i = 0; i <= division; ++i) {
    float r = (M_PI * 2.0 * i) / division;

    // 反時計回りが表面となる
    vtx.push_back(radius_x * -std::sin(r) + center_x);
    vtx.push_back(radius_y * std::cos(r) + center_y);
  }

  glVertexPointer(2, GL_FLOAT, 0, &vtx[0]);
  
  glEnableClientState(GL_VERTEX_ARRAY);
  
  glDrawArrays(GL_TRIANGLE_FAN, 0, GLsizei(vtx.size() / 2));
  
  glDisableClientState(GL_VERTEX_ARRAY);
}

// 塗り潰し円(回転、拡大縮小つき)
// center_x, center_y 円の中心位置
// radius_x, radius_y 半径(横と縦)
// division           円の分割数(数値が大きいと滑らかな円になる)
// color              色
// angle_rad          回転角度(ラジアン)
// scaling            横、縦の拡大縮小率
// origin             矩形の原点位置
void drawFillCircle(const float center_x, const float center_y,
                    const float radius_x, const float radius_y,
                    const int division,
                    const Color& color,
                    const float angle_rad,
                    const Vec2f& scaling,
                    const Vec2f& origin) {
  
  // 回転、拡大縮小の行列を生成
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(center_x, center_y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawFillCircle(-origin.x(), -origin.y(),
                 radius_x, radius_y,
                 division,
                 color);

  // 行列を元に戻す
  glPopMatrix();
}


// 円弧を描画
// center_x, center_y  円の中心位置
// radius_x, radius_y  半径(横と縦)
// start_rad, end_rad  開始・終了角度
// division            円の分割数(数値が大きいと滑らかな円になる)
// line_width          線幅
// color               色
void drawArc(const float center_x, const float center_y,
             const float radius_x, const float radius_y,
             const float start_rad, const float end_rad,
             const int division,
             const float line_width,
             const Color& color) {
  // 線分の太さを指示
  glLineWidth(line_width);

  // 色を設定
  color.setToGl();

  // 頂点データを生成
  std::vector<GLfloat> vtx;
  vtx.reserve(division * 2);                        // TIPS:正確な値である必要はない
  for (int i = 0; i <= division; ++i) {
    float r = ((end_rad - start_rad) * i) / division + start_rad;

    vtx.push_back(radius_x * std::sin(r) + center_x);
    vtx.push_back(radius_y * std::cos(r) + center_y);
  }

  glVertexPointer(2, GL_FLOAT, 0, &vtx[0]);
  
  glEnableClientState(GL_VERTEX_ARRAY);
  
  glDrawArrays(GL_LINE_STRIP, 0, GLsizei(vtx.size() / 2));
  
  glDisableClientState(GL_VERTEX_ARRAY);
}

// 円弧を描画(回転、拡大縮小つき)
// center_x, center_y  円の中心位置
// radius_x, radius_y  半径(横と縦)
// start_rad, end_rad  開始・終了角度
// division            円の分割数(数値が大きいと滑らかな円になる)
// line_width          線幅
// color               色
// angle_rad           回転角度(ラジアン)
// scaling             横、縦の拡大縮小率
// origin              矩形の原点位置
void drawArc(const float center_x, const float center_y,
             const float radius_x, const float radius_y,
             const float start_rad, const float end_rad,
             const int division,
             const float line_width,
             const Color& color,
             const float angle_rad,
             const Vec2f& scaling,
             const Vec2f& origin) {

  // 回転、拡大縮小の行列を生成
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(center_x, center_y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawArc(-origin.x(), -origin.y(),
          radius_x, radius_y,
          start_rad, end_rad,
          division,
          line_width,
          color);

  // 行列を元に戻す
  glPopMatrix();
}

// 塗り潰し円弧
// center_x, center_y  円の中心位置
// radius_x, radius_y  半径(横と縦)
// start_rad, end_rad  開始・終了角度
// division            円の分割数(数値が大きいと滑らかな円になる)
// color               色
void drawFillArc(const float center_x, const float center_y,
                 const float radius_x, const float radius_y,
                 const float start_rad, const float end_rad,
                 const int division,
                 const Color& color) {
  // 色を設定
  color.setToGl();

  // 頂点データを生成
  std::vector<GLfloat> vtx;
  vtx.reserve(division * 2);                        // TIPS:正確な値である必要はない
  vtx.push_back(center_x);
  vtx.push_back(center_y);
  for (int i = 0; i <= division; ++i) {
    float r = ((end_rad - start_rad) * i) / division + start_rad;

    vtx.push_back(radius_x * std::sin(r) + center_x);
    vtx.push_back(radius_y * std::cos(r) + center_y);
  }

  glVertexPointer(2, GL_FLOAT, 0, &vtx[0]);
  
  glEnableClientState(GL_VERTEX_ARRAY);
  
  glDrawArrays(GL_TRIANGLE_FAN, 0, GLsizei(vtx.size() / 2));
  
  glDisableClientState(GL_VERTEX_ARRAY);
}

// 塗り潰し円弧(回転、拡大縮小つき)
// center_x, center_y  円の中心位置
// radius_x, radius_y  半径(横と縦)
// start_rad, end_rad  開始・終了角度
// division            円の分割数(数値が大きいと滑らかな円になる)
// color               色
// angle_rad           回転角度(ラジアン)
// scaling             横、縦の拡大縮小率
// origin              矩形の原点位置
void drawFillArc(const float center_x, const float center_y,
                 const float radius_x, const float radius_y,
                 const float start_rad, const float end_rad,
                 const int division,
                 const Color& color,
                 const float angle_rad,
                 const Vec2f& scaling,
                 const Vec2f& origin) {

  // 回転、拡大縮小の行列を生成
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(center_x, center_y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawFillArc(-origin.x(), -origin.y(),
              radius_x, radius_y,
              start_rad, end_rad,
              division,
              color);

  // 行列を元に戻す
  glPopMatrix();
}


// 矩形
// start_x, start_y 始点
// width, height    幅、高さ
// line_width       線幅
// color            色
void drawBox(const float start_x, const float start_y,
             const float width, const float height,
             const float line_width,
             const Color& color) {

  // 線分の太さを指示
  glLineWidth(line_width);

  const float end_x = start_x + width;
  const float end_y = start_y + height;

  GLfloat vtx[] = { 
    start_x, start_y,
    start_x, end_y,
    end_x,   end_y,
    end_x,   start_y,
  };

  glVertexPointer(2, GL_FLOAT, 0, vtx);

  // 色を設定
  color.setToGl();

  glEnableClientState(GL_VERTEX_ARRAY);

  // OpenGLに線分の描画を指示
  glDrawArrays(GL_LINE_LOOP, 0, 4);

  glDisableClientState(GL_VERTEX_ARRAY);
}

// 矩形(回転、拡大縮小つき)
// start_x, start_y 始点
// width, height    幅、高さ
// line_width       線幅
// color            色
// angle_rad        回転角度(ラジアン)
// scaling          横、縦の拡大縮小率
// origin           矩形の原点位置
void drawBox(const float start_x, const float start_y,
             const float width, const float height,
             const float line_width,
             const Color& color,
             const float angle_rad,
             const Vec2f& scaling,
             const Vec2f& origin) {

  // 回転、拡大縮小の行列を生成
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(start_x, start_y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawBox(-origin.x(), -origin.y(),
          width, height,
          line_width,
          color);

  // 行列を元に戻す
  glPopMatrix();
}


// 塗り潰し矩形
// start_x, start_y 始点
// width, height    幅、高さ
// color            色
void drawFillBox(const float start_x, const float start_y,
                 const float width, const float height,
                 const Color& color) {

  const float end_x = start_x + width;
  const float end_y = start_y + height;
  
  GLfloat vtx[] = { 
    start_x, start_y,
    end_x,   start_y,
    start_x, end_y,
    end_x,   end_y
  };

  glVertexPointer(2, GL_FLOAT, 0, vtx);

  // 色を設定
  color.setToGl();

  glEnableClientState(GL_VERTEX_ARRAY);

  // OpenGLに線分の描画を指示
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableClientState(GL_VERTEX_ARRAY);
}

// 塗り潰し矩形(回転、拡大縮小つき)
// start_x, start_y 始点
// width, height    幅、高さ
// color            色
// angle_rad        回転角度(ラジアン)
// scaling          横、縦の拡大縮小率
// origin           矩形の原点位置
void drawFillBox(const float start_x, const float start_y,
                 const float width, const float height,
                 const Color& color,
                 const float angle_rad,
                 const Vec2f& scaling,
                 const Vec2f& origin) {

  // 回転、拡大縮小の行列を生成
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(start_x, start_y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawFillBox(-origin.x(), -origin.y(),
              width, height,
              color);

  // 行列を元に戻す
  glPopMatrix();
}


// 四角形
// x1, y1 ~ x4, y4  頂点
// line_width       線幅
// color            色
void drawQuad(const float x1, const float y1,
              const float x2, const float y2,
              const float x3, const float y3,
              const float x4, const float y4,
              const float line_width,
              const Color& color) {

  // 線分の太さを指示
  glLineWidth(line_width);

  GLfloat vtx[] = {
    x1, y1,
    x2, y2,
    x3, y3,
    x4, y4,
  };

  glVertexPointer(2, GL_FLOAT, 0, vtx);

  // 色を設定
  color.setToGl();

  glEnableClientState(GL_VERTEX_ARRAY);

  // OpenGLに線分の描画を指示
  glDrawArrays(GL_LINE_LOOP, 0, 4);

  glDisableClientState(GL_VERTEX_ARRAY);
}

// 四角形(回転、拡大縮小つき)
// x1, y1 ~ x4, y4  頂点
// line_width       線幅
// color            色
// angle_rad        回転角度(ラジアン)
// scaling          横、縦の拡大縮小率
// origin           矩形の原点位置
void drawQuad(const float x1, const float y1,
              const float x2, const float y2,
              const float x3, const float y3,
              const float x4, const float y4,
              const float line_width,
              const Color& color,
              const float angle_rad,
              const Vec2f& scaling,
              const Vec2f& origin) {

  // 回転、拡大縮小の行列を生成
  // 一番小さな値を原点座標にする
  float min_x = std::min(x1, std::min(x2, std::min(x3, x4)));
  float min_y = std::min(y1, std::min(y2, std::min(y3, y4)));
  
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(min_x, min_y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawQuad((x1 - min_x) - origin.x(), (y1 - min_y) - origin.y(),
           (x2 - min_x) - origin.x(), (y2 - min_y) - origin.y(),
           (x3 - min_x) - origin.x(), (y3 - min_y) - origin.y(),
           (x4 - min_x) - origin.x(), (y4 - min_y) - origin.y(),
           line_width,
           color);

  // 行列を元に戻す
  glPopMatrix();
}


// 塗り潰し四角
// x1, y1 ~ x4, y4  頂点
// color            色
void drawFillQuad(const float x1, const float y1,
                  const float x2, const float y2,
                  const float x3, const float y3,
                  const float x4, const float y4,
                  const Color& color) {

  GLfloat vtx[] = {
    x1, y1,
    x2, y2,
    x4, y4,
    x2, y2,
    x3, y3,
    x4, y4,
  };

  glVertexPointer(2, GL_FLOAT, 0, vtx);

  // 色を設定
  color.setToGl();

  glEnableClientState(GL_VERTEX_ARRAY);

  // OpenGLに線分の描画を指示
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glDisableClientState(GL_VERTEX_ARRAY);
}

// 塗り潰し四角(回転、拡大縮小つき)
// x1, y1 ~ x4, y4  頂点
// color            色
// angle_rad        回転角度(ラジアン)
// scaling          横、縦の拡大縮小率
// origin           矩形の原点位置
void drawFillQuad(const float x1, const float y1,
                  const float x2, const float y2,
                  const float x3, const float y3,
                  const float x4, const float y4,
                  const Color& color,
                  const float angle_rad,
                  const Vec2f& scaling,
                  const Vec2f& origin) {
  
  // 回転、拡大縮小の行列を生成
  // 一番小さな値を原点座標にする
  float min_x = std::min(x1, std::min(x2, std::min(x3, x4)));
  float min_y = std::min(y1, std::min(y2, std::min(y3, y4)));
  
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(min_x, min_y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawFillQuad((x1 - min_x) - origin.x(), (y1 - min_y) - origin.y(),
               (x2 - min_x) - origin.x(), (y2 - min_y) - origin.y(),
               (x3 - min_x) - origin.x(), (y3 - min_y) - origin.y(),
               (x4 - min_x) - origin.x(), (y4 - min_y) - origin.y(),
               color);

  // 行列を元に戻す
  glPopMatrix();
}                  


// 画像つき矩形の描画
// start_x, start_y              始点
// width, height                 幅、高さ
// start_tx, start_ty            画像切り抜き開始位置
// texture_width, texture_height 画像切り抜き幅、高さ
// texture                       画像
// color                         色
void drawTextureBox(const float start_x, const float start_y,
                    const float width, const float height,
                    const float start_tx, const float start_ty,
                    const float texture_width, const float texture_height,
                    const Texture& texture,
                    const Color& color) {

  const float end_x = start_x + width;
  const float end_y = start_y + height;
  GLfloat vtx[] = {
    start_x, start_y,
    end_x,   start_y,
    start_x, end_y,
    end_x,   end_y
  };

  glVertexPointer(2, GL_FLOAT, 0, vtx);
  
  GLfloat uv[] = {
    start_tx / texture.width(),                   (start_ty + texture_height) / texture.height(),
    (start_tx + texture_width) / texture.width(), (start_ty + texture_height) / texture.height(),
    start_tx / texture.width(),                   start_ty / texture.height(),
    (start_tx + texture_width) / texture.width(), start_ty / texture.height(),
  };
  
  glTexCoordPointer(2, GL_FLOAT, 0, uv);
  
  // 色を設定
  color.setToGl();

  glEnable(GL_TEXTURE_2D);
  texture.bind();
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  // OpenGLに線分の描画を指示
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  
  glDisable(GL_TEXTURE_2D);
  texture.unbind();
}

// 画像つき矩形の描画(回転、拡大縮小つき)
// start_x, start_y              始点
// width, height                 幅、高さ
// start_tx, start_ty            画像切り抜き開始位置
// texture_width, texture_height 画像切り抜き幅、高さ
// texture                       画像
// color                         色
// angle_rad                     回転角度(ラジアン)
// scaling                       横、縦の拡大縮小率
// origin                        矩形の原点位置
void drawTextureBox(const float start_x, const float start_y,
                    const float width, const float height,
                    const float start_tx, const float start_ty,
                    const float texture_width, const float texture_height,
                    const Texture& texture,
                    const Color& color,
                    const float angle_rad,
                    const Vec2f& scaling,
                    const Vec2f& origin) {

  // 回転、拡大縮小の行列を生成
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(start_x, start_y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawTextureBox(-origin.x(), -origin.y(),
                 width, height,
                 start_tx, start_ty,
                 texture_width, texture_height,
                 texture,
                 color);

  // 行列を元に戻す
  glPopMatrix();
}
