//
// GLFWのWindow管理
//

#include "glfwWindow.hpp"
#include "glExt.hpp"
#include <iostream>


// リンクするライブラリの指示(Windows)
#if defined (_MSC_VER)
#ifdef DEBUG
#pragma comment(lib, "glfw3d.lib")
#else
#pragma comment(lib, "glfw3.lib")
#endif
#pragma comment(lib, "opengl32.lib")
#endif


GlfwWindow::GlfwWindow(const int width, const int height,
                       const bool is_visible, const bool is_fullscreen) {
  DOUT << "GlfwWindow()" << std::endl;

  if (!glfwInit()) throw "Can't Initialize GLFW.";

  if (!is_visible) glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    
  window_ = glfwCreateWindow(width, height, PREPRO_TO_STR(PRODUCT_NAME),
                             is_fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

  if (!window_) throw "Can't create GLFW window.";
    
  glfwMakeContextCurrent(window_);
  glfwSwapInterval(1);

  // TIPS:GLEWの初期化はglfwMakeContextCurrentの後で
  if (!initGlExt()) throw "Can't use OpenGL extensions.";
}

GlfwWindow::~GlfwWindow() {
  DOUT << "~GlfwWindow()" << std::endl;

  glfwDestroyWindow(window_);
  glfwTerminate();
}

GLFWwindow* const GlfwWindow::operator()() { return window_; }
const GLFWwindow* const GlfwWindow::operator()() const { return window_; }
