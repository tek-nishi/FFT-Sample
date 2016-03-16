#pragma once

//
// グラフィック関連
//

#include "defines.hpp"
#include "texture.hpp"
#include "vector.hpp"


class Color {
  float red_;
  float green_;
  float blue_;
  float alpha_;

  
public:  
  Color();

  // 値を0.0〜1.0で指定
  Color(const float red, const float green, const float blue,
        const float alpha = 1.0f);

  // 色の取得
  // FIXME:定型色と名前が被ってしまったので
  //       略称を利用
  float r() const;
  float g() const;
  float b() const;
  float a() const;
  
  // 色の変更
  // FIXME:定型色と名前が被ってしまったので
  //       略称を利用
  void r(const float value);
  void g(const float value);
  void b(const float value);
  void a(const float value);

  // 参照でのアクセス
  float& r();
  float& g();
  float& b();
  float& a();

  // 簡単な演算機能
  Color operator +(const Color& rhs) const;
  Color& operator +=(const Color& rhs);
  Color operator -(const Color& rhs) const;
  Color& operator -=(const Color& rhs);
  Color operator *(const float rhs) const;
  Color& operator *=(const float rhs);
  Color operator /(const float rhs) const;
  Color& operator /=(const float rhs);
  
  // OpenGLへ描画色を指定
  void setToGl() const;

  // 定型色
  static const Color black;

  static const Color maroon;
  static const Color red;
  
  static const Color green;
  static const Color lime;

  static const Color olive;
  static const Color yellow;
  
  static const Color navy;
  static const Color blue;

  static const Color purple;
  static const Color magenta;

  static const Color teal;
  static const Color cyan;

  static const Color gray;
  static const Color white;
};


// 色を0~255で指定する
Color color256(const int red, const int green, const int blue, const int alpha = 255);

// 色をHSBで指定
// hue, saturation, brightness [0, 1.0]
Color colorHSB(const float hue, const float saturation, const float brightness);


// 点を描画
// x, y   位置
// radius 半径
// color  色
void drawPoint(const float x, const float y,
               const float radius,
               const Color& color);

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
               const Vec2f& origin);

// 線を描画
// start_x, start_y 始点
// end_x, end_y     終点
// line_width       線幅
// color            色
void drawLine(const float start_x, const float start_y,
              const float end_x, const float end_y,
              const float line_width,
              const Color& color);

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
              const Vec2f& origin);

// 三角形を描画
// x1, y1 ~ x3, y3 頂点
// line_width      線幅
// color           色
void drawTriangle(const float x1, const float y1,
                  const float x2, const float y2,
                  const float x3, const float y3,
                  const float line_width,
                  const Color& color);

// 三角形を描画(回転、拡大縮小つき)
// x1, y1 ~ x3, y3  頂点
// line_width       線幅
// color            色
// angle_rad        回転角度(ラジアン)
// scaling          横、縦の拡大縮小率
// origin           三角形の原点位置
void drawTriangle(const float x1, const float y1,
                  const float x2, const float y2,
                  const float x3, const float y3,
                  const float line_width,
                  const Color& color,
                  const float angle_rad,
                  const Vec2f& scaling,
                  const Vec2f& origin);

// 塗りつぶし三角形を描画
// x1, y1 ~ x3, y3 頂点
// color           色
void drawFillTriangle(const float x1, const float y1,
                      const float x2, const float y2,
                      const float x3, const float y3,
                      const Color& color);

// 塗りつぶし三角形を描画(回転、拡大縮小つき)
// x1, y1 ~ x3, y3  頂点
// color            色
// angle_rad        回転角度(ラジアン)
// scaling          横、縦の拡大縮小率
// origin           三角形の原点位置
void drawFillTriangle(const float x1, const float y1,
                      const float x2, const float y2,
                      const float x3, const float y3,
                      const Color& color,
                      const float angle_rad,
                      const Vec2f& scaling,
                      const Vec2f& origin);


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
                const Color& color);

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
                const Vec2f& origin);

// 塗り潰し円
// center_x, center_y 円の中心位置
// radius_x, radius_y 半径(横と縦)
// division           円の分割数(数値が大きいと滑らかな円になる)
// color              色
void drawFillCircle(const float center_x, const float center_y,
                    const float radius_x, const float radius_y,
                    const int division,
                    const Color& color);

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
                    const Vec2f& origin);

// 円弧を描画
// center_x, center_y  円の中心位置
// radius_x, radius_y  半径(横と縦)
// start_rad, end_rad  開始・終了角度
//                     角度が０で真上。プラスで時計回りの方向となる
//                     (時計と同じ)
// division            円の分割数(数値が大きいと滑らかな円になる)
// line_width          線幅
// color               色
void drawArc(const float center_x, const float center_y,
             const float radius_x, const float radius_y,
             const float start_rad, const float end_rad,
             const int division,
             const float line_width,
             const Color& color);

// 円弧を描画(回転、拡大縮小つき)
// center_x, center_y  円の中心位置
// radius_x, radius_y  半径(横と縦)
// start_rad, end_rad  開始・終了角度
//                     角度が０で真上。プラスで時計回りの方向となる
//                     (時計と同じ)
// division            円の分割数(数値が大きいと滑らかな円になる)
// line_width          線幅
// color               色
// angle_rad          回転角度(ラジアン)
// scaling            横、縦の拡大縮小率
// origin             矩形の原点位置
void drawArc(const float center_x, const float center_y,
             const float radius_x, const float radius_y,
             const float start_rad, const float end_rad,
             const int division,
             const float line_width,
             const Color& color,
             const float angle_rad,
             const Vec2f& scaling,
             const Vec2f& origin);

// 塗り潰し円弧
// center_x, center_y  円の中心位置
// radius_x, radius_y  半径(横と縦)
// start_rad, end_rad  開始・終了角度
//                     角度が０で真上。プラスで時計回りの方向となる
//                     (時計と同じ)
// division            円の分割数(数値が大きいと滑らかな円になる)
// color               色
void drawFillArc(const float center_x, const float center_y,
                 const float radius_x, const float radius_y,
                 const float start_rad, const float end_rad,
                 const int division,
                 const Color& color);

// 塗り潰し円弧(回転、拡大縮小つき)
// center_x, center_y  円の中心位置
// radius_x, radius_y  半径(横と縦)
// start_rad, end_rad  開始・終了角度
//                     角度が０で真上。プラスで時計回りの方向となる
//                     (時計と同じ)
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
                 const Vec2f& origin);


// 矩形
// start_x, start_y 始点
// width, height    幅、高さ
// line_width       線幅
// color            色
void drawBox(const float start_x, const float start_y,
             const float width, const float height,
             const float line_width,
             const Color& color);

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
             const Vec2f& origin);

// 塗り潰し矩形
// start_x, start_y 始点
// width, height    幅、高さ
// color            色
void drawFillBox(const float start_x, const float start_y,
                 const float width, const float height,
                 const Color& color);

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
                 const Vec2f& origin);


// 四角形
// x1, y1 ~ x4, y4  頂点
// line_width       線幅
// color            色
void drawQuad(const float x1, const float y1,
              const float x2, const float y2,
              const float x3, const float y3,
              const float x4, const float y4,
              const float line_width,
              const Color& color);

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
              const Vec2f& origin);

// 塗り潰し四角
// x1, y1 ~ x4, y4  頂点
// color            色
void drawFillQuad(const float x1, const float y1,
                  const float x2, const float y2,
                  const float x3, const float y3,
                  const float x4, const float y4,
                  const Color& color);

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
                  const Vec2f& origin);

  
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
                    const Color& color = Color::white);

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
                    const Vec2f& origin);
