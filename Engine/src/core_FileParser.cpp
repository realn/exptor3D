
#include <iostream>

#include <CBCore/StringFunctions.h>
#include <CBCore/StringConvert.h>

#include "StrEx.h"
#include "core_FileParser.h"

namespace core {
  FileParser::FileParser(const cb::string& filePath) 
    : file(filePath) {
  }

  FileParser::~FileParser() = default;

  bool FileParser::readLine() {
    if (!file)
      return false;

    do {
      cb::utf8string lineutf8;
      if (!std::getline(file, lineutf8))
        return false;

      auto line = cb::fromUtf8(lineutf8);
      parse(line);
    } while (cmd.empty() && file);
    
    if (!file)
      return false;
    return true;
  }

}
