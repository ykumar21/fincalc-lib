#ifndef BINOMIAL_MODEL
#define BINOMIAL_MODEL

#include "tree.h"
#include <math.h>

namespace BinomialTree {

struct Config {
  float up_factor;
  float down_factor;
  float market_probability;
  float risk_neutral_probability;
  float risk_free_interest;
  float initial_price;
  int steps;
}; // namespace Config

struct Config DEFAULT_CONFIG {
  .up_factor = 1.2, .down_factor = 0.8, .market_probability = 0.6,
  .risk_free_interest = 0.02, .initial_price = 100, .steps = 4
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

public:
  Model(Config parameters) {
    this->parameters = parameters;
    this->stock_price_tree = new Tree<double>();
    this->option_value_tree = new Tree<double>();
    // Compute risk neutral prob
    calculuate_risk_neutal_probability();
    // Build the price tree
    this->build_price_tree();
    std::cout << *stock_price_tree;
  };
};

} // namespace BinomialTree

#endif