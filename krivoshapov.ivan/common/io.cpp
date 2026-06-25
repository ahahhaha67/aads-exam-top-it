#include "io.hpp"

#include <cctype>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>

namespace krivoshapov
{
  namespace
  {
    bool containsId(const Vector<Person> &v, std::size_t id)
    {
      for (std::size_t i = 0; i < v.size_; ++i)
      {
        if (v.data_[i].id == id)
        {
          return true;
        }
      }
      return false;
    }

    void trimLeading(std::string &s)
    {
      std::size_t i = 0;
      while (i < s.size() && (s[i] == ' ' || s[i] == '\t'))
      {
        ++i;
      }
      s = s.substr(i);
    }

    void trimTrailing(std::string &s)
    {
      while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back())))
      {
        s.pop_back();
      }
    }

    bool isWhitespaceOnly(const std::string &s)
    {
      for (std::size_t i = 0; i < s.size(); ++i)
      {
        if (!std::isspace(static_cast<unsigned char>(s[i])))
          return false;
      }
      return true;
    }
  }

  void readPerson(std::istream &in, Vector<Person> &out)
  {
    std::string line;
    while (std::getline(in, line))
    {
      if (isWhitespaceOnly(line))
        continue;

      std::size_t idx = 0;
      std::size_t id = 0;
      try
      {
        id = static_cast<std::size_t>(std::stoul(line, &idx, 10));
      }
      catch (const std::exception &)
      {
        continue;
      }
      std::string info = line.substr(idx);
      trimLeading(info);
      trimTrailing(info);
      if (info.empty())
      {
        continue;
      }
      if (containsId(out, id))
      {
        continue;
      }
      Person p = {id, info};
      pushBack(out, p);
    }
  }

  void writePersons(std::ostream &out, const Vector<Person> &persons)
  {
    if (persons.size_ == 0)
    {
      out << '\n';
      return;
    }
    for (std::size_t i = 0; i < persons.size_; ++i)
    {
      out << persons.data_[i].id << ' ' << persons.data_[i].info << '\n';
    }
  }
}
