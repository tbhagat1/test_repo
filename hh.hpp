#ifndef __MY_UNORDERED_MAP_HPP__
#define __MY_UNORDERED_MAP_HPP__

#include <map>
#include <vector>
#include <memory>
#include <algorithm>

namespace util {

////////
/// unordered_map
////////
template <class K, class V, class H = std::hash<K>>
class unordered_map {
public:

  ////////
  /// iterator fwd decls
  ////////
  class iterator;
  class const_iterator;

  ////////
  /// key, value, hasher renamed
  ////////
  typedef K key;
  typedef V value;
  typedef H hasher;

  ////////
  /// [key,value] is the value type
  ////////
  typedef std::pair<key, value> value_type;

  ////////
  /// inserts return this
  ////////
  typedef std::pair<iterator, bool> insert_result;

  ////////
  /// constructor semantics ->
  /// - allocates n buckets
  /// - each bucket is initially empty
  ////////
  unordered_map(const size_t n = 10);

  ////////
  /// copy constructor semantics ->
  /// - allocates as many buckets as specified by m
  /// - copies each bucket from m into self
  ////////
  unordered_map(const unordered_map& m);

  ////////
  /// assignment operator semantics ->
  /// - noop if self assignment
  /// - clears self
  /// - allocates as many buckets as specified by m
  /// - copies each bucket from m into self
  ////////
  unordered_map& operator=(const unordered_map& m);

  ////////
  /// move constructor semantics ->
  /// - moves buckets from m to self
  /// - nullptrs out m
  ////////
  unordered_map(unordered_map&& m);

  ////////
  /// move assignment semantics ->
  /// - noop if self assignment
  /// - moves buckets from m to self
  /// - nullptrs out m
  ////////
  unordered_map& operator=(unordered_map&& m);

  ////////
  /// destructor semantics ->
  /// - invokes clear
  ////////
  ~unordered_map();

  ////////
  /// find semantics ->
  /// - hashes key using hasher to find slot
  /// - find's key within bucket map
  /// - returns iterator to result
  ////////
  iterator find(const key& k);

  ////////
  /// const find semantics ->
  /// - hashes key using hasher to find slot
  /// - find's key within bucket map
  /// - returns const_iterator to result
  ////////
  const_iterator find(const key& k) const;
  
  ////////
  /// clear semantics ->
  /// - clears all buckets
  ////////
  void clear();

  ////////
  /// insert semantics ->
  /// - hashes v.key using hasher to find slot
  /// - inserts entry into slot's map
  /// - increments size on insert success
  ////////
  insert_result insert(const value_type& v);

  ////////
  /// operator[] semantics ->
  /// - hashes key using hasher to find slot
  /// - uses slot's map's operator[] to create
  ///   default entry and return ref to it
  ////////
  value& operator[](const key& k);

  /// erase semantics ->
  /// - hashes v.key using hasher to find slot
  /// - erases entry from slot's map
  /// - decrements size on erase success
  ////////
  bool erase(const key& k); 

  ////////
  /// size semantics ->
  /// - number of total [k,v] items in container
  ////////
  size_t size() const;

  ////////
  /// empty semantics ->
  /// - true if size is zero
  ////////
  bool empty() const;

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
  const_iterator cbegin() const;

  ////////
  /// const end
  ////////
  const_iterator cend() const;

private:

  ////////
  /// we're using a map to hold the bucket
  /// chain instead of the typical list -
  /// faster lookups/insertions?
  ////////
  typedef std::map<key, value>             bucket;
  typedef std::unique_ptr<bucket>          bucket_ptr;
  typedef std::vector<bucket>              buckets;
  typedef typename bucket::iterator        bucket_iterator;
  typedef typename bucket::const_iterator  bucket_const_iterator;
  typedef std::pair<bucket_iterator, bool> insert_effect;

  friend class iterator;
  friend class const_iterator;
  friend class base_iterator;

  buckets  buckets_;
  hasher   hasher_;
  size_t   size_;
};

}

#include <hh.ipp>

#endif
