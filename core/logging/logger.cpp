#include "logger.hpp"

namespace logging {

ILogHandler::ILogHandler() : level(LogLevel::Error) {}

void ILogHandler::setLogLevel(LogLevel level)
{
  this->level = level;
}

bool ILogHandler::isLevelEnabled(LogLevel level) const
{
  return level <= this->level;
}

LogManager* LogManager::getInstance()
{
  static LogManager g_logger;
  return &g_logger;
}

void LogManager::addHandler(std::string name, ILogHandler* handler)
{
  handlers[name] = handler;
}

void LogManager::removeHandler(std::string name)
{
  handlers.erase(name);
}

void LogManager::log(std::string& msg, LogLevel level)
{
  for (auto const& kv : handlers) {
    auto handler = kv.second;
    if (handler->isLevelEnabled(level)) {
      switch (level) {
        case LogLevel::Info:
          handler->info(msg);
          break;
        case LogLevel::Warn:
          handler->warn(msg);
          break;
        case LogLevel::Error:
          handler->error(msg);
          break;
        case LogLevel::Debug:
          handler->debug(msg);
          break;
      }
    }
  }
}

void LogManager::info(std::string msg)
{
  log(msg, LogLevel::Info);
}

void LogManager::warn(std::string msg)
{
  log(msg, LogLevel::Warn);
}

void LogManager::error(std::string msg)
{
  log(msg, LogLevel::Error);
}

void LogManager::debug(std::string msg)
{
  log(msg, LogLevel::Debug);
}

} /* namespace logging */
