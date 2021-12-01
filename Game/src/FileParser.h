#pragma once

#include <fstream>

#include <CBCore/Defines.h>

namespace core {
  class FileParser {
  public:
    FileParser(const cb::string& filePath);
    ~FileParser();

    bool readLine();

    cb::string getCmd() const;
    cb::strvector getArgs() const;

  private:
    std::ifstream file;

    cb::string cmd;
    cb::strvector args;
  };
}