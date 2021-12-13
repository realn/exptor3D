#include <CBCore/StringConvert.h>
#include <CBCore/Log.h>

#include "core_object.h"

namespace core {
  Object::Object(const cb::string& className) : className(className) {
  }

  Object::~Object() = default;

  const cb::string& Object::getClassName() const {
    return className;
  }

  cb::string Object::getLogName() const {
    return getClassName() + L"():";
  }

  void Object::setClassName(const cb::string& name) {
    className = name;
  }

  void Object::debug(cb::string msg) const {
    writeLog(LogType::Debug, createLogLine(msg));
  }

  void Object::log(cb::string msg) const {
    writeLog(LogType::Info, createLogLine(msg));
  }

  void Object::error(cb::string msg) const {
    writeLog(LogType::Error, createLogLine(msg));
  }

  void Object::fatal(cb::string msg) const {
    writeLog(LogType::Fatal, createLogLine(msg));
  }

  void Object::report(cb::string msg) const {
    writeLog(LogType::Report, createLogLine(msg));
  }

  void Object::warning(cb::string msg) const {
    writeLog(LogType::Warning, createLogLine(msg));
  }

  void Object::writeLog(LogType type, cb::string line) const {
    switch (type) {
    case core::LogType::Debug:
      cb::log(cb::LogLvl::Debug, line);
      break;
    case core::LogType::Info:
      cb::log(cb::LogLvl::Info, line);
      break;
    case core::LogType::Report:
      cb::log(cb::LogLvl::Info, L"[R]" + line);
      break;
    case core::LogType::Warning:
      cb::log(cb::LogLvl::Warning, line);
      break;
    case core::LogType::Error:
      cb::log(cb::LogLvl::Error, line);
      break;
    case core::LogType::Fatal:
      cb::log(cb::LogLvl::Error, L"[F]" + line);
      break;
    }
  }

  void Object::writeLogLines(LogType type, const cb::strvector& lines) const {
    for (auto& line : lines) {
      writeLog(type, line);
    }
  }

  cb::string Object::createLogLine(cb::string& msg) const {
    return getLogName() + L" " + msg;
  }

  LogObject::LogObject(const cb::string& className) : Object(className) {}

  LogObject::~LogObject() = default;

  cb::strvector LogObject::getLogLines() const {
    return logLines;
  }

  void LogObject::writeLog(LogType, cb::string line) const {
    logLines.push_back(line);
  }
}