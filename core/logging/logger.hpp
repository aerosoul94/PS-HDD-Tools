#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <map>

#define rAddHandler(name, handler) \
  logging::Logger::getInstance()->addHandler(name, handler)

#define rInfo(msg)  logging::Logger::getInstance()->info(msg)
#define rWarn(msg)  logging::Logger::getInstance()->warn(msg)
#define rError(msg) logging::Logger::getInstance()->error(msg)
#define rDebug(msg) logging::Logger::getInstance()->debug(msg)

namespace logging {

enum class LogLevel
{
  Info,
  Warn,
  Error,
  Debug
};

class ILogHandler
{
public:
  ILogHandler();

  void setLogLevel(LogLevel level);
  bool isLevelEnabled(LogLevel level) const;
  virtual void info(std::string& msg) {};
  virtual void warn(std::string& msg) {};
  virtual void error(std::string& msg) {};
  virtual void debug(std::string& msg) {};

private:
  LogLevel level;
};

class Logger
{
public:
  static Logger* getInstance();

  void addHandler(std::string name, ILogHandler* handler);
  void removeHandler(std::string name);
  void log(std::string& msg, LogLevel level);
  void info(std::string msg);
  void warn(std::string msg);
  void error(std::string msg);
  void debug(std::string msg);

private:
  std::map<std::string, ILogHandler*> handlers;
};

} /* namespace logging */

#endif /* LOGGER_HPP */
