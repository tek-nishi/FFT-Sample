#pragma once

//
// OpenGLのテクスチャハンドリング
//

#include "defines.hpp"
#include <GLFW/glfw3.h>

class GlTexture {
	GLuint id_;

public:
	GlTexture();
	~GlTexture();

  // このクラスはコピー禁止
  GlTexture(const GlTexture&) = delete;
  GlTexture& operator=(const GlTexture&) = delete;
  
  // OpenGLのコンテキストに拘束する
	void bind() const;

  // 拘束を解除
	void unbind() const;
  
};
