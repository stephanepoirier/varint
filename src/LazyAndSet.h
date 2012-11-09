#ifndef LAZY_AND_SET_H__
#define LAZY_AND_SET_H__
#include "CompressedSet.h"

class LazyAndSet;

class LazyAndSetIterator : public Set::Iterator {
private:
	int lastReturn; 
	vector<shared_ptr<Set::Iterator> > iterators;
	const LazyAndSet& set;
public:
	LazyAndSetIterator(const LazyAndSet* parent);
	int docID();
	int nextDoc();
	int Advance(int target);
};

class LazyAndSet : Set {
 public:
	vector<shared_ptr<Set> > sets_;
	int nonNullSize;
	int setSize;
	LazyAndSet();
	
	LazyAndSet(vector<shared_ptr<Set> >& sets);
	
	inline bool find(unsigned int val) const;
	
	int size();

    shared_ptr<Set::Iterator> iterator() const;
	
};

#endif  // LAZY_AND_SET_H__
