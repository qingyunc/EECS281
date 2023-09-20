// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#ifndef BINARYPQ_H
#define BINARYPQ_H

#include "Eecs281PQ.h"
#include <algorithm>
#include <cstddef>
#include <type_traits>

// A specialized version of the priority queue ADT implemented as a binary
// heap.
template <typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
  // This is a way to refer to the base class object.
  using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
  // Description: Construct an empty PQ with an optional comparison functor.
  // Runtime: O(1)

  explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) : BaseClass{comp} {
    data.push_back(TYPE());

  } // BinaryPQ

  // Description: Construct a PQ out of an iterator range with an optional
  //              comparison functor.
  // Runtime: O(n) where n is number of elements in range.
  template <typename InputIterator>
  BinaryPQ(InputIterator start, InputIterator end,
           COMP_FUNCTOR comp = COMP_FUNCTOR())
      : BaseClass{comp} {
    data.push_back(TYPE());
    while (start != end) {
      data.push_back(*start);
      ++start;
    }
    updatePriorities();

  } // BinaryPQ

  // Description: Destructor doesn't need any code, the data vector will
  //              be destroyed automatically.
  virtual ~BinaryPQ() {} // ~BinaryPQ()

  // Description: Assumes that all elements inside the heap are out of
  //              order and 'rebuilds' the heap by fixing the heap
  //              invariant.
  // Runtime: O(n)
  virtual void updatePriorities() {
    for (size_t i = size() / 2; i > 0; i--) {
      fixDown(i);
    }
  } // updatePriorities()

  // Description: Add a new element to the PQ.
  // Runtime: O(log(n))
  virtual void push(const TYPE &val) {
    data.push_back(val);
    fixUp(size());
  }

  // Description: Remove the most extreme (defined by 'compare') element
  //              from the PQ.
  // Note: We will not run tests on your code that would require it to pop
  //       an element when the PQ is empty. Though you are welcome to if
  //       you are familiar with them, you do not need to use exceptions in
  //       this project.
  // Runtime: O(log(n))
  virtual void pop() {
    data[1] = data.back();
    data.pop_back();
    fixDown(1);
  } // pop()

  // Description: Return the most extreme (defined by 'compare') element of
  //              the PQ. This should be a reference for speed. It MUST
  //              be const because we cannot allow it to be modified, as
  //              that might make it no longer be the most extreme element.
  // Runtime: O(1)
  virtual const TYPE &top() const { return data[1]; }

  // Description: Get the number of elements in the PQ.
  // Runtime: O(1)
  virtual std::size_t size() const {
    // TODO: Implement this function. Might be very simple,
    // depending on your implementation.
    return data.size() - 1; // TODO: Delete or change this line
  }                         // size()

  // Description: Return true if the PQ is empty.
  // Runtime: O(1)
  virtual bool empty() const {
    // TODO: Implement this function. Might be very simple,
    // depending on your implementation.
    return data.size() == 1; // TODO: Delete or change this line
  }                          // empty()

private:
  // Note: This vector *must* be used for your PQ implementation.
  std::vector<TYPE> data;

  void fixUp(size_t a) {
    while (a > 1 && this->compare(data[a / 2], data[a])) {
      std::swap(data[a / 2], data[a]);
      a = a / 2;
    }
  }

  void fixDown(size_t a) {
    while (a * 2 <= size()) {
      size_t b = 2 * a;
      if (b < size() && this->compare(data[b], data[b + 1])) {
        ++b;
      }
      if (!this->compare(data[a], data[b])) {
        break;
      }
      std::swap(data[a], data[b]);
      a = b;
    }
  }
  // NOTE: You are not allowed to add any member variables. You don't need
  //       a "heapSize", since you can call your own size() member
  //       function, or check data.size().

  // TODO: Add any additional member functions you require here. For
  //       instance, you might add fixUp() and fixDown().
}; // BinaryPQ

#endif // BINARYPQ_H
