#include "tokenizer.h"
#include "tree.h"
#include "grammar.h"

#include <iostream>
#include <cassert>
#include <fstream>

int main(int argc, char const **argv) {
  assert(argc > 2);
  auto test = argv[1];
  auto error_file = argv[2];

  auto e = parser::Tokenizer(test);
  try {
    auto result = parser::grammar::S(e.begin(), e.end());
    std::cout << "graph {" << std::endl << "label=\"" << test << "\"" << std::endl << result << "}" << std::endl;
  } catch (std::runtime_error const &err) {
    std::cout << "graph {}" << std::endl;
    std::ofstream out(error_file);
    out << err.what() << std::endl;
  }
}
