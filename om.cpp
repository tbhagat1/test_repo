#include <fstream>
#include <om.hpp>
#include <iomanip>
#include <limits>
#include <boost/algorithm/string/trim.hpp>

namespace trade {

////////
/// constructor
////////
order_tracker::
order_tracker(const std::string& file) :
  file_(file),
  message_count_(0)
{}

////////
/// execute
////////
bool
order_tracker::
exec(support::error_code& err) {

  ////////
  /// attempt to open input file
  ////////
  std::ifstream ifs(file_);
  if ( !ifs) {
    std::string s = "Bad input file: <:" + file_ + ">";
    err = support::error_code(-1, s);
    return false;
  }
  ////////
  /// start reading each line from the file
  ////////
  std::string line;
  while (std::getline(ifs, line)) {

    ////////
    /// attempt to create an order from the line
    ////////
    order::ptr  op = std::make_shared<order>();
    if ( !op->init(err, line)) {
    }
    ////////
    /// handle new order
    ////////
    else if (op->action == action_t::new_order) {
      handle_new(err, op);
    }
    ////////
    /// handle cancel order
    ////////
    else if (op->action == action_t::cancel) {
      handle_cancel(err, op);
    }
    ////////
    /// handle modify order
    ////////
    else if (op->action == action_t::modify) {
      handle_modify(err, op);
    }
    ////////
    /// handle trade message
    ////////
    else if (op->action == action_t::trade) {
      handle_trade(err, op);
    }
    ////////
    /// trace every 10 messages - invalid or not ?
    ////////
    if (++message_count_ % 10 == 0) {
      std::cout << orders_ << std::endl;
    }
  }
  resolve();
  return err;
}

////////
/// order constructor
////////
order_tracker::
order::
order() :
  action  (action_t::unknown),
  prod    (-1),
  id      (-1),
  side    (side_t::unknown),
  quantity(-1),
  price   (-1)
{}

////////
/// tokenizer types
////////
typedef boost::tokenizer<boost::char_separator<char>> tokenizer_t;
static boost::char_separator<char> the_sep(",");

////////
/// parse int
////////
bool
parse_int(support::error_code& err,
          int& result,
          tokenizer_t::const_iterator p,
          const std::string& field,
          const std::string& line) {

  const std::string q = boost::trim_copy(*p);
  if (q.empty()) {
    std::string s = "Empty " + field + " for line <" + line + ">";
    err.append(-1, s);
    return false;
  }
  result = ::atoi(q.c_str());
  if (result <= 0) {
    std::string s = "Negative " + field + " for line <" + line + ">";
    err.append(-1, s);
    return false;
  }
  return true;
}

////////
/// initialize order
////////
bool
order_tracker::
order::
init(support::error_code& err,
     const std::string& line) {

  tokenizer_t tok(line, the_sep);
  tokenizer_t::iterator p = tok.begin();
  tokenizer_t::iterator q = tok.end();
  const size_t size = std::distance(p, q);

  ////////
  /// must be able to see action
  ////////
  if (!size) {
    std::string  s = "Cannot parse invalid line <" + line + ">";
    err.append(-1, s );
    return false;
  }
  ////////
  /// validate action type
  ////////
  const std::string t = boost::trim_copy(*p);
  if (t != "N" && t != "R" && t != "M" && t != "X") {
    std::string s = "Cannot parse, invalid action <" + line + ">";
    err.append(-1, s);
    return false;
  }
  action = t == "N" ? action_t::new_order :
           t == "R" ? action_t::cancel    :
           t == "M" ? action_t::modify    :
                      action_t::trade; 

  ////////
  /// each action has a different number of tokens
  ////////
  if (action == action_t::new_order && size != 6) {
    std::string s = "Cannot parse, invalid tokens for new line <";
    s += line + ">";
    err.append(-1, s);
    return false;
  }
  else if ((action == action_t::cancel ||
           action == action_t::modify) && size != 5) {
    std::string s = "Cannot parse, invalid tokens for cancel/modify <";
    s += line + ">";
    err.append(-1, s);
    return false;
  }
  else if (action == action_t::trade && size != 4) {
    std::string s = "Cannot parse, invalid tokens for trade <";
    s += line + ">";
    err.append(-1, s);
    return false;
  }
  ////////
  /// new
  ///   N,5,100000,S,1,1075
  /// cancel
  ///   R,100000,S,1,1075
  /// modify
  ///   M,100000,S,1,1075
  /// trade
  ///   X,5,2,1025
  ////////
  if (action == action_t::new_order) {

    ////////
    /// extract product id for new orders
    ////////
    ++p;
    if (!parse_int(err, prod, p, "product id", line)) return false;
  }
  ++p;
  if (action == action_t::new_order ||
      action == action_t::cancel    ||
      action == action_t::modify) {

    ////////
    /// new, cancel, modify have order id
    ////////
    if (!parse_int(err, id, p, "order id", line)) return false;
    ++p;   

    ////////
    /// followed by side (buy or sell)
    ////////
    const std::string q = boost::trim_copy(*p);
    if (q.empty() || (q != "B" && q != "S")) {
      std::string s = "Invalid buy/sell inidicator for line <" + line + ">";
      err.append(-1, s);
      return false;
    }
    side = q == "B" ? side_t::buy : side_t::sell;
    ++p;

    ////////
    /// followed by quantity and price
    ////////
    if (!parse_int(err, quantity, p, "quantity", line)) return false;
    ++p;
    if (!parse_int(err, price, p, "price", line)) return false;
  }
  else {

    ////////
    /// trade has product id at this point
    ////////
    if (!parse_int(err, prod, p, "product id", line)) return false;
    ++p;

    ////////
    /// followed by quantity and price
    ////////
    if (!parse_int(err, quantity, p, "quantity", line)) return false;
    ++p;
    if (!parse_int(err, price, p, "price", line)) return false;
  }
  return true;
}

////////
/// for upper bounds
////////
static int price_max = std::numeric_limits<int>::max();

////////
/// handle new
////////
void
order_tracker::
handle_new(support::error_code& err,
           order::ptr op) {

  ////////
  /// attempt to insert into container
  ////////
  order_id_ndx& ndx = orders_.get<order_id_tag>();
  std::pair<order_id_ndx::iterator, bool>  p = orders_.insert(op);
  if (!p.second) {
    std::string s = "Failed to add new order to order book - duplicate; ";
    s += "order id <" + std::to_string(op->id) + ">";
    err.append(-1, s );
  }
}

////////
/// handle cancel
////////
void
order_tracker::
handle_cancel(support::error_code& err,
              order::ptr op) {
  ////////
  /// attempt to find by order id in container
  ////////
  order_id_ndx& ndx = orders_.get<order_id_tag>();
  order_id_ndx::iterator i = ndx.find(op->id);

  if (i == ndx.end()) {
    std::string s = "Failed to cancel order - not found; ";
    s += "order id <" + std::to_string(op->id) + ">";
    err.append(-1, s);
  }
  ////////
  /// erase order from order table
  ////////
  else {
    ndx.erase(i);
  }
}

////////
/// handle modify
////////
void
order_tracker::
handle_modify(support::error_code& err,
              order::ptr op) {

  ////////
  /// attempt to find by order id in container
  ////////
  order_id_ndx& ndx = orders_.get<order_id_tag>();
  order_id_ndx::iterator i = ndx.find(op->id);
  if (i == ndx.end()) {
    std::string s = "Failed to modify order - not found; ";
    s += "order id <" + std::to_string(op->id) + ">";
    err.append(-1, s);
  }
  ////////
  /// guess all ok; update quantity
  /// -> or are we supposed to subtract quantity.....
  ////////
  else {
    (*i)->quantity = op->quantity;
  }
}

////////
/// rollback
////////
void
order_tracker::
rollback(composite_ndx::iterator p,
         const std::vector<int>& rollback) {
  for (size_t i = 0; i < rollback.size(); ++i, ++p) {
    (*p)->quantity = rollback[i];
  }
}

////////
/// handle trade
////////
void
order_tracker::
handle_trade(support::error_code& err,
             order::ptr op) {

  ////////
  /// for the buy side locate the range; i guess buyer is willing to 
  /// pay upto the trade price...
  ////////
  composite_ndx::iterator p, q;
  composite_ndx& cn = orders_.get<composite_tag>();
  p = cn.lower_bound(boost::make_tuple(op->prod, side_t::buy, op->price));
  q = cn.upper_bound(boost::make_tuple(op->prod, side_t::buy, price_max));
  composite_ndx::iterator bp = p;
  int qty = op->quantity;
  std::vector<int> buy_rollback;

  ////////
  /// iterate over the range while quantity remains
  ////////
  for (; p != q && qty > 0; ++p) {

    buy_rollback.push_back((*p)->quantity);
    int reduce_by = std::min(qty, (*p)->quantity);
    (*p)->quantity -= reduce_by;
    qty -= reduce_by;
  }
  ////////
  /// trade indicated quantity should have hit zero for buy
  ////////
  if (qty != 0) {

    std::string s = "Invalid trade (X) transaction; quantiy not zero ";
    s += "for buy side. Product " + std::to_string(op->prod) + " ";
    s += "price " + std::to_string(op->price) + " quantity ";
    s += std::to_string(op->quantity);
    err.append(-1, s);
    rollback(bp, buy_rollback);
    return;
  }

  ////////
  /// same for sell side; locate lower and upper bounds
  ////////
  p = cn.lower_bound(boost::make_tuple(op->prod, side_t::sell, op->price));
  q = cn.upper_bound(boost::make_tuple(op->prod, side_t::sell, price_max));
  composite_ndx::iterator sp = p;
  qty = op->quantity;
  std::vector<int> sell_rollback;

  ////////
  /// iterate over the range while quantity remains
  ////////
  for (; p != q && qty > 0; ++p) {

    sell_rollback.push_back((*p)->quantity);
    int reduce_by = std::min(qty, (*p)->quantity);
    (*p)->quantity -= reduce_by;
    qty -= reduce_by;
  }
  ////////
  /// trade indicated quantity should have hit zero for sell
  ////////
  if (qty != 0) {
    std::string s = "Invalid trade (X) transaction; quantiy not zero ";
    s += "for sell side. Product " + std::to_string(op->prod) + " ";
    s += "price " + std::to_string(op->price) + " quantity ";
    s += std::to_string(op->quantity);
    err.append(-1, s);
    rollback(sp, sell_rollback);
    rollback(bp, buy_rollback);
    return;
  }
  ////////
  /// not sure why we need this tracing
  ////////
  trace_trade_counts(op);
}

////////
/// trace trade counts
////////
void
order_tracker::
trace_trade_counts(order::ptr op) {

  ////////
  /// find by product id in trade counts map
  ////////
  trade_counts::iterator i = trade_counts_.find(op->prod);

  ////////
  /// insert new entry
  ////////
  if (i == trade_counts_.end()) {
    i = trade_counts_.insert(
      std::pair(op->prod, trade_count{op->quantity, op->price})).first;
  }
  ////////
  /// existing entry - price changed
  ////////
  else if (i->second.price != op->price) {
    i->second.count = op->quantity;
    i->second.price = op->price;
  }
  ////////
  /// existing entry - price did not change
  ////////
  else {
    i->second.count += op->quantity;
  }
  ////////
  /// finally trace the trade message
  ////////
  std::cout << "X,"
            << op->prod
            << ","
            << op->quantity
            << ","
            << op->price
            << " => "
            << "product "
            << op->prod
            << ": "
            << i->second.count
            << "@"
            << i->second.price
            << std::endl;
}

////////
/// resolve
////////
void
order_tracker::
resolve() {

  const order_id_ndx& oin = orders_.get<order_id_tag>();
  order_id_ndx::const_iterator p = oin.begin();
  order_id_ndx::const_iterator q = oin.end();

  for (; p != q; ++p) {

    const order::ptr& op = *p;
    side_t other = op->side == side_t::buy ? side_t::sell : side_t::buy;

    const composite_ndx& cn = orders_.get<composite_tag>();
    composite_ndx::const_iterator t, u;

    int lprice = op->side == side_t::buy ? 0         : op->price;
    int uprice = op->side == side_t::buy ? op->price : price_max;

    t = cn.lower_bound(boost::make_tuple(op->prod, other, lprice));
    u = cn.upper_bound(boost::make_tuple(op->prod, other, uprice));
    int qty = op->quantity;

    for (; t != u && qty > 0; ++t) {
      int reduce_by = std::min(qty, (*t)->quantity);
      const order::ptr& po = *t;
      if (reduce_by > 0) {
        potentials_.insert(*t);
      }
      qty -= reduce_by;
    }
  }
}

////////
/// operator<< (order)
////////
template <class T>
T& operator<<(T& out, const order_tracker::order& in) {

  const char* act =
    in.action == order_tracker::action_t::new_order ? "N" :
    in.action == order_tracker::action_t::cancel    ? "R" :
    in.action == order_tracker::action_t::modify    ? "M" :
    in.action == order_tracker::action_t::trade     ? "X" : "U";

  out << act << ", ";

  if (in.action == order_tracker::action_t::new_order ||
      in.action == order_tracker::action_t::trade) {
    out << in.prod << ", ";
  }
  if (in.action == order_tracker::action_t::new_order ||
      in.action == order_tracker::action_t::cancel    ||
      in.action == order_tracker::action_t::modify) {

    const char* side =
      in.side == order_tracker::side_t::buy  ? "B" :
      in.side == order_tracker::side_t::sell ? "S" : "U";

    out << in.id << ", " << side << ", ";
  }
  return out << in.quantity
             << ", "
             << in.price;
}

////////
/// operator<< (order_table)
////////
template <class T>
T& operator<<(T& out, const order_tracker::order_table& in) {

  ////////
  /// acquire index for product id
  ////////
  const order_tracker::prod_id_ndx& ndx = in.get<order_tracker::prod_id_tag>();
  order_tracker::prod_id_ndx::const_iterator p = ndx.begin();

  size_t traced = 0;
  int last_prod = -1;

  for (; p != ndx.end(); ++p) {

    order_tracker::order::ptr op = *p;

    ////////
    /// reset count when product changes
    ////////
    if (last_prod != op->prod) {
      last_prod = op->prod;
      traced = 0;
    }
    ////////
    /// noop if traced count hits 5??
    ////////
    if (traced == 5) {
    }
    ////////
    /// trace the order
    ////////
    else {
      out << *op << std::endl;
      ++traced;
    }
  }
  return out;
}
 
////////
/// operator<< (order_tracker)
////////
template <class T>
T& operator<<(T& out, const order_tracker& in) {

  out << in.orders_;

  if (!in.potentials_.empty()) {

    out << "Unresolved orders: " << std::endl;
    order_tracker::order_set::const_iterator p = in.potentials_.begin();
    order_tracker::order_set::const_iterator q = in.potentials_.end();

    for (; p != q; ++p) {
      const order_tracker::order::ptr& op = *p;
      out << *op << std::endl;
    }
  }
  return out;
}

}  /// namespace trade

int main(int argc, const char** argv) {

  if (argc != 2) {
    std::cout << "Usage: <" << argv[0] << "> <filename>" << std::endl;
    return -1;
  }
  trade::order_tracker ot(argv[1]);
  support::error_code err;
  bool rc = ot.exec(err);
  std::cout << ot;
  if (!rc) {
    std::cout << err;
  }
}
