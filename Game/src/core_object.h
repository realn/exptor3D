#pragma once

#include <CBCore/Defines.h>

namespace core {
  class Object {
  public:
    Object(const cb::string& className);
    virtual ~Object();

    const cb::string& getClassName() const;
    virtual cb::string getLogName() const;

  protected:
    void log(std::string msg) const;
    void error(std::string msg) const;
    void report(std::string msg) const;

    cb::string className;
  };
}
