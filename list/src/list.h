#pragma once
#include <iostream>
#include <iterator>

namespace task {

template <class T, class Alloc = std::allocator<T>>
class list {
 public:
  class Node {
   public:
    Node() {}

    Node(T data) : data_(std::move(data)) {}

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

  template <typename P, typename R>
  class iterator_base {
   public:
    friend class list;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = P;
    using reference = R;
    using iterator_category = std::bidirectional_iterator_tag;

    iterator_base(Node* iptr) : ptr(iptr) {}

    iterator_base(const iterator_base& other) { ptr = other.ptr; }

    iterator_base& operator=(const iterator_base& other) { ptr = other.ptr; }

    iterator_base& operator++() {
      ptr = ptr->getNext();
      return *this;
    }

    iterator_base operator++(int) {
      Node old_ptr = *ptr;
      ptr = ptr->getNext();
      iterator_base other = iterator_base(&old_ptr);
      return other;
    }

    reference operator*() const { return ptr->getData(); }

    pointer operator->() const {
      return const_cast<pointer>(&(ptr->getData()));
    }

    iterator_base& operator--() {
      ptr = ptr->getPrev();
      return *this;
    }

    iterator_base operator--(int) {
      Node old_ptr = *ptr;
      ptr = ptr->getPrev();
      iterator_base other = iterator_base(&old_ptr);
      return other;
    }

    bool operator==(iterator_base other) const { return ptr == other.ptr; }

    bool operator!=(iterator_base other) const { return ptr != other.ptr; }

    iterator_base operator+(difference_type n) {
      while (n != 0) {
        ptr = ptr->getNext();
        --n;
      }
      return *this;
    }

    iterator_base operator-(difference_type n) {
      while (n != 0) {
        ptr = ptr->getPrev();
        --n;
      }
      return *this;
    }

    Node* val() { return ptr; }

   private:
    Node* ptr = nullptr;
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
  typedef iterator_base<T*, T&> iterator;
  typedef iterator_base<const T*, const T&> const_iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<iterator>;

  list() {
    head_ = _traits::allocate(alloc_, 1);
    end_ = head_;
    start_ = head_;
  }

  explicit list(const Alloc& alloc) : alloc_(alloc) {
    head_ = _traits::allocate(alloc_, 1);
    end_ = head_;
  }

  list(size_t count, const T& value, const Alloc& alloc = Alloc())
      : alloc_(alloc) {
    Node* pnode = _traits::allocate(alloc_, count + 1);
    for (size_type i = 0; i < count + 1; ++i) {
      _traits::construct(alloc_, pnode++, value);
      baseConstruct(i, count, pnode, *(pnode - 1));
    }
    --length_;
  }

  explicit list(size_t count, const Alloc& alloc = Alloc()) {
    alloc_ = alloc;
    Node* pnode = _traits::allocate(alloc_, count + 1);
    for (size_type i = 0; i < count + 1; ++i) {
      _traits::construct(alloc_, pnode++);
      baseConstruct(i, count, pnode, *(pnode - 1));
    }
    --length_;
  }

  ~list() { clear_buffer(); }

  list(const list& other)
      : alloc_(_traits::select_on_container_copy_construction(other.alloc_)) {
    copy_base(other);
  }

  list(list&& other) { move_base(std::forward<list>(other)); }

  list& operator=(const list& other) {
    this->~list();
    copy_base(other);
    return *this;
  }

  list& operator=(list&& other) {
    this->~list();
    move_base(std::forward<list>(other));
  }

  Alloc get_allocator() const { return alloc_; }

  T& front() { return head_->getData(); }

  const T& front() const { return head_->getData(); }

  T& back() { return end_->getPrev()->getData(); }
  const T& back() const { return end_->getPrev()->getData(); }

  iterator begin() { return iterator(head_); }
  iterator end() { return iterator(end_); }

  const_iterator cbegin() const { return const_iterator(head_); }
  const_iterator cend() const { return const_iterator(end_); }

  reverse_iterator rbegin() { return reverse_iterator(end_); }
  reverse_iterator rend() { return reverse_iterator(head_); }

  reverse_iterator crbegin() const { return reverse_iterator(end_); }
  reverse_iterator crend() const { return reverse_iterator(head_); }

  bool empty() const { return length_ == 0; }
  size_t size() const { return length_; }
  size_t max_size() const { return length_; }
  void clear() { clear_buffer(); }

