#pragma once


#include <CBCore/Defines.h>

#include "core_StrArgList.h"

namespace core {
  class LineParser : public StrArgList {
  public:
    ~LineParser() override;

    void parse(const cb::string& line);

    cb::string getCmd() const;

  protected:
    cb::string cmd;
  };
}