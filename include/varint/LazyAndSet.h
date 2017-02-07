#ifndef LAZY_AND_SET_H__
#define LAZY_AND_SET_H__

#include "varint/Set.h"

#include <memory>
#include <vector>

class LazyAndSet;

class LazyAndSetIterator : public Set::Iterator {
  private:
    unsigned lastReturn;
    std::vector<std::shared_ptr<Set::Iterator> > iterators;
    const LazyAndSet& set;

  public:
    LazyAndSetIterator(const LazyAndSet* parent);
    unsigned int docID();
    unsigned int nextDoc();
    unsigned int Advance(unsigned int target);
};

class LazyAndSet : public Set {
  public:
    std::vector<std::shared_ptr<Set> > sets_;
    int nonNullSize;
    mutable unsigned int setSize;
    mutable bool init = false;

    LazyAndSet();
    LazyAndSet(std::vector<std::shared_ptr<Set> >& sets);
    LazyAndSet(std::shared_ptr<Set>& left, std::shared_ptr<Set>& right);

    inline bool find(unsigned int val) const;
    unsigned int size() const;
    std::shared_ptr<Set::Iterator> iterator() const;
};

#endif  // LAZY_AND_SET_H__
