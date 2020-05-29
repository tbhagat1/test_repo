#ifndef __MY_VECTOR_HPP__
#define __MY_VECTOR_HPP__

#include <a.hpp>

namespace util {

  ////////
  /// very rudimentary vector
  ////////
  template <class T, class A = heap_allocator<T>>
  class vector {
  public:

    ////////
    /// semantics ->
    /// everything is zero, no memory allocated
    ////////
    vector();

    ////////
    /// semantics ->
    /// - allocates size elements in contiguous memory
    /// - constructs each element with a copy of t
    /// - as a result size and capacity are the same
    ////////
    vector(const size_t size, const T& t = T());

    ////////
    /// semantics ->
    /// - allocate space as speficied by v
    /// - copy elements from v into self
    /// - update members
    ////////
    vector(const vector<T,A>& v);

    ////////
    /// semantics ->
    /// - noop if self assignment
    /// - clears existing memory
    /// - copies from v into self
    ////////
    vector<T,A>& operator=(const vector<T,A>& v);

    ////////
    /// semantics ->
    /// - moves v members into self
    /// - nullptrs out v members
    ////////
    vector(vector<T,A>&& v);

    ////////
    /// semantics ->
    /// - noop if self assignment
    /// - clears existing memory
    /// - moves v members into self
    /// - nullptrs out v members
    ////////
    vector<T,A>& operator=(vector<T,A>&& v);

    ////////
    /// semantics ->
    /// - destroys each initialized element
    /// - deallocates contiguous memory
    ////////
    ~vector();

    ////////
    /// semantics ->
    /// - noop if capacity_ is >= cap
    /// - allocates 'cap' contiguous memory
    /// - (re)constructs elements from old to new memory
    /// - destroys elements in old memory
    /// - deallocates old memory
    /// - updates members to reflect new memory
    ////////
    void reserve(const size_t cap);

    ////////
    /// semantics ->
    /// - call reserve with new size
    /// - construct any extra elements in memory
    /// - update end and size
    ////////
    void resize(const size_t size);

    ////////
    /// semantics ->
    /// -`if capacity reached, reserves more space by factor of 2
    /// - copy constructs new element at end_
    /// - increments size and end to reflect new element
    ////////
    void push_back(const T& t);

    ////////
    /// semantics ->
    /// -`similar to above but with variadic arguments
    /// -`if capacity reached, reserves more space by factor of 2
    /// - in-place constructs new element at end_ w/ args
    /// - increments size and end to reflect new element
    ////////
    template <class... U>
    void emplace_back(U&&... args);

    ////////
    /// iterator types
    ////////
    typedef T *       iterator;
    typedef const T * const_iterator;

    ////////
    /// semantics ->
    /// invokes overload with two iterators
    ////////
    void erase(iterator i);

    ////////
    /// semantics ->
    /// - copies elements over [first - second] range
    /// - erases elements after those shifted over
    ////////
    void erase(iterator first, iterator second);

    ////////
    /// semantics ->
    /// - destroy from start to end (size elements)
    /// - deallocate memory
    /// - reset all members to semantic zero
    ////////
    void clear();

    ////////
    /// operator[]
    ////////
    T& operator[](const size_t i);

    ////////
    /// operator[] (const)
    ////////
    const T& operator[](const size_t i) const;

    ////////
    /// size interrogators
    ////////
    bool empty() const;
    size_t size() const;
    size_t capacity() const;

     ////////
     /// begin
     ////////
     iterator begin();

     ////////
     /// end
     ////////
     iterator end();
    
     ////////
     /// const begin
     ////////
     const_iterator begin() const;

     ////////
     /// const end
     ////////
     const_iterator end() const;

  private:

    ////////
    /// start    -> beginning of contiguous memory
    /// end      -> end of initialized (constructed) memory
    /// cliff    -> end of contiguous raw memory
    /// size     -> end   - start or number of initialized elements
    /// capacity -> cliff - start or number of total elements
    ////////

