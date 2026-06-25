#include "io.hpp"
#include <iostream>

int main()
{
  krivoshapov::Vector<krivoshapov::Person> persons;
  krivoshapov::init(persons);
  krivoshapov::ReadResult r = krivoshapov::readPersons(std::cin, persons);
  std::cerr << r.valid << ' ' << r.ignored << '\n';
  krivoshapov::destroy(persons);
  return 0;
}
