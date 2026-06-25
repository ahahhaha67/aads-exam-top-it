#ifndef IO_HPP
#define IO_HPP

#include "person.hpp"
#include "vector.hpp"
#include <iosfwd>

namespace krivoshapov
{
  void readPersons(std::istream &in, Vector<Person> &out);
  void writePersons(std::ostream &out, const Vector<Person> &persons);
}

#endif
