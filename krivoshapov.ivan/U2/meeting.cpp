#include "meeting.hpp"

#include <istream>

namespace krivoshapov
{
  void initDb(Database &db) noexcept
  {
    init(db.persons);
    init(db.meetings);
  }

  void destroyDb(Database &db) noexcept
  {
    destroy(db.persons);
    destroy(db.meetings);
  }

  Person *findPerson(Database &db, std::size_t id) noexcept
  {
    for (std::size_t i = 0; i < db.persons.size_; ++i)
    {
      if (db.persons.data_[i].id == id)
      {
        return &db.persons.data_[i];
      }
    }
    return nullptr;
  }

  const Person *findPerson(const Database &db, std::size_t id) noexcept
  {
    for (std::size_t i = 0; i < db.persons.size_; ++i)
    {
      if (db.persons.data_[i].id == id)
      {
        return &db.persons.data_[i];
      }
    }
    return nullptr;
  }

  void ensurePerson(Database &db, std::size_t id)
  {
    if (findPerson(db, id) == nullptr)
    {
      Person anon = {id, ""};
      pushBack(db.persons, anon);
    }
  }

  bool readMeetings(std::istream &in, Database &db)
  {
    std::size_t a = 0, b = 0, dur = 0;
    while (in >> a)
    {
      if (!(in >> b >> dur))
      {
        return false;
      }
      if (a == b)
      {
        continue;
      }
      ensurePerson(db, a);
      ensurePerson(db, b);
      Meeting m = {a, b, dur};
      pushBack(db.meetings, m);
    }
    return in.eof();
  }
}
