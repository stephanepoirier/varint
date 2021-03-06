#ifndef COMPRESSED_SET_H__
#define COMPRESSED_SET_H__

#define BLOCK_SIZE_BIT 11

// should be a 2 ^ BLOCK_SIZE_BIT
// 256 should give good cache alignement
#define DEFAULT_BATCH_SIZE 2048

// should be DEFAULT_BATCH_SIZE -1
#define BLOCK_SIZE_MODULO 2047

//int i=-1;
//for(int x=DEFAULT_BATCH_SIZE; x>0; ++i, x = x>> 1) { }
//BLOCK_INDEX_SHIFT_BITS = i;
#define BLOCK_INDEX_SHIFT_BITS 11

#include "Codec.hpp"
#include "CompressedDeltaChunk.hpp"
#include "DeltaChunkStore.hpp"
#include "Set.hpp"

#include "bitpacking/memutil.h"

#include <istream>
#include <ostream>
#include <utility>

class CompressedSet;

class CompressedSet : public Set {
  private:
    int sizeOfCurrentNoCompBlock;  // the number of uncompressed elements that is hold in the currentNoCompBlock
    // Two separate arrays containing the last docID of each block in words in uncompressed form.
    std::vector<unsigned int> baseListForOnlyCompBlocks;

    const CompressedSet& operator=(const CompressedSet& other);

  public:
    class Iterator : public Set::Iterator {
      private:
        unsigned int lastAccessedDocId;
        int cursor;  // the current pointer of the input
        int totalDocIdNum;

        int compBlockNum;  // the number of compressed blocks
        std::vector<uint32_t, AlignedSTLAllocator<uint32_t, 64>> iterDecompBlock;
        std::vector<uint32_t, AlignedSTLAllocator<uint32_t, 64>> currentNoCompBlock;

        const CompressedSet* set;  // parent
        int advanceToTargetInTheFollowingCompBlocks(int target, int startBlockIndex);
        int advanceToTargetInTheFollowingCompBlocksNoPostProcessing(unsigned int target,
                                                                    int startBlockIndex);
        int getBlockIndex(int docIdIndex);

      public:
        Iterator(const CompressedSet* parentSet);
        Iterator(const CompressedSet::Iterator& other);
        ~Iterator();

        // assignator operator disabled for now
        CompressedSet::Iterator& operator=(const CompressedSet::Iterator& rhs);

        unsigned int docID();
        unsigned int nextDoc();
        unsigned int Advance(unsigned int target);
    };

    static Codec codec;  // varint encoding codec
    unsigned int totalDocIdNum;  // the total number of elemnts that have been inserted/accessed so far

    // Memory used to store uncompressed elements. Once the block is full, all its elements are compressed into sequencOfCompBlock and the block is cleared.
    std::vector<uint32_t, AlignedSTLAllocator<uint32_t, 64>> currentNoCompBlock;

    DeltaChunkStore sequenceOfCompBlocks;  // Store for list compressed delta chunk

    CompressedSet(const CompressedSet& other);
    CompressedSet();
    ~CompressedSet();

    /**
     * Swap the content of this bitmap with another bitmap.
     * without having to pay the cost of one copy constructor call
     * and two assignment operator call.
     */

    inline void swap(CompressedSet& x) throw() {  // No throw exception guarantee
      using std::swap;
      swap(this->sizeOfCurrentNoCompBlock, x.sizeOfCurrentNoCompBlock);
      swap(this->baseListForOnlyCompBlocks, x.baseListForOnlyCompBlocks);
      swap(this->totalDocIdNum, x.totalDocIdNum);
      swap(this->currentNoCompBlock, x.currentNoCompBlock);
      swap(this->sequenceOfCompBlocks, x.sequenceOfCompBlocks);
    }

    friend void swap(CompressedSet& lhs, CompressedSet& rhs) noexcept { lhs.swap(rhs); }

    void write(std::ostream& out);
    void read(std::istream& in);
    std::shared_ptr<Set::Iterator> iterator() const;

    /**
     * Add an array of sorted docIds to the set
     */
    void addDocs(unsigned int docids[], size_t start, size_t len);

    /**
     * Add document to this set
     * Note that you must set the bits in increasing order:
     * addDoc(1), addDoc(2) is ok;
     * addDoc(2), addDoc(1) is not ok.
     */
    void addDoc(unsigned int docId);

    CompressedSet unorderedAdd(unsigned int docId);
    void removeDocId(unsigned int docId);
    CompressedSet removeDoc(unsigned int docId);
    void compactBaseListForOnlyCompBlocks();
    void compact();
    void initSet();
    const std::shared_ptr<CompressedDeltaChunk> PForDeltaCompressOneBlock(unsigned int* block,
                                                                     size_t blocksize);
    const std::shared_ptr<CompressedDeltaChunk> PForDeltaCompressCurrentBlock();

    /**
     * Gets the number of ids in the set
     * @return docset size
     */
    unsigned int size() const;

    /**
     * if more then 1/8 of bit are set to 1 in range [minSetValue,maxSetvalue]
     * you should use EWAHBoolArray compression instead
     * because this compression will take at least 8 bits by positions
     */
    bool isDense();

    // This method will not work after a call to flush()
    bool find(unsigned int target) const;
};

#endif  // COMPRESSED_SET_H__
