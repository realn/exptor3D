
#include <iostream>

#include <CBCore/StringFunctions.h>
#include <CBCore/StringConvert.h>

#include "StrEx.h"
#include "core_FileParser.h"

namespace core {
  FileParser::FileParser(const cb::string& filePath) 
    : filename(filePath)
    , file(filePath) 
  {
    setClassName(L"FileParser");
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
      linenr++;
    } while (cmd.empty() && file);

    if (!file)
      return false;

    return true;
  }

  bool FileParser::isValid() const {
    return file.good();
  }

  FileParser::operator bool() const {
    return isValid();
  }

  bool FileParser::operator!() const {
    return !isValid();
  }

  cb::string FileParser::getLogName() const {
    return getClassName() + L"(" + filename + L":" + std::to_wstring(linenr) + L"):";
  }

}
