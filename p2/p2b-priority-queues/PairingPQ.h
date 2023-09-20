// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <cstddef>
#include <deque>
#include <type_traits>
#include <utility>
using namespace std;

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template <typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
  // This is a way to refer to the base class object.
  using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
  // Each node within the pairing heap
  class Node {
  public:
    // TODO: After you add add one extra pointer (see below), be sure
    //       to initialize it here.
    explicit Node(const TYPE &val)
        : elt{val}, child{nullptr}, sibling{nullptr}, parent{nullptr} {}

    // Description: Allows access to the element at that Node's
    //              position. There are two versions, getElt() and a
    //              dereference operator, use whichever one seems
    //              more natural to you.
    // Runtime: O(1) - this has been provided for you.
    const TYPE &getElt() const { return elt; }
    const TYPE &operator*() const { return elt; }

    // The following line allows you to access any private data
    // members of this Node class from within the PairingPQ class.
    // (ie: myNode.elt is a legal statement in PairingPQ's add_node()
    // function).
    friend PairingPQ;

  private:
    TYPE elt;
    Node *child;
    Node *sibling;
    Node *parent;
    // TODO: Add one extra pointer (parent or previous) as desired.
  }; // Node

  // Description: Construct an empty pairing heap with an optional
  //              comparison functor.
  // Runtime: O(1)
  explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) // check!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      : BaseClass{comp} { // check!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    root = nullptr;
    count = 0;
    // TODO: Implement this function.
  } // PairingPQ()

  // Description: Construct a pairing heap out of an iterator range with an
  //              optional comparison functor.
  // Runtime: O(n) where n is number of elements in range.
  template <typename InputIterator>
  PairingPQ(InputIterator start,// check!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            InputIterator end, 
            COMP_FUNCTOR comp = COMP_FUNCTOR())
      : BaseClass{comp} {
    root = nullptr;
    count = 0;
    for (InputIterator ita = start; ita != end; ++ita) {
      push(*ita);
    }
  }

  // Description: Copy constructor.
  // Runtime: O(n)
  PairingPQ(const PairingPQ &other)
      : BaseClass{other.compare} { 

    root = nullptr;
    count = 0;
    // if (other.empty()) {
    //   return;
    // }
    // if (&other == this) {
    //   return;
    // }
    if (!other.root) {
      return;
    }
    std::deque<Node *> deq;
    deq.push_back(other.root);
    Node *bac = nullptr;
    while (!deq.empty()) {
      bac = deq.back();
      deq.pop_back();
      if (bac->sibling) {
        deq.push_front(bac->sibling);
      }
      if (bac->child) {
        deq.push_front(bac->child);
      }

      push(bac->elt);
    }

    // TODO: Implement this function.
    // NOTE: The structure does not have to be identical to the original,
    //       but it must still be a valid pairing heap.
  } // PairingPQ()

  // Description: Copy assignment operator.
  // Runtime: O(n)
  PairingPQ &operator=(const PairingPQ &rhs) { 

    PairingPQ a(rhs);
    swap(root, a.root);
    swap(count, a.count);
    return *this;

  } // operator=()
    // Description: Destructor
  // Runtime: O(n)
  ~PairingPQ() { //  
    if (!root) {
      return;
    }
    Node *n = nullptr;
    deque<Node *> d;
    d.push_back(root);

    while (!d.empty()) {
      n = d.back();
      d.pop_back();
      if (n->sibling) {
        d.push_front(n->sibling);
      }
      if (n->child) {
        d.push_front(n->child);
      }
      delete n;
    }
    // TODO: Implement this function.
  } // ~PairingPQ()

  // Description: Assumes that all elements inside the pairing heap are out
  //              of order and 'rebuilds' the pairing heap by fixing the
  //              pairing heap invariant. You CANNOT delete 'old' nodes
  //              and create new ones!
  // Runtime: O(n)
  virtual void updatePriorities() { // check !!!!!!!!!!!!!!!!!!!!!!!!!!
    deque<Node *> dq;
    if (!root) {
      return;
    }
    dq.push_back(root);
    root = nullptr;

    while (!dq.empty()) {
      Node *n = dq.front();
      dq.pop_front();
      if (n->sibling) {
        dq.push_back(n->sibling);
      }
      if (n->child) {
        dq.push_back(n->child);
      }

      n->child = nullptr;
      n->sibling = nullptr;
      n->parent = nullptr;
      root = meld(root, n);
    }

    // TODO: Implement this function.
  } // updatePriorities()

  // Description: Add a new element to the pairing heap. This is already
  //              done. You should implement push functionality entirely in
  //              the addNode() function, and this function calls
  //              addNode().
  // Runtime: O(1)
  virtual void push(const TYPE &val) {
    addNode(val);
  } // check!!!!!!!!!!!!!!!!!!!!!1

  // Description: Remove the most extreme (defined by 'compare') element
  //              from the pairing heap.
  // Note: We will not run tests on your code that would require it to pop
  //       an element when the pairing heap is empty. Though you are
  //       welcome to if you are familiar with them, you do not need to use
  //       exceptions in this project.
  // Runtime: Amortized O(log(n))
  virtual void pop() { // check!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    Node *r_c = root->child;
    delete root;
    --count;
    if (!r_c) {
      root = nullptr;
      return;
    }
    if (!r_c->sibling) {
      root = r_c;
      root->parent = nullptr;
      return;
    }
    std::deque<Node *> dq;
    while (r_c) {
      dq.push_back(r_c);
      Node *s_b = r_c->sibling;
      r_c->parent = nullptr;
      r_c->sibling = nullptr;
      r_c = s_b;
    }
    while (dq.size() != 1) {
      Node *f = dq.front();
      dq.pop_front();
      Node *f_1 = dq.front();
      dq.pop_front();
      dq.push_back(meld(f, f_1));
    }

    root = dq.front();

    // TODO: Implement this function.
  } // pop()

  // Description: Return the most extreme (defined by 'compare') element of
  //              the pairing heap. This should be a reference for speed.
  //              It MUST be const because we cannot allow it to be
  //              modified, as that might make it no longer be the most
  //              extreme element.
  // Runtime: O(1)
  virtual const TYPE &top() const { return root->elt; } // top()

  // Description: Get the number of elements in the pairing heap.
  // Runtime: O(1)
  virtual std::size_t size() const { // check
    // TODO: Implement this function
    return count; // TODO: Delete or change this line
  }               // size()

  // Description: Return true if the pairing heap is empty.
  // Runtime: O(1)
  virtual bool empty() const { // check
    return count == 0;
    // TODO: Implement this function
    // TODO: Delete or change this line
  } // empty()

  // Description: Updates the priority of an element already in the pairing
  //              heap by replacing the element refered to by the Node with
  //              new_value. Must maintain pairing heap invariants.
  //
  // PRECONDITION: The new priority, given by 'new_value' must be more
  //               extreme (as defined by comp) than the old priority.
  //
  // Runtime: As discussed in reading material.
  void updateElt(Node *node, // check
                 const TYPE &new_value) {
    // node->elt = new_value;
    // if (root == node) {
    //   return;
    // }

    // if (this->compare(node->parent->elt, node->elt)) {
    //   if (p->child == node) {
    //     p->child = node->sibling;
    //     node->parent = nullptr;
    //     node->sibling = nullptr;
    //     root = meld(root, node);

    //   } else {
    //     Node *p_2 = p->child;
    //     while (p_2->sibling != node) {
    //       p_2 = p_2->sibling;
    //     }
    //     p_2->sibling = node->sibling;
    //     node->parent = nullptr;
    //     node->sibling = nullptr;
    //     root = meld(root, node);
    //   }
    // }

    node->elt = new_value;
    if (root == node) {
      return;
    }
    if (node->parent && this->compare(node->parent->elt, new_value)) {
      Node *n = node->parent->child;
      if (n == node) {
        node->parent->child = node->sibling;
      } else {
        while (n->sibling != node) {
          n = n->sibling;
        }
        n->sibling = node->sibling;
      }
      node->parent = nullptr;
      node->sibling = nullptr;
      root = meld(root, node);
    }
  }

  // updateElt()

  // Description: Add a new element to the pairing heap. Returns a Node*
  //              corresponding to the newly added element.
  // NOTE: Whenever you create a node, and thus return a Node *, you must
  //       be sure to never move or copy/delete that node in the future,
  //       until it is eliminated by the user calling pop(). Remember this
  //       when you implement updateElt() and updatePriorities().
  // Runtime: O(1)
  Node *addNode(const TYPE &val) { // check
    Node *n = new Node(val);
    if (!root) {
      root = n;
      count++;
      return n;

    } else {
      root = meld(root, n);
      count++;
      return n;
    }
  } // addNode()

private: // check
  Node *root;
  size_t count = 0;

  Node *meld(Node *first, Node *second) {
    if (!first) {
      return first;
    }
    if (!second) {
      return second;
    }

    if (this->compare(first->elt, second->elt)) {
      first->sibling = second->child;
      second->child = first;
      first->parent = second;
      return second;
    }

    else {
      second->sibling = first->child;
      first->child = second;
      second->parent = first;
      return first;
    }
  }
};

#endif // PAIRINGPQ_H
