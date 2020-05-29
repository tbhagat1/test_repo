namespace con {

  ////////
  /// constructor
  ////////
  template <class T>
  inline
  shared_ptr<T>::
  shared_ptr(T* p) :
    control_(nullptr),
    uses_mks_(false) {

    if (p) {
      control_ = new control_block();
      control_->count_ = 1;
      control_->underlying_ = p;
    }
  }

  ////////
  /// copy constructor
  ////////
  template <class T>
  inline
  shared_ptr<T>::
  shared_ptr(const shared_ptr<T>& p) :
    control_(nullptr),
    uses_mks_(false) {

    control_ = p.control_;
    uses_mks_ = p.uses_mks_;
    ++control_->count_;
  }

  ////////
  /// assignment operator
  ////////
  template <class T>
  inline shared_ptr<T>&
  shared_ptr<T>::
  operator=(const shared_ptr<T>& p) {

    if (this == &p) return *this;

    if (control_ && --control_->count_ == 0) {
      destroy();
    }
    control_ = p.control_;
    uses_mks_ = p.uses_mks_;
    if (control_) {
      ++control_->count_;
    }
    return *this;
  }

  ////////
  /// destructor
  ////////
  template <class T>
  inline
  shared_ptr<T>::
  ~shared_ptr() {
    if (control_ && --control_->count_ == 0) {
      destroy();
    }
  }

  ////////
  /// destroy
  ////////
  template <class T>
  inline void
  shared_ptr<T>::
  destroy() {

    ////////
    /// called only if control_ not nullptr
    ////////
    T* underlying = control_->underlying_;
    if (uses_mks_) {
      underlying->~T();
    }
    else {
      delete underlying;
    }
    delete control_;
  }

  ////////
  /// reset
  ////////
  template <class T>
  inline void
  shared_ptr<T>::
  reset(T* p) {

    if (control_ && --control_->count_ == 0) {
      destroy();
    }
    if (p) {
      control_ = new control_block();
      control_->count_ = 1;
      control_->underlying_ = p;
    }
    uses_mks_ = false;
  }

  ////////
  /// get
  ////////
  template <class T>
  inline T*
  shared_ptr<T>::
  get() {
    return control_->underlying_;
  }

  ////////
  /// const get
  ////////
  template <class T>
  inline const T*
  shared_ptr<T>::
  get() const {
    return control_->underlying_;
  }

  ////////
  /// operator*
  ////////
  template <class T>
  inline T&
  shared_ptr<T>::
  operator*() {
    return *control_->underlying_;
  }

  ////////
  /// const operator*
  ////////
  template <class T>
  inline const T&
  shared_ptr<T>::
  operator*() const {
    return *control_->underlying_;
  }

  ////////
  /// operator->
  ////////
  template <class T>
  inline T*
  shared_ptr<T>::
  operator->() {
    return control_->underlying_;
  }

  ////////
  /// const operator->
  ////////
  template <class T>
  inline const T*
  shared_ptr<T>::
  operator->() const {
    return control_->underlying_;
  }

  ////////
  /// control constructor
  ////////
  template <class T>
  inline
  shared_ptr<T>::
  shared_ptr(control_block* p) :
    control_(p),
    uses_mks_(true)
  {}

  ////////
  /// make shared
  ////////
  template <class U, class... V>
  inline shared_ptr<U>
  make_shared(V&&... args) {

    ////////
    /// allocate total memory first
    ////////
    typedef typename shared_ptr<U>::control_block control_block_t;
    const size_t size = sizeof(control_block_t) + sizeof(U);
    unsigned char*  p = new unsigned char[size];

    ////////
    /// construct control block first
    ////////
    new (p) control_block_t();
    control_block_t* pcb = reinterpret_cast<control_block_t*>(p);
    pcb->count_ = 1;

    ////////
    /// construct underlying object next
    ////////
    p += sizeof(control_block_t);
    new (p) U (std::forward<U>(args)...);
    pcb->underlying_ = reinterpret_cast<U*>(p);

    ////////
    /// return sp w/ setting to use make shared
    /// so underlying object isn't deleted but
    /// its destructor is called
    ////////
    return shared_ptr<U>(pcb);
  }
}
