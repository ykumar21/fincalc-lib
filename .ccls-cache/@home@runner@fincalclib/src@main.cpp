#include <iostream>
#include <stdio.h>

#include "includes/binomial_tree.h"

BinomialTree::Config build_config() {
  // Get configuration style
  int config_style;

  printf("Do you want to build config? (0/1) ");
  std::cin >> config_style;

  if (config_style == 0) {
    return BinomialTree::DEFAULT_CONFIG;
  } else {
    BinomialTree::Config cfg;
    std::cout << "Enter Up Factor: ";
    std::cin >> cfg.up_factor;
    std::cout << "Enter Down Factor: ";
    std::cin >> cfg.down_factor;
    std::cout << "Enter Initial Price: ";
    std::cin >> cfg.initial_price;
    return cfg;
  }
}

int main() {
  std::cout << "Hello World!\n";
  BinomialTree::Config config = build_config();
  BinomialTree::Model model(config);
}