#ifndef COLLECTION_HELPER_H__
#define COLLECTION_HELPER_H__

#include <vector>

/**
 * Binary search for the first element that is equal to or larger than the target
 *
 * @param in must be sorted and contains no duplicates
 * @return the index of the first element in the array that is equal or larger than the target. -1 if the target is out of range.
 */
int binarySearchForFirstElementEqualOrLargerThanTarget(const std::vector<unsigned int>& in, int start,
                                                       int end, unsigned int target) {
  int mid;
  while (start < end) {
    mid = (start + end) / 2;
    if (in[mid] < target)
      start = mid + 1;
    else if (in[mid] == target)
      return mid;
    else
      end = mid;
  }
  if (in[start] >= target)
    return start;
  else
    return -1;
}

/**
 * Binary search for the first element that is equal to or larger than the target
 *
 * @param in must be sorted and contains no duplicates
 * @return the index of the first element in the array that is equal or larger than the target. -1 if the target is out of range.
 */
int binarySearchForFirstElementEqualOrLargerThanTarget(const unsigned int* in, int start, int end,
                                                       unsigned int target) {
  int mid;
  while (start < end) {
    mid = (start + end) / 2;
    if (in[mid] < target)
      start = mid + 1;
    else if (in[mid] == target)
      return mid;
    else
      end = mid;
  }
  if (in[start] >= target)
    return start;
  else
    return -1;
}

/**
 *  Binary search in the base list for the block that may contain
 *  docId greater than or equal to the target
 */
int binarySearchInBaseListForBlockThatMayContainTarget(const std::vector<unsigned int>& in, int start,
                                                       int end, int target) {
  //the baseListForOnlyCompBlocks (in) contains all last elements of the compressed blocks.
  return binarySearchForFirstElementEqualOrLargerThanTarget(in, start, end, target);
}

/**
 * Regular Binary search for the the target
 *
 * @param vals must be sorted
 * @return the index of the target in the input array. -1 if the target is out of range.
 */
int binarySearchForTarget(std::vector<unsigned int>& vals, int start, int end, unsigned int target) {
  int mid;
  while (start <= end) {
    mid = (start + end) / 2;
    if (vals[mid] < target)
      start = mid + 1;
    else if (vals[mid] == target)
      return mid;
    else
      end = mid - 1;
  }
  return -1;
}
#endif  // COLLECTION_HELPER_H__
