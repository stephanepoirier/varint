/**
 * Implementation of the union set of multiple DocIdSets (which essentially is a merged set of thes DocIdSets).
 */
#ifndef LAZY_OR_SET_H__
#define LAZY_OR_SET_H__

#include "Set.h"

#include <memory>
#include <vector>

class LazyOrSetIterator : public Set::Iterator {
  private:
    class Item {
      public:
        std::shared_ptr<Set::Iterator> iter;
        unsigned int doc;
        Item(std::shared_ptr<Set::Iterator> it) {
          iter = it;
          doc = 0;
        }
    };

    unsigned _curDoc;
    std::vector<std::shared_ptr<Item>> _heap;
    int _size;

    void heapRemoveRoot();
    void heapAdjust();

  public:
    LazyOrSetIterator(std::vector<std::shared_ptr<Set>> sets);

    unsigned int docID();
    unsigned int nextDoc();
    unsigned int Advance(unsigned int target);
};

class LazyOrSet : public Set {
  private:
    const int INVALID = -1;
    std::vector<std::shared_ptr<Set>> sets;
    mutable int _size = INVALID;

  public:
    LazyOrSet(std::vector<std::shared_ptr<Set>> docSets);
    LazyOrSet(std::shared_ptr<Set>& left, std::shared_ptr<Set>& right);
    std::shared_ptr<Set::Iterator> iterator() const;

    unsigned int size() const; //Override

    bool find(unsigned int val) const;
};

#endif  //LAZY_OR_SET_H__
