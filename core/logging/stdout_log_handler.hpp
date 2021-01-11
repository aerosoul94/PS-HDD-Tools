#ifndef STDOUT_LOG_HANDLER_HPP
#define STDOUT_LOG_HANDLER_HPP

#include "logger.hpp"

namespace logging {

class StdOutLogHandler : public ILogHandler
{
public:
  void info(std::string& msg) override;
  void warn(std::string& msg) override;
  void error(std::string& msg) override;
  void debug(std::string& msg) override;
};

} /* namespace logging */

#endif /* STDOUT_LOG_HANDLER_HPP */
