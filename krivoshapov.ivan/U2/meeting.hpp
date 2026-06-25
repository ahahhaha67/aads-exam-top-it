#ifndef MEETING_HPP
#define MEETING_HPP

#include "../common/person.hpp"
#include "../common/vector.hpp"
#include <iosfwd>

namespace krivoshapov
{
  struct Meeting
  {
    std::size_t a;
    std::size_t b;
    std::size_t duration;
  };

  struct Database
  {
    Vector<Person> persons;
    Vector<Meeting> meetings;
  };

  void initDb(Database &db) noexcept;
  void destroyDb(Database &db) noexcept;

  Person *findPerson(Database &db, std::size_t id) noexcept;
  const Person *findPerson(const Database &db, std::size_t id) noexcept;
  void ensurePerson(Database &db, std::size_t id);

  bool readMeetings(std::istream &in, Database &db);
}

#endif
