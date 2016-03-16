#pragma once

//
// 文字表示
//

#include "defines.hpp"
#include <string>
#include <memory>
#include "vector.hpp"
#include "graph.hpp"
#include <FTGL/ftgl.h>


class Font {
  std::shared_ptr<FTFont> font_;


public:
  // フォント生成方式
  enum {
    // 一行ごとに文字を生成
    BUFFER,
    // 一文字ごとに生成してキャッシュする
    CACHE,
    // ポリゴンを生成
    POLYGON
  };

  // コンストラクタ
  // font_path フォントファイル(ttf,otf)
  // font_size 文字の大きさ
  // mode      フォント生成方式(BUFFER, CACHE, POLYGON)
  Font(const std::string& font_path, const int font_size,
       const int mode = BUFFER);

  // フォントを読み込み直す
  // font_path フォントファイル(ttf, otf)
  // font_size 文字の大きさ
  // mode      フォント生成方式(BUFFER, CACHE, POLYGON)
  void read(const std::string& font_path, const int font_size,
            const int mode = BUFFER);
  
  
  // フォントサイズ指定
  // FIXME:サイズを変更する度、内部キャッシュが破棄される
  //       →処理が重くなる
  void size(const int size);

  // 描画した時のサイズを取得
  Vec2f drawSize(const std::string& text);

  // 描画
  // text  表示文字列
  // pos   表示位置
  // color 表示色
  void draw(const std::string& text, const Vec2f& pos, const Color& color);


private:
  FTFont* setup(const std::string& font_path, const int mode);
  
};
