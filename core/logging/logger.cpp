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

Logger* Logger::getInstance()
{
  static Logger g_logger;
  return &g_logger;
}

void Logger::addHandler(std::string name, ILogHandler* handler)
{
  handlers[name] = handler;
}

void Logger::removeHandler(std::string name)
{
  handlers.erase(name);
}

void Logger::log(std::string& msg, LogLevel level)
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

void Logger::info(std::string msg)
{
  log(msg, LogLevel::Info);
}

void Logger::warn(std::string msg)
{
  log(msg, LogLevel::Warn);
}

void Logger::error(std::string msg)
{
  log(msg, LogLevel::Error);
}

void Logger::debug(std::string msg)
{
  log(msg, LogLevel::Debug);
}

} /* namespace logging */
