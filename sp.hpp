#ifndef __MY_SHARED_PTR_HPP__
#define __MY_SHARED_PTR_HPP__

#include <atomic>

namespace con {

  template <class T>
  class shared_ptr {
  public:

    ////////
    /// semantics ->
    /// - noop if p is null
    /// - creates control block
    /// - sets ref count to one
    /// - sets underlying_ to p
    ////////
    explicit shared_ptr(T* p = nullptr);

    ////////
    /// semantics ->
    /// - sets control block to p's control block
    /// - increases ref count
    ////////
    shared_ptr(const shared_ptr<T>& p);

    ////////
    /// semantics ->
    /// - noop if self assignment
    /// - if decreased ref count hits zero
    ///   - destroys underlying object
    ///   - destroys control block
    /// - sets control block to p's control block
    /// - increases ref count
    ////////
    shared_ptr<T>& operator=(const shared_ptr<T>& p);

    ////////
    /// semantics ->
    /// - if decreased ref count is zero
    ///   - destroys underlying object
    ///   - destroys control block
    ////////
    ~shared_ptr();

    ////////
    /// semantics ->
    /// - if decreased ref count hits zero
    ///   - destroys underlying object
    ///   - destroys control block
    /// - noop if p is null
    /// - creates control block
    /// - sets ref count to one
    /// - sets underlying_ to p
    ////////
    void reset(T* p = nullptr);

    ////////
    /// semantics ->
    /// - returns underlying_ ptr
    ////////
    T* get();

    ////////
    /// semantics ->
    /// - returns const underlying_ ptr
    ////////
    const T* get() const;

    ////////
    /// semantics ->
    /// - returns underlying_ ref
    ////////
    T& operator*();

    ////////
    /// semantics ->
    /// - returns const underlying_ ref
    ////////
    const T& operator*() const;

    ////////
    /// semantics ->
    /// - returns underlying_ ptr
    ////////
    T* operator->();

    ////////
    /// semantics ->
    /// - returns const underlying_ ptr
    ////////
    const T* operator->() const;

  private:

    ////////
    /// control block
    ////////
    struct control_block {

      ////////
      /// @todo:  weak counts
      ////////
      std::atomic<int> count_;
      T* underlying_;
    };

    ////////
    /// semantics ->
    /// - allocates memory to hold control block and object
    /// - creates control block using placement new
    /// - sets control block count to one
    /// - creates underlying object using placement new
    /// - sets control block underlying ptr
    /// - return shared_ptr created from control block
    ////////
    template <class U, class... V>
    friend shared_ptr<U> make_shared(V&&... args);

    ////////
    /// semantics ->
    /// - uses destructor on underlying_ if created with make_shared
    /// - otherwise deletes underlying_
    /// - deletes control block
    ////////
    void destroy();

    ////////
    /// semantics ->
    /// - for use with make_shared
    /// - sets control block and uses_mks_ to true
    ////////
    shared_ptr(control_block* p);

    ////////
    /// control block ptr
    ////////
    control_block* control_;

    ////////
    /// for deletion semantics
    ////////
    bool  uses_mks_;
  };

}

#include <sp.ipp>

#endif
