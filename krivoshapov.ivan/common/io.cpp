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
      for (char c : s)
      {
        if (!std::isspace(static_cast<unsigned char>(c)))
          return false;
      }
      return true;
    }
  }

  ReadResult readPersons(std::istream &in, Vector<Person> &out)
  {
    ReadResult result = {0, 0};
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
        ++result.ignored;
        continue;
      }
      std::string info = line.substr(idx);
      trimLeading(info);
      trimTrailing(info);
      if (info.empty())
      {
        ++result.ignored;
        continue;
      }
      if (containsId(out, id))
      {
        ++result.ignored;
        continue;
      }
      Person p = {id, info};
      pushBack(out, p);
      ++result.valid;
    }
    return result;
  }

  void writePersons(std::ostream &out, const Vector<Person> &persons)
  {
    for (std::size_t i = 0; i < persons.size_; ++i)
    {
      out << persons.data_[i].id << ' ' << persons.data_[i].info << '\n';
    }
  }
}
