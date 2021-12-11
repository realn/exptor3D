#pragma once


#include <CBCore/Defines.h>

#include "core_object.h"
#include "core_StrArgList.h"

namespace core {
  class LineParser : public LogObject, public StrArgList {
  public:
    LineParser();
    ~LineParser() override;

    void parse(const cb::string& line);

    cb::string getCmd() const;

    cb::string parseArgLine(const cb::string& line);

  protected:
    void converror(const cb::string& msg) const override;

    cb::string cmd;
  };
}