#ifndef COMPRESSED_DELTA_CHUNK_H__
#define COMPRESSED_DELTA_CHUNK_H__

#include "Common.h"
#include "Sink.h"
#include "Source.h"

#include "bitpacking/memutil.h"

#include <istream>
#include <ostream>
#include <stddef.h>
#include <vector>

class CompressedDeltaChunk {
  private:
    std::vector<uint8, cacheallocator> data_;
    size_t compressedSize_;

    //disable copy constructor
    CompressedDeltaChunk(const CompressedDeltaChunk& other);
    CompressedDeltaChunk& operator=(const CompressedDeltaChunk& other);

  public:
    CompressedDeltaChunk();
    CompressedDeltaChunk(size_t compressedSize);
    CompressedDeltaChunk(std::istream& in);
    ~CompressedDeltaChunk();

    void resize(size_t newsize);
    std::vector<uint8, cacheallocator>& getVector();
    size_t getCompressedSize();
    Sink getSink();
    Source getSource() const;
    void write(std::ostream& out) const;

    inline void swap(CompressedDeltaChunk& x) throw() {
      using std::swap;
      swap(this->data_, x.data_);
      swap(this->compressedSize_, x.compressedSize_);
    }
    friend void swap(CompressedDeltaChunk& lhs, CompressedDeltaChunk& rhs) noexcept { lhs.swap(rhs); }
} __attribute__((aligned(256)));
#endif  // COMPRESSED_DELTA_CHUNK_H__
