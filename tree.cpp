#include "tree.h"

#include <iostream>

namespace parser {

std::ostream &operator<<(std::ostream &os, Tree const &tree) {
  os << tree.Id() << "[label=\"" << tree.name << "\"]" << std::endl;
  for (const auto &child : tree.children) {
    os << child;
    os << tree.Id() << " -- " << child.Id() << std::endl;
  }
  return os;
}

void Tree::Add(Tree &&tree) {
  children.push_back(std::move(tree));
}
std::int64_t Tree::Id() const {
  return id_;
}

}