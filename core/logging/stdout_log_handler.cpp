#include "stdout_log_handler.hpp"

#include <iostream>

namespace logging {

void StdOutLogHandler::info(std::string& msg)
{
  std::cout << "\033[0m" << "[Info] " << msg << "\033[0m" << std::endl;
}

void StdOutLogHandler::warn(std::string& msg)
{
  std::cout << "\033[0;33m" << "[Warn] " << msg << "\033[0m" << std::endl;
}

void StdOutLogHandler::error(std::string& msg)
{
  std::cout << "\033[0;31m" << "[Error] " << msg << "\033[0m" << std::endl;
}

void StdOutLogHandler::debug(std::string& msg)
{
  std::cout << "\033[0;34m" << "[Debug] " << msg << "\033[0m" << std::endl;
}

} /* namespace logging */
