#ifndef __MY_DEQUE_HPP__
#define __MY_DEQUE_HPP__

#include <vector>

namespace util {

  ////////
  /// double ended queue
  ////////
  template <class T>
  class deque {
  public:

    ////////
    /// constructor semantics ->
    /// - sets size_ from input
    /// - noop if size is zero
    /// - computes number of slabs
    /// - creates first slab
    /// - iterates over any remaining slabs
    ///   - creates slab
    ///   - links slab to last
    ////////
    deque(const size_t size);

    ////////
    /// copy constructor semantics ->
    /// - sets size_ to other's size
    /// - iterates over other's slabs
    ///   - creates new slab
    ///   - copies other's slab_ to new slab
    ///   - links slab to last
    ////////
    deque(const deque& q);

    ////////
    /// assignment operator semantics ->
    /// - noop if self assignment
    /// - clears self
    /// - sets size_ to other's size
    /// - iterates over other's slabs
    ///   - creates new slab
    ///   - copies other's slab_ to new slab
    ///   - links slab to last
    ////////
    deque& operator=(const deque& q);

    ////////
    /// move constructor semantics ->
    /// - sets members from other
    /// - nullptrs out other
    ////////
    deque(deque&& q);

    ////////
    /// move assignment semantics ->
    /// - noop if self assignment
    /// - clears self
    /// - sets members from other
    /// - nullptrs out other
    ////////
    deque& operator=(deque&& q);

    ////////
    /// destructor semantics ->
    /// - calls clear
    ////////
    ~deque();

    ///////
    /// push_back semantics ->
    /// - checks if last slab is full
    /// - if full, creates a new slab and links to the end
    /// - pushes_back item to slab_ vector
    ////////
    void push_back(const T& t);

    ///////
    /// emplace_back semantics ->
    /// - checks if last slab is full
    /// - if full, creates a new slab and links to the end
    /// - emplaces_back item to slab_ vector
    ///////
    template <class... U>
    void emplace_back(U&&... args);

    ////////
    /// fwd decl for iterators
    ////////
    class iterator;
    class const_iterator;
    friend class iterator;
    friend class const_iterator;

    ////////
    /// iterators to the underlying slab structure
    ////////
    typedef typename std::vector<T>::iterator        slab_iterator;
    typedef typename std::vector<T>::const_iterator  slab_const_iterator;

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
    /// end
    ////////
    const_iterator end() const;

  private:

    ////////
    /// copy helper semantics ->
    /// - noop if q is empty
    /// - creates first slab
    /// - iterates over rest appending slabs
    ////////
    void copy(const deque& q);

    ////////
    /// move helper semantics ->
    /// - move members over from q
    /// - nullptrs out q
    ////////
    void move(deque&& q);

    ////////
    /// clear semantics ->
    /// - deletes each chained slab
    /// - nullptrs out members
    ////////
    void clear();

    ////////
    /// deque slab
    ////////
    struct slab {

      ////////
      /// constructor semantics ->
      /// - reserves size space in slab_
      ////////
      slab(const size_t size);

      ///////
      /// push_back semantics ->
      /// - pushes_back to slab [precond - size not reached]
      ////////
      void push_back(const T& t);

      ///////
      /// emplace_back semantics ->
      /// - emplaces_back to slab [precond - size not reached]
      ////////
      template <class... U>
      void emplace_back(U&&... args);

      ////////
      /// full semantics ->
      /// - true if slab_ size is size_
      ////////
      bool full() const;

      ////////
      /// empty semantics ->
      /// - true if the slab_ is empty
      ////////
      bool empty() const;

      ////////
      /// begin semantics ->
      /// - begin() on slab_
      ////////
      slab_iterator begin();

      ////////
      /// end semantics ->
      /// - end() on slab_
      ////////
      slab_iterator end();

      ////////
      /// const begin semantics ->
      /// - begin() on slab_
      ////////
      slab_const_iterator begin() const;

      ////////
      /// const end semantics ->
      /// - end() on slab_
      ////////
      slab_const_iterator end() const;
      
      ////////
      /// maximum slab size before it's full and
      /// another slab needs to be chained
      ////////
      size_t size_;

      ////////
      /// a slab has a vector of items in it
      /// that composition is the real slab
      ////////
      typedef std::vector<T> slab_t;
      slab_t slab_;

      ////////
      /// the next slab in the chain or nullptr
      ////////
      slab*  link_;
    };

    slab*  first_;
    slab*  last_;
    size_t size_;

    static const size_t slab_size_ = 100;

  };

}

#include <d.ipp>

#endif
