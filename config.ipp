namespace support {

/**-----------------------------------------------------------------------------
 * Instance
 */
inline config&
config::
instance() {
  static config instance_;
  return instance_;
}

/**-----------------------------------------------------------------------------
 * Constructor
 */
inline
config::
config()
{}

/**-----------------------------------------------------------------------------
 * Initialize
 */
inline bool
config::
init(support::error_code& err,
     const std::string& file) {

  ////////
  /// open config file
  ////////
  file_ = file;
  std::ifstream ifs(file_, std::ifstream::in);

  if (!ifs) {
    /// build up error code
    return false;
  }

  ////////
  /// slurp contents into string
  ////////
  std::string contents(std::istreambuf_iterator<char>(ifs),
                       std::istreambuf_iterator<char>());
  
  ////////
  /// parse into json root
  ////////

  ////////
  /// iterate over children and insert_or_assign into maps
  ////////

  return true;
}

/**-----------------------------------------------------------------------------
 * Insert or Assign
 */
template <class T>
inline bool
config::
insert_or_assign(const std::string& k,
                 const T& v) {

  if constexpr (std::is_same_v<T, bool>) {
    return bool_map_.insert_or_assign(k, v).second;
  }
  else if constexpr (std::is_same_v<T, int>) {
    return int_map_.insert_or_assign(k, v).second;
  }
  else if constexpr (std::is_same_v<T, int>) {
    return float_map_.insert_or_assign(k, v).second;
  }
  else if constexpr (std::is_same_v<T, std::string>) {
    return string_map_.insert_or_assign(k, v).second;
  }
  return false;
}

/**-----------------------------------------------------------------------------
 * Find
 */
template <class T>
inline auto
config::
find(const std::string& k) const {

  if constexpr (std::is_same_v<T, bool>) {
    auto p = bool_map_.find(k);
    return p == bool_map_.end() ? T() : p->second;
  }
  else if constexpr (std::is_same_v<T, int>) {
    auto p = int_map_.find(k);
    return p == int_map_.end() ? T() : p->second;
  }
  else if constexpr (std::is_same_v<T, float>) {
    auto p = float_map_.find(k);
    return p == float_map_.end() ? T() : p->second;
  }
  else if constexpr (std::is_same_v<T, std::string>) {
    auto p = string_map_.find(k);
    return p == string_map_.end() ? T() : p->second;
  }
}

/**-----------------------------------------------------------------------------
 * As String
 */
template <class T>
inline std::string
config::
as_string(const std::string& k) const {

  static const std::string s;

  if constexpr (std::is_same_v<T, bool>) {
    auto p = bool_map_.find(k);
    return p == bool_map_.end() ? s : p->second ? "true" : "false";
  }
  else if constexpr (std::is_same_v<T, int>) {
    auto p = int_map_.find(k);
    return p == int_map_.end() ? s : std::to_string(p->second);
  }
  else if constexpr (std::is_same_v<T, float>) {
    auto p = float_map_.find(k);
    return p == float_map_.end() ? s : std::to_string(p->second);
  }
  else if constexpr (std::is_same_v<T, std::string>) {
    auto p = string_map_.find(k);
    return p == string_map_.end() ? s : p->second;
  }
}

/**-----------------------------------------------------------------------------
 * Serialize
 */
inline bool
config::
serialize(support::error_code& err) {

  ////////
  /// populate json root w/ all the type maps
  ////////

  ////////
  /// open config file for writing
  ////////

  ////////
  /// stream json root into config file stream
  ////////
  return true;
}

}
