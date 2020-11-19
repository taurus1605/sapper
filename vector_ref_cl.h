#ifndef VECTOR_REF_CL_H
#define VECTOR_REF_CL_H


#include <vector>
#include <functional>

#include "lib.h"

template<class T> class Vector_ref_cl
{
  std::vector<T*> v;
  std::vector<T*> owned;

  public:
  Vector_ref_cl ()  { }
  Vector_ref_cl (T* a, T* b = nullptr, T* c = nullptr, T* d = nullptr)
  {
    if (a) push_back(a);
    if (b) push_back(b);
    if (c) push_back(c);
    if (d) push_back(d);
  }

  ~Vector_ref_cl ()  { for (unsigned int i = 0; i < owned.size(); ++i) delete owned[i]; }

  void push_back (T& s)  { v.push_back(&s); }
  void push_back (T* p)  { v.push_back(p); owned.push_back(p); }

  void clear()
  {
    v.clear();
    owned.clear();
  }

  void pop_back ()
  {
    v.pop_back();
    owned.pop_back();
  }

  T& operator[] (int i)  { return *v.at(i); }
  const T& operator[] (int i) const  { return *v.at(i); }

  int size () const  { return v.size(); }
};

#endif // VECTOR_REF_CL_H
