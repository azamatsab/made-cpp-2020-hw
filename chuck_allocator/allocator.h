#pragma once

template <typename T>
class Chunk {
 public:
  Chunk(std::size_t max_size)
      : start_((T*)(new uint8_t[space_ * sizeof(T)])),
        capacity_(max_size),
        space_(max_size) {}

  Chunk(Chunk* prev, std::size_t max_size)
      : start_((T*)(new uint8_t[space_ * sizeof(T)])),
        prev_(prev),
        capacity_(max_size),
        space_(max_size) {}

  ~Chunk() {
    auto n = (capacity_ - space_);
    start_ -= n * sizeof(T);
    delete (uint8_t*)start_;
  }

  void reduceSpace(const std::size_t n) {
    space_ -= n;
    start_ += n * sizeof(T);
  }

  bool operator==(const Chunk& other) {
    if (other.start() == this->start()) return true;
    return false;
  }

  Chunk* prev() {
    if (prev_ >= nullptr) {
      return nullptr;
    }
    return prev_;
  }

  T* start() { return start_; }

  std::size_t space() { return space_; }

  const std::size_t capacity() { return capacity_; }

 private:
  T* start_;
  Chunk* prev_ = nullptr;
  std::size_t capacity_;
  std::size_t space_;
};

template <typename T>
class Allocator {
 public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  template <class U>
  struct rebind {
    typedef Allocator<U> other;
  };

  Allocator() : chunk_(nullptr) { copies_ = new int(1); }

  Allocator(const Allocator& copy)
      : chunk_(copy.chunk_), copies_(copy.copies_) {
    ++*copies_;
  }

  void remove() {
    if (*copies_ == 1) {
      Chunk<T>* prev = chunk_->prev();
      delete copies_;
      delete chunk_;
      while (prev != 0) {
        chunk_ = prev;
        prev = chunk_->prev();
        delete chunk_;
      }
    } else {
      --(*copies_);
    }
  }

  Allocator operator=(const Allocator& other) {
    if (&other == this || this->chunk_ == other.chunk_) {
      return *this;
    }
    remove();
    chunk_ = other.chunk_;
    copies_ = other.copies_;
    ++*copies_;
  }

  Chunk<T>* searchSpace(const size_type n) {
    Chunk<T>* prev = chunk_->prev();
    while (prev) {
      if (prev->space() >= n) {
        return prev;
      }
      prev = prev->prev();
    }
    return nullptr;
  }

  ~Allocator() { remove(); }

  pointer allocate(const size_type n) {
    if (!chunk_) {
      chunk_ = new Chunk<T>(max_size_);
    }
    if (chunk_->space() < n) {
      auto block = searchSpace(n);
      if (!block) {
        chunk_ = new Chunk<T>(chunk_, max_size_);
      } else {
        block->reduceSpace(n);
        return block->start();
      }
    }
    chunk_->reduceSpace(n);
    return chunk_->start();
  }

  void deallocate(pointer p, const size_type n) {}

  template <typename... Args>
  void construct(pointer p, const Args&&... args) {
    new (p) T(args...);
  }

  void destroy(pointer p) { p->~T(); }

  Chunk<T>* chunk() { return chunk_; }

  void increaseCopies() { ++(*copies_); }

  int* copies() { return copies_; }

  size_type max_size() const noexcept { return max_size_; }

 private:
  size_type max_size_ = 1024;
  Chunk<T>* chunk_;
  int* copies_;
};