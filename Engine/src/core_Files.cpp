
#include <filesystem>

#include "core_Files.h"

namespace core {
  cb::strvector getFileList(const cb::string& path, const cb::strvector& exts) {
    auto result = cb::strvector();
    if (exts.empty()) {
      auto dir = std::filesystem::directory_iterator{ path };
      for (const auto& entry : dir) {
        if (entry.is_directory())
          continue;

        result.push_back(entry.path().filename().wstring());
      }
      return result;
    }

    auto dir = std::filesystem::directory_iterator(path);
    for (const auto& entry : dir) {
      if (entry.is_directory())
        continue;

      auto ext = entry.path().extension().wstring();
      if (std::find(exts.begin(), exts.end(), ext) == exts.end())
        continue;

      result.push_back(entry.path().filename().wstring());
    }

    return result;
  }
}