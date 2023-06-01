#ifndef BINOMIAL_MODEL
#define BINOMIAL_MODEL

#include "tree.h"
#include <math.h>

namespace BinomialTree {

struct Config {
  double up_factor;
  double down_factor;
  double market_probability;
  double risk_neutral_probability;
  double risk_free_interest;
  double initial_price;
  int steps;
  double strike_price;
}; // namespace Config

struct Config DEFAULT_CONFIG {
  .up_factor = 1.2, .down_factor = 0.8, .market_probability = 0.6,
  .risk_free_interest = 0.02, .initial_price = 100, .steps = 4,
  .strike_price = 110
};

class Model {
private:
  Tree<double> *stock_price_tree;
  Tree<double> *option_value_tree;

  Config parameters;

  float risk_neutral_probability = 0;

  void calculuate_risk_neutal_probability() {
    risk_neutral_probability =
        (exp(parameters.risk_free_interest) - parameters.down_factor) /
        (parameters.up_factor - parameters.down_factor);
  }

  void build_price_tree() {
    // add the initial price of the stock
    (*stock_price_tree).add(parameters.initial_price);
    for (int i = 1; i < (int)pow(2, parameters.steps) - 1; ++i) {
      // get parent element
      double previous_price = (*stock_price_tree).get_parent(i);
      // compute the current price
      double price_multipler = parameters.down_factor * (i % 2 != 0) +
                               parameters.up_factor * (i % 2 == 0);
      double cur_price = previous_price * price_multipler;
      (*stock_price_tree).add(cur_price);
    }
  }

  void build_option_tree() {
    for (int i = std::pow(2, parameters.steps - 1) - 1;
         i < pow(2, parameters.steps); ++i) {
      double cur_opt_val =
          std::max(0.0, (*stock_price_tree).get(i) - parameters.strike_price);
      (*option_value_tree).set(i, cur_opt_val);
    }

    for (int i = std::pow(2, parameters.steps - 1) - 2; i >= 0; --i) {
      std::pair<double, double> next_values =
          (*option_value_tree).get_children(i);
      double early_excercise_value =
          std::max(0.0, (*stock_price_tree).get(i) - parameters.strike_price);
      double expectation_value =
          std::exp(-parameters.risk_free_interest) *
          (risk_neutral_probability * next_values.second +
           (1 - risk_neutral_probability) * next_values.first);
      (*option_value_tree)
          .set(i, std::max(early_excercise_value, expectation_value));
    }
  }

public:
  Model(Config parameters) {
    this->parameters = parameters;
    this->stock_price_tree = new Tree<double>();
    this->option_value_tree = new Tree<double>();
  };

  void _exec() {
    // Compute risk neutral prob
    calculuate_risk_neutal_probability();
    // Build the price tree
    this->build_price_tree();
    this->build_option_tree();
    printf("-- Current Value Of Call Option = %f\n", (*option_value_tree).get(0));
  }
};

} // namespace BinomialTree

#endif