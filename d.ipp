namespace util {

  ////////
  /// iterator
  ////////
  template <class T>
  class deque<T>::iterator {
  public:

    typedef typename deque<T>::slab          slab;
    typedef typename deque<T>::slab_iterator slab_iterator;

    ////////
    /// default constructor semantics ->
    /// - sets members to defaults
    ////////
    iterator();

    ////////
    /// constructor semantics ->
    /// - sets members from parameters
    ////////
    iterator(slab* p, slab_iterator pos);

    ////////
    /// operator++ (prefix) semantics ->
    /// - if incrementing pos_ hits slab_ end
    ///   - moves pos_ to next slab
    /// - increments pos_
    /// - returns ref to self
    ////////
    iterator& operator++();

    ////////
    /// operator++ (postfix) semantics ->
    /// - creates temporary from self
    /// - increments self using prefix operator++
    /// - returns temporary
    ////////
    iterator operator++(int);

    ////////
    /// operator* semantics ->
    /// - derefs and returns pos_
    ////////
    T& operator*();

    ////////
    /// operator-> semantics ->
    /// - returns pos_
    ////////
    T* operator->();

    ////////
    /// const operator* semantics ->
    /// - derefs and returns pos_
    ////////
    const T& operator*() const;

    ////////
    /// const operator-> semantics ->
    /// - returns pos_
    ////////
    const T* operator->() const;

    ////////
    /// const_iterator conversion operator
    ////////
    operator const_iterator() const;

    ////////
    /// operator!=
    ////////
    template <class U>
    friend bool operator!=(const U& l, const U& r);

    ////////
    /// operator==
    ////////
    template <class U>
    friend bool operator==(const U& l, const U& r);

  private:

    slab*         slab_;
    slab_iterator pos_;

  };

  ////////
  /// constructor
  ////////
  template <class T>
  inline
  deque<T>::
  iterator::
  iterator() :
    slab_(nullptr) {
  }

  ////////
  /// constructor
  ////////
  template <class T>
  inline
  deque<T>::
  iterator::
  iterator(slab* p,
           slab_iterator pos) :
    slab_(p),
    pos_(pos) {
  }

  ////////
  /// operator++
  ////////
  template <class T>
  inline typename deque<T>::iterator&
  deque<T>::
  iterator::
  operator++() {

    if (++pos_ == slab_->end()) {
      slab_ = slab_->link_;
      pos_ = slab_->begin();
    }
    return *this;
  }

  ////////
  /// operator++ (postfix)
  ////////
  template <class T>
  inline typename deque<T>::iterator
  deque<T>::
  iterator::
  operator++(int) {

    iterator tmp(*this);
    ++*this;
    return tmp;
  }

  ////////
  /// operator*
  ////////
  template <class T>
  inline T&
  deque<T>::
  iterator::
  operator*() {
    return *pos_;
  }

  ////////
  /// operator->
  ////////
  template <class T>
  inline T*
  deque<T>::
  iterator::
  operator->() {
    return &(*pos_);
  }

  ////////
  /// const operator*
  ////////
  template <class T>
  inline const T&
  deque<T>::
  iterator::
  operator*() const {
    return *pos_;
  }

  ////////
  /// operator->
  ////////
  template <class T>
  inline const T*
  deque<T>::
  iterator::
  operator->() const {
    return &(*pos_);
  }

  ////////
  /// const_iterator conversion operator
  ////////
  template <class T>
  inline
  deque<T>::
  iterator::
  operator const_iterator() const {
    return const_iterator(slab_, pos_);
  }

  ////////
  /// const_iterator
  ////////
  template <class T>
  class deque<T>::const_iterator {
  public:

    typedef typename deque<T>::slab                slab;
    typedef typename deque<T>::slab_const_iterator slab_const_iterator;

    ////////
    /// default constructor semantics ->
    /// - sets members to defaults
    ////////
    const_iterator();

    ////////
    /// constructor semantics ->
    /// - sets members from parameters
    ////////
    const_iterator(slab* p, slab_const_iterator pos);

    ////////
    /// operator++ (prefix) semantics ->
    /// - if incrementing pos_ hits slab_ end
    ///   - moves pos_ to next slab
    /// - increments pos_
    /// - returns ref to self
    ////////
    const_iterator& operator++();

    ////////
    /// operator++ (postfix) semantics ->
    /// - creates temporary from self
    /// - increments self using prefix operator++
    /// - returns temporary
    ////////
    const_iterator operator++(int);

    ////////
    /// const operator* semantics ->
    /// - derefs and returns pos_
    ////////
    const T& operator*() const;

    ////////
    /// const operator-> semantics ->
    /// - returns pos_
    ////////
    const T* operator->() const;

    ////////
    /// operator!=
    ////////
    template <class U>
    friend bool operator!=(const U& l, const U& r);

    ////////
    /// operator==
    ////////
    template <class U>
    friend bool operator==(const U& l, const U& r);

  private:

    slab*                slab_;
    slab_const_iterator  pos_;

  };

  ////////
  /// constructor
  ////////
  template <class T>
  inline
  deque<T>::
  const_iterator::
  const_iterator() :
    slab_(nullptr) {
  }

  ////////
  /// constructor
  ////////
  template <class T>
  inline
  deque<T>::
  const_iterator::
  const_iterator(slab* p,
                 slab_const_iterator pos) :
    slab_(p),
    pos_(pos) {
  }

  ////////
  /// operator++
  ////////
  template <class T>
  inline typename deque<T>::const_iterator&
  deque<T>::
  const_iterator::
  operator++() {

    if (++pos_ == slab_->end()) {
      slab_ = slab_->link_;
      pos_ = slab_->begin();
    }
    return *this;
  }

  ////////
  /// operator++ (postfix)
  ////////
  template <class T>
  inline typename deque<T>::const_iterator
  deque<T>::
  const_iterator::
  operator++(int) {

    const_iterator tmp(*this);
    ++*this;
    return tmp;
  }

  ////////
  /// operator*
  ////////
  template <class T>
  inline const T&
  deque<T>::
  const_iterator::
  operator*() const {
    return *pos_;
  }

  ////////
  /// operator->
  ////////
  template <class T>
  inline const T*
  deque<T>::
  const_iterator::
  operator->() const {
    return &(*pos_);
  }

  ////////
  /// operator!=
  ////////
  template <class T>
  inline bool
  operator!=(const T& l,
             const T& r) {
    return l.slab_ != r.slab_ || l.pos_ != r.pos_;
  }
  
  ////////
  /// operator==
  ////////
  template <class T>
  inline bool
  operator==(const T& l,
             const T& r) {
    return l.slab_ == r.slab_ && l.pos_ == r.pos_;
  }

  ////////
  /// constructor
  ////////
  template <class T>
  inline
  deque<T>::
  deque(const size_t size) :
    first_(nullptr),
    last_ (nullptr),
    size_ (0) {

    if (!size) return;

    ////////
    /// compute number of slabs
    ////////
    const size_t nblocks = (size / slab_size_) +
                           (size % slab_size_);
    ////////
    /// create first slab
    ////////
    first_ = new slab(slab_size_);
    last_  = first_;

    ////////
    /// create any remaining slabs
    ////////
    for (size_t i = 1; i < nblocks; ++i) {
      slab* p = new slab(slab_size_);
      last_->link_ = p;
      last_ = p;
    }
  }

  ////////
  /// copy constructor
  ////////
  template <class T>
  inline
  deque<T>::
  deque(const deque<T>& q) :
    first_(nullptr),
    last_ (nullptr),
    size_ (0) {
    copy(q);
  }

  ////////
  /// assignment operator
  ////////
  template <class T>
  inline deque<T>&
  deque<T>::
  operator=(const deque<T>& q) {

    ////////
    /// noop for self assignment
    ////////
    if (this == &q) return *this;

    ////////
    /// clear self and copy from q
    ////////
    clear();
    copy(q);
    return *this;
  }

  ////////
  /// move constructor
  ////////
  template <class T>
  inline
  deque<T>::
  deque(deque&& q) :
    first_(nullptr),
    last_(nullptr),
    size_(0) {

    ////////
    /// clear self and move members over
    ////////
    clear();
    move(q);
  }

  ////////
  /// move assignment
  ////////
  template <class T>
  inline deque<T>&
  deque<T>::
  operator=(deque&& q) {

    ////////
    /// noop if self assignment
    ////////
    if (this == &q) return *this;

    ////////
    /// clear self and move members over
    ////////
    clear();
    move(q);
    return *this;
  }

  ////////
  /// destructor
  ////////
  template <class T>
  inline
  deque<T>::
  ~deque() {
    clear();
  }

  ////////
  /// copy
  ////////
  template <class T>
  inline void
  deque<T>::
  copy(const deque<T>& q) {
  
    ////////
    /// noop if empty
    ////////
    size_ = q.size_;
    if (!q.first_) return;

    ////////
    /// create the first slab
    ////////
    first_ = new slab(q.first_->size_);
    first_->slab_ = q.first_->slab_;
    last_ = first_;

    ////////
    /// iterate over the rest copying over
    ////////
    for (slab* p = q.first_->link_; p; p = p->link_) {

      slab* s = new slab(p->size_);
      s->slab_ = p->slab_;
      last_->link_ = s;
      last_ = s;
    }
  }

  ////////
  /// move
  ////////
  template <class T>
  inline void
  deque<T>::
  move(deque&& q) {

    ////////
    /// set members from q
    ////////
    first_ = q.first_;
    last_ = q.last_;
    size_ = q.size_;

    ////////
    /// nullptr out q
    ////////
    q.first_ = q.last_ = nullptr;
    q.size_ = 0;
  }

  ////////
  /// clear
  ////////
  template <class T>
  inline void
  deque<T>::
  clear() {

    for (slab* p = first_; p; p = p->link_) {
      slab* q = p;
      p = p->link_;
      delete q;
    }
    size_ = 0;
    first_ = last_ = nullptr;
  }

  ////////
  /// push_back
  ////////
  template <class T>
  inline void
  deque<T>::
  push_back(const T& t) {
  
    if (!last_->full()) {

      ////////
      /// add to last slab
      ////////
      last_->push_back(t);
    }
    else {

      ////////
      /// link another slab
      ////////
      slab* p = new slab(slab_size_);
      p->push_back(t);
      last_->link_ = p;
      last_ = p;
    }
    ++size_;
  }

  ////////
  /// emplace_back
  ////////
  template <class T>
  template <class... U>
  inline void
  deque<T>::
  emplace_back(U&&... args) {
  
    if (!last_->full()) {

      ////////
      /// add to last slab
      ////////
      last_->emplace_back(std::forward<U>(args)...);
    }
    else {

      ////////
      /// link another slab
      ////////
      slab* p = new slab(slab_size_);
      p->emplace_back(std::forward<U>(args)...);
      last_->link_ = p;
      last_ = p;
    }
    ++size_;
  }

  ////////
  /// push_front
  ////////
  template <class T>
  inline void
  deque<T>::
  push_front(const T& t) {
  
    if (!first_->full()) {

      ////////
      /// prepend to first slab
      ////////
      first_->insert(first_->begin(), t);
    }
    else {

      ////////
      /// prelink another slab
      ////////
      slab* p = new slab(slab_size_);
      p->insert(p->begin(), t);
      first_->link_ = p;
      first_ = p;
    }
    ++size_;
  }

  ////////
  /// emplace_front
  ////////
  template <class T>
  template <class... U>
  inline void
  deque<T>::
  emplace_front(U&&... args) {
  
    if (!first_->full()) {

      ////////
      /// prepend to first slab
      ////////
      first_->emplace(first_->begin(), std::forward<U>(args)...);
    }
    else {

      ////////
      /// prelink another slab
      ////////
      slab* p = new slab(slab_size_);
      p->emplace(p->begin(), std::forward<U>(args)...);
      first_->link_ = p;
      first_ = p;
    }
    ++size_;
  }

  ////////
  /// begin
  ////////
  template <class T>
  inline typename deque<T>::iterator
  deque<T>::
  begin() {
    return iterator(first_, first_ ? first_->begin()
                                   : slab_iterator());
  }

  ////////
  /// end
  ////////
  template <class T>
  inline typename deque<T>::iterator
  deque<T>::
  end() {
    return iterator(last_, last_ ? last_->end()
                                 : slab_iterator());
  }

  ////////
  /// const begin
  ////////
  template <class T>
  inline typename deque<T>::const_iterator
  deque<T>::
  begin() const {
    return const_iterator(first_, first_ ? first_->begin()
                                         : slab_const_iterator());
  }

  ////////
  /// end
  ////////
  template <class T>
  inline typename deque<T>::const_iterator
  deque<T>::
  end() const {
    return const_iterator(last_, last_ ? last_->end()
                                       : slab_const_iterator());
  }

  ////////
  /// constructor
  ////////
  template <class T>
  inline
  deque<T>::
  slab::
  slab(const size_t size) :
    size_(size),
    link_(nullptr) {
    slab_.reserve(size);
  }

  ////////
  /// push_back
  ////////
  template <class T>
  inline void
  deque<T>::
  slab::
  push_back(const T& t) {
    slab_.push_back(t);
  }

  ////////
  /// emplace_back
  ////////
  template <class T>
  template <class... U>
  inline void
  deque<T>::
  slab::
  emplace_back(U&&... args) {
    slab_.emplace_back(std::forward<U>(args)...);
  }

  ////////
  /// insert
  ////////
  template <class T>
  inline void
  deque<T>::
  slab::
  insert(slab_iterator pos,
         const T& t) {
    slab_.insert(pos, t);
  }

  ////////
  /// emplace
  ////////
  template <class T>
  template <class... U>
  inline void
  deque<T>::
  slab::
  emplace(slab_iterator pos,
          U&&... args) {
    slab_.emplace(pos, std::forward<U>(args)...);
  }

  ////////
  /// full
  ////////
  template <class T>
  inline bool
  deque<T>::
  slab::
  full() const {
    return slab_.size() == size_;
  }

  ////////
  /// empty
  ////////
  template <class T>
  inline bool
  deque<T>::
  slab::
  empty() const {
    return slab_.empty();
  }

  ////////
  /// begin
  ////////
  template <class T>
  inline typename deque<T>::slab_iterator
  deque<T>::
  slab::
  begin() {
    return slab_.begin();
  }

  ////////
  /// end
  ////////
  template <class T>
  inline typename deque<T>::slab_iterator
  deque<T>::
  slab::
  end() {
    return slab_.end();
  }

  ////////
  /// const begin
  ////////
  template <class T>
  inline typename deque<T>::slab_const_iterator
  deque<T>::
  slab::
  begin() const {
    return slab_.begin();
  }

  ////////
  /// end
  ////////
  template <class T>
  inline typename deque<T>::slab_const_iterator
  deque<T>::
  slab::
  end() const {
    return slab_.end();
  }

}
