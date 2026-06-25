#include "commands.hpp"
#include "meeting.hpp"
#include "../common/io.hpp"

#include <fstream>
#include <iostream>

int main(int, char **)
{
  krivoshapov::Database db;
  krivoshapov::initDb(db);
  krivoshapov::processCommands(std::cin, std::cout, db);
  krivoshapov::destroyDb(db);
  return 0;
}