  void fixNodes(Node* inode, Node* pnode) {
    if (inode == head_ || inode == end_) {
      if (length_ == 0) {
        head_ = pnode;
        *end_ = *head_;
        head_->setNext(end_);
        end_->setPrev(head_);
        head_->setPrev(end_);
        end_->setNext(head_);
      } else if (inode == head_) {
        pnode->setNext(head_);
        pnode->setPrev(end_);
        head_->setPrev(pnode);
        head_ = pnode;
      } else if (inode == end_) {
        pnode->setNext(end_);
        pnode->setPrev(end_->getPrev());
        end_->getPrev()->setNext(pnode);
        end_->setPrev(pnode);
      }
    } else {
      Node* ptr = inode;
      pnode->setNext(ptr);
      pnode->setPrev(ptr->getPrev());
      ptr->getPrev()->setNext(pnode);
      ptr->setPrev(pnode);
    }
    ++length_;
  }

  Node* allocate_and_construct(Node* inode) {
    Node* pnode = _traits::allocate(alloc_, 1);
    _traits::construct(alloc_, pnode);
    fixNodes(inode, pnode);
    return pnode;
  }

  Node* allocate_and_construct(Node* inode, const T& value) {
    Node* pnode = _traits::allocate(alloc_, 1);
    _traits::construct(alloc_, pnode, value);
    fixNodes(inode, pnode);
    return pnode;
  }

  template <class... Args>
  Node* allocate_and_construct_with_args(Node* inode, Args&&... args) {
    Node* pnode = _traits::allocate(alloc_, 1);
    T* temp = other_traits::allocate(allocator_, 1);
    other_traits::construct(allocator_, temp, std::forward<Args>(args)...);
    _traits::construct(alloc_, pnode, *temp);
    fixNodes(inode, pnode);
    return pnode;
  }

  iterator insert(iterator pos, const T& value) {
    Node* pnode = allocate_and_construct(pos.val(), value);
    return iterator(pnode);
  }

  iterator insert(iterator pos, T&& value) {
    Node* pnode = allocate_and_construct(pos.val(), std::forward<T>(value));
    return iterator(pnode);
  }

  iterator insert(iterator pos, size_t count, const T& value) {
    iterator temp(head_);
    for (size_type i = 0; i < count; ++i) {
      pos = insert(pos, value);
    }
    return pos;
  }

  void removeNode(Node* ptr) {
    ptr->getPrev()->setNext(ptr->getNext());
    ptr->getNext()->setPrev(ptr->getPrev());
  }

  iterator erase(iterator pos) {
    Node* ptr = pos.val();
    if (length_ != 0) {
      if (length_ == 1) {
        _traits::destroy(alloc_, ptr);
        --length_;
        head_ = end_;
      } else {
        removeNode(ptr);
        if (ptr == head_) {
          head_ = head_->getNext();
        }
        _traits::destroy(alloc_, ptr);
        --length_;
      }
    }
  }

  iterator erase(iterator first, iterator last) {
    for (; first != last; ++first) {
      erase(first);
    }
  }

  void push_back(const T& value) { insert(iterator(end_), value); }

  void push_back(T&& value) { insert(iterator(end_), value); }

  void pop_back() { erase(iterator(end_->getPrev())); }

  void push_front(const T& value) { insert(iterator(head_), value); }

  void push_front(T&& value) { insert(iterator(head_), value); }

  void pop_front() { erase(iterator(head_)); }

  template <class... Args>
  iterator emplace(iterator pos, Args&&... args) {
    Node* node = allocate_and_construct_with_args(pos.val(),
                                                  std::forward<Args>(args)...);
    return iterator(node);
  }

  template <class... Args>
  void emplace_back(Args&&... args) {
    emplace(iterator(end_), std::forward<Args>(args)...);
  }

  template <class... Args>
  void emplace_front(Args&&... args) {
    emplace(iterator(head_), std::forward<Args>(args)...);
  }

  void resize(size_t count) {
    if (count == length_) {
      return;
    }
    if (count > length_) {
      size_type diff = count - length_;
      for (size_type i = 0; i < diff; ++i) {
        allocate_and_construct(head_);
      }
    } else {
      size_type diff = length_ - count;
      for (size_type i = 0; i < diff; ++i) {
        pop_back();
      }
    }
  }

