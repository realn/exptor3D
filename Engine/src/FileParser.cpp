
#include <iostream>

#include <CBCore/StringFunctions.h>
#include <CBCore/StringConvert.h>

#include "FileParser.h"

namespace core {
  namespace {
    auto whitespaceChars = std::vector<wchar_t>{L' ', L'\t', L'\r', L'\n'};
    bool isWhitespace(wchar_t value) {
      return std::find(whitespaceChars.begin(), whitespaceChars.end(), value) != whitespaceChars.end();
    }

    cb::string trimLeft(cb::string line) {
      if (line.empty())
        return cb::string();
      while (isWhitespace(*line.begin())) {
        line.erase(line.begin());
      }
      return line;
    }

    cb::string trimRight(cb::string line) {
      if (line.empty())
        return cb::string();
      while (isWhitespace(*line.rbegin())) {
        line.pop_back();
      }
      return line;
    }

    cb::string trim(cb::string line) {
      if (line.empty())
        return cb::string();
      return trimRight(trimLeft(line));
    }

    cb::string::size_type findNonQuoted(const cb::string& line, cb::string::value_type value, cb::string::size_type begpos = 0) {
      bool quote = false;
      for(auto pos = begpos; pos < line.length(); pos++) {
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

    cb::string parseCmd(const cb::string& line) {
      auto pos = line.find(L"(");
      if (pos == cb::string::npos)
        return trim(line);

      return trim(line.substr(0, pos));
    }

    cb::string parseArgLine(const cb::string& line) {
      auto pos = line.find(L"(");
      if (pos == cb::string::npos)
        return cb::string();

      pos++;
      auto endpos = findNonQuoted(line, ')', pos);
      if (endpos == cb::string::npos) {
        return cb::string();
      }

      return trim(line.substr(pos, endpos - pos));
    }

    cb::strvector parseArgList(const cb::string& line) {
      cb::string::size_type pos = 0;
      auto endpos = pos;

      auto result = cb::strvector();
      do {
        endpos = findNonQuoted(line, ',', pos);
        auto item = line.substr(pos, endpos != cb::string::npos ? endpos - pos : endpos);

        result.push_back(trim(item));
        if (endpos != cb::string::npos)
          endpos++;
        pos = endpos;
      } while (pos != cb::string::npos);

      return result;
    }
  }

  FileParser::FileParser(const cb::string& filePath) 
    : file(filePath) {
  }

  FileParser::~FileParser() = default;

  bool FileParser::readLine() {
    if (!file)
      return false;

    do {
      cb::utf8string lineutf8;
      if (!std::getline(file, lineutf8))
        return false;

      auto line = cb::fromUtf8(lineutf8);

      cmd = parseCmd(line);
      auto argLine = parseArgLine(line);
      args = parseArgList(argLine);
    } while (cmd.empty() && file);
    
    if (!file)
      return false;
    return true;
  }

  cb::string FileParser::getCmd() const {
    return cmd;
  }

  cb::strvector FileParser::getArgs() const {
    return args;
  }

  cb::string FileParser::getArg(size_t index, cb::string def) const {
    if (index >= args.size())
      return def;
    return args[index];
  }

  float FileParser::getFloat(size_t index) const {
    float result;
    if(!cb::fromStr(getArg(index), result))
      return 0.0f;
    return result;
  }

  cb::s32 FileParser::getInt(size_t index) const {
    cb::s32 result;
    if (!cb::fromStr(getArg(index), result))
      return 0;
    return result;
  }

  cb::u32 FileParser::getUInt(size_t index) const {
    cb::u32 result;
    if (!cb::fromStr(getArg(index), result))
      return 0;
    return result;
  }

  glm::vec2 FileParser::getVec2FromArgs(size_t index) const {
    return glm::vec2{
      getFloat(index + 0),
      getFloat(index + 1)
    };
  }

  glm::vec3 FileParser::getVec3FromArgs(size_t index) const {
    return glm::vec3{
      getFloat(index + 0),
      getFloat(index + 1),
      getFloat(index + 2)
    };
  }

  glm::vec4 FileParser::getVec4FromArgs(size_t index) const {
    return glm::vec4{
      getFloat(index + 0),
      getFloat(index + 1),
      getFloat(index + 2),
      getFloat(index + 3)
    };
  }
}
