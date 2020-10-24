#pragma once

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

  void handleCopies(Allocator& b) {
    b.increaseCopies();
    copies_ = b.copies();
  }

    Allocator(const Allocator& copy) : chunk_(copy.chunk_), copies_(copy.copies_) {
        ++ *copies_;
    }

  Allocator operator=(Allocator& b) {
    if (&b == this || this->chunk() == b.chunk()) {
      return *this;
    }
    remove();
    chunk_ = b.chunk();
    handleCopies(b);
  }

  class Chunk {
   public:
    Chunk() : start_((T*)(new uint8_t[space_ * sizeof(T)])) {}

    Chunk(Chunk* prev)
        : start_((T*)(new uint8_t[space_ * sizeof(T)])), prev_(prev) {}

    ~Chunk() {
      int n = (capacity_ - space_);
      start_ -= n * sizeof(T);
      delete (uint8_t*)start_;
    }

    T* start() { return start_; }

    std::size_t space() { return space_; }

    const std::size_t capacity() { return capacity_; }

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

   private:
    T* start_;
    Chunk* prev_ = nullptr;
    const std::size_t capacity_ = 32;
    std::size_t space_ = capacity_;
  };

  Chunk* searchSpace(const std::size_t n) {
    Chunk* prev = chunk_->prev();
    while (prev) {
      if (prev->space() >= n) {
        return prev;
      }
      prev = prev->prev();
    }
    return nullptr;
  }

  T* allocate(const std::size_t n) {
    std::cout << "Allocating " << n << std::endl;
    if (!chunk_) {
      chunk_ = new Chunk();
    }
    if (chunk_->space() < n) {
      auto block = searchSpace(n);
      if (!block) {
        chunk_ = new Chunk(chunk_);
      } else {
        block->reduceSpace(n);
        return block->start();
      }
    }

    if (chunk_->capacity() < n) {
      std::cout << "Too much capacity request" << std::endl;
      return nullptr;
    }

    chunk_->reduceSpace(n);
    return chunk_->start();
  }

  void deallocate(T* p, const std::size_t n) {}

  template <typename... Args>
  void construct(T* p, const Args&&... args) {
    new (p) T(args...);
  }

  void destroy(T* p) { p->~T(); }

  void remove() {
    std::cout << (*copies_) << std::endl;
    if (*copies_ == 1) {
      Chunk* prev = chunk_->prev();
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

  ~Allocator() {
    std::cout << "Destructor" << std::endl;
    remove();
  }

  Chunk* chunk() { return chunk_; }

  void increaseCopies() { ++(*copies_); }

  int* copies() { return copies_; }

 private:
  Chunk* chunk_;
  int* copies_;
};