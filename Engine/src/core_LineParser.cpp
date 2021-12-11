
#include <CBCore/StringConvert.h>

#include "StrEx.h"
#include "core_LineParser.h"

namespace core {
  namespace {
    cb::string::size_type findNonQuoted(const cb::string& line, cb::string::value_type value, cb::string::size_type begpos = 0) {
      bool quote = false;
      for (auto pos = begpos; pos < line.length(); pos++) {
        auto val = line[pos];
        if (val == '"') {
          quote = !quote;
          continue;
        }
        if (val == '\\' && pos + 1 < line.length() && line[pos + 1] != '\\') {
          pos++;
          continue;
        }
        if (quote)
          continue;

        if (val == value)
          return pos;
      }
      return cb::string::npos;
    }

    cb::string removeQuotes(cb::string text) {
      if (text.empty())
        return text;
      if (text.front() == L'"')
        text.erase(text.begin());
      if (text.empty())
        return text;
      if (text.back() == L'"')
        text.pop_back();
      return text;
    }

    cb::string parseCmd(const cb::string& line) {
      if (trimLeft(line).empty())
        return cb::string();

      auto pos = line.find(L"(");
      if (pos == cb::string::npos)
        return trim(line);

      return trim(line.substr(0, pos));
    }

    cb::strvector parseArgList(const cb::string& line) {
      cb::string::size_type pos = 0;
      auto endpos = pos;

      auto result = cb::strvector();
      do {
        endpos = findNonQuoted(line, ',', pos);
        auto item = line.substr(pos, endpos != cb::string::npos ? endpos - pos : endpos);
        item = removeQuotes(item);

        result.push_back(trim(item));
        if (endpos != cb::string::npos)
          endpos++;
        pos = endpos;
      } while (pos != cb::string::npos);

      return result;
    }
  }

  LineParser::LineParser() : LogObject(L"LineParser") {}

  LineParser::~LineParser() = default;

  void LineParser::parse(const cb::string& line) {
    cmd = cb::string();
    args.clear();

    if (line.empty()) {
      debug(L"empty line, skipping.");
      return;
    }

    if (trimLeft(line).front() == L'#') {
      debug(L"found comment, skipping line.");
      return;
    }

    cmd = parseCmd(line);
    if (cmd.empty()) {
      error(L"command not found on line.");
      return;
    }

    auto argLine = parseArgLine(line);
    args = parseArgList(argLine);
  }

  cb::string LineParser::getCmd() const {
    return cmd;
  }

  cb::string LineParser::parseArgLine(const cb::string& line) {
    auto pos = line.find(L"(");
    if (pos == cb::string::npos) {
      error(L"no braces for args found on line.");
      return cb::string();
    }

    pos++;
    auto endpos = findNonQuoted(line, ')', pos);
    if (endpos == cb::string::npos) {
      warning(L"no closing brace for args found on line.");
      return cb::string();
    }

    return trim(line.substr(pos, endpos - pos));
  }

  void LineParser::converror(const cb::string& msg) const {
    error(msg);
  }
}