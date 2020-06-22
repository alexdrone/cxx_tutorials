#pragma once

#include <iostream>
#include <cassert>

template<typename T>
class Node {
public:
  Node(const Node<T>& other) noexcept {
    std::cout << "cctor arg." << std::endl;
    value_ = other.value_;
    next_ = other.next_;
    prev_ = other.prev_;
  }
  Node(Node<T>&& other) {
    std::cout << "mctor arg." << std::endl;
    value_ = other.value_;
    next_ = other.next_;
    prev_ = other.prev_;
    other.value_ = nullptr;
    other.next_ = nullptr;
    other.prev_ = nullptr;
  }
  Node(const T& value): value_{value} {
    std::cout << "cctor." << std::endl;

  }
  Node(T&& value): value_{std::move(value)} {
    std::cout << "mctor." << std::endl;
  }
  ~Node() {
    std::cout << "dtor." << std::endl;
  }
  const T& operator*() const {
    return value_;
  }
  
  const T& Value() const { return value_; }
  
  Node<T>& Next() { return *next_; }
  Node<T>& Prev() { return *prev_; }
  
  const Node<T>& Next() const { return next_; }
  const Node<T>& Prev() const { return prev_; }

  void SetNext(const Node<T>& node) const { next_ = node; }
  void SetPrev(const Node<T>& node) const { prev_ = node; }
  
  void Remove() {
    if (prev_ != nullptr) { prev_->next_ = next_; }
    if (next_ != nullptr) { next_->prev_ = prev_; }
    delete this;
  }
  
  void Append(const T& value) {
    AppendNode(new Node<T>{value});
  }
  
  void Append(T&& value) {
    AppendNode(new Node<T>{std::move(value)});
  }
    
  class Iterator {
  public:
    Iterator(Node<T> *node): node_{node} {}
    
    Iterator& operator=(Node<T>* node) {
      this->node_ = node;
      return *this;
    }
    Iterator& operator++() {
      if (node_ == nullptr) { return *this; }
      node_ = node_->next_;
      return *this;
    }
    Iterator operator++(int) {
      const auto ret = *this;
      this->operator++();
      return ret;
    }
    bool operator!=(const Iterator& iterator) {
      return node_ != iterator.node_;
    }
    const T& operator*() const {
      if (node_ == nullptr) {
        assert(false); // Dereferencing a null-node.
      }
      return node_->Value();
    }
  private:
    Node<T> *node_;
  };
    
  inline Iterator begin() {
    return Iterator{this};
  }
    
  inline Iterator end() {
    return Iterator{nullptr};
  }
private:
  inline void AppendNode(Node<T> *node) {
    auto n = this;
    while (n->next_ != nullptr) {
      n = n->next_;
    }
    n->next_ = node;
    node->prev_ = this;
  }
  
  T value_;
  Node<T> *next_ = nullptr;
  Node<T> *prev_ = nullptr;
};

// Demo.

extern void LinkedListTestSuite();
