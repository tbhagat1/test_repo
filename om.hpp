#ifndef __EXP_ORDER_TRACKER_HPP__
#define __EXP_ORDER_TRACKER_HPP__

#include <memory>
#include <set>
#include <boost/tokenizer.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <ec.hpp>

namespace trade {

namespace mti = boost::multi_index;

////////
/// basic order tracker
////////
class order_tracker {
public:

  ////////
  /// constructor w/ file name
  ////////
  order_tracker(const std::string& file);

  ////////
  /// execute
  ////////
  bool exec(support::error_code& err);

private:

  ////////
  /// buy or sell side
  ////////
  enum class side_t { buy, sell, unknown };

  ////////
  /// action types - new, cancel or trade
  ////////
  enum class action_t { new_order, cancel, modify, trade, unknown };

  ////////
  /// order info
  ////////
  struct order {

    ////////
    /// default constructor
    ////////
    order();

    ////////
    /// initialize
    ////////
    bool init(support::error_code& err, const std::string& line);

    action_t  action;    /// new, cancel, trade
    int       prod;      /// product id
    int       id;        /// order id
    side_t    side;      /// buy or sell
    int       quantity;  /// order quanity
    int       price;     /// order price

    ////////
    /// shared ptr to order
    ////////
    typedef std::shared_ptr<order> ptr;
  };

  ////////
  /// mti tags
  ////////
  struct order_id_tag  {};
  struct composite_tag {};
  struct prod_id_tag   {};

  ////////
  /// mti container keyed by:
  /// - [order id] -> must be unique but doesn't have to be ordered
  ///   used in new, cancel, modify
  /// - [prod id] -> non-unique for tracing purposes
  /// - [side, product id, price] -> not unique but must be ordered
  ///   to assist in matching/reconciling; used in trade
  ////////
  typedef mti::multi_index_container<
    order::ptr,
    mti::indexed_by<
      mti::hashed_unique<
        mti::tag<order_id_tag>,
        mti::member<order, int, &order::id>
      >,
      mti::ordered_non_unique<
        mti::tag<prod_id_tag>,
        mti::member<order, int, &order::prod>
      >,
      mti::ordered_non_unique<
        mti::tag<composite_tag>,
        mti::composite_key<
          order::ptr,
          mti::member<order, int,    &order::prod>,
          mti::member<order, side_t, &order::side>,
          mti::member<order, int,    &order::price>
        >
      >
    >
  > order_table;

  ////////
  /// index shortcuts
  ////////
  typedef order_table::index<order_id_tag>::type   order_id_ndx;
  typedef order_table::index<composite_tag>::type  composite_ndx;
  typedef order_table::index<prod_id_tag>::type    prod_id_ndx;

  ////////
  /// trade counts`
  ////////
  struct trade_count {

    trade_count(int c, int p) : count(c), price(p) {}

    int count;
    int price;
  };
  ////////
  /// maps [product id] -> [trade count]
  ////////
  typedef std::map<int, trade_count>  trade_counts;

  ////////
  /// needed for some kind of reconciliation at the end
  ////////
  typedef std::set<order::ptr>  order_set;

  ////////
  /// handle new
  ////////
  void handle_new(support::error_code& err, order::ptr order);

  ////////
  /// handle cancel
  ////////
  void handle_cancel(support::error_code& err, order::ptr order);

  ////////
  /// handle modify
  ////////
  void handle_modify(support::error_code& err, order::ptr order);

  ////////
  /// handle trade
  ////////
  void handle_trade(support::error_code& err, order::ptr order);

  ////////
  /// rollback
  ////////
  void rollback(composite_ndx::iterator p,
                const std::vector<int>& rollback);

  ////////
  /// trace trade counts
  ////////
  void trace_trade_counts(order::ptr op);

  ////////
  /// resolve
  ////////
  void resolve();

  ////////
  /// for tracing order
  ////////
  template <class T>
  friend T& operator<<(T& out, const order& in);

  ////////
  /// for tracing order table
  ////////
  template <class T>
  friend T& operator<<(T& out, const order_table& in);

  ////////
  /// for tracing order tracker
  ////////
  template <class T>
  friend T& operator<<(T& out, const order_tracker& in);

  ////////
  /// input file
  ////////
  const std::string file_;

  ////////
  /// the main order table
  ////////
  order_table  orders_;

  ////////
  /// processed message count - used for tracing
  ////////
  size_t message_count_;

  /////////
  /// for some tracing trade reason
  /////////
  trade_counts  trade_counts_;

  ////////
  /// potential matches
  ////////
  order_set potentials_;
};

};

#endif
