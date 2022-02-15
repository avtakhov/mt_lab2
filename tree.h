#pragma once

#include "tokenizer.h"

#include <vector>

namespace parser {

inline std::int64_t next_id = 0;

struct Tree {

  Tree(std::string_view name) : name(name) {}

  std::string_view name;
  std::vector<Tree> children;

  void Add(Tree &&tree);

  std::int64_t Id() const;
 private:
  std::int64_t id_ = next_id++;
};

std::ostream &operator<<(std::ostream &os, Tree const &tree);

}