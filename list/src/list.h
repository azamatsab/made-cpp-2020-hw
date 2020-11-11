#pragma once
#include <iostream>
#include <iterator>
#include <typeinfo>

namespace task {

template <class T, class Alloc = std::allocator<T>>
class list {
 public:
  class Node {
   public:
    Node() {}

    Node(T data) : data_(std::move(data)) {}

    Node(T&& other) { data_ = std::move(other.data_); }

    void setData(T& data) { data_ = data; }

    T& getData() { return data_; }

    const T& getData() const { return data_; }

    void setPrev(Node* prev) { prev_ = prev; }

    Node* getPrev() const { return prev_; }

    void setNext(Node* next) { next_ = next; }

    Node* getNext() const { return next_; }

    bool operator==(const Node& other) { return other.data_ == data_; }

    bool operator!=(const Node& other) { return other.data_ != data_; }

   private:
    T data_;
    Node* prev_ = nullptr;
    Node* next_ = nullptr;
  };

  class iterator {
   public:
    using difference_type = std::ptrdiff_t;
    using value_type = Node;
    using pointer = Node*;
    using reference = Node&;
    using iterator_category = std::bidirectional_iterator_tag;

    iterator(Node* iptr) : ptr(iptr) {}

    iterator(const iterator& other) { ptr = other.ptr; }
    iterator& operator=(const iterator& other) {
      // other = const_cast<iterator>(other);
      ptr = other.ptr;
    }

    iterator& operator++() {
      ptr = ptr->getNext();
      return *this;
    }

    iterator operator++(int) {
      value_type old_ptr = *ptr;
      ptr = ptr->getNext();
      iterator other = iterator(&old_ptr);
      return other;
    }

    T& operator*() const {
      std::cout << "Calling *, return " << ptr->getData() << std::endl;
      // if (ptr->getPrev() != 0)
      //   std::cout << ptr->getPrev()->getData() << std::endl;
      std::cout << difference_type(ptr - ptr->getNext()) << std::endl;

      return ptr->getData();
    }

    T* operator->() const { return const_cast<T*>(&(ptr->getData())); }

    iterator& operator--() {
      ptr = ptr->getPrev();
      return *this;
    }

    iterator operator--(int) {
      value_type old_ptr = *ptr;
      ptr = ptr->getPrev();
      iterator other = iterator(&old_ptr);
      return other;
    }

    bool operator==(iterator other) const { return ptr == other.ptr; }

    bool operator!=(iterator other) const { return ptr != other.ptr; }

    pointer val() { return ptr; }

   private:
    pointer ptr = nullptr;
  };

  class const_iterator {
   public:
    using difference_type = std::ptrdiff_t;
    using value_type = Node;
    using pointer = const Node*;
    using reference = Node&;
    using iterator_category = std::bidirectional_iterator_tag;

    const_iterator(Node* iptr) : ptr(iptr) {}

    const_iterator(const const_iterator& other) { ptr = other.ptr; }

    const_iterator& operator=(const const_iterator& other) { ptr = other.ptr; }

    const_iterator& operator++() {
      ptr = ptr->getNext();
      return *this;
    }

    const_iterator operator++(int) {
      value_type old_ptr = *ptr;
      ptr = ptr->getNext();
      const_iterator other = const_iterator(&old_ptr);
      return other;
    }

    T& operator*() const { return ptr->getData(); }

    T* operator->() const { return const_cast<T*>(&(ptr->getData())); }

    const_iterator& operator--() {
      ptr = ptr->getPrev();
      return *this;
    }

    const_iterator operator--(int) {
      value_type old_ptr = *ptr;
      ptr = ptr->getPrev();
      const_iterator other = const_iterator(&old_ptr);
      return other;
    }

    bool operator==(iterator other) const { return ptr == other.ptr; }

    bool operator!=(iterator other) const { return ptr != other.ptr; }

    pointer val() { return ptr; }

   private:
    pointer ptr = nullptr;
  };

  using node_allocator = typename Alloc::template rebind<Node>::other;
  typedef std::allocator_traits<node_allocator> _traits;
  using pointer_to_node = typename node_allocator::pointer;
  using value_type = T;
  using allocator_type = Alloc;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = typename _traits::pointer;
  using const_pointer = typename _traits::const_pointer;
  using iterator = iterator;
  using const_iterator = const_iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  list() {}

  explicit list(const Alloc& alloc) { alloc_ = alloc; }

  // void setBack()

  void base_construct(size_type i, size_type count, Node* pnode, Node& node) {
    if (i < count) {
      node.setNext(pnode);
    } else {
      end_ = &node;
      node.setNext(head_);
      head_->setPrev(end_);
    }
    if (i > 0) {
      node.setPrev(pnode - 2);
    }
    if (i == 0) {
      head_ = &node;
    }
    ++length_;
  }

