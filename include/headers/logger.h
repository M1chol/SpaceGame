#pragma once

#include <engine.h>

class Logger
{
public:
  Logger(const std::string &name) : _name(name) {}

  template <typename T>
  Logger &operator<<(const T &message)
  {
    _output << message;
    return *this;
  }

  Logger &operator<<(std::ostream &(*manip)(std::ostream &))
  {
    manip(_output);
    return *this;
  }

  ~Logger()
  {
    std::cout << _name << " " << _output.str();
    std::cout.flush();
  }

private:
  std::string _name;
  std::ostringstream _output;
};