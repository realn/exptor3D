#pragma once

#include <CBCore/Defines.h>

namespace core {
  enum class LogType {
    Debug = 0,
    Info,
    Report,
    Warning,
    Error,
    Fatal
  };

  class Object {
  public:
    Object(const cb::string& className);
    virtual ~Object();

    const cb::string& getClassName() const;
    virtual cb::string getLogName() const;

  protected:
    void setClassName(const cb::string& name);

    void debug(cb::string msg) const;
    void log(cb::string msg) const;
    void report(cb::string msg) const;
    void warning(cb::string msg) const;
    void error(cb::string msg) const;
    void fatal(cb::string msg) const;

    virtual void writeLog(LogType type, cb::string line) const;
    void writeLogLines(LogType type, const cb::strvector& lines) const;

    cb::string className;

  private:
    cb::string createLogLine(cb::string& msg) const;
  };


  class LogObject : public Object {
  public:
    LogObject(const cb::string& className);
    ~LogObject() override;

    cb::strvector getLogLines() const;

  protected:
    void writeLog(LogType type, cb::string line) const override;

    mutable cb::strvector logLines;
  };
}
