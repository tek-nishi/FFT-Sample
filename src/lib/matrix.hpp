#pragma once

//
// 行列定義
// FIXME:OpenGLに依存している
//

#include "defines.hpp"
#include <utility>
#include <Eigen/Geometry>
#include <GLFW/glfw3.h>
#include "vector.hpp"


typedef Eigen::Translation<float, 3> Translation;
typedef Eigen::Transform< GLfloat, 3, Eigen::Affine> Affinef;

// FIXME:コンストラクタでx,y,z全て渡さないと初期値が未定義になる
typedef Eigen::DiagonalMatrix<GLfloat, 3> Scaling; 

typedef Eigen::Matrix<GLfloat, 4, 4> Mat3f;
typedef Eigen::Matrix<GLfloat, 4, 4> Mat4f;


// 回転、スケーリング、平行移動から変換行列を生成(2D向け)
// rotate    回転量(ラジアン)
// transtate 平行移動量
// scaling   スケーリング
Affinef transformMatrix2D(const float rotate_rad, const Vec3f& transtate, const Vec3f& scaling);

// 正投影行列を生成
// SOURCE:mesa
Affinef orthoMatrix(const GLfloat left, const GLfloat right,
                    const GLfloat bottom, const GLfloat top,
                    const GLfloat nearval, const GLfloat farval);

// 透視投影行列を生成
// SOURCE:mesa
Affinef frustumMatrix(const GLfloat left, const GLfloat right,
                      const GLfloat bottom, const GLfloat top,
                      const GLfloat nearval, const GLfloat farval);
