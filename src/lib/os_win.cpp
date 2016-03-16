//
// OS依存処理(Windows版)
//

#if defined (_MSC_VER)

#include "os_win.hpp"
#include <iostream>
#include <streambuf>


std::streambuf::int_type DbgStreambuf::overflow(int_type c) {
  str_.push_back(c);
  return c;
}
	
int DbgStreambuf::sync() {
	str_.push_back('\0');

	OutputDebugString(&str_[0]);
	str_.clear();
	return 0;
}


Os::Os() :
  resource_path_("res/"),
  document_path_("")
{
  // エラーが発生したらメッセージボックスに表示する
  _set_error_mode(_OUT_TO_MSGBOX);

  // デバッグコンソールにcoutできるようにする
  stream_ = std::cout.rdbuf(&dbgStream_);

  DOUT << "Os()" << std::endl;
}

Os::~Os() {
  DOUT << "~Os()" << std::endl;

  // coutの挙動を元に戻す
  std::cout.rdbuf(stream_);
}

const std::string& Os::resourcePath() const { return resource_path_; }
const std::string& Os::documentPath() const { return document_path_; }

#endif
