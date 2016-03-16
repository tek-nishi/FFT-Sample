//
// 行列定義
// FIXME:OpenGLに依存している
//

#include "matrix.hpp"


// 回転、スケーリング、平行移動から変換行列を生成(2D向け)
// rotate    回転量(ラジアン)
// transtate 平行移動量
// scaling   スケーリング
Affinef transformMatrix2D(const float rotate_rad, const Vec3f& transtate, const Vec3f& scaling) {
  Affinef matrix;

  Translation t(transtate);
  Scaling s(scaling);
  Quatf r(Eigen::AngleAxisf(rotate_rad, Vec3f::UnitZ()));

  matrix = t * r * s;
  
  return matrix;
}


// 正投影行列を生成
// SOURCE:mesa
Affinef orthoMatrix(const GLfloat left, const GLfloat right,
                    const GLfloat bottom, const GLfloat top,
                    const GLfloat nearval, const GLfloat farval) {
	Affinef m;

	m(0,0) = 2.0f / (right-left);
	m(0,1) = 0.0f;
	m(0,2) = 0.0f;
	m(0,3) = -(right+left) / (right-left);

	m(1,0) = 0.0f;
	m(1,1) = 2.0f / (top-bottom);
	m(1,2) = 0.0f;
	m(1,3) = -(top+bottom) / (top-bottom);

	m(2,0) = 0.0f;
	m(2,1) = 0.0f;
	m(2,2) = -2.0f / (farval-nearval);
	m(2,3) = -(farval+nearval) / (farval-nearval);

	m(3,0) = 0.0f;
	m(3,1) = 0.0f;
	m(3,2) = 0.0f;
	m(3,3) = 1.0f;

  return m;
}

// 透視投影行列を生成
// SOURCE:mesa
Affinef frustumMatrix(const GLfloat left, const GLfloat right,
                      const GLfloat bottom, const GLfloat top,
                      const GLfloat nearval, const GLfloat farval) {
	GLfloat x, y, a, b, c, d;

	x = (2.0f * nearval) / (right-left);
	y = (2.0f * nearval) / (top-bottom);
	a = (right+left) / (right-left);
	b = (top+bottom) / (top-bottom);
	c = -(farval+nearval) / ( farval-nearval);
	d = -(2.0f * farval*nearval) / (farval-nearval);  /* error? */

	Affinef m;
	m(0,0) = x;
	m(0,1) = 0.0f;
	m(0,2) = a;
	m(0,3) = 0.0f;
	 
	m(1,0) = 0.0f;
	m(1,1) = y;
	m(1,2) = b;
	m(1,3) = 0.0f;
	 
	m(2,0) = 0.0f;
	m(2,1) = 0.0f;
	m(2,2) = c;
	m(2,3) = d;
	 
	m(3,0) = 0.0f;
	m(3,1) = 0.0f;
	m(3,2) = -1.0f;
	m(3,3) = 0.0f;

  return m;
}
