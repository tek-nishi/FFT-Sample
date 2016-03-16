#pragma once

//
// ベクトル定義
// FIXME:OpenGLに依存している
//

#include "defines.hpp"
#include <Eigen/Geometry>
#include <GLFW/glfw3.h>


typedef Eigen::Matrix<GLint, 2, 1>	 Vec2i;
typedef Eigen::Matrix<GLfloat, 2, 1> Vec2f;
typedef Eigen::Matrix<GLfloat, 3, 1> Vec3f;
typedef Eigen::Matrix<GLfloat, 4, 1> Vec4f;

typedef Eigen::Quaternion<GLfloat> Quatf;


// 向きがランダムで長さが１のベクトルを作成
template<typename T>
T randomVector() {
  T v = T::Random();
  if (!v.squaredNorm()) {
    // 長さが０の場合はx成分を1にしておく
    v.x() = 1.0f;
  }
  return v.normalized();
}
