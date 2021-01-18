#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <map>

/// Add a log handler to the logger.
#define rAddHandler(name, handler) \
  logging::LogManager::getInstance()->addHandler(name, handler)

/// Print an info message.
#define rInfo(msg)  logging::LogManager::getInstance()->info(msg)
/// Print a warning message.
#define rWarn(msg)  logging::LogManager::getInstance()->warn(msg)
/// Print an error message.
#define rError(msg) logging::LogManager::getInstance()->error(msg)
/// Print a debug message.
#define rDebug(msg) logging::LogManager::getInstance()->debug(msg)

namespace logging {

enum class LogLevel
{
  Info,
  Warn,
  Error,
  Debug
};

/**
 * @brief An interface for a log handler.
 * 
 * Implement this interface, then add it to the LogManager.
 */
class ILogHandler
{
public:
  ILogHandler();

  /**
   * @brief Set the log level for this log handler.
   * 
   * @param level 
   */
  void setLogLevel(LogLevel level);

  /**
   * @brief Check whether or not this log level is enabled.
   * 
   * @param level 
   * @return true 
   * @return false 
   */
  bool isLevelEnabled(LogLevel level) const;

  /**
   * @brief Handles printing of info messages.
   * 
   * @param msg 
   */
  virtual void info(std::string& msg) {};

  /**
   * @brief Handles printing of warning messages.
   * 
   * @param msg 
   */
  virtual void warn(std::string& msg) {};
  
  /**
   * @brief Handles printing of error messages.
   * 
   * @param msg 
   */
  virtual void error(std::string& msg) {};

  /**
   * @brief Handles printing of debug messages.
   * 
   * @param msg 
   */
  virtual void debug(std::string& msg) {};

private:
  LogLevel level;
};

class LogManager
{
public:
  static LogManager* getInstance();

  /**
   * @brief Add a log handler.
   * 
   * @param name Name of this log handler.
   * @param handler The log handler.
   */
  void addHandler(std::string name, ILogHandler* handler);

  /**
   * @brief Remove a log handler by name.
   * 
   * @param name Name of the log handler to remove.
   */
  void removeHandler(std::string name);

  /**
   * @brief Push a log message to each log handler.
   * 
   * @param msg The message.
   * @param level The log level to print.
   */
  void log(std::string& msg, LogLevel level);

  /**
   * @brief Push an info message to each log handler.
   * 
   * @param msg The message.
   */
  void info(std::string msg);

  /**
   * @brief Push a warning message to each log handler.
   * 
   * @param msg The message.
   */
  void warn(std::string msg);

  /**
   * @brief Push an error message to each log handler.
   * 
   * @param msg The message.
   */
  void error(std::string msg);

  /**
   * @brief Push a debug message to each log handler.
   * 
   * @param msg The message.
   */
  void debug(std::string msg);

private:
  std::map<std::string, ILogHandler*> handlers;
};

} /* namespace logging */

#endif /* LOGGER_HPP */
