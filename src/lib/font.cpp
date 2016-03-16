//
// フォント
//

#include "font.hpp"
#include <iostream>


// 文字表示ライブラリのリンクを指示
#if defined (_MSC_VER)
#pragma comment (lib, "glu32.lib")
#ifdef _DEBUG
#pragma comment (lib, "freetyped.lib")
#pragma comment (lib, "ftgld.lib")
#else
#pragma comment (lib, "freetype.lib")
#pragma comment (lib, "ftgl.lib")
#endif
#endif


// コンストラクタ
// font_path フォントファイル(ttf,otf)
// mode      フォント生成方式
Font::Font(const std::string& font_path, const int font_size,
           const int mode) {
  font_ = std::shared_ptr<FTFont>(setup(font_path, mode));
  font_->UseDisplayList(false);
  size(font_size);
  
  DOUT << "Font(" << font_path << ")" << std::endl;
}

// フォントを読み込み直す
// font_path フォントファイル(ttf, otf)
// mode      フォント生成方式(BUFFER, CACHE, POLYGON)
void Font::read(const std::string& font_path, const int font_size,
                const int mode) {
  font_ = std::shared_ptr<FTFont>(setup(font_path, mode));
  font_->UseDisplayList(false);
  size(font_size);

  DOUT << "Font::read(" << font_path << ")" << std::endl;
}


// フォントサイズ指定
void Font::size(const int size) {
  font_->FaceSize(size);
}

// 描画した時のサイズを取得
Vec2f Font::drawSize(const std::string& text) {
  float llx, lly, llz;
  float urx, ury, urz;
  font_->BBox(text.c_str(), llx, lly, llz, urx, ury, urz);
  return Vec2f(urx - llx, ury - lly);
}

// 描画
// text  表示文字列
// pos   表示位置
// color 表示色
void Font::draw(const std::string& text, const Vec2f& pos, const Color& color) {
  color.setToGl();

  // OpenGLの行列スタックを利用
  glPushMatrix();
  font_->Render(text.c_str(), -1, FTPoint(pos.x(), pos.y()), FTPoint());
  glPopMatrix();
}


// 適当なFontを生成して返却
// font_path  Font fine path
// mode       生成モード(BUFFER, CACHE, POLYGON)
FTFont* Font::setup(const std::string& font_path, const int mode) {
  switch (mode) {
  case BUFFER:
    return new FTBufferFont(font_path.c_str());

  case CACHE:
    return new FTTextureFont(font_path.c_str());

  case POLYGON:
    return new FTPolygonFont(font_path.c_str());

  default:
    assert(0 && "Font type error.");
    return nullptr;
  }
}
