#pragma once

//
// OS依存処理(OSX版)
//

#if defined (__APPLE__) && !(TARGET_OS_IPHONE)

#include "defines.hpp"
#include <string>


class Os {
	std::string resource_path_;
	std::string document_path_;

  
public:
	Os();

  // TIPS:このクラスはコピー禁止
  Os(const Os&) = delete;
  Os& operator=(const Os&) = delete;

  
	const std::string& resourcePath() const;
	const std::string& documentPath() const;


private:
  static std::string currentPath();
};

#endif
