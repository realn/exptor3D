
#include <CBCore/Logger.h>

#include "Application.h"

using namespace std::string_literals;

int WinMain() {
  auto logger = cb::Logger::getInstance();
  auto filename = L"main.log"s;
  logger->addStream(std::make_shared<std::wofstream>(filename, std::ios::out));

  auto app = std::make_shared<mdlview::Application>();

  return app->exec();
}