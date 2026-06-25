#include "commands.hpp"
#include "meeting.hpp"
#include "../common/io.hpp"

#include <cstring>
#include <fstream>
#include <iostream>

namespace
{
  struct Args
  {
    const char *inFile;
    const char *dataFile;
    bool valid;
  };

  Args parseArgs(int argc, char **argv)
  {
    Args args = {nullptr, nullptr, true};
    for (int i = 1; i < argc; ++i)
    {
      const char *arg = argv[i];
      if (std::strncmp(arg, "in:", 3) == 0)
      {
        if (args.inFile != nullptr)
        {
          return {nullptr, nullptr, false};
        }
        args.inFile = arg + 3;
      }
      else if (std::strncmp(arg, "data:", 5) == 0)
      {
        if (args.dataFile != nullptr)
        {
          return {nullptr, nullptr, false};
        }
        args.dataFile = arg + 5;
      }
      else
      {
        return {nullptr, nullptr, false};
      }
    }
    if (args.dataFile == nullptr)
    {
      return {nullptr, nullptr, false};
    }
    return args;
  }
}

int main(int argc, char **argv)
{
  Args args = parseArgs(argc, argv);
  if (!args.valid)
  {
    std::cerr << "Invalid arguments\n";
    return 1;
  }

  krivoshapov::Database db;
  krivoshapov::initDb(db);

  if (args.inFile != nullptr)
  {
    std::ifstream inStream(args.inFile);
    if (!inStream.is_open())
    {
      std::cerr << "Cannot open file: " << args.inFile << '\n';
      krivoshapov::destroyDb(db);
      return 2;
    }
    krivoshapov::readPersons(inStream, db.persons);
  }

  std::ifstream dataStream(args.dataFile);
  if (!dataStream.is_open())
  {
    std::cerr << "Cannot open file: " << args.dataFile << '\n';
    krivoshapov::destroyDb(db);
    return 2;
  }
  if (!krivoshapov::readMeetings(dataStream, db))
  {
    std::cerr << "Invalid meetings data\n";
    krivoshapov::destroyDb(db);
    return 3;
  }

  krivoshapov::processCommands(std::cin, std::cout, db);
  krivoshapov::destroyDb(db);
  return 0;
}
