#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>

namespace krivoshapov
{
  template <class T>
  struct Vector
  {
    T *data_;
    std::size_t size_;
    std::size_t cap_;
  };

  template <class T>
  void init(Vector<T> &v) noexcept
  {
    v.data_ = nullptr;
    v.size_ = 0;
    v.cap_ = 0;
  }

  template <class T>
  void destroy(Vector<T> &v) noexcept
  {
    delete[] v.data_;
    v.data_ = nullptr;
    v.size_ = 0;
    v.cap_ = 0;
  }

  template <class T>
  void reserve(Vector<T> &v, std::size_t n)
  {
    if (n <= v.cap_)
    {
      return;
    }
    T *nd = new T[n];
    std::size_t i = 0;
    try
    {
      for (; i < v.size_; ++i)
      {
        nd[i] = v.data_[i];
      }
    }
    catch (...)
    {
      delete[] nd;
      throw;
    }
    delete[] v.data_;
    v.data_ = nd;
    v.cap_ = n;
  }

  template <class T>
  void pushBack(Vector<T> &v, const T &val)
  {
    if (v.size_ == v.cap_)
    {
      reserve(v, v.cap_ ? v.cap_ * 2 : 1);
    }
    v.data_[v.size_] = val;
    ++v.size_;
  }
}

#endif
