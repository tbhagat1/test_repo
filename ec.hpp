#ifndef __EXP_ERROR_CODE_HPP__
#define __EXP_ERROR_CODE_HPP__

#include <string>
#include <vector>
#include <iostream>

namespace support {

////////
/// basic error container
////////
struct error_code {

  ////////
  /// default constructor
  ////////
  error_code();

  ////////
  /// constructor
  ////////
  error_code(int code, const std::string& text);

  ////////
  /// append
  ////////
  void append(int code, const std::string& text);

  ////////
  /// boolean operator
  ////////
  operator bool() const;

  typedef std::vector<error_code>  chain_t;

  int         code;
  std::string text;
  chain_t     chain;

  ////////
  /// operator<<
  /// - traces no error if code is zero and chain empty
  /// - traces its code and text
  /// - traces chain
  ////////
  template <class T>
  friend T& operator<<(T& out, const error_code& ec);
};

////////
/// default constructor
////////
inline
error_code::
error_code() : code(0) {}

////////
/// constructor
////////
inline
error_code::
error_code(int code,
           const std::string& text) :
  code(code),
  text(text) {
}

////////
/// append
////////
void
error_code::
append(int cod,
       const std::string& txt) {

  if ( !code) {
    code = cod;
    text = txt;
  }
  else {
    chain.push_back(error_code(cod, txt));
  }
}

////////
/// boolean operator
////////
inline
error_code::
operator bool() const {
  return code == 0 && chain.empty();
}

////////
/// operator<<
////////
template <class T>
inline T&
operator<<(T& out, const error_code& ec) {

  if (!ec.code && ec.chain.empty()) {
    return out << "no errors" << std::endl;
  }

  out << "code: "
      << ec.code
      << ", text: "
      << ec.text
      << std::endl;

  for (size_t i = 0; i < ec.chain.size(); ++i) {
    out << "code: "
        << ec.chain[i].code
        << ", text: "
        << ec.chain[i].text
        << std::endl;
  }
  return out;
}

};

#endif
