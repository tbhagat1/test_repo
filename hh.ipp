namespace util {

////////
/// class iterator
////////
template <class K, class V, class H>
class unordered_map<K,V,H>::iterator {
public:

  ////////
  /// - unordered_map type needed for bucket iteration
  /// - bucket_iterator for iterating over underlying map
  ////////
  typedef unordered_map<K,V,H>               owner_t;
  typedef typename owner_t::bucket_iterator  bucket_iterator;
  typedef typename owner_t::value_type       value_type;

  ////////
  /// constructor semantics ->
  /// - sets members from parameters
  ////////
  iterator(size_t slot, owner_t* owner, bucket_iterator pos);

  ////////
  /// operator++ semantics ->
  /// - increment pos_
  /// - invoke scroll to possibly scroll forward
  /// - return ref to self
  ////////
  iterator& operator++();

  ////////
  /// operator++ (postfix) semantics ->
  /// - increment pos_
  /// - invoke scroll to possibly scroll forward
  /// - return ref to self
  ////////
  iterator operator++(int);

  ////////
  /// operator* semantics ->
  /// - return deref of pos_
  ////////
  value_type& operator*();

  ////////
  /// operator-> semantics ->
  /// - return address of value_type beneath pos_
  ////////
  value_type* operator->();

  ////////
  /// scroll semantics ->
  /// - noop if current slot_ not empty
  /// - else iterate over remaining slots looking for a non-empty one
  /// - if non-empty slots not found, set pos_ to end iterator
  /// - else set pos to stopped at iterator
  ////////
  void scroll();

private:

  ////////
  /// operator!= semantics ->
  /// - compares by slot and pos for inequality
  ////////
  friend bool operator!=(const iterator& l,
                         const iterator& r) {
    return l.slot_ != r.slot_ || l.pos_ != r.pos_;
  }

  ////////
  /// operator== semantics ->
  /// - compares by slot and pos for equality
  ////////
  friend bool operator==(const iterator& l,
                         const iterator& r) {
    return l.slot_ == r.slot_ && l.pos_ == r.pos_;
  }

  size_t           slot_;
  owner_t*         owner_;
  bucket_iterator  pos_;

};

////////
/// constructor
////////
template <class K, class V, class H>
inline
unordered_map<K,V,H>::
iterator::
iterator(size_t slot,
         owner_t* owner,
         bucket_iterator pos) :
  slot_(slot),
  owner_(owner),
  pos_(pos) {
}

////////
/// operator++
////////
template <class K, class V, class H>
inline typename unordered_map<K,V,H>::iterator&
unordered_map<K,V,H>::
iterator::
operator++() {
  ++pos_;
  scroll();
  return *this;
}

////////
/// operator++
////////
template <class K, class V, class H>
inline typename unordered_map<K,V,H>::iterator
unordered_map<K,V,H>::
iterator::
operator++(int) {
  iterator tmp(*this);
  ++*this;
  return tmp;
}

////////
/// operator*
////////
template <class K, class V, class H>
inline typename unordered_map<K,V,H>::value_type&
unordered_map<K,V,H>::
iterator::
operator*() {
  return *pos_;
}

////////
/// operator->
////////
template <class K, class V, class H>
inline typename unordered_map<K,V,H>::value_type*
unordered_map<K,V,H>::
iterator::
operator->() {
  return &(*pos_);
}

////////
/// scroll
////////
template <class K, class V, class H>
inline void
unordered_map<K,V,H>::
iterator::
scroll() {

  ////////
  /// short-circuit if not at end of current slot
  ////////
  typename owner_t::buckets& buckets = owner_->buckets_;
  if (pos_ != buckets[slot_].end()) return; 

  ////////
  /// else scroll to next non-empty slot
  ////////
  while (++slot_ < buckets.size() && buckets[slot_].empty());

  ////////
  /// update pos_
  ////////
  if (slot_ == buckets.size()) {
    pos_ = buckets[slot_ - 1].end();
  }
  else {
    pos_ = buckets[slot_].begin();
  }
}

////////
/// class const_iterator
////////
template <class K, class V, class H>
class unordered_map<K,V,H>::const_iterator {
public:

  ////////
  /// - unordered_map type needed for bucket iteration
  /// - bucket_iterator for iterating over underlying map
  ////////
  typedef unordered_map<K,V,H>                     owner_t;
  typedef typename owner_t::bucket_const_iterator  bucket_const_iterator;
  typedef typename owner_t::value_type             value_type;

  ////////
  /// constructor semantics ->
  /// - sets members from parameters
  ////////
  const_iterator(size_t slot, const owner_t* owner, bucket_const_iterator pos);

  ////////
  /// operator++ semantics ->
  /// - increment pos_
  /// - invoke scroll to possibly scroll forward
  /// - return ref to self
  ////////
  const_iterator& operator++();

  ////////
  /// operator++ (postfix) semantics ->
  /// - increment pos_
  /// - invoke scroll to possibly scroll forward
  /// - return ref to self
  ////////
  const_iterator operator++(int);

  ////////
  /// operator* semantics ->
  /// - return deref of pos_
  ////////
  const value_type& operator*() const;

  ////////
  /// operator-> semantics ->
  /// - return address of value_type beneath pos_
  ////////
  const value_type* operator->() const;

  ////////
  /// scroll semantics ->
  /// - noop if current slot_ not empty
  /// - else iterate over remaining slots looking for a non-empty one
  /// - if non-empty slots not found, set pos_ to end iterator
  /// - else set pos to stopped at iterator
  ////////
  void scroll();

private:

  ////////
  /// operator!= semantics ->
  /// - compares by slot and pos for inequality
  ////////
  friend bool operator!=(const const_iterator& l,
                         const const_iterator& r) {
    return l.slot_ != r.slot_ || l.pos_ != r.pos_;
  }

  ////////
  /// operator== semantics ->
  /// - compares by slot and pos for equality
  ////////
  friend bool operator==(const const_iterator& l,
                         const const_iterator& r) {
    return l.slot_ == r.slot_ && l.pos_ == r.pos_;
  }

  size_t                 slot_;
  const owner_t*         owner_;
  bucket_const_iterator  pos_;

};

////////
/// constructor
////////
template <class K, class V, class H>
inline
unordered_map<K,V,H>::
const_iterator::
const_iterator(size_t slot,
               const owner_t* owner,
               bucket_const_iterator pos) :
  slot_(slot),
  owner_(owner),
  pos_(pos) {
}

////////
/// operator++
////////
template <class K, class V, class H>
inline typename unordered_map<K,V,H>::const_iterator&
unordered_map<K,V,H>::
const_iterator::
operator++() {
  ++pos_;
  scroll();
  return *this;
}

////////
/// operator++
////////
template <class K, class V, class H>
inline typename unordered_map<K,V,H>::const_iterator
unordered_map<K,V,H>::
const_iterator::
operator++(int) {
  const_iterator tmp(*this);
  ++*this;
  return tmp;
}

////////
/// operator*
////////
template <class K, class V, class H>
inline const typename unordered_map<K,V,H>::value_type&
unordered_map<K,V,H>::
const_iterator::
operator*() const {
  return *pos_;
}

////////
/// operator->
////////
template <class K, class V, class H>
inline const typename unordered_map<K,V,H>::value_type*
unordered_map<K,V,H>::
const_iterator::
operator->() const {
  return &(*pos_);
}

////////
/// scroll
////////
template <class K, class V, class H>
inline void
unordered_map<K,V,H>::
const_iterator::
scroll() {

  ////////
  /// short-circuit if not at end of current slot
  ////////
  const typename owner_t::buckets& buckets = owner_->buckets_;
  if (pos_ != buckets[slot_].end()) return; 

  ////////
  /// else scroll to next non-empty slot
  ////////
  while (++slot_ < buckets.size() && buckets[slot_].empty());

  ////////
  /// update pos_
  ////////
  if (slot_ == buckets.size()) {
    pos_ = buckets[slot_ - 1].end();
  }
  else {
    pos_ = buckets[slot_].begin();
  }
}

////////
/// constructor
////////
template <class K, class V, class H>
unordered_map<K,V,H>::
unordered_map(const size_t n) :
  size_(0) {
  buckets_.resize(n);
}

////////
/// copy constructor
////////
template <class K, class V, class H>
inline
unordered_map<K,V,H>::
unordered_map(const unordered_map& m) :
  size_(m.size_) {

  buckets_.reserve(size_);
  std::copy(m.buckets_.begin(),
            m.buckets_.end(),
            std::back_inserter(buckets_));
}

////////
/// assignment operator
////////
template <class K, class V, class H>
inline unordered_map<K,V,H>&
unordered_map<K,V,H>::
operator=(const unordered_map& m) {

  if (this == &m) return *this;

  clear();
  size_ = m.size_;
  buckets_.reserve(size_);
  std::copy(m.buckets_.begin(),
            m.buckets_.end(),
            std::back_inserter(buckets_));
  return *this;
}

////////
/// move constructor
////////
template <class K, class V, class H>
inline
unordered_map<K,V,H>::
unordered_map(unordered_map&& m) :
  size_(0) {

  size_ = m.size_;
  buckets_ = std::move(m.buckets_);
  m.size_ = 0;
}

////////
/// move assignment
////////
template <class K, class V, class H>
inline unordered_map<K,V,H>&
unordered_map<K,V,H>::
operator=(unordered_map&& m) {

  if (this == &m) return *this;

  size_ = m.size_;
  buckets_ = std::move(m.buckets_);
  m.size_ = 0;
}

////////
/// destructor
////////
template <class K, class V, class H>
inline
unordered_map<K,V,H>::
~unordered_map() {
  clear();
}

////////
/// find
////////
template <class K, class V, class H>
inline typename unordered_map<K,V,H>::iterator
unordered_map<K,V,H>::
find(const K& k) {
  size_t i = hasher_(k) % buckets_.size();
  bucket_iterator p = buckets_[i].find(k);
  return iterator(i, this, p);
}

////////
/// const find
////////
template <class K, class V, class H>
inline typename unordered_map<K,V,H>::const_iterator
unordered_map<K,V,H>::
find(const K& k) const {
  size_t i = hasher_(k) % buckets_.size();
  bucket_const_iterator p = buckets_[i].find(k);
  return const_iterator(i, this, p);
}

////////
/// clear
////////
template <class K, class V, class H>
inline void
unordered_map<K,V,H>::
clear() {
  buckets_.clear();
  size_ = 0;
}

////////
/// insert
////////
template <class K, class V, class H>
inline typename unordered_map<K,V,H>::insert_result
unordered_map<K,V,H>::
insert(const value_type& v) {

  size_t i = hasher_(v.first) % buckets_.size();
  insert_effect ie = buckets_[i].insert(v);
  iterator pos(i, this, ie.first);
  if (ie.second) ++size_;
  return insert_result(pos, ie.second);
}

////////
/// operator[]
////////
template <class K, class V, class H>
inline typename unordered_map<K,V,H>::value&
unordered_map<K,V,H>::
operator[](const key& k) {
  size_t i = hasher_(k) % buckets_.size();
  insert_effect ie = buckets_[i].insert(std::pair(k, V()));
  ++size_;
  return ie.first->second;
}

////////
/// erase
////////
template <class K, class V, class H>
inline bool
unordered_map<K,V,H>::
erase(const key& k) {
  size_t i = hasher_(k) % buckets_.size();
  bool b = buckets_[i].erase(k) == 1;
  if (b) --size_;
  return b;
}

////////
/// size
////////
template <class K, class V, class H>
inline size_t
unordered_map<K,V,H>::
size() const {
  return size_;
}

////////
/// empty
////////
template <class K, class V, class H>
inline bool
unordered_map<K,V,H>::
empty() const {
  return size_ == 0;
}

////////
/// begin
////////
template <class K, class V, class H>
inline typename unordered_map<K,V,H>::iterator
unordered_map<K,V,H>::
begin() {
  iterator i(0, this, buckets_[0].begin());
  i.scroll();
  return i;
}

////////
/// end
////////
template <class K, class V, class H>
inline typename unordered_map<K,V,H>::iterator
unordered_map<K,V,H>::
end() {
  return iterator(buckets_.size(),
                  this,
                  buckets_[buckets_.size() - 1].end());
}

////////
/// const begin
////////
template <class K, class V, class H>
inline typename unordered_map<K,V,H>::const_iterator
unordered_map<K,V,H>::
cbegin() const {
  const_iterator i(0, this, buckets_[0].begin());
  i.scroll();
  return i;
}

////////
/// const end
////////
template <class K, class V, class H>
inline typename unordered_map<K,V,H>::const_iterator
unordered_map<K,V,H>::
cend() const {
  return const_iterator(buckets_.size(),
                        this,
                        buckets_[buckets_.size() - 1].end());
}

}
