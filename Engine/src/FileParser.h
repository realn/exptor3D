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

    cb::string getArg(size_t index, cb::string def = cb::string()) const;
    cb::utf8string getArgUtf8(size_t index, cb::utf8string def = cb::utf8string()) const;

    float getFloat(size_t index, float def = 0.0f) const;
    cb::s32 getInt(size_t index, cb::s32 def = 0) const;
    cb::u32 getUInt(size_t index, cb::u32 def = 0) const;

    glm::vec2 getVec2FromArgs(size_t index, glm::vec2 def = glm::vec2()) const;
    glm::vec3 getVec3FromArgs(size_t index, glm::vec3 def = glm::vec3()) const;
    glm::vec4 getVec4FromArgs(size_t index, glm::vec4 def = glm::vec4()) const;

  private:
    std::ifstream file;
    cb::string cmd;
    cb::strvector args;
  };
}