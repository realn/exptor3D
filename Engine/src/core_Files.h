#pragma once

#include <CBCore/Defines.h>

namespace core {
  cb::strvector getFileList(const cb::string& path, const cb::strvector& exts = cb::strvector());
}