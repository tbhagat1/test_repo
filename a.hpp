#ifndef __MY_ALLOCATOR_HPP__
#define __MY_ALLOCATOR_HPP__

#include <cstddef>
#include <utility>

namespace util {

  ////////
  /// heap allocator
  ////////
  template <class T>
  class heap_allocator {
  public:

    ////////
    /// allocate space of T * size bytes
    ////////
    T* allocate(const size_t size);

    ////////
    /// copy construct at location p from T
    ////////
    T* construct(void* p, const T& from = T());

    ////////
    /// copy construct at p from forwarded args
    ////////
    template <class... U>
    T* construct(void* p, U&&... args);

    ////////
    /// invoke destructor for T at location p
    ////////
    void destroy(T* p);

    ////////
    /// delete byte array at p
    ////////
    void deallocate(void* p);
  };

  ////////
  /// allocate
  ////////
  template <class T>
  inline T*
  heap_allocator<T>::
  allocate(const size_t size) {
    if (!size) return nullptr;
    return (T*) new unsigned char[sizeof(T) * size];
  }

  ////////
  /// construct
  ////////
  template <class T>
  inline T*
  heap_allocator<T>::
  construct(void* p, const T& from) {
    return new (p) T (from);
  }

  ////////
  /// construct [forwarded]
  ////////
  template <class T>
  template <class... U>
  inline T*
  heap_allocator<T>::
  construct(void* p, U&&... args) {
    return new (p) T(std::forward<U>(args)...);
  }

  ////////
  /// destroy
  ////////
  template <class T>
  inline void
  heap_allocator<T>::
  destroy(T* p) {
    p->~T();
  }

  ////////
  /// deallocate
  ////////
  template <class T>
  inline void
  heap_allocator<T>::
  deallocate(void* p) {
    delete [] (unsigned char*) p;
  }

}

#endif
