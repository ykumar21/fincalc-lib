#ifndef BINOMIAL_MODEL
#define BINOMIAL_MODEL

#include "tree.h"
#include <math.h>
#include <stdint.h>  /* types */

#define _PRICE_TREE_DEST "price.txt"
#define _OPTION_TREE_DEST "value.txt"

namespace BinomialTree {

/**
 * @brief Market parameters for model 
 * 
 */
struct Config {
  double up_factor;
  double down_factor;
  double market_probability;
  double risk_neutral_probability;
  double risk_free_interest;
  double initial_price;
  int    steps;
  double strike_price;
}; // namespace Config


/**
 * @brief Default configuration for the binomial tree
 * 
 */
struct Config DEFAULT_CONFIG {
  .up_factor = 1.2, .down_factor = 0.8, .market_probability = 0.6,
  .risk_free_interest = 0.02, .initial_price = 100, .steps = 4,
  .strike_price = 110
};

/**
 * @brief Binomial Tree model to price options 
 * 
 */
class Model {
private:
  Tree::BTree<double> *stock_price_tree;  /* future price movement of stock */
  Tree::BTree<double> *option_value_tree; /* future option value */
  Config       parameters; /* market parameters */
  float        risk_neutral_probability = 0;

  void calculuate_risk_neutal_probability() {
    risk_neutral_probability =
        (exp(parameters.risk_free_interest) - parameters.down_factor) /
        (parameters.up_factor - parameters.down_factor);
  }

  void build_price_tree() {
    // add the initial price of the stock
    (*stock_price_tree).add(parameters.initial_price);

    for (int i = 1; i < (int)pow(2, parameters.steps); ++i) {
      std::cout << *stock_price_tree << std::endl;
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
    this->stock_price_tree =
        new Tree::BTree<double>((int)std::pow(2, this->parameters.steps) - 1);
    this->option_value_tree =
        new Tree::BTree<double>((int)std::pow(2, this->parameters.steps) - 1);
  };

  void run() {
    // Compute risk neutral prob
    calculuate_risk_neutal_probability();
    // Build the price tree
    printf("Building Price Tree... \n");
    this->build_price_tree();

    printf("Building Option Value Tree... \n");
    this->build_option_tree();
    printf("-- Current Value Of Call Option = %f\n",
           (*option_value_tree).get(0));
  }

  /**
   * @brief Save model outputs to text file 
   * 
   * @return std::string Output ID  
   */
  std::string save() {}
};
} // namespace BinomialTree

#endif