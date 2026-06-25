#include "meeting.hpp"

#include <iostream>

int main()
{
  krivoshapov::Database db;
  krivoshapov::initDb(db);
  krivoshapov::destroyDb(db);
  return 0;
}
