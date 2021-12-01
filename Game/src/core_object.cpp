
#include <CBCore/StringConvert.h>

#include "Log.h"

#include "core_object.h"

namespace core {
  Object::Object(const cb::string& className) {
  }

  Object::~Object() = default;

  const cb::string& Object::getClassName() const {
    return className;
  }

  cb::string Object::getLogName() const {
    return getClassName() + L"():";
  }

  void Object::log(std::string msg) const {
    auto line = cb::toUtf8(getLogName() + L" ") + msg;
    Log.Log(line);
  }

  void Object::error(std::string msg) const {
    auto line = cb::toUtf8(getLogName() + L" ") + msg;
    Log.Error(line);
  }

  void Object::report(std::string msg) const {
    auto line = cb::toUtf8(getLogName() + L" ") + msg;
    Log.Report(line);
  }
}