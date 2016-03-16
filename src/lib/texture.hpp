#pragma once

//
// テクスチャ管理
// FIXME:OpenGLに依存している
//

#include "defines.hpp"
#include "glTexture.hpp"
#include <string>
#include <memory>
#include <GLFW/glfw3.h>


class Texture {
  std::shared_ptr<GlTexture> gl_texture_;

  int width_;
  int height_;

	
public:
  Texture();
  
	explicit Texture(const std::string& filename);

  // サイズを返す
  int width() const;
  int height() const;

  // OpenGLのコンテキストに拘束する
	void bind() const;

  // 拘束を解除
	void unbind() const;


private:
  // テクスチャの基本的なパラメーター設定を行う
  static void setupParam();
  
	void setupPng(const std::string& filename);
  
};
