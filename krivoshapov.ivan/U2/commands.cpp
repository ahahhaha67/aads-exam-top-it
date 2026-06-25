#include "commands.hpp"

#include "../common/io.hpp"
#include "../common/vector.hpp"

#include <algorithm>
#include <fstream>
#include <istream>
#include <ostream>
#include <string>

namespace krivoshapov
{
  namespace
  {
    void invalidCmd(std::ostream &out)
    {
      out << "<INVALID COMMAND>\n";
    }

    void skipLine(std::istream &in)
    {
      in.clear();
      std::string dummy;
      std::getline(in, dummy);
    }

    struct MeetEntry
    {
      std::size_t otherId;
      std::size_t duration;
    };

    bool meetEntryCmp(const MeetEntry &x, const MeetEntry &y)
    {
      if (x.otherId != y.otherId)
      {
        return x.otherId < y.otherId;
      }
      return x.duration < y.duration;
    }

    bool meetEntryCmpById(const MeetEntry &x, const MeetEntry &y)
    {
      return x.otherId < y.otherId;
    }

    // showDuration: true – выводить "номер длительность", false – только номер
    void printMeets(std::ostream &out, const Database &db, std::size_t id,
                    std::size_t limit, bool useLess, bool useGreater, bool showDuration)
    {
      Vector<MeetEntry> entries;
      init(entries);
      for (std::size_t i = 0; i < db.meetings.size_; ++i)
      {
        const Meeting &m = db.meetings.data_[i];
        std::size_t other = 0;
        bool found = false;
        if (m.a == id)
        {
          other = m.b;
          found = true;
        }
        else if (m.b == id)
        {
          other = m.a;
          found = true;
        }
        if (!found)
        {
          continue;
        }
        if (useLess && m.duration >= limit)
        {
          continue;
        }
        if (useGreater && m.duration <= limit)
        {
          continue;
        }
        MeetEntry e = {other, m.duration};
        pushBack(entries, e);
      }
      if (showDuration)
      {
        std::sort(entries.data_, entries.data_ + entries.size_, meetEntryCmp);
        for (std::size_t i = 0; i < entries.size_; ++i)
        {
          out << entries.data_[i].otherId << ' ' << entries.data_[i].duration << '\n';
        }
      }
      else
      {
        std::sort(entries.data_, entries.data_ + entries.size_, meetEntryCmpById);
        for (std::size_t i = 0; i < entries.size_; ++i)
        {
          out << entries.data_[i].otherId << '\n';
        }
      }
      destroy(entries);
    }

    void doAnons(std::ostream &out, const Database &db)
    {
      Vector<std::size_t> ids;
      init(ids);
      for (std::size_t i = 0; i < db.persons.size_; ++i)
      {
        if (db.persons.data_[i].info.empty())
        {
          pushBack(ids, db.persons.data_[i].id);
        }
      }
      std::sort(ids.data_, ids.data_ + ids.size_);
      for (std::size_t i = 0; i < ids.size_; ++i)
      {
        out << ids.data_[i] << '\n';
      }
      destroy(ids);
    }

    void doDeanon(std::istream &in, std::ostream &out, Database &db)
    {
      std::size_t anonId = 0, id = 0;
      if (!(in >> anonId >> id))
      {
        skipLine(in);
        invalidCmd(out);
        return;
      }
      const Person *anon = findPerson(db, anonId);
      if (anon == nullptr || !anon->info.empty())
      {
        invalidCmd(out);
        return;
      }
      const Person *person = findPerson(db, id);
      if (person == nullptr || person->info.empty())
      {
        invalidCmd(out);
        return;
      }
      for (std::size_t i = 0; i < db.meetings.size_; ++i)
      {
        if (db.meetings.data_[i].a == anonId)
        {
          db.meetings.data_[i].a = id;
        }
        if (db.meetings.data_[i].b == anonId)
        {
          db.meetings.data_[i].b = id;
        }
      }
      removeIf(db.meetings, [](const Meeting &m)
               { return m.a == m.b; });
      removeIf(db.persons, [anonId](const Person &p)
               { return p.id == anonId; });
    }

    void doRedesc(std::istream &in, std::ostream &out, Database &db)
    {
      std::size_t id = 0;
      if (!(in >> id))
      {
        skipLine(in);
        invalidCmd(out);
        return;
      }
      Person *p = findPerson(db, id);
      if (p == nullptr)
      {
        skipLine(in);
        invalidCmd(out);
        return;
      }
      char ch = 0;
      bool foundQuote = false;
      while (in.get(ch))
      {
        if (ch == '"')
        {
          foundQuote = true;
          break;
        }
        if (ch != ' ' && ch != '\t')
        {
          skipLine(in);
          invalidCmd(out);
          return;
        }
      }
      if (!foundQuote)
      {
        invalidCmd(out);
        return;
      }
      std::string desc;
      while (in.get(ch) && ch != '"')
      {
        desc += ch;
      }
      p->info = desc;
    }

