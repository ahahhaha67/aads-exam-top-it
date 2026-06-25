#include "../common/io.hpp"

#include <cstring>
#include <fstream>
#include <iostream>

namespace
{
  struct Args
  {
    const char *inFile;
    const char *outFile;
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
      else if (std::strncmp(arg, "out:", 4) == 0)
      {
        if (args.outFile != nullptr)
        {
          return {nullptr, nullptr, false};
        }
        args.outFile = arg + 4;
      }
      else
      {
        return {nullptr, nullptr, false};
      }
    }
    return args;
  }
}

int main(int argc, char **argv)
{
  if (argc > 3)
  {
    std::cerr << "Invalid arguments\n";
    return 1;
  }
  Args args = parseArgs(argc, argv);
  if (!args.valid)
  {
    std::cerr << "Invalid arguments\n";
    return 1;
  }

  std::ifstream inStream;
  if (args.inFile != nullptr)
  {
    inStream.open(args.inFile);
    if (!inStream.is_open())
    {
      std::cerr << "Cannot open file: " << args.inFile << '\n';
      return 2;
    }
  }
  std::istream &in = args.inFile ? static_cast<std::istream &>(inStream) : std::cin;

  krivoshapov::Vector<krivoshapov::Person> persons;
  krivoshapov::init(persons);

  krivoshapov::readPersons(in, persons);

  if (args.inFile != nullptr)
  {
    inStream.close();
  }

  if (args.outFile != nullptr)
  {
    std::cout << "in file " << args.outFile << '\n';
  }

  krivoshapov::writePersons(std::cout, persons);

  if (args.outFile != nullptr)
  {
    std::ofstream outStream;
    outStream.open(args.outFile);
    if (!outStream.is_open())
    {
      std::cerr << "Cannot open file: " << args.outFile << '\n';
      krivoshapov::destroy(persons);
      return 2;
    }
    krivoshapov::writePersons(outStream, persons);
    outStream.close();
  }

  krivoshapov::destroy(persons);
  return 0;
}
