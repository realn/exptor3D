#pragma once

#include <fstream>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <CBCore/Defines.h>

namespace core {
  class FileParser {
  public:
    FileParser(const cb::string& filePath);
    ~FileParser();

    bool readLine();

    cb::string getCmd() const;
    cb::strvector getArgs() const;

    cb::string getArg(size_t index, cb::string default = cb::string()) const;

    float getFloat(size_t index) const;
    cb::s32 getInt(size_t index) const;
    cb::u32 getUInt(size_t index) const;

    glm::vec2 getVec2FromArgs(size_t index) const;
    glm::vec3 getVec3FromArgs(size_t index) const;
    glm::vec4 getVec4FromArgs(size_t index) const;

  private:
    std::ifstream file;
    cb::string cmd;
    cb::strvector args;
  };
}