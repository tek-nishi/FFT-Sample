//
// テクスチャ管理
// FIXME:OpenGLに依存している
//

#include "texture.hpp"
#include <iostream>
#include <string>
#include <cassert>
#include "png.hpp"
#include "utils.hpp"


Texture::Texture()
  : width_(0),
    height_(0)
{}

Texture::Texture(const std::string& filename) :
  gl_texture_(std::make_shared<GlTexture>())
{
  DOUT << "Texture()" << std::endl;
  setupPng(filename);
}
	

// サイズを返す
int Texture::width() const { return width_; }
int Texture::height() const { return height_; }

// OpenGLのコンテキストに拘束する
void Texture::bind() const {
  assert(gl_texture_ && "Empty texture.");
  gl_texture_->bind();
}

// 拘束を解除
void Texture::unbind() const {
  glBindTexture(GL_TEXTURE_2D, 0);
}


// テクスチャの基本的なパラメーター設定を行う
void Texture::setupParam() {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

  
void Texture::setupPng(const std::string& filename) {
  Png png_obj(filename);
  width_ = png_obj.width();
  height_ = png_obj.height();
  if ((width_ != int2pow(width_)) || (height_ != int2pow(height_))) {
    DOUT << "Texture size error " << width_ << ":" << height_ << std::endl;
    // サイズが2のべき乗でなければエラー
    return;
  }

  bind();
  setupParam();

  GLint type = (png_obj.type() == PNG_COLOR_TYPE_RGB) ? GL_RGB : GL_RGBA;
  glTexImage2D(GL_TEXTURE_2D, 0, type, width_, height_, 0, type, GL_UNSIGNED_BYTE, png_obj.image());
		
  DOUT << "Texture:" << ((type == GL_RGB) ? " RGB" : " RGBA") << std::endl;
}
