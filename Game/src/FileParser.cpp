
#include <iostream>

#include <CBCore/StringFunctions.h>
#include <CBCore/StringConvert.h>

#include "FileParser.h"

namespace core {
  FileParser::FileParser(const cb::string& filePath) 
    : file(filePath) {
  }

  FileParser::~FileParser() = default;

  bool FileParser::readLine() {
    if (!file)
      return false;

    auto list = cb::strvector();
    do {
      cb::utf8string lineutf8;
      if (!std::getline(file, lineutf8))
        return false;

      auto line = cb::fromUtf8(lineutf8);
      list = cb::split(line, cb::strvector{ L" ", L"\t", L"\r" }, true);
    } while (list.empty());
    
    cmd = list[0];
    list.erase(list.begin());
    args.clear();
    args.insert(list.begin(), list.begin(), list.end());

    return true;
  }

  cb::string FileParser::getCmd() const {
    return cmd;
  }

  cb::strvector FileParser::getArgs() const {
    return args;
  }
}