    void doDesc(std::istream &in, std::ostream &out, const Database &db)
    {
      std::size_t id = 0;
      if (!(in >> id))
      {
        skipLine(in);
        invalidCmd(out);
        return;
      }
      const Person *p = findPerson(db, id);
      if (p == nullptr)
      {
        invalidCmd(out);
        return;
      }
      if (p->info.empty())
      {
        out << "<ANON>\n";
      }
      else
      {
        out << p->info << '\n';
      }
    }

    void doMeets(std::istream &in, std::ostream &out, const Database &db)
    {
      std::size_t id = 0;
      if (!(in >> id))
      {
        skipLine(in);
        invalidCmd(out);
        return;
      }
      printMeets(out, db, id, 0, false, false, true);
    }

    void doCommons(std::istream &in, std::ostream &out, const Database &db)
    {
      std::size_t id1 = 0, id2 = 0;
      if (!(in >> id1 >> id2))
      {
        skipLine(in);
        invalidCmd(out);
        return;
      }
      Vector<std::size_t> set1, set2;
      init(set1);
      init(set2);
      for (std::size_t i = 0; i < db.meetings.size_; ++i)
      {
        const Meeting &m = db.meetings.data_[i];
        if (m.a == id1)
        {
          pushBack(set1, m.b);
        }
        else if (m.b == id1)
        {
          pushBack(set1, m.a);
        }
        if (m.a == id2)
        {
          pushBack(set2, m.b);
        }
        else if (m.b == id2)
        {
          pushBack(set2, m.a);
        }
      }
      Vector<std::size_t> result;
      init(result);
      for (std::size_t i = 0; i < set1.size_; ++i)
      {
        std::size_t x = set1.data_[i];
        bool inSet2 = false;
        for (std::size_t j = 0; j < set2.size_; ++j)
        {
          if (set2.data_[j] == x)
          {
            inSet2 = true;
            break;
          }
        }
        if (!inSet2)
        {
          continue;
        }
        bool alreadyIn = false;
        for (std::size_t j = 0; j < result.size_; ++j)
        {
          if (result.data_[j] == x)
          {
            alreadyIn = true;
            break;
          }
        }
        if (!alreadyIn)
        {
          pushBack(result, x);
        }
      }
      std::sort(result.data_, result.data_ + result.size_);
      for (std::size_t i = 0; i < result.size_; ++i)
      {
        out << result.data_[i] << '\n';
      }
      destroy(set1);
      destroy(set2);
      destroy(result);
    }

    void doLess(std::istream &in, std::ostream &out, const Database &db)
    {
      std::size_t limit = 0, id = 0;
      if (!(in >> limit >> id))
      {
        skipLine(in);
        invalidCmd(out);
        return;
      }
      printMeets(out, db, id, limit, true, false, false);
    }

    void doGreater(std::istream &in, std::ostream &out, const Database &db)
    {
      std::size_t limit = 0, id = 0;
      if (!(in >> limit >> id))
      {
        skipLine(in);
        invalidCmd(out);
        return;
      }
      printMeets(out, db, id, limit, false, true, false);
    }

    void doOutPersons(std::istream &in, std::ostream &out, const Database &db)
    {
      std::string filename;
      if (!(in >> filename))
      {
        skipLine(in);
        invalidCmd(out);
        return;
      }
      std::ofstream file(filename);
      if (!file.is_open())
      {
        invalidCmd(out);
        return;
      }
      bool first = true;
      for (std::size_t i = 0; i < db.persons.size_; ++i)
      {
        const Person &p = db.persons.data_[i];
        if (!p.info.empty())
        {
          if (!first)
          {
            file << '\n';
            out << '\n';
          }
          file << p.id << ' ' << p.info;
          out << p.id << ' ' << p.info;
          first = false;
        }
      }
      if (!first)
      {
        file << '\n';
        out << '\n';
      }
    }
  }

  void processCommands(std::istream &in, std::ostream &out, Database &db)
  {
    std::string cmd;
    while (in >> cmd)
    {
      if (cmd == "anons")
      {
        doAnons(out, db);
      }
      else if (cmd == "deanon")
      {
        doDeanon(in, out, db);
      }
      else if (cmd == "redesc")
      {
        doRedesc(in, out, db);
      }
      else if (cmd == "desc")
      {
        doDesc(in, out, db);
      }
      else if (cmd == "meets" || cmd == "meet")
      {
        doMeets(in, out, db);
      }
      else if (cmd == "commons")
      {
        doCommons(in, out, db);
      }
      else if (cmd == "less")
      {
        doLess(in, out, db);
      }
      else if (cmd == "greater")
      {
        doGreater(in, out, db);
      }
      else if (cmd == "out-persons")
      {
        doOutPersons(in, out, db);
      }
      else
      {
        skipLine(in);
        invalidCmd(out);
      }
    }
  }
}
