
#include <iostream>

#include <CBCore/StringFunctions.h>
#include <CBCore/StringConvert.h>

#include "StrEx.h"
#include "FileParser.h"

namespace core {
  namespace {

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
      if (trimLeft(line).empty() || trimLeft(line).front() == L'#')
        return cb::string();

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
        item = removeQuotes(item);

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

    cmd = cb::string();
    args.clear();

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

  cb::utf8string FileParser::getArgUtf8(size_t index, cb::utf8string def) const {
    return cb::toUtf8(getArg(index, cb::fromUtf8(def)));
  }

  float FileParser::getFloat(size_t index, float def) const {
    float result;
    if(!cb::fromStr(getArg(index), result))
      return def;
    return result;
  }

  cb::s32 FileParser::getInt(size_t index, cb::s32 def) const {
    cb::s32 result;
    if (!cb::fromStr(getArg(index), result))
      return def;
    return result;
  }

  cb::u32 FileParser::getUInt(size_t index, cb::u32 def) const {
    cb::u32 result;
    if (!cb::fromStr(getArg(index), result))
      return def;
    return result;
  }

  glm::vec2 FileParser::getVec2FromArgs(size_t index, glm::vec2 def) const {
    return glm::vec2{
      getFloat(index + 0, def.x),
      getFloat(index + 1, def.y)
    };
  }

  glm::vec3 FileParser::getVec3FromArgs(size_t index, glm::vec3 def) const {
    return glm::vec3{
      getFloat(index + 0, def.x),
      getFloat(index + 1, def.y),
      getFloat(index + 2, def.z)
    };
  }

  glm::vec4 FileParser::getVec4FromArgs(size_t index, glm::vec4 def) const {
    return glm::vec4{
      getFloat(index + 0, def.x),
      getFloat(index + 1, def.y),
      getFloat(index + 2, def.z),
      getFloat(index + 3, def.w)
    };
  }
}
