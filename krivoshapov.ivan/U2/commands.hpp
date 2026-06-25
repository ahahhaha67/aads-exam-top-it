#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "meeting.hpp"
#include <iosfwd>

namespace krivoshapov
{
  void processCommands(std::istream &in, std::ostream &out, Database &db);
}

#endif
