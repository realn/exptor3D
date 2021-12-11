
#include <CBCore/StringConvert.h>

#include "core_StrArgList.h"

namespace core {
  StrArgList::StrArgList() = default;

  StrArgList::StrArgList(const cb::strvector& args) : args(args) {}

  StrArgList::~StrArgList() = default;

  cb::strvector StrArgList::getArgs() const {
    return args;
  }

  cb::string StrArgList::getArg(size_t index, cb::string def) const {
    if (index >= args.size()) {
      converror(L"Requested non existing arg nr " + std::to_wstring(index));
      return def;
    }
    return args[index];
  }

  cb::utf8string StrArgList::getArgUtf8(size_t index, cb::utf8string def) const {
    return cb::toUtf8(getArg(index, cb::fromUtf8(def)));
  }

  float StrArgList::getFloat(size_t index, float def) const {
    float result;
    if (!cb::fromStr(getArg(index), result)) {
      converror(L"Failed to convert arg nr " + std::to_wstring(index) + L" to float, returning def value");
      return def;
    }
    return result;
  }

  cb::s32 StrArgList::getInt(size_t index, cb::s32 def) const {
    cb::s32 result;
    if (!cb::fromStr(getArg(index), result)) {
      converror(L"Failed to convert arg nr " + std::to_wstring(index) + L" to int, returning def value");
      return def;
    }
    return result;
  }

  cb::u32 StrArgList::getUInt(size_t index, cb::u32 def) const {
    cb::u32 result;
    if (!cb::fromStr(getArg(index), result)) {
      converror(L"Failed to convert arg nr " + std::to_wstring(index) + L" to unsigned int, returning def value");
      return def;
    }
    return result;
  }

  glm::vec2 StrArgList::getVec2FromArgs(size_t index, glm::vec2 def) const {
    return glm::vec2{
      getFloat(index + 0, def.x),
      getFloat(index + 1, def.y)
    };
  }

  glm::vec3 StrArgList::getVec3FromArgs(size_t index, glm::vec3 def) const {
    return glm::vec3{
      getFloat(index + 0, def.x),
      getFloat(index + 1, def.y),
      getFloat(index + 2, def.z)
    };
  }

  glm::vec4 StrArgList::getVec4FromArgs(size_t index, glm::vec4 def) const {
    return glm::vec4{
      getFloat(index + 0, def.x),
      getFloat(index + 1, def.y),
      getFloat(index + 2, def.z),
      getFloat(index + 3, def.w)
    };
  }

  void StrArgList::converror(const cb::string& ) const {
  }

}