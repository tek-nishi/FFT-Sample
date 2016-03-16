#pragma once

//
// OS依存処理(Windows版)
//

#if defined (_MSC_VER)

#include "defines.hpp"
#include <string>
#include <vector>
#include <streambuf>


// デバッグコンソールにcoutする
class DbgStreambuf : public std::streambuf {
  std::vector<TCHAR> str_;
  // ↑OutputDebugString()がcharではなくTCHARを要求する


public:
  int_type overflow(int_type c = EOF);

  int sync();

};


class Os {
	DbgStreambuf dbgStream_;
	std::streambuf* stream_;

	std::string resource_path_;
	std::string document_path_;


public:
	Os();
	~Os();

  // このクラスはコピー禁止
  Os(const Os&) = delete;
  Os& operator=(const Os&) = delete;

  
	const std::string& resourcePath() const;
	const std::string& documentPath() const;
  
};

#endif
