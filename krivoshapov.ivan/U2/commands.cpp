#include "commands.hpp"

#include "../common/vector.hpp"

#include <algorithm>
#include <fstream>
#include <istream>
#include <ostream>
#include <string>

namespace krivoshapov
{
  void processCommands(std::istream &in, std::ostream &out, Database &db)
  {
    (void)db;
    std::string cmd;
    while (in >> cmd)
    {
      out << "<INVALID COMMAND>\n";
    }
  }
}
