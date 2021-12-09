#pragma once

#include <fstream>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <CBCore/Defines.h>

#include "core_LineParser.h"

namespace core {
  class FileParser : public LineParser {
  public:
    FileParser(const cb::string& filePath);
    ~FileParser() override;

    bool readLine();

  private:
    std::ifstream file;
  };
}