    T*      start_;
    T*      end_;
    T*      cliff_;
    size_t  size_;
    size_t  capacity_;

    typedef A allocator_t;
    allocator_t alloc_;
  };

  ////////
  /// default constructor
  ////////
  template <class T, class A>
  inline
  vector<T,A>::
  vector() :
    start_   (nullptr),
    end_     (nullptr),
    cliff_   (nullptr),
    size_    (0      ),
    capacity_(0      )
  {}
  
  ////////
  /// constructor
  ////////
  template <class T, class A>
  inline
  vector<T,A>::
  vector(const size_t  size,
         const T&      from) :
    start_   (nullptr),
    end_     (nullptr),
    cliff_   (nullptr),
    size_    (0      ),
    capacity_(0      ) {

    T* p = alloc_.allocate(size);
    T* q = p;
    for (size_t i = 0; i < size; ++i) {
      alloc_.construct(q, from);
      ++q;
    }
    start_ = p;
    end_   = q;
    cliff_ = q;
    size_  = capacity_ = size;
  }

  ////////
  /// copy constructor
  ////////
  template <class T, class A>
  inline
  vector<T,A>::
  vector(const vector<T,A>& v) :
    start_   (nullptr),
    end_     (nullptr),
    cliff_   (nullptr),
    size_    (0      ),
    capacity_(0      ) {
  
    reserve(v.capacity_);
    size_ = v.size_;

    T* p = start_;
    for (size_t i = 0; i < size_; ++i) {
      alloc_.construct(p, v[i]);
      ++p;
    }
    end_ = start_ + size_;
  }

  ////////
  /// assignment operator
  ////////
  template <class T, class A>
  inline vector<T,A>&
  vector<T,A>::
  operator=(const vector<T,A>& v) {

    if (this == &v) return *this;

    clear();
    size_ = v.size_;
    capacity_ = v.capacity_;
    start_ = alloc_.allocate(capacity_);

    T* p = start_;
    for (size_t i = 0; i < size_; ++i) {
      alloc_.construct(p, v[i]);
      ++p;
    }
    end_ = start_ + size_;
    cliff_ = start_ + capacity_;
    return *this;
  }

  ////////
  /// move constructor
  ////////
  template <class T, class A>
  inline
  vector<T,A>::
  vector(vector<T,A>&& v) :
    start_   (nullptr),
    end_     (nullptr),
    cliff_   (nullptr),
    size_    (0      ),
    capacity_(0      ) {

    capacity_ = v.capacity_;
    size_ = v.size_;
    start_ = v.start_;
    end_ = v.end_;
    cliff_ = v.cliff_;

    v.capacity_ = v.size_ = 0;
    v.start_ = v.end_ = v.cliff_ = nullptr;
  }

  ////////
  /// move assignment
  ////////
  template <class T, class A>
  inline vector<T,A>&
  vector<T,A>::
  operator=(vector<T,A>&& v) {

    if (this == &v) return *this;
    clear();

    capacity_ = v.capacity_;
    size_ = v.size_;
    start_ = v.start_;
    end_ = v.end_;
    cliff_ = v.cliff_;

    v.capacity_ = v.size_ = 0;
    v.start_ = v.end_ = v.cliff_ = nullptr;
    return *this;
  }

  ////////
  /// reserve
  ////////
  template <class T, class A>
  inline void
  vector<T,A>::
  reserve(const size_t cap) {
    if (cap <= capacity_) return;

    T* p = alloc_.allocate(cap);
    T* q = p;
    T* t = start_;

    ////////
    /// copy construct old elements into new slots
    ////////
    for (size_t i = 0; i < size_; ++i) {
      alloc_.construct(q, t[i]);
      ++q;
    }
    ////////
    /// destroy elements in old slots
    ////////
    t = start_;
    for (size_t i = 0; i < size_; ++i) {
      alloc_.destroy(t);
      ++t;
    }
    alloc_.deallocate(start_);

    start_ = p;
    end_   = q;
    cliff_ = start_ + cap;
    capacity_ = cap;
  }

