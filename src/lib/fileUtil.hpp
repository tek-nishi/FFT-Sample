#pragma once

//
// ファイル関連の雑用
//

#include "defines.hpp"
#include <sys/stat.h>
#include <string>


// ディレクトリ名を返す
// ex) hoge/fuga/piyo.txt -> hoge/fuga
std::string getDirectoryname(const std::string& path);

// ファイル名を返す
// ex) hoge/fuga/piyo.txt -> piyo.txt
std::string getFilename(const std::string& path);

// ディレクトリ名と拡張子を除いたファイル名を返す
// ex) hoge/fuga/piyo.txt -> piyo
std::string getFilenameNoExt(const std::string& path);

// パスから拡張子を返す
// ex) hoge/fuga/piyo.txt -> txt
std::string getFilenameExt(const std::string& path);

// 拡張子を変更する
// ex) hoge/fuga/piyo.txt -> hoge/fuga/piyo.data
std::string replaceFilenameExt(const std::string& path, const std::string& ext);

// パスの有効判定
bool isValidPath(const std::string& path);