  void swap(list& other) {
    list temp = std::move(*this);
    *this = std::move(other);
    other = std::move(temp);
  }

  iterator moveNode(iterator ofirst, iterator first) {
    removeNode(ofirst.val());
    Node* pnode = ofirst.val();
    iterator temp = ++ofirst;
    fixNodes(first.val(), pnode);
    return temp;
  }

  void merge(list& other) {
    iterator first = begin();
    iterator second = end();

    iterator ofirst = other.begin();
    iterator osecond = other.end();

    size_type length = other.length_;
    for (size_type i = 0; i < length; ++i) {
      bool inserted = false;
      for (; first != second; ++first) {
        if (*first >= *ofirst) {
          ofirst = moveNode(ofirst, first);
          inserted = true;
          break;
        }
      }
      if (!inserted) {
        ofirst = moveNode(ofirst, second);
      }
    }
    other.head_ = other.end_;
    other.length_ = 0;
  }

  void splice(iterator pos, list& other) {
    iterator first = other.begin();
    for (size_type i = 0; i < other.length_; ++i) {
      removeNode(first.val());
      Node* pnode = first.val();
      iterator temp = ++first;
      fixNodes(pos.val(), pnode);
      first = temp;
    }
  }

  void remove(const T& value) {
    iterator first = begin();
    iterator second = end();
    for (; first != second; ++first) {
      if (*first == value) {
        erase(first);
      }
    }
  }

  void iterSwap(iterator a, iterator b) {
    T temp = *b;
    b.val()->setData(*a);
    a.val()->setData(temp);
  }

  void reverse() {
    for (size_t i = 0; i < length_ / 2; ++i) {
      iterSwap(begin() + i, end() - i - 1);
    }
  }

  void unique() {
    iterator st = ++begin();
    size_type t_length = length_;
    for (size_type i = 0; i <= t_length; ++i) {
      iterator pr = --st;
      ++st;
      iterator temp = iterator(st.val()->getNext());
      if (*pr == *st) {
        erase(st);
        st = temp;
      } else
        ++st;
    }
  }

  void sort() {
    iterator start = begin();
    for (size_type i = 1; i < length_; ++i) {
      ++start;
      iterator st = start;
      size_type j = i;
      while (j > 0) {
        --j;
        iterator cur = st;
        iterator prev = --st;
        if (*cur < *prev) {
          Node temp = *cur.val();
          cur.val()->setData(*prev);
          prev.val()->setData(temp.getData());
        }
      }
    }
  }

  size_type length() { return length_; }

  Node* head() { return head_; }

  Node* tail() { return end_; }

 private:
  size_type length_ = 0;
  Node* head_ = nullptr;
  Node* start_ = nullptr;
  Node* end_ = nullptr;
  T* temp_;
  node_allocator alloc_;
  std::allocator<T> allocator_;
  typedef std::allocator_traits<std::allocator<T>> other_traits;

  void baseConstruct(size_type i, size_type count, Node* pnode, Node& node) {
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
      start_ = head_;
    }
    ++length_;
  }

  void baseDestruct() {
    Node* node = head_;
    for (size_type i = 0; i < length_; ++i) {
      _traits::destroy(alloc_, node);
      node = node->getNext();
    }
  }

  void clear_buffer() {
    if (length_ > 0) {
      baseDestruct();
      _traits::deallocate(alloc_, start_, length_);
      length_ = 0;
    }
  }

  void copy_base(const list& other) {
    Node* pnode = _traits::allocate(alloc_, other.length_ + 1);
    Node* onode = other.head_;
    for (size_type i = 0; i < other.length_ + 1; ++i) {
      _traits::construct(alloc_, pnode++, onode->getData());
      onode = onode->getNext();
      baseConstruct(i, other.length_, pnode, *(pnode - 1));
    }
    --length_;
  }

  void move_base(list&& other) {
    alloc_ = other.alloc_;
    allocator_ = other.allocator_;
    length_ = std::move(other.length_);
    head_ = std::move(other.head_);
    end_ = std::move(other.end_);
    start_ = std::move(other.start_);
    temp_ = std::move(other.temp_);

    other.length_ = 0;
    other.head_ = nullptr;
    other.end_ = nullptr;
    other.start_ = nullptr;
    other.temp_ = nullptr;
  }
};

}  // namespace task
