//
// カメラ(2D向け)
//

#include "camera2D.hpp"
#include <iostream>
#include <utility>
#include "matrix.hpp"


Camera2D::Camera2D() :
  z_(1.0f)
{
  DOUT << "Camera()" << std::endl;
}
  
  
std::pair<Affinef, Affinef> Camera2D::operator()(const Vec2f& view_size) const {
  // z = 0 の時に正しい大きさで表示されるように透視変換行列を作成
  float twice_z = z_ * 2.0f;
  return std::make_pair(frustumMatrix(-view_size.x() / twice_z, view_size.x() / twice_z,
                                      -view_size.y() / twice_z, view_size.y() / twice_z,
                                      1.0f, twice_z),
                        Affinef(Translation(0.0f, 0.0f, -z_)));
}