  ////////
  /// resize
  ////////
  template <class T, class A>
  inline void
  vector<T,A>::
  resize(const size_t size) {

    reserve(size);
    T* p = end_;
    for (size_t i = size_; i < size; ++i) {
      alloc_.construct(p);
      ++p;
    }
    size_ = size;
    end_  = p;
  }

  ////////
  /// destructor
  ////////
  template <class T, class A>
  inline
  vector<T,A>::
  ~vector() {
    clear();
  }

  ////////
  /// clear
  ////////
  template <class T, class A>
  inline void
  vector<T,A>::
  clear() {

    T* p = start_;
    for (size_t i = 0; i < size_; ++i) {
      alloc_.destroy(p);
      ++p;
    }
    alloc_.deallocate(start_);
    start_ = end_ = cliff_ = nullptr;
    size_ = capacity_ = 0;
  }

  ////////
  /// push back
  ////////
  template <class T, class A>
  inline void
  vector<T,A>::
  push_back(const T& t) {
    if (size_ == capacity_) {
      if (!capacity_) capacity_ = 100;
      reserve(capacity_ * 2);
    }
    alloc_.construct(end_, t);
    ++end_;
    ++size_;
  }

  ////////
  /// emplace back
  ////////
  template <class T, class A>
  template <class... U>
  inline void
  vector<T,A>::
  emplace_back(U&&... args) {

    if (size_ == capacity_) {
      if (!capacity_) capacity_ = 100;
      reserve(capacity_ * 2);
    }
    alloc_.construct(end_, args...);
    ++end_;
    ++size_;
  }

  ////////
  /// erase
  ////////
  template <class T, class A>
  inline void
  vector<T,A>::
  erase(iterator i) {
    erase(i, i);
  }

  ////////
  /// erase
  ////////
  template <class T, class A>
  inline void
  vector<T,A>::
  erase(iterator first,
        iterator second) {

    ////////
    /// copy elements over
    ////////
    iterator p = first;
    iterator q = ++second;
    while (q != end_) {
      *p = *q;
      ++p; ++q;
    }

    ////////
    /// destroy left over elements
    ////////
    iterator n = p;
    while (p != end_) {
      alloc_.destroy(p);
      ++p;
    }
    size_ -= second - first;
    end_ = n;
  }

  ////////
  /// empty
  ////////
  template <class T, class A>
  inline bool
  vector<T,A>::
  empty() const {
    return !size_;
  }

  ////////
  /// size
  ////////
  template <class T, class A>
  inline size_t
  vector<T, A>::
  size() const {
    return size_;
  }

  ////////
  /// capacity
  ////////
  template <class T, class A>
  inline size_t
  vector<T,A>::
  capacity() const {
    return capacity_;
  }

  ////////
  /// operator[]
  ////////
  template <class T, class A>
  inline T&
  vector<T,A>::
  operator[](const size_t i) {
    return start_[i];
  }

  ////////
  /// operator[] (const)
  ////////
  template <class T, class A>
  inline const T&
  vector<T,A>::
  operator[](const size_t i) const {
    return start_[i];
  }

  ////////
  /// begin
  ////////
  template <class T, class A>
  inline typename vector<T,A>::iterator
  vector<T,A>::
  begin() {
    return start_;
  }
  
  ////////
  /// end
  ////////
  template <class T, class A>
  inline typename vector<T,A>::iterator
  vector<T,A>::
  end() {
    return end_;
  }
  
  ////////
  /// const begin
  ////////
  template <class T, class A>
  inline typename vector<T,A>::const_iterator
  vector<T,A>::
  begin() const {
    return start_;
  }
  
  ////////
  /// end
  ////////
  template <class T, class A>
  inline typename vector<T,A>::const_iterator
  vector<T,A>::
  end() const {
    return end_;
  }

}

#include <a.hpp>

#endif
