#pragma once

#include <map>
#include <string>
#include <type_traits>
#include <fstream>
#include <iterator>

namespace support {

class error_code {};

/**-----------------------------------------------------------------------------
 * Configuration Holder
 */
class config {
public:

  /**---------------------------------------------------------------------------
   * Instance
   *
   * @return  myers singleton instance
   */
  static config& instance();

  /**---------------------------------------------------------------------------
   * Initialize
   *
   * - Opens json configuration file.
   * - Loads json configuration file into json string.
   * - Uses [some kind of] json parser to parse json string.
   * - Iterates over json children and adds values.
   *
   * @param[inout]  err   contains any errors
   * @param[in]     file  json configuration file
   * @return              true if successful
   */
  bool init(support::error_code& err, const std::string& file = "config.json");

  /**---------------------------------------------------------------------------
   * Insert or Assign
   *
   * - Uses static if to determine template type.
   * - Calls insert_or_assign on relevant map.
   * - Returns true if inserted, false if assigned.
   *
   * @param[in]  k  key
   * @param[in]  v  value
   * @return        true if inserted, false if assigned
   */
  template <class T>
  bool insert_or_assign(const std::string& k, const T& v);

  /**---------------------------------------------------------------------------
   * Find
   *
   * - Uses static if to determine template type.
   * - Tries to locate k in relevant map.
   * - Returns value or default for T [throw exception??].
   *
   * @param[in]  k  key
   * @return        value if found or default for T
   */
  template <class T>
  auto find(const std::string& s) const;

  /**---------------------------------------------------------------------------
   * As String
   *
   * - Uses static if to determine template type.
   * - Tries to locate k in relevant map.
   * - Returns value as string or '' if not found [throw exception??].
   *
   * @param[in]  k  key
   * @return        value if found or ''
   */
  template <class T>
  std::string as_string(const std::string& k) const;

  /**---------------------------------------------------------------------------
   * Serialize
   *
   * - Builds json tree using each type map.
   * - Opens config file for write [losing old contents].
   * - Serializes json tree into file.
   *
   * @param[in]  err  contains any errors
   * @return          true if successful, false otherwise
   */
  bool serialize(support::error_code& err);

private:

  /**---------------------------------------------------------------------------
   * Default Constructor [for singleton instantiation]
   */
  config();

  /**---------------------------------------------------------------------------
   * Copy Constructor [disabled]
   */
  config(const config&) = delete;

  /**---------------------------------------------------------------------------
   * Assignment Operator [disabled]
   */
  config& operator=(const config&) = delete;

  /**---------------------------------------------------------------------------
   * Move Assignment [disabled]
   */
  config(config&&) = delete;

  /**---------------------------------------------------------------------------
   * Copy Assignment [disabled]
   */
  config& operator=(config&&) = delete;

  ////////
  /// is there a way to have a generic map?
  ////////
  using bool_map   = std::map<std::string, bool>;
  using int_map    = std::map<std::string, int>;
  using float_map  = std::map<std::string, float>;
  using string_map = std::map<std::string, std::string>;

  std::string  file_;
  bool_map     bool_map_;
  int_map      int_map_;
  float_map    float_map_;
  string_map   string_map_;

};

}

#include <config.ipp>