  void base_destruct() {
    Node* node = head_;
    for (size_type i = 0; i < length_; ++i) {
      _traits::destroy(alloc_, node);
      node = node->getNext();
    }
    // other_traits::destroy(allocator_, temp_);
  }

  void clear_buffer() {
    if (length_ > 0) {
      base_destruct();
      _traits::deallocate(alloc_, head_, length_);
      // other_traits::deallocate(allocator_, temp_, 1);
      length_ = 0;
    }
  }

  list(size_t count, const T& value, const Alloc& alloc = Alloc()) {
    alloc_ = alloc;
    Node* pnode = _traits::allocate(alloc_, count + 1);
    for (size_type i = 0; i < count + 1; ++i) {
      _traits::construct(alloc_, pnode++, value);
      base_construct(i, count, pnode, *(pnode - 1));
    }
    --length_;
    std::cout << "Length " << difference_type(end_ - head_) << std::endl;
  }

  explicit list(size_t count, const Alloc& alloc = Alloc()) {
    alloc_ = alloc;
    Node* pnode = _traits::allocate(alloc_, count + 1);
    for (size_type i = 0; i < count + 1; ++i) {
      _traits::construct(alloc_, pnode++);
      base_construct(i, count, pnode, *(pnode - 1));
    }
  }

  ~list() { clear_buffer(); }

  list(const list& other)
      : alloc_(_traits::select_on_container_copy_construction(other.alloc_)) {
    Node* pnode = _traits::allocate(alloc_, other.length_ + 1);
    Node onode = *other.head_;
    for (size_type i = 0; i < other.length_ + 1; ++i) {
      _traits::construct(alloc_, pnode++, onode.getData());
      onode = onode.getNext();
      base_construct(i, other.length_, pnode, *(pnode - 1));
    }
  }

  list(list&& other) : alloc_(other.alloc_) {
    length_ = std::move(other.length_);
    head_ = std::move(other.head_);
    other.length_ = 0;
    other.head_ = nullptr;
  }

  list& operator=(const list& other) {
    this->~list();
    alloc_ = _traits::select_on_container_copy_construction(other.alloc_);
    Node* pnode = _traits::allocate(alloc_, other.length_ + 1);
    Node onode = *other.head_;
    for (size_type i = 0; i < other.length_ + 1; ++i) {
      _traits::construct(alloc_, pnode++, onode.getData());
      onode = onode.getNext();
      base_construct(i, other.length_, pnode, *(pnode - 1));
    }
  }

  list& operator=(list&& other) {
    this->~list();
    alloc_ = other.alloc_;
    length_ = std::move(other.length_);
    head_ = std::move(other.head_);
    other.length_ = 0;
    other.head_ = nullptr;
  }

  Alloc get_allocator() const { return alloc_; }

  T& front() { return head_->getData(); }

  const T& front() const { return head_->getData(); }

  T& back() { return end_->getPrev()->getData(); }
  const T& back() const { return end_->getPrev()->getData(); }

  iterator begin() { return iterator(head_); }
  // iterator end() { return iterator(head_->getPrev()); }
  iterator end() { return iterator(end_); }

  const_iterator cbegin() const { return const_iterator(head_); }
  // const_iterator cend() const { return const_iterator(head_->getPrev()); }
  const_iterator cend() const { return const_iterator(end_); }

  // reverse_iterator rbegin() { return reverse_iterator(head_->getPrev()); }
  reverse_iterator rbegin() { return reverse_iterator(end_); }
  reverse_iterator rend() { return reverse_iterator(head_); }

  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(cend());
  }
  const_reverse_iterator crend() const {
    return const_reverse_iterator(cbegin());
  }

  bool empty() const { return length_ == 0; }
  size_t size() const { return length_; }
  size_t max_size() const { return length_; }
  void clear() { clear_buffer(); }

  Node& allocate_and_construct(const_iterator pos) {
    Node* pnode;
    if (length_ == 0) {
      pnode = _traits::allocate(alloc_, 2);
    } else {
      pnode = _traits::allocate(alloc_, 1);
    }
    _traits::construct(alloc_, pnode);
    ++length_;
    return fix_nodes(pnode, pos);
  }

  Node& allocate_and_construct(iterator pos, const T& value) {
    Node* pnode;
    if (length_ == 0) {
      pnode = _traits::allocate(alloc_, 2);
    } else {
      pnode = _traits::allocate(alloc_, 1);
    }
    _traits::construct(alloc_, pnode, value);
    ++length_;
    return fix_nodes(pnode, pos);
  }

  Node& allocate_and_construct(iterator pos) {
    Node* pnode;
    if (length_ == 0) {
      pnode = _traits::allocate(alloc_, 2);
    } else {
      pnode = _traits::allocate(alloc_, 1);
    }
    _traits::construct(alloc_, pnode);
    ++length_;
    return fix_nodes(pnode, pos);
  }

