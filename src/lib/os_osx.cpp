//
// OS依存処理(OSX版)
//

#if defined (__APPLE__) && !(TARGET_OS_IPHONE)

#include "os_osx.hpp"
#include <unistd.h>
#include <iostream>
#include <string>


Os::Os() {
  DOUT << "Os()" << std::endl;

  std::string path = currentPath();
  resource_path_ = path + "res/";
  document_path_ = path;
}

  
const std::string& Os::resourcePath() const { return resource_path_; }
const std::string& Os::documentPath() const { return document_path_; }


#ifdef DEBUG

#ifndef SRCROOT
#define SRCROOT ./
#endif

// デバッグ時はプロジェクトのディレクトリからファイルを読む
std::string Os::currentPath() {
  return std::string(PREPRO_TO_STR(SRCROOT));
}

#else

// リリース時は.app内のファイルを読み込む
std::string Os::currentPath() {
  return std::string("");
}

#endif

#endif
