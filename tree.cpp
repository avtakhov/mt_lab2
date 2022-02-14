#include "tree.h"

#include <iostream>

namespace parser {

namespace {

void Error(Iterator const& it) {
  throw std::runtime_error("Unexpected token '" + std::string(it->value) + "' at pos " + std::to_string(it.Position()));
}

Tree ExpectTerm(Iterator &it, Token::Type expected_type) {
  auto result = it++;
  if (result->type != expected_type) {
    Error(result);
  }
  return Tree(result->value);
}

Tree N(Iterator &it) {
  return ExpectTerm(it, Token::Type::kWord);
}

Tree V(Iterator &it) {
  switch (it->type) {
    case Token::Type::kWord: {
      return ExpectTerm(it, Token::Type::kWord);
    }
    case Token::Type::kStar: {
      Tree result("nonterm V -> * V");
      result.Add(ExpectTerm(it, Token::Type::kStar));
      result.Add(V(it));
      return result;
    }
    default: {
      Error(it);
    }
  }
  throw std::runtime_error("Unreachable");
}

Tree Vars(Iterator &it) {
  switch (it->type) {
    case Token::Type::kComma: {
      Tree result("nonterm Vars -> , V Vars");
      result.Add(ExpectTerm(it, Token::Type::kComma));
      result.Add(V(it));
      result.Add(Vars(it));
      return result;
    }
    case Token::Type::kSemicolon: {
      return Tree("nonterm Vars -> eps");
    }
    default: {
      Error(it);
    }
  }
  throw std::runtime_error("Unreachable");
}

}

Tree S(Iterator it) {
  Tree result("nonterm S -> N V Vars ;");
  result.Add(N(it));
  result.Add(V(it));
  result.Add(Vars(it));
  result.Add(ExpectTerm(it, Token::Type::kSemicolon));
  result.Add(ExpectTerm(it, Token::Type::kEnd));
  return result;
}

std::ostream& operator<<(std::ostream& os, Tree const& tree) {
  os << tree.Id() << "[label=\"" << tree.name << "\"]" << std::endl;
  for (const auto& child : tree.children) {
    os << child;
    os << tree.Id() << " -- " << child.Id() << std::endl;
  }
  return os;
}

void Tree::Add(Tree &&tree)  {
  children.push_back(std::move(tree));
}
std::int64_t Tree::Id() const {
  return id_;
}

}