  template <class... Args>
  Node& allocate_and_construct_with_args(iterator pos, Args&&... args) {
    Node* pnode;
    if (length_ == 0) {
      pnode = _traits::allocate(alloc_, 2);
    } else {
      pnode = _traits::allocate(alloc_, 1);
    }
    temp_ = other_traits::allocate(allocator_, 1);
    other_traits::construct(allocator_, temp_, std::forward<Args>(args)...);
    _traits::construct(alloc_, pnode, *temp_);
    ++length_;
    return fix_nodes(pnode, pos);
  }

  iterator insert(iterator pos, const T& value) {
    Node* pnode = _traits::allocate(alloc_, 1);
    _traits::construct(alloc_, pnode, value);
    if (pos.val() == head_) {
      Node node = *head_;
    }
    ++length_;
    return fix_nodes(pnode, pos);
  }

  iterator insert(iterator pos, T&& value) {
    allocate_and_construct(pos, std::forward<T>(value));
  }

  iterator insert(const_iterator pos, const T& value) {
    allocate_and_construct(pos, value);
  }

  iterator insert(const_iterator pos, T&& value) {
    allocate_and_construct(pos, std::forward<T>(value));
  }

  iterator insert(iterator pos, size_t count, const T& value) {
    for (size_type i = 0; i < count; ++i) {
      insert(pos++, value);
    }
  }

  iterator erase(const_iterator pos) {
    pos.val()->getPrev()->setNext(pos.val()->getNext());
    pos.val()->getNext()->setPrev(pos.val()->getPrev());
    _traits::destroy(alloc_, pos.val());
    --length_;
  }

  iterator erase(const_iterator first, const_iterator last) {
    for (; first != last; ++first) {
      erase(first);
    }
  }

  iterator erase(iterator pos) {
    std::cout << "AAA" << pos.val() << std::endl;
    pos.val()->getPrev()->setNext(pos.val()->getNext());
    pos.val()->getNext()->setPrev(pos.val()->getPrev());
    _traits::destroy(alloc_, pos.val());
    --length_;
  }

  iterator erase(iterator first, iterator last) {
    for (; first != last; ++first) {
      erase(first);
    }
  }

  void push_back(const T& value) { insert(const_iterator(end_), value); }

  void push_back(T&& value) { insert(const_iterator(end_), value); }

  void pop_back() {
    std::cout << (end_->getPrev() == nullptr) << std::endl;
    erase(const_iterator(end_->getPrev()));
  }

  void push_front(const T& value) {
    insert(const_iterator(head_), value);
    if (length_ != 1) {
      head_ = head_->getPrev();
    }
  }

  void push_front(T&& value) {
    insert(const_iterator(head_), value);
    if (length_ != 1) {
      head_ = head_->getPrev();
    }
  }

  void pop_front() {
    Node node = *head_;
    head_ = head_->getNext();
    end_->setNext(head_);
    head_->setPrev(end_);
    erase(const_iterator(&node));
  }

  template <class... Args>
  iterator emplace(iterator pos, Args&&... args) {
    Node node =
        allocate_and_construct_with_args(pos, std::forward<Args>(args)...);
    return iterator(&node);
  }

  template <class... Args>
  void emplace_back(Args&&... args) {
    emplace(iterator(end_), std::forward<Args>(args)...);
  }

  template <class... Args>
  void emplace_front(Args&&... args) {
    emplace(iterator(head_), std::forward<Args>(args)...);
    head_ = head_->getPrev();
  }

  void resize(size_t count) {
    if (count == length_) {
      return;
    }
    if (count > length_) {
      size_type diff = count - length_;
      for (size_type i = 0; i < diff; ++i) {
        allocate_and_construct(const_iterator(head_));
      }
    } else {
      size_type diff = length_ - count;
      for (size_type i = 0; i < diff; ++i) {
        pop_back();
      }
    }
  }

  void swap(list& other) {
    if (std::allocator_traits<
            allocator_type>::propagate_on_container_swap::value) {
      if (get_allocator() == other.get_allocator()) {
        list temp = std::move(*this);
        *this = std::move(other);
        other = std::move(*this);
      }
    }
  }

  // void merge(list& other);
  // void splice(const_iterator pos, list& other);
  // void remove(const T& value);
  // void reverse();
  // void unique();
  // void sort();

  // Your code goes here?..

  //  private:
  size_type length_ = 0;
  Node* head_ = nullptr;
  Node* end_ = nullptr;
  T* temp_;
  node_allocator alloc_;
  std::allocator<T> allocator_;
  typedef std::allocator_traits<std::allocator<T>> other_traits;
};

// Your template function definitions may go here...

}  // namespace task
